import { defineStore } from 'pinia';
import { ref } from 'vue';

export const useGlobalStore = defineStore('global', () => {
  const mensagemErro = ref<string|null>(null);

  const notificarErro = (mensagem: string) => {
    mensagemErro.value = mensagem;
    setTimeout(() => {
      mensagemErro.value = null;
    }, 100);
  };

  return {
    mensagemErro,
    notificarErro,
  };
});
