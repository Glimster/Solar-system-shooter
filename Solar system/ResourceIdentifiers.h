#pragma once

namespace Textures
{
  enum class ID { // Planets
                  Mercury, 
                  Venus, 
                  Earth, 
                  Mars, 
                  Jupiter, 
                  Saturn, 
                  Uranus, 
                  Neptune,
                  // Stars
                  Sun,
                  // Ships
                  StarShip,
                  // Backgrounds
                  Space };
}

// Forward declarations
namespace sf
{
  class Texture;
}

template < typename Resource, typename Identifier >
class ResourceHolder;

typedef ResourceHolder< sf::Texture, Textures::ID > TextureHolder;
