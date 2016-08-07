#include "stdafx.h"
#include "Planet.h"

#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"

#include "Utilities.h"

using namespace std;

Planet::Planet( const std::string& name, float mass, float radius,
                int hitPoints,
                const TextureHolder& textureHolder,
                const CoordinateSystemHandler& csHandler ):
  GameEntity( mass, hitPoints, csHandler ),
  name_( name ),
  sprite_( textureHolder.get( toTextureID_()) )
{
  // TODO, consider making utility function as well
  const float scaleX = radius * csHandler_.unitOfLength2Pixel() * 2.0f / sprite_.getTextureRect().width;
  const float scaleY = radius * csHandler_.unitOfLength2Pixel() * 2.0f / sprite_.getTextureRect().height;
  transformable_.setScale( scaleX, scaleY );

  Utilities::centerOrigin( sprite_ );
}

Planet::~Planet()
{}

Textures::ID Planet::toTextureID_() const
{
  if( name_ == "Mercury" )
  {
    return Textures::ID::Mercury;
  }
  else if( name_ == "Venus" )
  {
    return Textures::ID::Venus;
  }
  else if( name_ == "Earth" )
  {
    return Textures::ID::Earth;
  }
  else if( name_ == "Mars" )
  {
    return Textures::ID::Mars;
  }
  else if( name_ == "Jupiter" )
  {
    return Textures::ID::Jupiter;
  }
  else if( name_ == "Saturn" )
  {
    return Textures::ID::Saturn;
  }
  else if( name_ == "Uranus" )
  {
    return Textures::ID::Uranus;
  }
  else if( name_ == "Neptune" )
  {
    return Textures::ID::Neptune;
  }
  else if( name_ == "Sun" )
  {
    return Textures::ID::Sun;
  }
  else
  {
    throw invalid_argument( "Missing texture " + name_ );
    return Textures::ID::Sun;
  }
}

void Planet::drawCurrent_(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(sprite_, states);
}

void Planet::updateCurrentGraphics_()
{
  auto positionInDisplayCS = csHandler_.convertToDisplayCS( position_ );
  transformable_.setPosition( positionInDisplayCS(0), positionInDisplayCS(1) );
}