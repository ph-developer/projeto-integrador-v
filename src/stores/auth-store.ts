import { defineStore } from 'pinia';
import { ref } from 'vue';
import { useGlobalStore } from 'stores/global-store';
import { useFirebase } from 'src/boot/firebase';
import {
  collection,
  getDocs,
  query,
  where,
} from 'firebase/firestore';
import { encrypt } from 'src/helpers/crypto';
import { useQuasar } from 'quasar';
import { User } from './types/user';

export const useAuthStore = defineStore('auth', () => {
  const $q = useQuasar();
  const firebase = useFirebase();
  const globalStore = useGlobalStore();

  const currentUser = ref<User | null>(null);

  const tryLoginWithAccessToken = async () => {
    const accessToken = $q.localStorage.getItem('__at');
    if (accessToken === null || typeof accessToken !== 'string') return;

    const db = firebase.db();
    const usersRef = collection(db, 'users');
    const q = query(
      usersRef,
      where('accessToken', '==', accessToken),
    );
    const snapshot = await getDocs(q);

    if (snapshot.empty) {
      $q.localStorage.remove('__at');
      return;
    }

    const doc = snapshot.docs[0];
    currentUser.value = {
      id: doc.id,
      username: doc.data().username,
    };
  };

  const doLogin = async (username: string, password: string) => {
    const db = firebase.db();
    const usersRef = collection(db, 'users');
    const q = query(
      usersRef,
      where('username', '==', username),
      where('password', '==', await encrypt(password)),
    );
    const snapshot = await getDocs(q);

    if (snapshot.empty) {
      globalStore.notifyError('Credenciais inválidas.');
      $q.localStorage.remove('__at');
      currentUser.value = null;
      return;
    }

    const doc = snapshot.docs[0];
    currentUser.value = {
      id: doc.id,
      username: doc.data().username,
    };
    $q.localStorage.set('__at', await encrypt(`${username}::${password}`));
  };

  const doLogout = () => {
    $q.localStorage.remove('__at');
    currentUser.value = null;
  };

  return {
    currentUser,
    tryLoginWithAccessToken,
    doLogin,
    doLogout,
  };
});
