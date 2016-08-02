#pragma once

#include "ResourceIdentifiers.h"
#include "SpaceObject.h"

class StarShip : public SpaceObject
{
public:
  StarShip( float mass,
            const TextureHolder& textureHolder,
            const CoordinateSystemHandler& csHandler );
  
  ~StarShip();

  virtual void computeLinearForce( Eigen::Vector2f& force ) const override; // TODO, ta bort
  virtual void computeLinearForceOverM( Eigen::Vector2f& fOverM ) const override;
  virtual float computeTorque() const override;

  void setAftThrusters( bool thrusters ) { aftThrusters_ = thrusters; }
  void setLeftRotationThrusters( bool thrusters ) { leftRotationThrusters_ = thrusters; }
  void setRightRotationThrusters( bool thrusters ) { rightRotationThrusters_ = thrusters; }

  unsigned int getCategory() const;

  friend class Game;
private:
  void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;
  void updateCurrentGraphics_();

  sf::Sprite sprite_;

  bool aftThrusters_;
  bool leftRotationThrusters_;
  bool rightRotationThrusters_;
};