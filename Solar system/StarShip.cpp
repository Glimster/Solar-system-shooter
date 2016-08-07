#include "stdafx.h"
#include "StarShip.h"

#include "GameDataTables.h"
#include "GameEntity.h"
#include "Category.h"
#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"
#include "Physics.h"
#include "Utilities.h"
#include "TextNode.h"

using namespace std;

// Anonymous namespace to avoid name collission
namespace
{
  const std::vector< StarShipData > Table = setupStarShipData();
}

StarShip::StarShip( Type type,
                    const TextureHolder& textureHolder,
                    const FontHolder& fonts,
                    const CoordinateSystemHandler& csHandler ):
  GameEntity( Table[type].mass, Table[type].hitPoints, csHandler ),
  type_( type ),
  sprite_( textureHolder.get( Table[type].texture ) ),
  aftThrusters_( false ),
  leftRotationThrusters_( false ),
  rightRotationThrusters_( false )
{
  const float shipWidth = Table[type].length * sprite_.getTextureRect().height / sprite_.getTextureRect().width;
  const float scaleX = Table[type].length * csHandler_.unitOfLength2Pixel() / sprite_.getTextureRect().width;
  const float scaleY = shipWidth * csHandler_.unitOfLength2Pixel() / sprite_.getTextureRect().height;
  transformable_.setScale( scaleX, scaleY );

  Utilities::centerOrigin( sprite_ );

  unique_ptr< TextNode > healthDisplay( new TextNode( fonts, "" ) );
  healthDisplay_ = healthDisplay.get();
  attachChild( std::move( healthDisplay ) );

  // Approximate ship with rectangle
  momentOfInertia_ = Physics::computeRotationalInertiaForRectangle( Table[type].mass, Table[type].length, shipWidth );
}

StarShip::~StarShip()
{}

void StarShip::computeLinearForceOverM( Eigen::Vector2f& fOverM ) const
{
  // TODO, lägg in variabel kraft?
  if( aftThrusters_ )
  {
    const float magnitude = /*mass_ **/ 0.0172f; // Will accelerate to v ~= 0.0172 in dt = 1
    fOverM = orientation_ * magnitude;
  }
  else 
  {
    fOverM.setZero();
  }
}

float StarShip::computeTorque() const
{
  float torque = 0.0f;

  // TODO, lägg in variabel kraft?
  if( leftRotationThrusters_ )
  {
    torque += momentOfInertia_ * float( M_PI ) / 50.0f; // Will accelerate to w = pi/50 in dt = 1
  }
  if( rightRotationThrusters_ )
  {
    torque -= momentOfInertia_ * float( M_PI ) / 50.0f;
  }

  return torque;
}

unsigned int StarShip::getCategory() const
{
  return Category::Player;
}

void StarShip::drawCurrent_(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw( sprite_, states );
}

void StarShip::updateCurrentGraphics_()
{
  const auto positionInDisplayCS = csHandler_.convertToDisplayCS( position_ );
  transformable_.setPosition( positionInDisplayCS(0), positionInDisplayCS(1) );

  const float angleInDisplayCS = csHandler_.computeAngleInDisplayCS( orientation_ );
  transformable_.setRotation( angleInDisplayCS );

  healthDisplay_->setString( to_string( getHitPoints() ) + " HP" );
  healthDisplay_->setPosition( 0.f, 500.f ); // TODO, gör att texten snurrar runt skeppet. Fixa!
  healthDisplay_->setRotation( -transformable_.getRotation() );
}