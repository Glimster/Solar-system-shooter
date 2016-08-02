#include "stdafx.h"
#include "PhysicsTest.h"

#include <gtest/gtest.h>

#include "SpaceObject.h"
#include "Planet.h"
#include "Physics.h"
#include "PhysicalData.h"
#include "PhysicalConstants.h"
#include "MathUtil.h"

#include "CoordinateSystemHandler.h"

#include "MotionManager.h"

using namespace std;

PhysicsTest::PhysicsTest( bool createGUI ):
  mainWindow_(),
  textureHolder_(),
  view_(),
  csHandler_(),
  sceneLayers_(),
  sceneGraph_(),
  spaceObjects_()
{
  if( createGUI )
  {
    mainWindow_.create( sf::VideoMode::getDesktopMode(), "Physics test" );
    view_ = sf::View( mainWindow_.getDefaultView() );

    const float spaceLength = 50.0; // au
    const float screenWidth = mainWindow_.getDefaultView().getSize().x;
    const float screenHeight = mainWindow_.getDefaultView().getSize().y;
    csHandler_ = CoordinateSystemHandler( min( screenWidth, screenHeight ) / (spaceLength * 2.0f ),
      Eigen::Vector2f( screenWidth / 2.0f, screenHeight / 2.0f ) );

    const float height = spaceLength * csHandler_.unitOfLength2Pixel();
    const float width = height * screenWidth / screenHeight;
    view_.setSize( width, height );

    textureHolder_.load( Textures::ID::Sun, "Resources/Sun.jpg" );
    textureHolder_.load( Textures::ID::Earth, "Resources/Earth.jpg" );
  }
}

