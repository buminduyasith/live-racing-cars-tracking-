
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import { getFirestore} from "firebase/firestore";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
/* const firebaseConfig = {
  apiKey: "AIzaSyCOZnnAX_Bxbn7Gvga8ZUZiXhrFthrdr60",
  authDomain: "ipccw-98afd.firebaseapp.com",
  databaseURL: "https://ipccw-98afd-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "ipccw-98afd",
  storageBucket: "ipccw-98afd.appspot.com",
  messagingSenderId: "830852744265",
  appId: "1:830852744265:web:8a2e7708f837649ad9ab98",
  measurementId: "G-P0YTNEDNLQ"
};
 */
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
  /* const analytics = getAnalytics(app); */
  
  const db = getFirestore(app);
  // Initialize Firebase
  
  export default db;