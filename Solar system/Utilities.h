#pragma once

// Various utility functions. Consider breaking apart in future.
namespace Utilities
{
  void centerOrigin( sf::Sprite& sprite );
  void centerOrigin( sf::Text& text );

  std::string toString( const sf::Vector2f& vec );
  std::string toString( const Eigen::Vector2f& vec );
};

