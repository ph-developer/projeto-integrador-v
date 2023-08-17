import { defineStore } from 'pinia';
import { ref } from 'vue';
import { useGlobalStore } from 'stores/global-store';
import { IClientOptions, MqttClient, connect as connectMqtt } from 'mqtt';

export const useDashStore = defineStore('dash', () => {
  const globalStore = useGlobalStore();

  const mqttClient = ref<MqttClient|null>(null);
  const mqttConectado = ref<boolean>(false);
  const statusIrrigacao = ref<boolean|null>(null);
  const umidadeAtual = ref<number|null>(null);

  const conectarMqtt = async () => {
    const url = 'wss://97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud:8884/mqtt';
    const options: IClientOptions = {
      username: 'projeto-integrador-v',
      password: 'univesp001004',
    };
    const client = connectMqtt(url, options);

    client.on('connect', () => {
      setTimeout(() => {
        mqttConectado.value = true;
      }, 1000);
    });
    client.on('error', () => {
      globalStore.notificarErro('Ocorreu um erro ao tentar conectar ao servidor.');
    });
    client.on('message', (topic, message) => {
      if (topic === 'pi_v/status_irrigacao') {
        statusIrrigacao.value = message.toString() === 'true';
      }
      if (topic === 'pi_v/umidade_atual') {
        umidadeAtual.value = parseInt(message.toString(), 10);
      }
    });

    client.subscribe('pi_v/status_irrigacao');
    client.subscribe('pi_v/umidade_atual');

    mqttClient.value = client;
  };

  const desconectarMqtt = async () => {
    mqttClient.value?.end();
    mqttClient.value = null;
    mqttConectado.value = false;
  };

  const irrigacaoManual = async () => {
    mqttClient.value?.publish('pi_v/irrigacao_manual', 'true');
  };

  return {
    mqttConectado,
    statusIrrigacao,
    umidadeAtual,
    conectarMqtt,
    desconectarMqtt,
    irrigacaoManual,
  };
});
