#include "stdafx.h"
#include "MathUtil.h"

#include "Physics.h" // TODO, can be removed later?

using namespace std;

const float MathUtil::oneOverSix_ = 1.0f / 6.0f;

void MathUtil::eulerStep( const vector< SpaceObject* >& objects, 
                          const float dt,
                          vector< Eigen::Vector2f >& drs, 
                          vector< Eigen::Vector2f >& dvs )
{
  drs.resize( objects.size() );
  dvs.resize( objects.size() );
  for( size_t i = 0; i != objects.size(); ++i )
  {
    const float m1 = objects[i]->getMass();
    const Eigen::Vector2f& r1 = objects[i]->getPosition();

    Eigen::Vector2f Ftot( 0.0f, 0.0f );
    for( size_t j = 0; j != objects.size(); ++j ) {
      if ( i == j )
        continue;

      Eigen::Vector2f F( 0.0f, 0.0f );
      Physics::forceOfGravity( m1, objects[j]->getMass(), r1 - objects[j]->getPosition(), F );
      
      Ftot += F;
    }

    // Thrusters
    Eigen::Vector2f F2( 0.0f, 0.0f );
    objects[i]->computeLinearForce( F2 );

    Ftot += F2;

    drs[i] = objects[i]->getVelocity() * dt;
    dvs[i] = Ftot / m1 * dt;
  }
}

void MathUtil::RK4Step( const vector< SpaceObject* >& objects, 
                        const float dt,
                        vector< Eigen::Vector2f >& drs, 
                        vector< Eigen::Vector2f >& dvs )
{
  const size_t nbOfObjects = objects.size();

  vector< Eigen::Vector2f > k1_r( nbOfObjects ), k1_v( nbOfObjects );
  vector< Eigen::Vector2f > k2_r( nbOfObjects ), k2_v( nbOfObjects );
  vector< Eigen::Vector2f > k3_r( nbOfObjects ), k3_v( nbOfObjects );
  vector< Eigen::Vector2f > k4_r( nbOfObjects ), k4_v( nbOfObjects );

  computeDerivatives_( objects, 0.0f, 
                       vector< Eigen::Vector2f >( nbOfObjects ), vector< Eigen::Vector2f >( nbOfObjects ),
                       k1_r, k1_v );

  computeDerivatives_( objects, 0.5f * dt, 
                       k1_r, k1_v,
                       k2_r, k2_v );

  computeDerivatives_( objects, 0.5f * dt, 
                       k2_r, k2_v,
                       k3_r, k3_v );

  computeDerivatives_( objects, dt, 
                       k3_r, k3_v,
                       k4_r, k4_v );

  drs.resize( objects.size() );
  dvs.resize( objects.size() );
  for( size_t i = 0; i < drs.size(); ++i ) {
    drs[i] = 1.0f / 6.0f * ( k1_r[i] + 2.0f * ( k2_r[i] + k3_r[i] ) + k4_r[i] ) * dt;
    dvs[i] = 1.0f / 6.0f * ( k1_v[i] + 2.0f * ( k2_v[i] + k3_v[i] ) + k4_v[i] ) * dt;
  }
}

void MathUtil::computeDerivatives_( const vector< SpaceObject* >& objects, 
                                    const float dt,
                                    const vector< Eigen::Vector2f >& drdtIn, 
                                    const vector< Eigen::Vector2f >& dvdtIn,
                                    vector< Eigen::Vector2f >& drdtOut, 
                                    vector< Eigen::Vector2f >& dvdtOut )
{
  vector< Eigen::Vector2f > rs( objects.size() );
  vector< Eigen::Vector2f > vs( objects.size() );
  for( size_t i = 0; i < objects.size(); ++i ) {
    rs[i] = objects[i]->getPosition() + drdtIn[i] * dt;
    vs[i] = objects[i]->getVelocity() + dvdtIn[i] * dt;
  }

  for( size_t i = 0; i < objects.size(); ++i ) {
    const float m1 = objects[i]->getMass();

    Eigen::Vector2f Ftot( 0.0f, 0.0f );
    for( size_t j = 0; j != objects.size(); ++j ) {
      if ( i == j )
        continue;

      Eigen::Vector2f F;
      Physics::forceOfGravity( m1, objects[j]->getMass(), rs[i] - rs[j], F );
      
      Ftot += F;
    }

    // Thrusters
    Eigen::Vector2f F2;
    objects[i]->computeLinearForce( F2 );

    Ftot += F2;

    drdtOut[i] = vs[i];
    dvdtOut[i] = Ftot / m1;
  }
}