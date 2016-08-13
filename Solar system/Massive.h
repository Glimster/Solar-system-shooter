#pragma once
class Massive
{
public:
  Massive( float mass );
  ~Massive();

  virtual float getMass() const { return mass_; }
  float getMomentOfInertia() const { return momentOfInertia_; }
  float getAngularMomentum() const { return angularMomentum_; }

  void updateAngularMomentum( float dL );
  
  // TODO, hitta p� b�ttre namn! Det ska vara krafter fr�n detta objekt, inte utifr�n
  virtual float computeTorque() const { return 0.0f; };
  virtual void computeLinearForceOverM( Eigen::Vector2f& fOverM ) const { fOverM.setZero(); };

protected:
  float mass_;
  float momentOfInertia_;
  float angularMomentum_;
};

