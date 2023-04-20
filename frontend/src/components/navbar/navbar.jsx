import React from 'react'
import { Nav } from 'react-bootstrap';
import Link from 'next/link'

export default function Navbar() {
  return (
    <Nav className="navbar navbar-expand-lg navbar-dark bg-dark">
  <div className="container-xl">
    <Link legacyBehavior href="/">
      <a className="navbar-brand">F1 Tracking System</a>
    </Link>
    <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarsExample07XL" aria-controls="navbarsExample07XL" aria-expanded="false" aria-label="Toggle navigation">
      <span className="navbar-toggler-icon"></span>
    </button>

    <div className="collapse navbar-collapse" id="navbarsExample07XL">
      <ul className="navbar-nav mr-auto">
        <li className="nav-item active">
          <Link legacyBehavior href="/">
            <a className="nav-link">Track Live</a>
          </Link>
        </li>
        <li className="nav-item">
          <Link legacyBehavior href="/history/history">
            <a className="nav-link">History</a>
          </Link>
        </li>
      </ul>
    </div>
  </div>
</Nav>
  )
}