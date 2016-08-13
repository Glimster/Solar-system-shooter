#include "stdafx.h"
#include "PhysicalObject.h"

#include "Utilities.h"

using namespace std;
PhysicalObject::PhysicalObject( float mass ):
  Massive( mass ),
  position_( 0.0f, 0.0f ),
  velocity_( 0.0f, 0.0f ),
  orientation_( 0.0f, 0.0f )
{}

PhysicalObject::~PhysicalObject()
{}

void PhysicalObject::setPosition( const Eigen::Vector2f& position )
{
  position_ = position;
}

void PhysicalObject::setVelocity( const Eigen::Vector2f& velocity )
{
  velocity_ = velocity;
}

void PhysicalObject::setOrientation( const Eigen::Vector2f& orientation )
{
  orientation_ = orientation;
  orientation_.normalize();
}

void PhysicalObject::updateState( const Eigen::Vector2f& dr, const Eigen::Vector2f& dv )
{
  position_ += dr;
  velocity_ += dv;
}

void PhysicalObject::rotate( float angle )
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

void PhysicalObject::printPOData_( string& string ) const
{
  string.append( ", p = " + Utilities::toString( position_ ) );
  string.append( ", v = " + Utilities::toString( velocity_ ) );
  string.append( ", o = " + Utilities::toString( orientation_ ) );
  string.append( ", mass = " + to_string( getMass() ) );
}