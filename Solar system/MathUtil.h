#pragma once
#include "SpaceObject.h"

class MathUtil
{
public:
  template< typename FuncType >
  inline static float forwardEulerStep1d( FuncType derivative,
                                          const float dt )
  {
    return derivative() * dt;
  };

  //template< typename FuncType >
  //inline static Eigen::Vector2f forwardEulerStep( FuncType derivative,
  //                                                const float dt )
  //{
  //  Eigen::Vector2f der( 0.0f, 0.0f );
  //  derivative( der );
  //  return der * dt;
  //};

  template< typename FuncType >
  inline static Eigen::Vector2f forwardEulerStep( FuncType derivative,
                                                  const float dt )
  {
    return derivative() * dt;
  };

  // TODO, av någon anledning så bli dr fel när denna används med funktorer (dock inte lambda)!?
  //template< typename FuncType >
  //inline static auto forwardEulerStep( FuncType derivative,
  //                                     const float dt ) -> decltype( derivative() * dt )
  //{
  //  std::cout << "fes: der = " << derivative() << std::endl;
  //  std::cout << "fes: dt = " << dt << std::endl;
  //  return derivative() * dt;
  //};

  inline static float RK4Step( const float k1,
                               const float k2,
                               const float k3,
                               const float k4,
                               const float dt )
  {
    return oneOverSix_ * ( k1 + 2.0f * ( k2 + k3 ) + k4 ) * dt;
  }

  inline static Eigen::Vector2f RK4Step( const Eigen::Vector2f& k1,
                                         const Eigen::Vector2f& k2,
                                         const Eigen::Vector2f& k3,
                                         const Eigen::Vector2f& k4,
                                         const float dt )
  {
    return oneOverSix_ * ( k1 + 2.0f * ( k2 + k3 ) + k4 ) * dt;
  }

  // TODO, ta bort:
  static void eulerStep( const std::vector< SpaceObject* >& objects, 
                         const float dt,
                         std::vector< Eigen::Vector2f >& drs, 
                         std::vector< Eigen::Vector2f >& dvs );
  // TODO, ta bort:
  static void RK4Step( const std::vector< SpaceObject* >& objects, 
                       const float dt,
                       std::vector< Eigen::Vector2f >& drs, 
                       std::vector< Eigen::Vector2f >& dvs );

private:

  // TODO, ta bort:
  static void computeDerivatives_( const std::vector< SpaceObject* >& objects, 
                                   const float dt,
                                   const std::vector< Eigen::Vector2f >& drdtIn, 
                                   const std::vector< Eigen::Vector2f >& dvdtIn,
                                   std::vector< Eigen::Vector2f >& drdtOut, 
                                   std::vector< Eigen::Vector2f >& dvdtOut );

  static const float oneOverSix_;
};
