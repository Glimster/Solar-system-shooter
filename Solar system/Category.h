#pragma once

namespace category
{
  // Bitwise identification
  // Enables combination of types using the bitwise OR operator |
  // 
  // e.g. unsigned int everything = Category::Scene | Category::Player;
  enum class Type
  {
    None = 0,
    Scene = 1 << 0,
    Player = 1 << 1,
  };
}