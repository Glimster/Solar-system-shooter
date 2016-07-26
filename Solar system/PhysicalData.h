#pragma once

class PhysicalData
{
public:

  struct PlanetData
  {
    std::string name;
    float mass;
    float radius;
    Eigen::Vector2f velocity;
    Eigen::Vector2f position;
  };

  static void setupHeavySunLightPlanet( std::vector< PlanetData >& planetData );
  static void setupTwoBodySystem( std::vector< PlanetData >& planetData );
  static void setupThreeBodySystem( std::vector< PlanetData >& planetData );
  static void setupPlanetarySystem( std::vector< PlanetData >& planetData );
  static void setupRealisticSolarSystem( std::vector< PlanetData >& planetData );

  static void setupAlotOfPlanets( std::vector< PlanetData >& planetData );
};

