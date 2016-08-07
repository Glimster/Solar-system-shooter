#pragma once
class PhysicalObject;

// TODO, ska detta också vara ett namespace, pss som Utilities? Verkar vara bättre c++-standard!
class MathUtil
{
public:

  // Ellipses
  // a - semi major axis
  // b - semi minor axis
  
  static float fociCenterDistanceEllipse( float a, float b );
  static float areaEllipse( float a, float b );

  // Ramanujan approximation, 
  // se https://www.mathsisfun.com/geometry/ellipse-perimeter.html
  static float circumferenceEllipse( float a, float b );
  

  // C++11 baby!
  template< typename VecType >
  inline static auto angleBetween( const VecType& v1, VecType& v2 ) -> decltype( v1.norm() )
  {
    return acos( v1.dot( v2 ) / ( v1.norm() * v2.norm() ) );
  }

  // Eigen extensions
  template< typename VecType >
  inline static auto cross2D( const VecType& v1, VecType& v2 ) -> decltype( v1.norm() )
  {
    return abs( v1( 0 ) * v2( 1 ) - v1( 1 ) * v2( 0 ) );
  }
  
  // 
  template< typename FuncType >
  inline static float forwardEulerStep1d( FuncType derivative,
                                          const float dt )
  {
    return derivative() * dt;
  };

  template< typename FuncType >
  inline static Eigen::Vector2f forwardEulerStep( FuncType derivative,
                                                  const float dt )
  {
    return derivative() * dt;
  };

  // TODO, av någon anledning så bli det fel när denna används med funktorer (dock inte lambda)!?
  //template< typename FuncType >
  //inline static auto forwardEulerStep( FuncType derivative,
  //                                     const float dt ) -> decltype( derivative() * dt )
  //{
  //  std::cout << "fes: der = " << derivative() << std::endl;
  //  std::cout << "fes: dt = " << dt << std::endl;
  //  return derivative() * dt;
  //};

  // TODO, går det att templatisera följande två?
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
  static void eulerStep( const std::vector< PhysicalObject* >& objects,
                         const float dt,
                         std::vector< Eigen::Vector2f >& drs, 
                         std::vector< Eigen::Vector2f >& dvs );
  static void RK4Step( const std::vector< PhysicalObject* >& objects,
                       const float dt,
                       std::vector< Eigen::Vector2f >& drs, 
                       std::vector< Eigen::Vector2f >& dvs );

private:

  // TODO, ta bort:
  static void computeDerivatives_( const std::vector< PhysicalObject* >& objects,
                                   const float dt,
                                   const std::vector< Eigen::Vector2f >& drdtIn, 
                                   const std::vector< Eigen::Vector2f >& dvdtIn,
                                   std::vector< Eigen::Vector2f >& drdtOut, 
                                   std::vector< Eigen::Vector2f >& dvdtOut );

  static const float oneOverSix_;
};
