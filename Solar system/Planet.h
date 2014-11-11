#pragma once

#include "ResourceIdentifiers.h"
#include "SpaceObject.h"

class Planet : public SpaceObject
{
public:
  Planet( const std::string& name, float mass, float radius,
          const TextureHolder& textureHolder,
          const CoordinateSystemHandler& csHandler );
  ~Planet();

  const std::string& getName() const { return name_; }

  void updateGraphics();
  
  void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

private:

  // Maps name to texture ID
  Textures::ID toTextureID_() const;

  std::string name_;
  sf::Sprite sprite_;
};

