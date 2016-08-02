#pragma once

#include "SceneNode.h"

// TODO, har lagt till Transformable här för att få position etc. Ska det bort härifrån, 
// eller läggas till i SceneNode, alltså för SpaceObject etc. etc. ?
// De har ju sin egen state-hantering som oftast är mer avancerad... Kanske är bra som det är nu!?
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