//TEST( Keplers, laws )
void PhysicsTest::keplersLawsForHeavySun()
{
  // Setup
  MotionManager motionManager( MotionManager::Integration::RK4, 
                               MotionManager::Technique::functor );

  CoordinateSystemHandler csHandler;
  vector< unique_ptr< SpaceObject > > planets;
  spaceObjects_.clear();

  vector< PhysicalData::PlanetData > spaceObjectData;
  PhysicalData::setupHeavySunLightPlanet( spaceObjectData );
  const auto& sunData = spaceObjectData[0];
  for( auto& sod : spaceObjectData )
  {
    const PhysicalData::PlanetData& data = sod;
    unique_ptr< SpaceObject > object( new SpaceObject( data.mass, csHandler ) );
    object->setPosition( data.position );
    object->setVelocity( data.velocity );

    spaceObjects_.push_back( object.get() );
    planets.push_back( std::move( object ) );
  }
  const auto& sun = *planets[0];
  const auto& planet = *planets[1];

  // Act
  size_t updates = 0;
  float totalTime = 0.0f;
  float distanceTravelled = 0.0f;
  float totalArea = 0.0f;
  float totalAngle = 0.0f;
  
  vector< float > sweptAreas;

  auto startingPosition = planet.getPosition();
  auto previousPosition = startingPosition;
  auto minPosition = startingPosition;
  auto maxPosition = startingPosition;
  // Distances to sun
  float dPericenter = ( startingPosition - sun.getPosition() ).norm();
  float dApocenter = ( startingPosition - sun.getPosition() ).norm();
  float vPericenter = planet.getVelocity().norm();
  float vApocenter = planet.getVelocity().norm();
  auto pericenter = startingPosition;
  auto apocenter = startingPosition;

  float sweptArea = 0.0f;
  float dt = 1.0f / 1000.0f;
  size_t segmentAreaStepInterval = size_t( 0.5f / dt );
  while( totalAngle <= 2.0f * float( M_PI ) )
  {
    updates++;

    motionManager.updateLinearMotion( dt, spaceObjects_ );

    auto r_prev = previousPosition - sun.getPosition();
    auto r = planet.getPosition() - sun.getPosition();
    auto d = ( planet.getPosition() - previousPosition ).norm();
    
    totalTime += dt;
    distanceTravelled += d;
    totalArea += MathUtil::cross2D( r, r_prev ) / 2.0f; // Area of triangle
    sweptArea += MathUtil::cross2D( r, r_prev ) / 2.0f; // Area of triangle
    if( updates % segmentAreaStepInterval == 0 )
    {
      sweptAreas.push_back( sweptArea );
      sweptArea = 0.0f;
    }
    { // Floating point precision not enough, use double
      Eigen::Vector2d rd = r.cast< double >();
      Eigen::Vector2d r_prevd = r_prev.cast< double >();
      double dAngle = MathUtil::angleBetween( rd, r_prevd );
      totalAngle += float( dAngle );
    }

    minPosition( 0 ) = min( planet.getPosition()( 0 ), minPosition( 0 ) );
    minPosition( 1 ) = min( planet.getPosition()( 1 ), minPosition( 1 ) );
    maxPosition( 0 ) = max( planet.getPosition()( 0 ), maxPosition( 0 ) );
    maxPosition( 1 ) = max( planet.getPosition()( 1 ), maxPosition( 1 ) );

    if( r.norm() < dPericenter )
    {
      pericenter = planet.getPosition();
      vPericenter = planet.getVelocity().norm();
      dPericenter = r.norm();
    }
    if( r.norm() > dApocenter )
    {
      apocenter = planet.getPosition();
      vApocenter = planet.getVelocity().norm();
      dApocenter = r.norm();
    }

    previousPosition = planet.getPosition();
  }

  cout << "Number of steps = " << updates << endl;
  PhysicsTest::PVAR( planet.getPosition(), "Final position" );
  EXPECT_NEAR( ( startingPosition - planet.getPosition() ).norm(), 0.0, 1e-3 );

  // -------------------------------------------
  cout << endl << "Kepler 1: The orbit of a planet is an ellipse with the Sun at one of the two foci." << endl;
  auto centerPosition = ( maxPosition + minPosition ) / 2.0f;
  auto axes = maxPosition - minPosition;
  float semiMajorAxis = axes.maxCoeff() / 2.0f;
  float semiMinorAxis = axes.minCoeff() / 2.0f;
  cout << "Semi major axis = " << semiMajorAxis << endl;
  cout << "Semi minor axis = " << semiMinorAxis << endl;

  PhysicsTest::PVAR( pericenter, "pericenter" );
  PhysicsTest::PVAR( apocenter, "apocenter" );

  // According to https://en.wikipedia.org/wiki/Apsis
  float eccentricity = ( dApocenter - dPericenter ) / ( dApocenter + dPericenter );
  float standardGravitationalParameter = Phys::PhysicalConstants::kSquared * ( sun.getMass() );
  cout << "Distance at pericenter = " << dPericenter << endl;
  cout << "Speed at pericenter = " << vPericenter << endl;
  cout << "Distance at apocenter = " << dApocenter << endl;
  cout << "Speed at apocenter = " << vApocenter << endl;
  EXPECT_NEAR( dPericenter, ( 1 - eccentricity ) * semiMajorAxis, 1e-3 );
  EXPECT_NEAR( vPericenter, sqrt( ( 1 + eccentricity ) * standardGravitationalParameter / ( ( 1 - eccentricity ) * semiMajorAxis ) ), 1e-3 );
  EXPECT_NEAR( dApocenter, ( 1 + eccentricity ) * semiMajorAxis, 1e-3 );
  EXPECT_NEAR( vApocenter, sqrt( ( 1 - eccentricity ) * standardGravitationalParameter / ( ( 1 + eccentricity ) * semiMajorAxis ) ), 1e-3 );

  float f = MathUtil::fociCenterDistanceEllipse( semiMajorAxis, semiMinorAxis );
  Eigen::Vector2f F1( centerPosition( 0 ) - f, centerPosition( 1 ) );
  Eigen::Vector2f F2( centerPosition( 0 ) + f, centerPosition( 1 ) );
  PhysicsTest::PVAR( F1, "Focal point #1" );
  PhysicsTest::PVAR( F2, "Focal point #2 (sun)" );
  EXPECT_NEAR( ( sun.getPosition() - sunData.position ).norm(), 0.0, 1e-4 ); // Sun "infinitely" heavy
  EXPECT_NEAR( ( F2 - sun.getPosition() ).norm(), 0.0, 1e-4 ); // Sun at focal point two
  
  cout << "Distance travelled = " << distanceTravelled << endl;
  EXPECT_NEAR( distanceTravelled, MathUtil::circumferenceEllipse( semiMajorAxis, semiMinorAxis ), 1e-3 );

  cout << "Total swept area = " << totalArea << endl;
  EXPECT_NEAR( totalArea, MathUtil::areaEllipse( semiMajorAxis, semiMinorAxis ), 1e-3 );

  cout << "Total angle = " << totalAngle << endl;
  EXPECT_NEAR( totalAngle, 2 * M_PI, 1e-3 );

  // -------------------------------------------
  cout << endl << "Kepler 2: A line joining a planet and the Sun sweeps out equal areas during equal intervals of time." << endl;
  cout << "Swept area in time interval = " << sweptAreas[0] << endl;
  for( size_t i = 1; i < sweptAreas.size(); ++i )
  {
    EXPECT_NEAR( sweptAreas[i], sweptAreas[0], 1e-4 );
  }

  // -------------------------------------------
  cout << endl << "Kepler 3: The square of the orbital period of a planet is directly proportional to the cube of the semi-major axis of its orbit." << endl;
  const float constant = 4.0f * float( M_PI * M_PI ) / standardGravitationalParameter;
  float T = sqrt( pow( semiMajorAxis, 3 ) * constant );
  cout << "Orbital period = " << totalTime << endl;
  EXPECT_NEAR( totalTime, T, 1e-2 );
}

