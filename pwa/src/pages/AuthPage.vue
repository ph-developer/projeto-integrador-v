<script setup lang="ts">
import { useAuthStore } from 'src/stores/auth-store';
import { ref } from 'vue';

const authStore = useAuthStore();

const senha = ref('');

const autenticar = () => {
  authStore.autenticar(senha.value);
};
</script>

<template>
  <q-page class="column justify-center text-center">
    <q-img class="self-center q-mb-md" src="../assets/logo.png" height="60px" width="60px" />

    <span class="text-bold font-24">Irrigação Automatizada</span>
    <span class="font-18">Projeto Integrador V</span>
    <span class="font-16">DRP12-PJI510-SALA-001 GRUPO-004</span>

    <form @submit.prevent="autenticar" class="q-mt-md q-mx-auto">
      <q-input outlined v-model="senha" label="Senha" dense stack-label autofocus />

      <q-btn color="primary" type="submit" label="Entrar" class="q-mt-sm" stretch/>
    </form>
  </q-page>
</template>

<style lang="scss" scoped>
.font-24 {
  font-size: 24px;
}
.font-18 {
  font-size: 18px;
}
.font-16 {
  font-size: 16px;
}
.mw-250 {
  max-width: 250px;
}
</style>
