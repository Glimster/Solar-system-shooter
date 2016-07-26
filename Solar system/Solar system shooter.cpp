// Solar system shooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"

#include "PhysicsTest.h"
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
      //testing::InitGoogleTest( &argc, argv );
      //return RUN_ALL_TESTS();

      PhysicsTest test( false );
      test.keplersLawsForHeavySun();
      //test.testStabilityWRTTotalEnergy();

      //PhysicsTest test( true );
      //test.testGUI();
    }
  }
  catch( exception& e )
  {
    cout << "\nEXCEPTION: " << e.what() << endl;
  }
}