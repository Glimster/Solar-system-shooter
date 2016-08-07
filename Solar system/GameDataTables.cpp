#include "stdafx.h"
#include "GameDataTables.h"

#include "StarShip.h"

using namespace std;

const vector< StarShipData > setupStarShipData()
{
  vector< StarShipData > data( StarShip::TypeCount );
  data[StarShip::Standard].hitPoints = 100;
  data[StarShip::Standard].mass = 1e-10f; // Solar masses
  data[StarShip::Standard].length = 0.17f; // a.u.
  data[StarShip::Standard].texture = Textures::ID::StarShip;

  return data;
}