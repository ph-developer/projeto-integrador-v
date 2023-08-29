<script setup lang="ts">
import { useAuthStore } from 'src/stores/auth-store';
import { useDashStore } from 'stores/dash-store';
import { computed, onBeforeMount } from 'vue';
import { useSensorEditStore } from 'stores/sensor-edit-store';

const authStore = useAuthStore();
const dashStore = useDashStore();
const sensorEditStore = useSensorEditStore();

const humidityIndicatorColor = computed(() => {
  if (dashStore.currentHumidity == null) return 'gray';
  if (dashStore.currentHumidity <= 20) return 'red';
  if (dashStore.currentHumidity <= 35) return 'deep-orange';
  if (dashStore.currentHumidity <= 50) return 'orange';
  if (dashStore.currentHumidity <= 65) return 'amber';
  if (dashStore.currentHumidity <= 80) return 'lime';
  return 'green';
});

onBeforeMount(async () => {
  await dashStore.loadSensors();
  await dashStore.connectCurrentSensor();
});

const onEditSensorClick = () => {
  sensorEditStore.showDialog(dashStore.currentSensor);
};

const onNewSensorClick = () => {
  sensorEditStore.showDialog(null);
};
</script>

<template>
  <q-page v-if="dashStore.isLoading" class="column items-center justify-center">
    <q-spinner color="primary" size="4em"/>
    <span class="text-bold font-14 q-mt-md">Carregando...</span>
  </q-page>

  <q-page v-else class="column items-center justify-center">
    <q-header class="row bg-transparent text-black q-mt-lg q-mx-sm justify-center items-center">
      <q-btn color="primary" flat rounded icon="more_vert" class="q-mr-auto">
        <q-menu>
          <q-list>
            <q-item clickable v-close-popup v-if="dashStore.hasSensors" @click="onEditSensorClick">
              <q-item-section>Editar Sensor</q-item-section>
            </q-item>
            <q-item clickable v-close-popup @click="onNewSensorClick">
              <q-item-section>Adicionar Sensor</q-item-section>
            </q-item>
          </q-list>
        </q-menu>
      </q-btn>

      <span v-if="dashStore.hasSensors">
        <q-btn @click="dashStore.goToPreviousSensor()" color="primary" flat icon="navigate_before"
               :disable="!dashStore.hasPreviousSensor"/>
        <span class="text-bold q-mx-sm">{{ dashStore.currentSensor?.name }}</span>
        <q-btn @click="dashStore.goToNextSensor()" color="primary" flat icon="navigate_next"
               :disable="!dashStore.hasNextSensor"/>
      </span>

      <q-btn @click="authStore.doLogout" color="primary" flat rounded icon="logout"
             class="q-ml-auto"/>
    </q-header>

    <div v-if="!dashStore.hasSensors" class="column items-center justify-center">
      <span class="text-bold">Não há sensores atribuídos a este usuário.</span>
    </div>

    <div v-else-if="dashStore.isConnectingSensor" class="column items-center justify-center">
      <q-spinner color="primary" size="4em"/>
      <span class="text-bold font-14 q-mt-md">Conectando...</span>
    </div>

    <div v-else class="column items-center justify-center">
      <q-knob readonly :model-value="dashStore.currentHumidity as number" show-value size="150px"
              :thickness="0.22" :color="humidityIndicatorColor"
              :track-color="humidityIndicatorColor + '-3'"
              :class="'q-ma-md text-' + humidityIndicatorColor">
        <div class="column items-center">
          <span class="font-14 text-bold">Umidade</span>
          <span v-if="dashStore.currentHumidity != null">{{ dashStore.currentHumidity }}%</span>
          <span v-else class="font-14 text-bold q-pt-md">...</span>
        </div>
      </q-knob>

      <span class="text-bold" v-if="dashStore.irrigationStatus == null">...</span>
      <span class="text-bold" v-else-if="dashStore.irrigationStatus">Irrigação Ligada</span>
      <span class="text-bold" v-else>Irrigação Desligada</span>

      <q-btn @click="dashStore.triggerIrrigation" color="primary" label="Acionar Irrigação"
             class="q-mt-sm" :disable="!!dashStore.irrigationStatus"/>
    </div>
  </q-page>
</template>
