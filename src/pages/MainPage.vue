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

const errorMessage = computed(() => globalStore.errorMessage);

watch(errorMessage, (message) => {
  if (message !== null) {
    $q.notify({
      type: 'negative',
      message,
      progress: true,
      timeout: 3000,
    });
  }
});

const isLoggedIn = computed(() => authStore.currentUser !== null);
</script>

<template>
  <q-layout view="hHh lpR fFf">
    <q-page-container>
      <DashPage v-if="isLoggedIn"/>
      <AuthPage v-else/>
    </q-page-container>
  </q-layout>
</template>
