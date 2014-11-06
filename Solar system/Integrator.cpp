#include "stdafx.h"
#include "Integrator.h"

#include "SpaceObject.h"

#include "Physics.h"
#include "MathUtil.h"

using namespace std;

void Integrator::N2EulerStepLambdas( const vector< SpaceObject* >& objects, 
                                     const float dt,
                                     vector< Eigen::Vector2f >& drs, 
                                     vector< Eigen::Vector2f >& dvs )
{
  drs.resize( objects.size() );
  dvs.resize( objects.size() );
  for( size_t i = 0; i != objects.size(); ++i )
  { 
    auto fOverM = [&]()
    {
      Eigen::Vector2f fOverM( 0.0f, 0.0f );
      for( size_t j = 0; j != objects.size(); ++j ) {
        if ( i == j )
          continue;

        Eigen::Vector2f forceOfGravityOverM( 0.0f, 0.0f );
        Physics::forceOfGravityOverM( objects[j]->getMass(), 
                                      objects[i]->getPosition() - objects[j]->getPosition(), 
                                      forceOfGravityOverM );
      
        fOverM += forceOfGravityOverM;
      }

      // Thrusters
      Eigen::Vector2f thrusterForceOverM( 0.0f, 0.0f );
      objects[i]->computeLinearForceOverM( thrusterForceOverM );

      fOverM += thrusterForceOverM;

      return fOverM;
    };

    dvs[i] = MathUtil::forwardEulerStep( fOverM, dt );

    auto velocity = [&]()
    {
      return objects[i]->getVelocity();
    };

    drs[i] = MathUtil::forwardEulerStep( velocity, dt );
  }
}

void Integrator::N2EulerStepFunctorsState( const vector< SpaceObject* >& objects, 
                                           const float dt,
                                           vector< Eigen::Vector2f >& drs, 
                                           vector< Eigen::Vector2f >& dvs )
{
  const size_t nbOfObjects = objects.size();
  vector< StateLin > initialStates( nbOfObjects );
  for( size_t i = 0; i < nbOfObjects; ++i ) {
    initialStates[i].mass = objects[i]->getMass();
    initialStates[i].position = objects[i]->getPosition();
    initialStates[i].velocity = objects[i]->getVelocity();
    objects[i]->computeLinearForceOverM( initialStates[i].linearForceOverM );
  }

  drs.resize( nbOfObjects );
  dvs.resize( nbOfObjects );
  for( size_t i = 0; i < nbOfObjects; ++i )
  {
    // dv/dt = a = F / m
    LinearForceOverM fOverM( initialStates, i );
    dvs[i] = MathUtil::forwardEulerStep( fOverM, dt );

    // dr/dt = v
    drs[i] = MathUtil::forwardEulerStep( [&]() { return initialStates[i].velocity; }, dt );
  }
}

void Integrator::N2RK4StepFunctors( const vector< SpaceObject* >& objects, 
                                    const float dt,
                                    vector< Eigen::Vector2f >& drs, 
                                    vector< Eigen::Vector2f >& dvs )
{
  const size_t nbOfObjects = objects.size();
  vector< StateLin > initialStates( nbOfObjects );
  for( size_t i = 0; i < nbOfObjects; ++i ) {
    initialStates[i].mass = objects[i]->getMass();
    initialStates[i].position = objects[i]->getPosition();
    initialStates[i].velocity = objects[i]->getVelocity();
    objects[i]->computeLinearForceOverM( initialStates[i].linearForceOverM );
  }

  vector< Eigen::Vector2f > k1_r( nbOfObjects ), k1_v( nbOfObjects );
  vector< Eigen::Vector2f > k2_r( nbOfObjects ), k2_v( nbOfObjects );
  vector< Eigen::Vector2f > k3_r( nbOfObjects ), k3_v( nbOfObjects );
  vector< Eigen::Vector2f > k4_r( nbOfObjects ), k4_v( nbOfObjects );

  computeRK4Derivatives_( initialStates, 0.0f, 
                          vector< Eigen::Vector2f >( nbOfObjects, Eigen::Vector2f( 0.0f, 0.0f ) ), vector< Eigen::Vector2f >( nbOfObjects, Eigen::Vector2f( 0.0f, 0.0f ) ),
                          k1_r, k1_v );

  computeRK4Derivatives_( initialStates, 0.5f * dt, 
                          k1_r, k1_v,
                          k2_r, k2_v );

  computeRK4Derivatives_( initialStates, 0.5f * dt, 
                          k2_r, k2_v,
                          k3_r, k3_v );

  computeRK4Derivatives_( initialStates, dt, 
                          k3_r, k3_v,
                          k4_r, k4_v );

  drs.resize( nbOfObjects );
  dvs.resize( nbOfObjects );
  for( size_t i = 0; i < drs.size(); ++i ) {
    drs[i] = MathUtil::RK4Step( k1_r[i], k2_r[i], k3_r[i], k4_r[i], dt );
    dvs[i] = MathUtil::RK4Step( k1_v[i], k2_v[i], k3_v[i], k4_v[i], dt );
  }
}

