#pragma once
class CoordinateSystemHandler
{
public:
  CoordinateSystemHandler();
  CoordinateSystemHandler( float unitOfLength2Pixel, const Eigen::Vector2f& originInDisplayCS );
  ~CoordinateSystemHandler();

  inline float unitOfLength2Pixel() const { return unitOfLength2Pixel_; }

  // Display CS:            World CS:
  // ------------->  x            ^ y
  // |                            |
  // |                            |
  // |                     -------|------> x
  // |                            |
  // v  y                         |
  Eigen::Vector2f convertToDisplayCS( const Eigen::Vector2f& position ) const;
  Eigen::Vector2f convertToWorldCS( const Eigen::Vector2f& position ) const;

  // Also converts from radians to degrees
  float computeAngleInDisplayCS( const Eigen::Vector2f& orientationInWorldCS ) const;

private:
  float unitOfLength2Pixel_;
  Eigen::Vector2f unitOfLength2PixelV_;
  Eigen::Vector2f originInDisplayCS_;
};

