// Solar system shooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"

#include "PhysicsTest.h"
#include "GUITests.h"
#include <gtest/gtest.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  bool runGame = true;
  try {
    if( runGame )
    {
      Game game;
      game.run();
    }
    else
    {
#if 1
      testing::InitGoogleTest( &argc, argv );
      return RUN_ALL_TESTS(); 
#endif
#if 0
      PhysicsTest test;
      //test.aLotOfPlanets();
      //test.testPerformance();
      //test.testSimpleCollision();
#else
      GUITests test;
      //test.init( GUITests::Setup::Space );
      //test.runSpaceSimulation();
      test.init( GUITests::Setup::RollingBalls );
      test.runCollisionSimulation();
#endif
    }
  }
  catch( exception& e )
  {
    cout << "\nEXCEPTION: " << e.what() << endl;
  }
}