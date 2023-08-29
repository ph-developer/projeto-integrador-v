import { defineStore } from 'pinia';
import { useQuasar } from 'quasar';
import { Sensor } from 'stores/types/sensor';
import SensorEditDialog from 'src/dialogs/SensorEditDialog.vue';
import { computed, ref } from 'vue';
import {
  doc, deleteDoc, updateDoc, collection, addDoc,
} from 'firebase/firestore';
import { useFirebase } from 'boot/firebase';
import { useAuthStore } from 'stores/auth-store';

export const useSensorEditStore = defineStore('sensorEdit', () => {
  const $q = useQuasar();
  const firebase = useFirebase();
  const authStore = useAuthStore();

  const currentEditingSensor = ref<Sensor | null>(null);
  const isEditingSensor = computed(() => currentEditingSensor.value !== null);

  const showDialog = (sensor: Sensor | null) => {
    currentEditingSensor.value = sensor;
    $q.dialog({
      component: SensorEditDialog,
    });
  };

  const createSensor = async (sensor: Sensor) => {
    const currentUserId = authStore.currentUser?.id || null;
    if (currentUserId === null) return null;

    const db = firebase.db();
    const sensorsRef = collection(db, `users/${currentUserId}/sensors`);
    const createdSensor = await addDoc(sensorsRef, {
      name: sensor.name,
      topicTriggerIrrigation: sensor.topicTriggerIrrigation,
      topicCurrentHumidity: sensor.topicCurrentHumidity,
      topicIrrigationStatus: sensor.topicIrrigationStatus,
    });
    return createdSensor.id;
  };

  const updateSensor = async (sensor: Sensor) => {
    const currentUserId = authStore.currentUser?.id || null;
    if (currentUserId === null) return;

    const db = firebase.db();
    const sensorsRef = collection(db, `users/${currentUserId}/sensors`);
    const sensorDoc = doc(sensorsRef, sensor.id);
    await updateDoc(sensorDoc, {
      name: sensor.name,
      topicTriggerIrrigation: sensor.topicTriggerIrrigation,
      topicCurrentHumidity: sensor.topicCurrentHumidity,
      topicIrrigationStatus: sensor.topicIrrigationStatus,
    });
  };

  const deleteSensor = async (sensor: Sensor) => {
    const currentUserId = authStore.currentUser?.id || null;
    if (currentUserId === null) return;

    const db = firebase.db();
    const sensorsRef = collection(db, `users/${currentUserId}/sensors`);
    const sensorDoc = doc(sensorsRef, sensor.id);
    await deleteDoc(sensorDoc);
  };

  return {
    showDialog,
    createSensor,
    updateSensor,
    deleteSensor,
    currentEditingSensor,
    isEditingSensor,
  };
});
