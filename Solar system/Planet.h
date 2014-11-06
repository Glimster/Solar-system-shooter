#pragma once

#include "ResourceIdentifiers.h"
#include "SpaceObject.h"

class Planet : public SpaceObject
{
public:
  Planet( const std::string& name, float mass, float radius,
          Textures::ID textureID, // TODO, perhaps not so nice?
          const TextureHolder& textureHolder,
          const CoordinateSystemHandler& csHandler );
  ~Planet();

  const std::string& getName() const { return name_; }

  void updateGraphics();
  void render( sf::RenderWindow& renderWindow ) const;

private:

  std::string name_;
  sf::Sprite sprite_;
};

