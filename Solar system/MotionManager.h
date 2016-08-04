#pragma once

class PhysicalObject;

// TODO, hitta p� b�ttre namn?
class MotionManager
{
public:
  enum class Integration { EulerForward, RK4 };
  enum class Technique { standard, lambda, functor };

  MotionManager( Integration integration, 
                 Technique technique );
  ~MotionManager();

  void updateLinearMotion( const float dt,
                           std::vector< PhysicalObject* > physicalObjects );
private:
  Integration integration_;
  Technique technique_;
};

