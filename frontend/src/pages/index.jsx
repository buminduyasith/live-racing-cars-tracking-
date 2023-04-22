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
                longitude:  13.084622,
                latitude: 80.248357,
                zoom: 14
              }}
              style={{ width: "100%", height: " calc(100vh - 77px)" }}
              mapStyle="https://api.maptiler.com/maps/streets/style.json?key=6KlK94OorBZ584iRkFjn">

              <NavigationControl position='top-left' />
              <Marker longitude={13.084622} latitude={80.208357} color='#212529' />
              <Marker longitude={11.084622} latitude={80.208357} color='#212529' />
              <Marker longitude={10.084622} latitude={80.208357} color='#212529' />
              </Map>
          </div>
        </div>
      </section>
    </Layout>
  )
}