void PhysicsTest::testStabilityWRTTotalEnergy()
{
  cout << endl << "--------------------------------" << endl;
  cout << "Test stability" << endl;
  // Setup
  MotionManager motionManager( MotionManager::Integration::RK4,
                               MotionManager::Technique::functor );

  vector< unique_ptr< SpaceObject > > planets;
  spaceObjects_.clear();

  vector< PhysicalData::PlanetData > spaceObjectData;
  PhysicalData::setupHeavySunLightPlanet( spaceObjectData );
  const auto& sunData = spaceObjectData[0];
  for( auto& sod : spaceObjectData )
  {
    const PhysicalData::PlanetData& data = sod;
    unique_ptr< SpaceObject > object( new SpaceObject( data.mass, csHandler_ ) );
    object->setPosition( data.position );
    object->setVelocity( data.velocity );

    spaceObjects_.push_back( object.get() );
    planets.push_back( std::move( object ) );
  }
  const auto& sun = *planets[0];
  const auto& planet = *planets[1];

  auto computeTotalKE = [&]()
  {
    float totalKE = 0.0f;
    for( const auto& sp : spaceObjects_ )
    {
      totalKE += Physics::kineticEnergy( sp->getMass(), sp->getVelocity() );
    }
    return totalKE;
  };

  auto computeTotalPE = [&]() 
  {
    float totalPE = 0.0f;
    for( size_t i = 0; i < spaceObjects_.size() - 1; ++i )
    {
      for( size_t j = i + 1; j < spaceObjects_.size(); ++j )
      {
        const Eigen::Vector2f r = spaceObjects_[i]->getPosition() - spaceObjects_[j]->getPosition();
        totalPE = Physics::gravitationalPotentialEnergy( spaceObjects_[i]->getMass(), spaceObjects_[j]->getMass(), r );
      }
    }
    return totalPE;
  };

  // Act
  float totalAngle = 0.0f;
  PhysicsTest::PVAR( planet.getPosition(), "Initial position" );
  auto initialR = planet.getPosition() - sun.getPosition();
  cout << "Initial distance = " << initialR.norm() << endl;
  const float initialEnergy = computeTotalPE() + computeTotalKE();
  cout << "Initial total energy = " << initialEnergy << endl;

  auto startingPosition = planet.getPosition();
  auto previousPosition = startingPosition;
  float dt = 1.0f / 1000.0f;

  int numberOfTurns = 1000;
  while( int( totalAngle / ( 2.0f * float( M_PI ) ) ) < numberOfTurns )
  {
    motionManager.updateLinearMotion( dt, spaceObjects_ );

    auto r_prev = previousPosition - sun.getPosition();
    auto r = planet.getPosition() - sun.getPosition();

    { // Floating point computation not sufficient, use double precision
      Eigen::Vector2d rd = r.cast< double >();
      Eigen::Vector2d r_prevd = r_prev.cast< double >();
      double dAngle = MathUtil::angleBetween( rd, r_prevd );
      totalAngle += float( dAngle );
    }

    const float totalEnergy = computeTotalPE() + computeTotalKE();
    if( fabs( totalEnergy - initialEnergy ) > fabs( initialEnergy * 0.001f ) )
    {
      cout << "Total energy of system not constant, terminating!!!" << endl;
      break;
    }
    previousPosition = planet.getPosition();
  }

  cout << "---------------------------------------" << endl;
  PhysicsTest::PVAR( planet.getPosition(), "Final position" );

  cout << "Total angle = " << totalAngle << endl;
  cout << "Number of turns = " << totalAngle / ( 2.0f * float( M_PI ) ) << endl;
  EXPECT_NEAR( totalAngle, numberOfTurns * 2 * M_PI, 1e-3 );

  float finalEnergy = computeTotalPE() + computeTotalKE();
  cout << "Final total energy = " << finalEnergy << endl;
  EXPECT_NEAR( initialEnergy, finalEnergy, 1e-3 );
}

