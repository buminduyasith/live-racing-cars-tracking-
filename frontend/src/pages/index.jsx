import Head from 'next/head'
import Image from 'next/image'
import { Inter } from 'next/font/google'
import styles from '@/styles/Home.module.css'
import Layout from '@/components/layout/layout'

import Map, { NavigationControl } from 'react-map-gl';
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
                longitude:  5.947824,
                latitude: 80.5483,
                zoom: 10
              }}
              style={{ width: "100%", height: " calc(100vh - 77px)" }}
              mapStyle="https://api.maptiler.com/maps/streets/style.json?key=6KlK94OorBZ584iRkFjn"
            />
          </div>
        </div>
      </section>
    </Layout>
  )
}
