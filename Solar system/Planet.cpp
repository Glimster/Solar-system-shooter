#include "stdafx.h"
#include "Planet.h"

#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"

using namespace std;

Planet::Planet( const std::string& name, float mass, float radius,
                Textures::ID textureID, // TODO, perhaps not so nice?
                const TextureHolder& textureHolder,
                const CoordinateSystemHandler& csHandler ):
  SpaceObject( mass, csHandler ),
  name_( name ),
  sprite_()
{
  sprite_.setTexture( textureHolder.get( textureID ) );
    
  const float scaleX = radius * csHandler_.unitOfLength2Pixel() * 2.0f / sprite_.getTextureRect().width;
  const float scaleY = radius * csHandler_.unitOfLength2Pixel() * 2.0f / sprite_.getTextureRect().height;
  sprite_.setScale( scaleX, scaleY );

  sf::FloatRect origin = sprite_.getLocalBounds();
  sprite_.setOrigin( origin.width / 2.0f, origin.height / 2.0f );
}

Planet::~Planet()
{}

void Planet::updateGraphics()
{
  Eigen::Vector2f positionInDisplayCS = position_;
  csHandler_.convertToDisplayCS( positionInDisplayCS );

  sprite_.setPosition( positionInDisplayCS(0), positionInDisplayCS(1) );
}

void Planet::render( sf::RenderWindow& renderWindow ) const
{
  renderWindow.draw( sprite_ );
}