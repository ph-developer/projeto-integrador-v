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
import { User } from './types/user';

export const useAuthStore = defineStore('auth', () => {
  const firebase = useFirebase();
  const globalStore = useGlobalStore();

  const currentUser = ref<User | null>(null);

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
      currentUser.value = null;
      return;
    }

    const doc = snapshot.docs[0];
    currentUser.value = {
      id: doc.id,
      username: doc.data().username,
    };
  };

  const doLogout = () => {
    currentUser.value = null;
  };

  return {
    currentUser,
    doLogin,
    doLogout,
  };
});
