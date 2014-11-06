#pragma once

#include "ResourceIdentifiers.h"
#include "SpaceObject.h"

class StarShip : public SpaceObject
{
public:
  StarShip( float mass,
            Textures::ID textureID,
            const TextureHolder& textureHolder,
            const CoordinateSystemHandler& csHandler );
  
  ~StarShip();

  void updateGraphics();
  void render( sf::RenderWindow& renderWindow ) const;

  void computeLinearForce( Eigen::Vector2f& force ) const; // TODO, ta bort
  void computeLinearForceOverM( Eigen::Vector2f& fOverM ) const;
  float computeTorque() const;

  void setAftThrusters( bool thrusters ) { aftThrusters_ = thrusters; }
  void setLeftRotationThrusters( bool thrusters ) { leftRotationThrusters_ = thrusters; }
  void setRightRotationThrusters( bool thrusters ) { rightRotationThrusters_ = thrusters; }

private:
  sf::Sprite sprite_;

  bool aftThrusters_;
  bool leftRotationThrusters_;
  bool rightRotationThrusters_;
};