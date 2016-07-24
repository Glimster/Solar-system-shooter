#include "stdafx.h"
#include "SpaceObject.h"

using namespace std;

SpaceObject::SpaceObject( float mass,
                          const CoordinateSystemHandler& csHandler ):
  mass_( mass ),
  position_( 0.0f, 0.0f ),
  velocity_( 0.0f, 0.0f ),
  momentOfInertia_( 0.0f ),
  angularMomentum_( 0.0f ),
  orientation_( 0.0f, 0.0f ),
  csHandler_( csHandler )
{}

SpaceObject::~SpaceObject()
{}

void SpaceObject::setPosition( const Eigen::Vector2f& position )
{
  position_ = position;
}

void SpaceObject::setVelocity( const Eigen::Vector2f& velocity )
{
  velocity_ = velocity;
}

void SpaceObject::setOrientation( const Eigen::Vector2f& orientation )
{
  orientation_ = orientation;
  orientation_.normalize();
}

void SpaceObject::updateState( const Eigen::Vector2f& dr, const Eigen::Vector2f& dv )
{
  position_ += dr;
  velocity_ += dv;
}

void SpaceObject::updateAngularMomentum( float dL )
{
  angularMomentum_ += dL;
}

void SpaceObject::rotate( float angle )
{
  if( false )
  {
    // TODO:
    //Eigen::Rotation2D< float > rot( angle );
    //orientation_ = orientation_ * rot;
  }
  else
  {
    const float cosAngle = cos( angle );
    const float sinAngle = sin( angle );
    float newX = orientation_(0) * cosAngle - orientation_(1) * sinAngle;
    float newY = orientation_(0) * sinAngle + orientation_(1) * cosAngle;

    orientation_(0) = newX;
    orientation_(1) = newY;
  }
}

void SpaceObject::updateGraphics()
{
  assert( false );
}