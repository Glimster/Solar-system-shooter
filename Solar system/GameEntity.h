#pragma once

#include "PhysicalObject.h"
#include "SceneNode.h"

class CoordinateSystemHandler;

class GameEntity : public PhysicalObject, public SceneNode
{
public:
  GameEntity( float mass, const CoordinateSystemHandler& csHandler );
  ~GameEntity();

  // Game mechanics
  //void damage( int points ) { hitPoints_ -= points; };
  //void repair( int points ) { hitPoints_ += points; };
  //void destroy() { hitPoints_ = 0; };

  //int getHitPoints() const { return hitPoints_; };
  //bool isDestroyed() const { return hitPoints_ <= 0; };

protected:
  const CoordinateSystemHandler& csHandler_;
};

