#pragma once

#include "SceneNode.h"

// TODO, har lagt till Transformable h�r f�r att f� position etc. Ska det bort h�rifr�n, 
// eller l�ggas till i SceneNode, allts� f�r SpaceObject etc. etc. ?
// De har ju sin egen state-hantering som oftast �r mer avancerad... Kanske �r bra som det �r nu!?
class SpriteNode : public SceneNode, public sf::Transformable
{
public:
  explicit SpriteNode( const sf::Texture& texture, const sf::Vector2f scaleFactor );

  SpriteNode( const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f scaleFactor );

private:
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
  sf::Sprite sprite_;

};