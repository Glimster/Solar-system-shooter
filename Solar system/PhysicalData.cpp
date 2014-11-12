#include "stdafx.h"
#include "PhysicalData.h"

#include "Physics.h"
#include "PhysicalConstants.h"

using namespace std;

void PhysicalData::setupSolarSystem( vector< PlanetData >& planetData )
{
  {
    PlanetData data;
    data.name = "Sun";
    data.mass = 1.0f; // Solar mass
    data.radius = 0.1f; // au
    const float speed = 0.03f; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 0.1f; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    //data.velocity = Physics::circularOrbitVelocity( data.position, 1.0f, data.mass );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Sun";
    data.mass = 1.0f; // Solar mass
    data.radius = 0.1f; // au
    const float speed = 0.03f; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, -speed );
    const float distance = 0.1f; // au
    data.position = Eigen::Vector2f( distance, 0.0f );
    //data.velocity = Physics::circularOrbitVelocity( data.position, 1.0f, data.mass );
    planetData.push_back( data );
  }

  {
    PlanetData data;
    data.name = "Mercury";
    data.mass = 1e-7f; // Solar mass
    data.radius = 0.05f; // au
    const float distance = 0.7f; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    data.velocity = Physics::circularOrbitVelocity( data.position, 2.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Venus";
    data.mass = 5e-7f; // Solar mass
    data.radius = 0.08f; // au
    const float distance = 1.0f; // au
    data.position = Eigen::Vector2f( 0.0f, distance );
    data.velocity = Physics::circularOrbitVelocity( data.position, 2.0f );

    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Earth";
    data.mass = 1e-6f; // Solar mass
    data.radius = 0.1f; // au
    const float distance = 1.5f; // au
    data.position = Eigen::Vector2f( distance, 0.0f );
    data.velocity = Physics::circularOrbitVelocity( data.position, 2.0f );
    planetData.push_back( data );
  }
}

void PhysicalData::setupRealisticSolarSystem( vector< PlanetData >& planetData )
{
  // Data fr�n http://nssdc.gsfc.nasa.gov/planetary/factsheet/
  {
    PlanetData data;
    data.name = "Mercury";
    data.mass = 0.330e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 4.879e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 47.9e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 57.9e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Venus";
    data.mass = 4.87e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 12.104e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 35.0e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 108.2e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Earth";
    data.mass = 5.97e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 12.756e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 29.8e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 149.6e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Mars";
    data.mass = 0.642e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 6.792e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 24.1e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 227.9e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Jupiter";
    data.mass = 1898e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 142.984e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 13.1e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 778.6e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Saturn";
    data.mass = 568e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 120.536e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 9.7e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 1433.5e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Uranus";
    data.mass = 86.8e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 51.118e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 6.8e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 2872.5e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Neptune";
    data.mass = 102.0e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 49.528e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 1000.0f; // S� att n�nting syns
    const float speed = 5.4e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 4495.1e9f / Phys::PhysicalConstants::au2m; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
  {
    PlanetData data;
    data.name = "Sun";
    data.mass = 1.989e30f / Phys::PhysicalConstants::S2kg; // Solar mass
    data.radius = 6.955e8f / Phys::PhysicalConstants::au2m; // au
    data.radius *= 10.0f; // S� att n�nting syns
    const float speed = 0.0f; // au / D
    data.velocity = Eigen::Vector2f( 0.0f, speed );
    const float distance = 0.0f; // au
    data.position = Eigen::Vector2f( -distance, 0.0f );
    planetData.push_back( data );
  }
}