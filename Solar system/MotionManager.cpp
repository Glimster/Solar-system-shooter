#include "stdafx.h"
#include "MotionManager.h"
#include "Integrator.h"
#include "MathUtil.h"

using namespace std;

MotionManager::MotionManager( Integration integration,
                              Technique technique ):
integration_( integration ),
technique_( technique )
{}

MotionManager::~MotionManager() {}

void MotionManager::updateLinearMotion( const float dt,
                                        vector< SpaceObject* > spaceObjects )
{
  vector< Eigen::Vector2f > drs, dvs;
  switch( technique_ )
  {
  case( Technique::standard ) :
  {
    if( integration_ == Integration::EulerForward ) {
      MathUtil::eulerStep( spaceObjects, dt, drs, dvs );
    }
    else {
      assert( integration_ == Integration::RK4 );
      MathUtil::RK4Step( spaceObjects, dt, drs, dvs );
    }
    break;
  }
  case( Technique::lambda ) :
  {
    if( integration_ == Integration::EulerForward )
      Integrator::N2EulerStepLambdas( spaceObjects, dt, drs, dvs );
    else
      throw invalid_argument( "RK4 not implemented for lambdas" );
    break;
  }
  case( Technique::functor ) :
  {
    if( integration_ == Integration::EulerForward ) {
      Integrator::N2EulerStepFunctorsState( spaceObjects, dt, drs, dvs );
    }
    else {
      assert( integration_ == Integration::RK4 );
      Integrator::N2RK4StepFunctors( spaceObjects, dt, drs, dvs );
    }
    break;
  }
  default:
    assert( false );
    break;
  }

  for( size_t i = 0; i != spaceObjects.size(); ++i )
  {
    spaceObjects[i]->updateState( drs[i], dvs[i] );
  }
}