import { defineStore } from 'pinia';
import { ref } from 'vue';
import { useGlobalStore } from 'stores/global-store';
import { IClientOptions, MqttClient, connect } from 'mqtt';
import { Sensor } from './types/sensor';

export const useMqttStore = defineStore('mqtt', () => {
  const globalStore = useGlobalStore();

  const currentSensorId = ref<string|null>(null);
  const mqttClient = ref<MqttClient | null>(null);
  const mqttIsConnected = ref<boolean>(false);
  const irrigationStatus = ref<boolean | null>(null);
  const currentHumidity = ref<number | null>(null);

  const connectMqtt = async (sensor: Sensor) => {
    currentSensorId.value = sensor.id;
    const url = 'wss://97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud:8884/mqtt';
    const options: IClientOptions = {
      username: 'projeto-integrador-v',
      password: 'univesp001004',
    };
    const client = connect(url, options);

    client.on('connect', () => {
      setTimeout(() => {
        mqttIsConnected.value = true;
      }, 1000);
    });
    client.on('error', () => {
      globalStore.notifyError(
        'Ocorreu um erro ao tentar conectar ao servidor.',
      );
    });
    client.on('message', (topic, message) => {
      if (currentSensorId.value !== sensor.id) {
        return;
      }
      if (topic === sensor.topicIrrigationStatus) {
        irrigationStatus.value = message.toString() === 'true';
      }
      if (topic === sensor.topicCurrentHumidity) {
        currentHumidity.value = parseInt(message.toString(), 10);
      }
    });
    client.on('disconnect', () => {
      console.log('dc', sensor.name);
    });

    client.subscribe(sensor.topicIrrigationStatus);
    client.subscribe(sensor.topicCurrentHumidity);

    mqttClient.value = client;
  };

  const disconnectMqtt = async () => {
    mqttClient.value?.end();
    mqttClient.value = null;
    mqttIsConnected.value = false;
    irrigationStatus.value = null;
    currentHumidity.value = null;
  };

  const triggerIrrigation = async (sensor: Sensor) => {
    mqttClient.value?.publish(sensor.topicTriggerIrrigation, 'true');
  };

  return {
    mqttIsConnected,
    irrigationStatus,
    currentHumidity,
    connectMqtt,
    disconnectMqtt,
    triggerIrrigation,
  };
});
