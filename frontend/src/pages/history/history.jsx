import Layout from "@/components/layout/layout";
import Table from 'react-bootstrap/Table';
import React, { useState, useEffect } from 'react'
import db from '../../services/firebase'
import { onSnapshot, collection, query } from "firebase/firestore";


export default function History() {

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
      <div className="row">
        <section className="text-center">
          <div className="container">
            <h1>History Form</h1>
          </div>
        </section>

      </div>
      <div className="row">
        <Table striped bordered hover variant="dark">
          <thead>
            <tr>
              <th>#</th>
              <th>Latitude</th>
              <th>Longitude</th>
              <th>Speed</th>
            </tr>
          </thead>
          <tbody>
            {cars?.map((item, index) => (
              <tr>
                <td>{++index}</td>
                <td>{item.latitude}</td>
                <td>{item.longitude}</td>
                <td>{item.speed}</td>
              </tr>
            ))}
          </tbody>
        </Table>
      </div>
    </Layout>
  )
}