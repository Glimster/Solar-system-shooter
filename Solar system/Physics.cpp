#include "stdafx.h"
#include "Physics.h"

using namespace std;
using namespace Phys;
float Physics::computeRotationalInertiaForRectangle( float mass, float a, float b )
{
  return mass * ( a * a + b * b ) / 12.0f;
}

Eigen::Vector2f Physics::circularOrbitVelocity( const Eigen::Vector2f& relPos, float centralMass )
{
  const float r = sqrt( relPos.dot( relPos ) );
  float speed = sqrt( PhysicalConstants::kSquared * centralMass / r );
  Eigen::Vector2f velocity( relPos(1), -relPos(0) );
  velocity.normalize();
  return velocity * speed;
}

float Physics::escapeVelocity( const Eigen::Vector2f& relPos, float centralMass )
{
  const float r = sqrt( relPos.dot( relPos ) );
  return sqrt( 2.0f * PhysicalConstants::kSquared * centralMass / r );
}