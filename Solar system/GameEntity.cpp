#include "stdafx.h"
#include "GameEntity.h"


GameEntity::GameEntity( float mass, const CoordinateSystemHandler& csHandler ):
  PhysicalObject( mass ),
  csHandler_( csHandler )
{}

GameEntity::~GameEntity()
{}
