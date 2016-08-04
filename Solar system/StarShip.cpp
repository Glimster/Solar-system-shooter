#include "stdafx.h"
#include "StarShip.h"

#include "GameEntity.h"
#include "Category.h"
#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"
#include "Physics.h"

using namespace std;

StarShip::StarShip( float mass, 
                    const TextureHolder& textureHolder,
                    const CoordinateSystemHandler& csHandler ):
  GameEntity( mass, csHandler ),
  sprite_( textureHolder.get( Textures::ID::StarShip ) ),
  aftThrusters_( false ),
  leftRotationThrusters_( false ),
  rightRotationThrusters_( false )
{
  const float shipLength = 0.17f; // au, TODO, how big should it be?
  const float shipWidth = shipLength * sprite_.getTextureRect().height / sprite_.getTextureRect().width;
  const float scaleX = shipLength * csHandler_.unitOfLength2Pixel() / sprite_.getTextureRect().width;
  const float scaleY = shipWidth * csHandler_.unitOfLength2Pixel() / sprite_.getTextureRect().height;
  sprite_.setScale( scaleX, scaleY );

  sf::FloatRect bb = sprite_.getLocalBounds();
  sprite_.setOrigin( bb.width / 2.0f, bb.height / 2.0f );

  // Approximate ship with rectangle
  momentOfInertia_ = Physics::computeRotationalInertiaForRectangle( mass, shipLength, shipWidth );
}

StarShip::~StarShip()
{}

void StarShip::computeLinearForceOverM( Eigen::Vector2f& fOverM ) const
{
  // TODO, l�gg in variabel kraft?
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

  // TODO, l�gg in variabel kraft?
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
  target.draw(sprite_, states);
}

void StarShip::updateCurrentGraphics_()
{
  auto positionInDisplayCS = csHandler_.convertToDisplayCS( position_ );
  sprite_.setPosition(positionInDisplayCS(0), positionInDisplayCS(1));

  const float angleInDisplayCS = csHandler_.computeAngleInDisplayCS(orientation_);
  sprite_.setRotation(angleInDisplayCS);
}