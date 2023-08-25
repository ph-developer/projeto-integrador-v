<script setup lang="ts">
import { useAuthStore } from 'src/stores/auth-store';
import { computed, watch } from 'vue';
import { useGlobalStore } from 'stores/global-store';
import { useQuasar } from 'quasar';
import AuthPage from './AuthPage.vue';
import DashPage from './DashPage.vue';

const $q = useQuasar();
const globalStore = useGlobalStore();
const authStore = useAuthStore();

const mensagemErro = computed(() => globalStore.mensagemErro);

watch(mensagemErro, (mensagem) => {
  if (mensagem != null) {
    $q.notify({
      type: 'negative',
      message: mensagem,
      progress: true,
    });
  }
});

const autenticado = computed(() => authStore.autenticado);
</script>

<template>
  <q-layout view="hHh lpR fFf">
    <q-page-container>
      <DashPage v-if="autenticado" />
      <AuthPage v-else />
    </q-page-container>
  </q-layout>
</template>
