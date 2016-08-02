#pragma once
#include <array>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "CoordinateSystemHandler.h"
#include "PhysicalData.h"

class SpaceObject;

class PhysicsTest
{
public:

  PhysicsTest( bool createGUI );

  // Tests that a (light) planet orbiting around a (heavy) sun follows Kepler's
  // laws of planetary motion
  // https://en.wikipedia.org/wiki/Kepler%27s_laws_of_planetary_motion
  //
  // Fails for forward Euler
  // Succeeds for RK4
  void keplersLawsForHeavySun();

  // Tests that the total energy remains constant for 1000 turns
  // Fails for forward Euler
  // Succeeds for RK4
  void testStabilityWRTTotalEnergy();

  void testGUI();

private:
  enum Layer
  {
    Background,
    ActionLayer,
    LayerCount
  };

  void buildScene_( const std::vector< PhysicalData::PlanetData >& data );

  void render_();

  void loadTextures_();
  void buildScene_();

  // TODO, stoppa den här på lämpligt ställe!
  static void PVAR( Eigen::Vector2f v, std::string str )
  {
    std::cout << str + " = (" << v( 0 ) << ", " << v( 1 ) << ")" << std::endl;
  }

  sf::RenderWindow mainWindow_;
  TextureHolder textureHolder_;
  sf::View view_;
  CoordinateSystemHandler csHandler_;

  // sceneGraph_ owns all SceneNodes (i.e. SpaceObjects) as children (and grandchildren etc.)
  // sceneLayers keeps pointers to the top SceneNode for each layer
  // NB, layerCount gets its number from its place (which should be last) in the enum
  std::array< SceneNode*, LayerCount > sceneLayers_;
  SceneNode sceneGraph_;

  std::vector< SpaceObject* > spaceObjects_;
};