import Head from 'next/head'
import Image from 'next/image'
import { Inter } from 'next/font/google'
import styles from '@/styles/Home.module.css'
import Layout from '@/components/layout/layout'

import Map, { NavigationControl, Marker } from 'react-map-gl';
import maplibregl from 'maplibre-gl';
import 'maplibre-gl/dist/maplibre-gl.css';
import React, { useState, useEffect } from 'react'
import db from '../services/firebase'
import { setDoc, doc, onSnapshot, collection, query } from "firebase/firestore";

const inter = Inter({ subsets: ['latin'] })

export default function Home() {

  var [cars, setCars] = useState([]);
  useEffect(() => {
    var carlist = [];
    const q = query(collection(db, "cars"));
    const unsubscribe = onSnapshot(q, (querySnapshot) => {
      carlist = []
      querySnapshot.forEach((doc) => {
        var data = doc.data()
        carlist.push(data)
        console.log(data)
      });
      console.log("carlist", carlist)
      setCars(carlist);
    });
  }, [])

  return (
    <Layout>
      <section className="text-center">
        <div className="container">
          <h1>Live Track Map</h1>
          <div>
          </div>
          <Map mapLib={maplibregl}
            initialViewState={{
              longitude: 80.54993,
              latitude: 5.94332,
              zoom: 14
            }}
            style={{ width: "100%", height: " calc(100vh - 77px)" }}
            mapStyle="https://api.maptiler.com/maps/streets/style.json?key=6KlK94OorBZ584iRkFjn">

            <NavigationControl position='top-left' />
            {/*  lat, long geo:5.94303,80.54997?z=19 */}
            {cars?.map((item, index) => (
               <Marker longitude={item.longitude} latitude={item.latitude} color='#212529' key={index} />
            ))}
          </Map>
        </div>
      </section>
    </Layout>
  )
}
