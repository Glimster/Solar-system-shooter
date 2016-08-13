#pragma once

#include "SceneNode.h"

class SpriteNode : public SceneNode
{
public:
  explicit SpriteNode( const sf::Texture& texture, const sf::Vector2f& scaleFactor );
  SpriteNode( const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& scaleFactor );

  inline void setPositionDisplayCS( float x, float y ) { transformable_.setPosition( x, y ); }
  inline void setScaleDisplayCS( sf::Vector2f scale ) { transformable_.setScale( scale ); }

  const sf::IntRect& getTextureRect() const { return sprite_.getTextureRect(); }

private:
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const override;
  virtual void printCurrent_( std::string& string ) const override;

private:
  sf::Sprite sprite_;

};