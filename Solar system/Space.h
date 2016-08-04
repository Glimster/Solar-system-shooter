#pragma once
#include <array>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "CoordinateSystemHandler.h"
#include "SceneNode.h"
#include "CommandQueue.h"
#include "MotionManager.h"
#include "PhysicalData.h"

class PhysicalObject;
class StarShip;

class Space : sf::NonCopyable
{
public:
  explicit Space( sf::RenderWindow& mainWindow );
  ~Space();

  void processEvents();
  void update( const sf::Time& timeStep );
  void render();

  CommandQueue& getCommandQueue() { return commandQueue_; }

  // TODO, remove, for debug!
  const std::vector< PhysicalObject* >& getPhysicalObjects() const { return physicalObjects_; }

private:
  enum Layer
  {
    Background,
    ActionLayer,
    LayerCount
  };

  void loadTextures_();
  void buildScene_( const std::vector< PhysicalData::PlanetData >& planetarySystemData );

  sf::RenderWindow& mainWindow_;
  TextureHolder textureHolder_;

  Eigen::AlignedBox2f spaceBoundingBox_; // TODO, currently not used
  CoordinateSystemHandler csHandler_;
  sf::View playerView_;

  MotionManager motionManager_;

  CommandQueue commandQueue_;

  // sceneGraph_ owns all SceneNodes (i.e. PhysicalObject's) as children (and grandchildren etc.)
  // sceneLayers keeps pointers to the top SceneNode for each layer
  // NB, layerCount gets its number from its place (which should be last) in the enum
  std::array< SceneNode*, LayerCount > sceneLayers_;
  SceneNode sceneGraph_;

  std::vector< PhysicalObject* > physicalObjects_; // Should contain all space objects
  StarShip* player_;
};

