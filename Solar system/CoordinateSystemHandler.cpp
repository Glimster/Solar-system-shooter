#include "stdafx.h"
#include "CoordinateSystemHandler.h"

using namespace std;

// TODO, inte så snyggt, kanske ska ha en initialize-metod?
CoordinateSystemHandler::CoordinateSystemHandler():
  unitOfLength2Pixel_( 1.0f ),
  unitOfLength2PixelV_( Eigen::Vector2f( unitOfLength2Pixel_, -unitOfLength2Pixel_ ) ),
  originInDisplayCS_( Eigen::Vector2f() )
{}

CoordinateSystemHandler::CoordinateSystemHandler( float unitOfLength2Pixel, const Eigen::Vector2f& originInDisplayCS ):
  unitOfLength2Pixel_( unitOfLength2Pixel ),
  unitOfLength2PixelV_( Eigen::Vector2f( unitOfLength2Pixel_, -unitOfLength2Pixel_ ) ),
  originInDisplayCS_( originInDisplayCS )
{}

CoordinateSystemHandler::~CoordinateSystemHandler()
{}

void CoordinateSystemHandler::convertToDisplayCS( Eigen::Vector2f& position ) const
{
  position = position.cwiseProduct( unitOfLength2PixelV_ );
  position += originInDisplayCS_;
}

void CoordinateSystemHandler::convertToWorldCS( Eigen::Vector2f& position ) const
{
  position -= originInDisplayCS_;
  position.cwiseQuotient( unitOfLength2PixelV_ );
}

float CoordinateSystemHandler::computeAngleInDisplayCS( const Eigen::Vector2f& orientationInWorldCS ) const
{
  return atan2( -orientationInWorldCS(1), orientationInWorldCS(0) ) * 180.0f / float( M_PI );
}