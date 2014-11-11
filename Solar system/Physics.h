#pragma once
#include "PhysicalConstants.h"

class Physics
{
public:
 
  // Force of gravity using Gaussian gravitational constant
  //
  // TODO, kan tas bort?
  inline static void forceOfGravity( float m1, float m2, const Eigen::Vector2f& r, Eigen::Vector2f& F ) 
  { 
    const float force = Phys::PhysicalConstants::kSquared * m1 * m2 / r.dot( r );
    
    F = r;
    F.normalize();
    F *= -force;
  }

  // Force of gravity using Gaussian gravitational constant
  inline static void forceOfGravityOverM( float m2, const Eigen::Vector2f& r, Eigen::Vector2f& F ) 
  { 
    const float force = Phys::PhysicalConstants::kSquared * m2 / r.dot( r );
    
    F = r;
    F.normalize();
    F *= -force;
  }

  // Rotation inertia around z at center of rectangle
  static float computeRotationalInertiaForRectangle( float mass, float a, float b );

  // Computes the velocity of a satellite in circular orbit around a large central mass
  // The velocity will be perpendicular to the relative position vector 
  static Eigen::Vector2f circularOrbitVelocity( const Eigen::Vector2f& relPos, float centralMass );
  
  // Computes the escape velocity (really the speed) of a satellite around a large central mass
  static float escapeVelocity( const Eigen::Vector2f& relPos, float centralMass );
};

