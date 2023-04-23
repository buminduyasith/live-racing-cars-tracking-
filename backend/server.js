const express = require('express');
const dotenv = require('dotenv').config();
const { db } = require("./firebasconfig");
const { doc, setDoc } = require("firebase/firestore");

const app = express();
const port = process.env.PORT || 5000;


app.use(express.json());

app.get('/', async (req, res) => {

    console.log("request processing 1")
  
    console.log(req.query)

    if(req.query.DeviceId == null){
        return res.sendStatus(400);
    }

    await setDoc(doc(db, "cars", req?.query?.DeviceId), {
        latitude: req?.query?.latitude,
        longitude: req?.query?.longitude,
        speed: req?.query?.speed,
        time: req?.query?.time,
        date: req?.query?.date,
        deviceSecret:req?.query?.DeviceSecret
    });

    return res.sendStatus(200);

});


app.listen(port, () => {

    console.log(`server running on port ${port}`);
});