#pragma once
class PhysicalObject;

// TODO, hitta p� b�ttre namn?
class MotionManager
{
public:
  enum class Integration { EulerForward, RK4 };
  enum class Technique { standard, functor };

  MotionManager( Integration integration, 
                 Technique technique );
  ~MotionManager();

  void computeLinearDynamics( const float dt,
                              const std::vector< PhysicalObject* >& physicalObjects );

  void computeRotationalDynamics( const float dt,
                                  PhysicalObject& physicalObject );
  
  // No forces
  void computeLinearKinematics( const float dt,
                                std::vector< PhysicalObject* > physicalObjects );

private:
  Integration integration_;
  Technique technique_;
};

