#pragma once

class SpaceObject;

// TODO, hitta på bättre namn
class MotionManager
{
public:
  enum class Integration { EulerForward, RK4 };
  enum class Technique { standard, lambda, functor };

  MotionManager( Integration integration, 
                 Technique technique );
  ~MotionManager();

  void updateLinearMotion( const float dt,
                           std::vector< SpaceObject* > spaceObjects );

private:
  Integration integration_;
  Technique technique_;
};

