#pragma once
#include <array>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "CoordinateSystemHandler.h"
#include "SceneNode.h"

class SpaceObject;
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
  enum Layer
  {
    Background,
    ActionLayer,
    LayerCount
  };

  void handlePlayerInput_( sf::Keyboard::Key key, bool isPressed ); // TODO, ska den här ligga nån annan stans?

  void loadTextures_();
  void buildScene_();

  sf::RenderWindow& mainWindow_;
  CoordinateSystemHandler csHandler_;
  TextureHolder textureHolder_;
  sf::View playerView_;

  // sceneGraph_ owns all SceneNodes (i.e. SpaceObjects) as children (and grandchildren etc.)
  // sceneLayers keeps pointers to the top SceneNode for each layer
  // NB, layerCount gets its number from its place (which should be last) in the enum
  std::array< SceneNode*, LayerCount > sceneLayers_;
  SceneNode sceneGraph_;

  std::vector< SpaceObject* > spaceObjects_; // Should contain all space objects
  StarShip* player_;
};