void Integrator::N2RotEulerStepLambdas( const SpaceObject& object, 
                                        const float dt,
                                        float& dL,
                                        float& dTheta )
{
  auto tau = [&]()
  {
    return object.computeTorque();
  };
  dL = MathUtil::forwardEulerStep1d( tau, dt );

  auto omega = [&]()
  {
    return object.getAngularMomentum() / object.getMomentOfInertia();
  };    
  dTheta = MathUtil::forwardEulerStep1d( omega, dt );
}

void Integrator::N2RotEulerStepFunctor( const SpaceObject& object, 
                                        const float dt,
                                        float& dL,
                                        float& dTheta )
{
  StateRot initialState = { object.getAngularMomentum(), object.getMomentOfInertia(), object.computeTorque() };

  dL = MathUtil::forwardEulerStep1d( [&]() { return initialState.torque; }, dt );

  AngularVelocity omega( initialState );
  dTheta = MathUtil::forwardEulerStep1d( omega, dt );
}

void Integrator::N2RotRK4StepFunctor( const SpaceObject& object,
                                      const float dt,
                                      float& dL, 
                                      float& dTheta )
{
  StateRot initialState = { object.getAngularMomentum(), object.getMomentOfInertia(), object.computeTorque() };

  float k1_L = 0.0f, k2_L = 0.0f, k3_L = 0.0f, k4_L = 0.0f;
  float k1_theta = 0.0f, k2_theta = 0.0f, k3_theta = 0.0f, k4_theta = 0.0f;

  computeRK4Derivatives_( initialState, 0.0f, 
                          0.0f, 0.0f,
                          k1_L, k1_theta );

  computeRK4Derivatives_( initialState, 0.5f * dt, 
                          k1_L, k1_theta,
                          k2_L, k2_theta );

  computeRK4Derivatives_( initialState, 0.5f * dt, 
                          k2_L, k2_theta,
                          k3_L, k3_theta );

  computeRK4Derivatives_( initialState, dt, 
                          k3_L, k3_theta,
                          k4_L, k4_theta );

  dL = MathUtil::RK4Step( k1_L, k2_L, k3_L, k4_L, dt );
  dTheta = MathUtil::RK4Step( k1_theta, k2_theta, k3_theta, k4_theta, dt );
}

// ---------------------------------------------------------------------------

void Integrator::computeRK4Derivatives_( const StateRot& initialState,
                                         const float dt,
                                         const float dLdtIn, 
                                         const float dThetadtIn,
                                         float& dLdtOut, 
                                         float& dThetadtOut )
{
  StateRot intermediateState( initialState ); // Make sure all values are copied
  intermediateState.angularMomentum += dLdtIn * dt;

  dLdtOut = intermediateState.torque;

  AngularVelocity w( intermediateState );
  dThetadtOut = w();
}

void Integrator::computeRK4Derivatives_( const vector< StateLin >& initialStates, 
                                         const float dt,
                                         const vector< Eigen::Vector2f >& drdtIn, 
                                         const vector< Eigen::Vector2f >& dvdtIn,
                                         vector< Eigen::Vector2f >& drdtOut, 
                                         vector< Eigen::Vector2f >& dvdtOut )
{
  size_t nbOfStates = initialStates.size();
  vector< StateLin > intermediateStates( initialStates ); // Make sure all values are copied
  for( size_t i = 0; i < nbOfStates; ++i ) {
    intermediateStates[i].position += drdtIn[i] * dt;
    intermediateStates[i].velocity += dvdtIn[i] * dt;
  }

  for( size_t i = 0; i < nbOfStates; ++i ) {
    LinearForceOverM linearForceOverM( intermediateStates, i );
    Eigen::Vector2f fOverM = linearForceOverM();

    drdtOut[i] = intermediateStates[i].velocity;
    dvdtOut[i] = fOverM;
  }
}