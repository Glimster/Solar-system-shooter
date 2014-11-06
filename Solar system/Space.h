#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "CoordinateSystemHandler.h"

class SpaceObject;
class Planet;
class StarShip;

class Space : sf::NonCopyable
{
public:
  Space( sf::RenderWindow& mainWindow );
  ~Space();

  void processEvents();
  void update( const sf::Time& timeStep );
  void render();

  // TODO, remove, for debug!
  const std::vector< SpaceObject* >& getSpaceObjects() const { return spaceObjects_; } 

private:

  void handlePlayerInput_( sf::Keyboard::Key key, bool isPressed ); // TODO, ska den här ligga nån annan stans?

  void loadTextures_();
  void setupPlanets_();

  sf::RenderWindow& mainWindow_;
  CoordinateSystemHandler csHandler_;
  TextureHolder textureHolder_;

  sf::View playerView_;

  std::vector< SpaceObject* > spaceObjects_; // Should contain all space objects

  std::vector< std::unique_ptr< Planet > > planets_;
  std::unique_ptr< StarShip > player_;
};

