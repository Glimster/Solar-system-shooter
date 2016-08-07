#pragma once

#include "ResourceIdentifiers.h"
#include "GameEntity.h"

class TextNode;

class StarShip : public GameEntity
{
public:
  enum Type
  {
    Standard,
    TypeCount
  };

public:
  StarShip( Type type,
            const TextureHolder& textureHolder,
            const FontHolder& fonts,
            const CoordinateSystemHandler& csHandler );
  
  ~StarShip();

  virtual void computeLinearForceOverM( Eigen::Vector2f& fOverM ) const override;
  virtual float computeTorque() const override;

  void setAftThrusters( bool thrusters ) { aftThrusters_ = thrusters; }
  void setLeftRotationThrusters( bool thrusters ) { leftRotationThrusters_ = thrusters; }
  void setRightRotationThrusters( bool thrusters ) { rightRotationThrusters_ = thrusters; }

  unsigned int getCategory() const;

  friend class Game; // TODO, bara f�r debug antar jag
private:
  void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;
  void updateCurrentGraphics_();

private:
  Type type_;
  sf::Sprite sprite_;
  TextNode* healthDisplay_;

  bool aftThrusters_;
  bool leftRotationThrusters_;
  bool rightRotationThrusters_;
};