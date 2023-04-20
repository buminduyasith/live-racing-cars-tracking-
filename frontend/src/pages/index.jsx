import { Inter } from 'next/font/google'
import Layout from '@/components/layout/layout'
import { GoogleMap, useLoadScript, Marker } from '@react-google-maps/api'

const inter = Inter({ subsets: ['latin'] })

export default function Home() {

  const { isLoaded } = useLoadScript({
    googleMapsApiKey: ''
  })


  if (!isLoaded) return <div>Loading</div>

  return (
    <Layout>
      <section className="text-center">
        <div className="container">
          <h1>Live Track Map</h1>
          <Map />
        </div>
      </section>
    </Layout>
  )
}

function Map() {
  return <GoogleMap zoom={19} center={{ lat: 6.0329, lng: 80.2168 }} mapContainerClassName='map-container'>
    <Marker position={{ lat: 6.0329, lng: 80.2168 }} />
    <Marker position={{ lat: 6.0429, lng: 80.2268 }} />
  </GoogleMap>
}
