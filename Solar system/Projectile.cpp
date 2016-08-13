#include "stdafx.h"
#include "Projectile.h"

#include "GameDataTables.h"
#include "Utilities.h"
#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"
#include "Category.h"


namespace
{
  const std::vector< ProjectileData > Table = setupProjectileData();
}

Projectile::Projectile( Type type, const TextureHolder& textureHolder, const CoordinateSystemHandler& csHandler ):
  GameEntity( Table[type].mass, Table[type].hitPoints, csHandler ),
  type_( type ),
  sprite_( textureHolder.get( Table[type].texture ) )
{
  // TODO, consider making utility function as well
  const float scaleX = Table[type].radius * csHandler_.world2DisplayLength() * 2.0f / sprite_.getTextureRect().width;
  const float scaleY = Table[type].radius * csHandler_.world2DisplayLength() * 2.0f / sprite_.getTextureRect().height;
  transformable_.setScale( scaleX, scaleY );

  Utilities::centerOrigin( sprite_ );
}

Projectile::~Projectile()
{}

unsigned int Projectile::getCategory() const
{
  return Category::Projectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
  return sprite_.getGlobalBounds(); // TODO, ehh?
}

//float Projectile::getMaxSpeed() const 
//{}

int Projectile::getDamage() const
{
  return Table[type_].damage;
}

void Projectile::drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const
{
  target.draw( sprite_, states );
}

void Projectile::updateCurrent_( sf::Time dt, CommandQueue& commands )
{
  const auto positionInDisplayCS = csHandler_.convertToDisplayCS( position_ );
  transformable_.setPosition( positionInDisplayCS(0), positionInDisplayCS(1) );
}

void Projectile::printCurrent_( string& string ) const
{
  string.append( "Projectile: category = " + to_string( getCategory() ) );
  printDefaultData_( string );
  printPOData_( string );
  string.append( "\n" );
}
