#include "stdafx.h"
#include "SpriteNode.h"

SpriteNode::SpriteNode( const sf::Texture& texture, const sf::Vector2f scaleFactor ):
  sprite_( texture )
{
  sf::FloatRect origin = sprite_.getLocalBounds();
  sprite_.setOrigin( origin.width / 2.0f, origin.height / 2.0f );

  sprite_.setScale( scaleFactor );
}

SpriteNode::SpriteNode( const sf::Texture& texture, 
                        const sf::IntRect& textureRect, 
                        const sf::Vector2f scaleFactor ):
  sprite_( texture, textureRect )
{
  sf::FloatRect origin = sprite_.getLocalBounds();
  sprite_.setOrigin( origin.width / 2.0f, origin.height / 2.0f );

  sprite_.setScale( scaleFactor );
}

void SpriteNode::drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const
{
  states.transform *= getTransform();

  target.draw( sprite_, states );
}