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

  static void setupSolarSystem( std::vector< PlanetData >& planetData );
  static void setupRealisticSolarSystem( std::vector< PlanetData >& planetData );
};

