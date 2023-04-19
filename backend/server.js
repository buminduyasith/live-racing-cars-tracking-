const express = require('express');
const dotenv = require('dotenv').config();
const { db } = require("./firebasconfig");
const { doc, setDoc } = require("firebase/firestore");

const app = express();
const port = process.env.PORT || 5000;


app.use(express.json());

app.get('/', async (req, res) => {

    console.log("request processing 1")
  
    await setDoc(doc(db, "cities", "LA"), {
        name: "Los Angeles",
        state: "CA",
        country: "USA"
      });

    return res.status(200).json({
        "done": true
    })
});


app.listen(port, () => {

    console.log(`server running on port ${port}`);
});