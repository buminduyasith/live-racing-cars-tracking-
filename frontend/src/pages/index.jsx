import Head from 'next/head'
import Image from 'next/image'
import { Inter } from 'next/font/google'
import styles from '@/styles/Home.module.css'
import Layout from '@/components/layout/layout'

import Map, { NavigationControl, Marker } from 'react-map-gl';
import maplibregl from 'maplibre-gl';
import 'maplibre-gl/dist/maplibre-gl.css';

const inter = Inter({ subsets: ['latin'] })

export default function Home() {
  return (
    <Layout>
      <section className="text-center">
        <div className="container">
          <h1>Live Track Map</h1>
          <div>
            <Map mapLib={maplibregl}
              initialViewState={{
                longitude:  80.54993,
                latitude: 5.94332,
                zoom: 14
              }}
              style={{ width: "100%", height: " calc(100vh - 77px)" }}
              mapStyle="https://api.maptiler.com/maps/streets/style.json?key=6KlK94OorBZ584iRkFjn">

              <NavigationControl position='top-left' />
             {/*  lat, long geo:5.94303,80.54997?z=19 */}
              <Marker longitude={80.54928} latitude={5.94292} color='#212529' />
              <Marker longitude={80.54997} latitude={5.94303} color='#d81d61' />
              <Marker longitude={80.54997} latitude={5.94483} color='#ffd54f' />
              </Map>
          </div>
        </div>
      </section>
    </Layout>
  )
}
