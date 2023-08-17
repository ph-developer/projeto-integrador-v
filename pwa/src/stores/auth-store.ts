import { defineStore } from 'pinia';
import { ref } from 'vue';
import { useGlobalStore } from 'stores/global-store';

export const useAuthStore = defineStore('auth', () => {
  const globalStore = useGlobalStore();

  const autenticado = ref(false);

  const autenticar = (senha: string) => {
    if (senha === '001004') {
      autenticado.value = true;
    } else {
      globalStore.notificarErro('Senha inválida.');
      autenticado.value = false;
    }
  };

  return {
    autenticado,
    autenticar,
  };
});
