#pragma once
#include <array>
#include "PhysicalData.h"

class PhysicalObject;

class PhysicsTest
{
public:

  //PhysicsTest();

  // Tests that the total energy remains constant for 1000 turns
  // Fails for forward Euler
  // Succeeds for RK4
  void testStabilityWRTTotalEnergy();

  void aLotOfPlanets();

  void testPerformance();

private:

};