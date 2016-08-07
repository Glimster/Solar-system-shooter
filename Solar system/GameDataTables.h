#pragma once
#include "ResourceIdentifiers.h"

struct StarShipData
{
  float mass;
  float length;
  int hitPoints;
  Textures::ID texture;
};

const std::vector< StarShipData > setupStarShipData();
