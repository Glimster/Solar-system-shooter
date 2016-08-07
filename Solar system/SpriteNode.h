#pragma once

#include "SceneNode.h"

class SpriteNode : public SceneNode
{
public:
  explicit SpriteNode( const sf::Texture& texture, const sf::Vector2f& scaleFactor );
  SpriteNode( const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& scaleFactor );

  inline void setPosition( float x, float y ) { transformable_.setPosition( x, y ); }

private:
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
  sf::Sprite sprite_;

};