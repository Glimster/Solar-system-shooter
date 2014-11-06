#include "stdafx.h"
#include "StarShip.h"

#include "CoordinateSystemHandler.h"
#include "ResourceHolder.h"
#include "Physics.h"

using namespace std;

StarShip::StarShip( float mass, 
                    Textures::ID textureID, 
                    const TextureHolder& textureHolder,
                    const CoordinateSystemHandler& csHandler ):
  SpaceObject( mass, csHandler ),
  aftThrusters_( false ),
  leftRotationThrusters_( false ),
  rightRotationThrusters_( false )
{
  sprite_.setTexture( textureHolder.get( textureID ) );

  const float shipLength = 2000.0f * 12.756e6f / Phys::PhysicalConstants::au2m; // TODO, how big should it be?
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

void StarShip::updateGraphics()
{
  Eigen::Vector2f positionInDisplayCS = position_;
  csHandler_.convertToDisplayCS( positionInDisplayCS );
  sprite_.setPosition( positionInDisplayCS(0), positionInDisplayCS(1) );

  const float angleInDisplayCS = csHandler_.computeAngleInDisplayCS( orientation_ );
  sprite_.setRotation( angleInDisplayCS );
}

void StarShip::render( sf::RenderWindow& renderWindow ) const
{
  renderWindow.draw( sprite_ );
}

void StarShip::computeLinearForce( Eigen::Vector2f& force ) const
{
  if( aftThrusters_ )
  {
    const float magnitude = mass_ * 0.0172f; // Will accelerate to v ~= 0.0172 in dt = 1
    force = orientation_ * magnitude;
  }
  else 
  {
    force.setZero();
  }
}

void StarShip::computeLinearForceOverM( Eigen::Vector2f& fOverM ) const
{
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