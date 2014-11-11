#pragma once

#include "SceneNode.h"

class CoordinateSystemHandler;

class SpaceObject : public SceneNode
{
public:
  SpaceObject( float mass,
               const CoordinateSystemHandler& csHandler );
  virtual ~SpaceObject();

  float getMass() const { return mass_; }
  const Eigen::Vector2f& getPosition() const { return position_; }
  const Eigen::Vector2f& getVelocity() const { return velocity_; }
  //const Eigen::Vector2f& getOrientation() const { return orientation_; }
  float getMomentOfInertia() const { return momentOfInertia_; }
  float getAngularMomentum() const { return angularMomentum_; }

  void setPosition( const Eigen::Vector2f& position );
  void setVelocity( const Eigen::Vector2f& velocity );
  void setOrientation( const Eigen::Vector2f& orientation );

  void updateState( const Eigen::Vector2f& dr, const Eigen::Vector2f& dv );
  void updateAngularMomentum( float dL );
  void rotate( float rotation );

  // TODO, �r det h�r en bra konstruktion?
  virtual void computeLinearForce( Eigen::Vector2f& force ) const { force.setZero(); }; // TODO, ta bort
  virtual void computeLinearForceOverM( Eigen::Vector2f& fOverM ) const { fOverM.setZero(); };
  virtual float computeTorque() const { return 0.0f; };

  // TODO, det h�r m�ste funderas p�:
  virtual void updateGraphics() = 0; 

protected:
  const CoordinateSystemHandler& csHandler_;

  // Linear dynamics
  float mass_;
  Eigen::Vector2f position_;
  Eigen::Vector2f velocity_;

  // Rotational dynamics
  float momentOfInertia_;
  float angularMomentum_;

  Eigen::Vector2f orientation_;

private:
};

