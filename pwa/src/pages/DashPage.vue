<script setup lang="ts">
import { useDashStore } from 'stores/dash-store';
import { computed, onMounted, onUnmounted } from 'vue';

const dashStore = useDashStore();

const carregando = computed(
  () => !dashStore.mqttConectado
    || dashStore.umidadeAtual == null
    || dashStore.statusIrrigacao == null,
);
const statusIrrigacao = computed(() => dashStore.statusIrrigacao);
const umidadeAtual = computed(() => dashStore.umidadeAtual);

const corIndicadorUmidade = computed(() => {
  if (umidadeAtual.value == null) return 'gray';
  if (umidadeAtual.value <= 20) return 'red';
  if (umidadeAtual.value <= 35) return 'deep-orange';
  if (umidadeAtual.value <= 50) return 'orange';
  if (umidadeAtual.value <= 65) return 'amber';
  if (umidadeAtual.value <= 80) return 'lime';
  return 'green';
});

const irrigacaoManual = () => dashStore.irrigacaoManual();

onMounted(() => {
  dashStore.conectarMqtt();
});

onUnmounted(() => {
  dashStore.desconectarMqtt();
});
</script>

<template>
  <q-page v-if="carregando" class="column items-center justify-center">
    <q-spinner
      color="primary"
      size="4em"
    />
    <span class="text-bold font-14 q-mt-md">Conectando...</span>
  </q-page>

  <q-page v-else class="column items-center justify-center">
    <q-knob
      readonly
      v-model="umidadeAtual"
      show-value
      size="150px"
      :thickness="0.22"
      :color="corIndicadorUmidade"
      :track-color="corIndicadorUmidade + '-3'"
      :class="'q-ma-md text-' + corIndicadorUmidade"
    >
      <div class="column items-center">
        <span class="font-14 text-bold">Umidade</span>
        {{umidadeAtual}}%
      </div>
    </q-knob>

    <span class="text-bold">Irrigação {{ statusIrrigacao ? 'Ligada' : 'Desligada' }}</span>

    <q-btn @click="irrigacaoManual" color="primary" label="Iniciar Irrigação" class="q-mt-sm"
           :disable="!!statusIrrigacao" />
  </q-page>
</template>

<style lang="scss" scoped>
.font-14 {
  font-size: 14px;
}
</style>
