<script setup>
import { useDialogPluginComponent, useQuasar } from 'quasar';
import { useSensorEditStore } from 'stores/sensor-edit-store';
import { reactive } from 'vue';
import { useGlobalStore } from 'stores/global-store';
import { useDashStore } from 'stores/dash-store';

defineEmits([
  ...useDialogPluginComponent.emits,
]);

const $q = useQuasar();
const {
  dialogRef,
  onDialogHide,
  onDialogOK,
  onDialogCancel,
} = useDialogPluginComponent();
const sensorEditStore = useSensorEditStore();
const dashStore = useDashStore();
const globalStore = useGlobalStore();

const sensor = reactive({
  id: sensorEditStore.currentEditingSensor?.id ?? null,
  name: sensorEditStore.currentEditingSensor?.name || '',
  topicCurrentHumidity: sensorEditStore.currentEditingSensor?.topicCurrentHumidity || '',
  topicIrrigationStatus: sensorEditStore.currentEditingSensor?.topicIrrigationStatus || '',
  topicTriggerIrrigation: sensorEditStore.currentEditingSensor?.topicTriggerIrrigation || '',
});

const onSaveClick = async () => {
  if (sensor.name === '') {
    globalStore.notifyError('O nome do sensor deve ser preenchido.');
    return;
  }
  if (sensor.topicCurrentHumidity === '') {
    globalStore.notifyError('O tópico "Umidade Atual" deve ser preenchido.');
    return;
  }
  if (sensor.topicIrrigationStatus === '') {
    globalStore.notifyError('O tópico "Status da Irrigação" deve ser preenchido.');
    return;
  }
  if (sensor.topicTriggerIrrigation === '') {
    globalStore.notifyError('O tópico "Acionar Irrigação" deve ser preenchido.');
    return;
  }
  let sensorId;
  if (sensor.id === null) {
    sensorId = await sensorEditStore.createSensor(sensor);
  } else {
    sensorId = await sensorEditStore.updateSensor(sensor);
  }
  onDialogOK?.();
  await dashStore.loadSensors();
  await dashStore.goToSensorId(sensorId);
};
const onDeleteClick = async () => {
  $q.dialog({
    title: `Excluir sensor "${sensor.name}"?`,
    message: `Deseja realmente excluir o sensor "${sensor.name}"? Esta ação é irreversível.`,
    cancel: {
      label: 'Cancelar',
    },
    ok: {
      label: 'Excluir',
    },
    persistent: true,
  }).onOk(async () => {
    await sensorEditStore.deleteSensor(sensor);
    onDialogOK?.();
    await dashStore.loadSensors();
    await dashStore.connectCurrentSensor();
  });
};
</script>

<template>
  <q-dialog ref="dialogRef" persistent @hide="onDialogHide">
    <q-card class="q-dialog-plugin">
      <q-card-section>
        <div v-if="sensor.id === null" class="text-h6">Adicionar Sensor</div>
        <div v-else class="text-h6">Alterar Sensor</div>
      </q-card-section>

      <q-card-section>
        <q-input outlined v-model="sensor.name" label="Nome" dense stack-label />

        <div class="text-subtitle2 q-mt-md">Tópicos</div>

        <q-input outlined v-model="sensor.topicCurrentHumidity" label="Umidade Atual" dense
                 stack-label class="q-mt-sm" />

        <q-input outlined v-model="sensor.topicIrrigationStatus" label="Status da Irrigação"
                 dense stack-label class="q-mt-sm" />

        <q-input outlined v-model="sensor.topicTriggerIrrigation" label="Acionar Irrigação"
                 dense stack-label class="q-mt-sm" />
      </q-card-section>

      <q-card-actions align="right">
        <q-btn v-if="sensor.id !== null" color="black" class="q-mr-auto" label="Excluir"
               @click="onDeleteClick"/>
        <q-btn color="primary" label="Cancelar" @click="onDialogCancel"/>
        <q-btn color="primary" label="Salvar" @click="onSaveClick"/>
      </q-card-actions>
    </q-card>
  </q-dialog>
</template>