void PhysicsTest::testGUI()
{
  MotionManager motionManager( MotionManager::Integration::RK4,
                               MotionManager::Technique::functor );

  vector< PhysicalData::PlanetData > spaceObjectData;
  //PhysicalData::setupHeavySunLightPlanet( spaceObjectData );
  //PhysicalData::setupTwoBodySystem( spaceObjectData );
  //PhysicalData::setupThreeBodySystem( spaceObjectData );
  PhysicalData::setupAlotOfPlanets( spaceObjectData );
  buildScene_( spaceObjectData );
  const auto& sunData = spaceObjectData[0];
  const auto& sun = *spaceObjects_[0];
  const auto& planet = *spaceObjects_[1];

  sf::Clock clock;
  sf::Time totalTime;
  float timeScale = 1.0f;

  sf::Time dt = sf::seconds( 1.0f / 50.0f );
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while( mainWindow_.isOpen() )
  {
    sf::Time elapsedAbsoluteTime = clock.restart();
    timeSinceLastUpdate += elapsedAbsoluteTime * timeScale;
    while( timeSinceLastUpdate >= dt )
    {
      motionManager.updateLinearMotion( dt.asSeconds(), spaceObjects_ );

      timeSinceLastUpdate -= dt;
      totalTime += dt;
    }
    
    render_();
  }
}

void PhysicsTest::buildScene_( const std::vector< PhysicalData::PlanetData >& planetData )
{
  // Add one empty SceneNode for each scene layer
  for( size_t i = 0; i < LayerCount; ++i )
  {
    SceneNode::Ptr layer( new SceneNode() );
    sceneLayers_[i] = layer.get();
    sceneGraph_.attachChild( std::move( layer ) );
  }

  spaceObjects_.clear();
  for( size_t i = 0; i < planetData.size(); ++i )
  {
    const PhysicalData::PlanetData& data = planetData[i];
    unique_ptr< Planet > planet( new Planet( data.name, data.mass, data.radius, textureHolder_, csHandler_ ) );
    planet->setPosition( data.position );
    planet->setVelocity( data.velocity );

    spaceObjects_.push_back( planet.get() );

    sceneLayers_[ActionLayer]->attachChild( std::move( planet ) );
  }
}

void PhysicsTest::render_()
{
  mainWindow_.clear( sf::Color::Black );
  mainWindow_.setView( view_ );

  sceneGraph_.updateGraphics();
  mainWindow_.draw( sceneGraph_ );

  //renderDebugPrintouts_();
  mainWindow_.display();
}
