import { boot } from 'quasar/wrappers';
import { FirebaseApp, initializeApp } from 'firebase/app';
import { getFirestore, Firestore } from 'firebase/firestore';

const firebaseConfig = {
  apiKey: 'AIzaSyBw7NqROtcW4Z_ly46GUK4Yvi-pAy2ijZc',
  authDomain: 'irrigacao-pi-v.firebaseapp.com',
  projectId: 'irrigacao-pi-v',
  storageBucket: 'irrigacao-pi-v.appspot.com',
  messagingSenderId: '943359154388',
  appId: '1:943359154388:web:824a7ec2f9896d5f210341',
};

let app: FirebaseApp;
let db: Firestore;

export default boot(() => {
  app = initializeApp(firebaseConfig);
  db = getFirestore(app);
});

export const useFirebase = () => ({
  app: () => app,
  db: () => db,
});
