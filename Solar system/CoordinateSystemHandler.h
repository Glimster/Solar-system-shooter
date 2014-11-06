#pragma once
class CoordinateSystemHandler
{
public:
  CoordinateSystemHandler();
  CoordinateSystemHandler( float meterToPixel, const Eigen::Vector2f& originInDisplayCS );
  ~CoordinateSystemHandler();

  inline float unitOfLength2Pixel() const { return unitOfLength2Pixel_; }

  // Display CS:            World CS:
  // ------------->  x            ^ y
  // |                            |
  // |                            |
  // |                     -------|------> x
  // |                            |
  // v  y                         |
  void convertToDisplayCS( Eigen::Vector2f& position ) const;
  void convertToWorldCS( Eigen::Vector2f& position ) const;

  float computeAngleInDisplayCS( const Eigen::Vector2f& orientationInWorldCS ) const;

private:
  float unitOfLength2Pixel_;
  Eigen::Vector2f unitOfLength2PixelV_;
  Eigen::Vector2f originInDisplayCS_;
};

