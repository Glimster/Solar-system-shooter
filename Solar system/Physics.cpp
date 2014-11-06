#include "stdafx.h"
#include "Physics.h"

float Physics::computeRotationalInertiaForRectangle( float mass, float a, float b )
{
  return mass * ( a * a + b * b ) / 12.0f;
}