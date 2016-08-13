#pragma once

#include "ResourceIdentifiers.h"
#include "GameEntity.h"

class Planet : public GameEntity
{
public:
  Planet( const std::string& name, float mass, float radius,
          int hitPoints, // TODO, ska planeter ha hit points?
          const TextureHolder& textureHolder,
          const CoordinateSystemHandler& csHandler );
  ~Planet();

  const std::string& getName() const { return name_; }

  virtual unsigned int getCategory() const override;
private:
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const override;
  virtual void updateCurrent_( sf::Time dt, CommandQueue& commands ) override;
  virtual void printCurrent_( std::string& string ) const override;

  // Maps name to texture ID
  Textures::ID toTextureID_() const;

  std::string name_;
  sf::Sprite sprite_;
};

