import { initializeApp } from "firebase/app";

const firebaseConfig = {

  apiKey: "AIzaSyAVR46UnJjn7KGKs8TNIDXzdcOiUMJ2b2M",
  authDomain: "ipccw-98afd.firebaseapp.com",
  projectId: "ipccw-98afd",
  storageBucket: "ipccw-98afd.appspot.com",
  messagingSenderId: "830852744265",
  appId: "1:830852744265:web:d0083cdb6783c821d9ab98",
  measurementId: "G-0J2XQNFWJ3"

};


// Initialize Firebase

const app = initializeApp(firebaseConfig);

export default app;