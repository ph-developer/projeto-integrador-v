import { defineStore } from 'pinia';
import { computed, ref } from 'vue';
import { useMqttStore } from 'stores/mqtt-store';
import { useAuthStore } from 'stores/auth-store';
import { useFirebase } from 'boot/firebase';
import {
  collection, getDocs, orderBy, query,
} from 'firebase/firestore';
import { Sensor } from './types/sensor';

export const useDashStore = defineStore('dash', () => {
  const firebase = useFirebase();
  const mqttStore = useMqttStore();
  const authStore = useAuthStore();

  const isLoading = ref(false);
  const sensors = ref<Sensor[]>([]);
  const hasSensors = computed(() => sensors.value.length > 0);

  const isConnectingSensor = computed(() => !mqttStore.mqttIsConnected);
  const currentSensorIndex = ref(0);
  const currentSensor = computed<Sensor | null>(
    () => sensors.value[currentSensorIndex.value] || null,
  );

  const connectCurrentSensor = async () => {
    await mqttStore.disconnectMqtt();
    if (currentSensor.value != null) await mqttStore.connectMqtt(currentSensor.value);
  };

  const loadSensors = async () => {
    isLoading.value = true;
    sensors.value = [];

    const currentUserId = authStore.currentUser?.id || null;
    if (currentUserId === null) return;

    const db = firebase.db();
    const sensorsRef = collection(db, `users/${currentUserId}/sensors`);
    const q = query(sensorsRef, orderBy('name'));
    const snapshot = await getDocs(q);

    if (!snapshot.empty) {
      sensors.value = snapshot.docs.map((doc) => ({
        id: doc.id,
        name: doc.data().name,
        topicTriggerIrrigation: doc.data().topicTriggerIrrigation,
        topicCurrentHumidity: doc.data().topicCurrentHumidity,
        topicIrrigationStatus: doc.data().topicIrrigationStatus,
      }));
    }

    if (currentSensorIndex.value > sensors.value.length - 1) {
      currentSensorIndex.value = sensors.value.length - 1;
    }

    isLoading.value = false;
  };

  const hasPreviousSensor = computed(
    () => currentSensorIndex.value > 0,
  );
  const goToPreviousSensor = () => {
    if (hasPreviousSensor.value) {
      currentSensorIndex.value -= 1;
      connectCurrentSensor();
    }
  };
  const hasNextSensor = computed(
    () => sensors.value !== null && currentSensorIndex.value < sensors.value.length - 1,
  );
  const goToNextSensor = () => {
    if (hasNextSensor.value) {
      currentSensorIndex.value += 1;
      connectCurrentSensor();
    }
  };

  const goToSensorId = (sensorId: string) => {
    if (sensors.value.length > 0) {
      const sensorIndex = sensors.value.findIndex(
        (sensor) => sensor.id === sensorId,
      );
      if (sensorIndex >= 0) currentSensorIndex.value = sensorIndex;
      connectCurrentSensor();
    }
  };

  const currentHumidity = computed(() => mqttStore.currentHumidity);
  const irrigationStatus = computed(() => mqttStore.irrigationStatus);
  const triggerIrrigation = async () => {
    if (currentSensor.value != null) {
      await mqttStore.triggerIrrigation(currentSensor.value);
    }
  };

  return {
    hasSensors,
    currentSensor,
    isLoading,
    isConnectingSensor,
    loadSensors,
    hasPreviousSensor,
    goToPreviousSensor,
    hasNextSensor,
    goToNextSensor,
    goToSensorId,
    currentHumidity,
    irrigationStatus,
    triggerIrrigation,
    connectCurrentSensor,
  };
});
