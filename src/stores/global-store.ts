import { defineStore } from 'pinia';
import { ref } from 'vue';

export const useGlobalStore = defineStore('global', () => {
  const errorMessage = ref<string|null>(null);

  const notifyError = (message: string) => {
    errorMessage.value = message;
    setTimeout(() => {
      errorMessage.value = null;
    }, 100);
  };

  return {
    errorMessage,
    notifyError,
  };
});
