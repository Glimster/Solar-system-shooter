#include "stdafx.h"
#include "Space.h"

#include "Planet.h"
#include "StarShip.h"

#include "PhysicalConstants.h"
#include "MathUtil.h"

#include "Integrator.h"

using namespace std;

Space::Space( sf::RenderWindow& mainWindow ):
  mainWindow_( mainWindow ),
  csHandler_(),
  textureHolder_(),
  playerView_(),
  planets_()
{
  {
    const float earthToSun = 1.5e11f / Phys::PhysicalConstants::au2m; // au
    const float neptuneToSun = 4495.1e9f / Phys::PhysicalConstants::au2m; // au
    const float screenWidth = mainWindow_.getDefaultView().getSize().x;
    const float screenHeight = mainWindow_.getDefaultView().getSize().y;
    csHandler_ = CoordinateSystemHandler( min( screenWidth, screenHeight ) / ( neptuneToSun * 2.0f ), 
                                          Eigen::Vector2f( screenWidth / 2.0f, screenHeight / 2.0f ) );

    Eigen::Vector2f origo( 0.0f, 0.0f );
    csHandler_.convertToDisplayCS( origo );
    playerView_.setCenter( origo(0), origo(1) );

    const float height = earthToSun * 3.0f * csHandler_.unitOfLength2Pixel();
    //const float height = neptuneToSun / 2.0f * 3.0f * csHandler_.meterToPixel();
    const float width = height * screenWidth / screenHeight;
    playerView_.setSize( width, height );
  }

  loadTextures_();
  setupPlanets_();

  {
    //float mass = 0.330e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float mass = 0.330e3f / Phys::PhysicalConstants::S2kg; // Solar mass
    player_ = unique_ptr< StarShip >( new StarShip( mass, Textures::ID::StarShip, textureHolder_, csHandler_ ) );
    player_->setPosition( Eigen::Vector2f( 1.0f, 1.0f ) );
    player_->setVelocity( Eigen::Vector2f( 0.0f, 0.0f ) );
    player_->setOrientation( Eigen::Vector2f( 1.0f, 0.0f ) );
  }

  // Add pointers to all objects
  for( size_t i = 0; i < planets_.size(); ++i ) {
    spaceObjects_.push_back( planets_[i].get() );
  }
  
  spaceObjects_.push_back( player_.get() );
}

Space::~Space()
{}

void Space::processEvents()
{
  sf::Event event;
  while( mainWindow_.pollEvent( event ) )
  {
    switch( event.type ) 
    {
      case sf::Event::KeyPressed:
      {
        handlePlayerInput_( event.key.code, true );
        break;
      }
      case sf::Event::KeyReleased:
      {
        handlePlayerInput_( event.key.code, false );
        break;
      }
      case sf::Event::Closed:
      {
        mainWindow_.close();
        break;
      }
    }
  }
}

void Space::update( const sf::Time& timeStep )
{
  const float dt = timeStep.asSeconds();

  // TODO, städa upp detta och lägg på lämplig plats:
  enum class Integration { EulerForward, RK4 };
  enum class Technique { standard, lambda, functor };

  { // Linear motion
    Integration integration = Integration::EulerForward;
    Technique technique = Technique::functor;

    vector< Eigen::Vector2f > drs, dvs;
    switch( technique )
    {
      case( Technique::standard ):
      {
        if( integration == Integration::EulerForward ) {
          MathUtil::eulerStep( spaceObjects_, dt, drs, dvs );
        }
        else {
          assert( integration == Integration::RK4 );
          MathUtil::RK4Step( spaceObjects_, dt, drs, dvs );
        }
        break;
      }
      case( Technique::lambda ):
      {
        assert( integration != Integration::RK4 );
        Integrator::N2EulerStepLambdas( spaceObjects_, dt, drs, dvs );
        break;
      }
      case( Technique::functor ):
      {
        if( integration == Integration::EulerForward ) {
          Integrator::N2EulerStepFunctorsState( spaceObjects_, dt, drs, dvs );
        }
        else {
          assert( integration == Integration::RK4 );
          Integrator::N2RK4StepFunctors( spaceObjects_, dt, drs, dvs );
        }
        break;
      }
      default:
        assert( false );
        break;
    }

    for( size_t i = 0; i != spaceObjects_.size(); ++i )
    {
      spaceObjects_[i]->updateState( drs[i], dvs[i] );
    }
  }

  { // Angular motion
    Integration integration = Integration::EulerForward; // Must be sufficient
    Technique technique = Technique::functor;

    float dL, dTheta;
    switch( technique )
    {
      case( Technique::standard ):
      {
        assert( integration != Integration::RK4 );
        const float torque = player_->computeTorque();
        dL = torque * dt;

        const float angularVelocity = player_->getAngularMomentum() / player_->getMomentOfInertia();
        dTheta = angularVelocity * dt;
        break;
      }
      case( Technique::lambda ):
      {
        assert( integration != Integration::RK4 );
        Integrator::N2RotEulerStepLambdas( *player_, dt, dL, dTheta );
        break;
      }
      case( Technique::functor ):
      {
        if( integration == Integration::EulerForward ) {
          Integrator::N2RotEulerStepFunctor( *player_, dt, dL, dTheta );
        }
        else {
          assert( integration == Integration::RK4 );
          Integrator::N2RotRK4StepFunctor( *player_, dt, dL, dTheta );
        }
        break;
      }
      default:
        assert( false );
        break;
    }

    player_->updateAngularMomentum( dL );
    player_->rotate( dTheta );
  }
}

void Space::render()
{
  mainWindow_.setView( playerView_ );

  for( const unique_ptr< Planet >& p : planets_ )
  {
    p->updateGraphics(); // TODO: not so nice, when to update sprite/shape?
    p->render( mainWindow_ );
  }

  player_->updateGraphics(); // TODO: not so nice, when to update sprite/shape?
  player_->render( mainWindow_ );
}

void Space::handlePlayerInput_( sf::Keyboard::Key key, bool isPressed )
{
  if( key == sf::Keyboard::Up )
  {
    player_->setAftThrusters( isPressed );
  }
  else if( key == sf::Keyboard::Left )
  {
    player_->setLeftRotationThrusters( isPressed );
  }
  else if( key == sf::Keyboard::Right )
  {
    player_->setRightRotationThrusters( isPressed );
  }
}

void Space::loadTextures_()
{
  textureHolder_.load( Textures::ID::Mercury,   "Resources/Mercury.jpg" );
  textureHolder_.load( Textures::ID::Venus,     "Resources/Venus.jpg" );
  textureHolder_.load( Textures::ID::Earth,     "Resources/Earth.jpg" );
  textureHolder_.load( Textures::ID::Mars,      "Resources/Mars.jpg" );
  textureHolder_.load( Textures::ID::Jupiter,   "Resources/Jupiter.jpg" );
  textureHolder_.load( Textures::ID::Saturn,    "Resources/Saturn.jpg" );
  textureHolder_.load( Textures::ID::Uranus,    "Resources/Uranus.jpg" );
  textureHolder_.load( Textures::ID::Neptune,   "Resources/Neptune.jpg" );

  textureHolder_.load( Textures::ID::Sun,       "Resources/Sun.jpg" );

  textureHolder_.load( Textures::ID::StarShip,  "Resources/StarShip.jpg" );
}

void Space::setupPlanets_()
{
  // Data från http://nssdc.gsfc.nasa.gov/planetary/factsheet/
  float mult = 1000.0f;
  {

    float mass = 0.330e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 4.879e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 47.9e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 57.9e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > mercury( new Planet( "Mercury", mass, radius * mult, Textures::ID::Mercury, textureHolder_, csHandler_ ) );
    mercury->setPosition( Eigen::Vector2f( -averageDistanceToSun, 0.0f ) );
    mercury->setVelocity( Eigen::Vector2f( 0.0f, approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( mercury ) );
  }
  {
    float mass = 4.87e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 12.104e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 35.0e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 108.2e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > venus( new Planet( "Venus", mass * 1e3f, radius * mult, Textures::ID::Venus, textureHolder_, csHandler_ ) );
    venus->setPosition( Eigen::Vector2f( -averageDistanceToSun, 0.0f ) );
    venus->setVelocity( Eigen::Vector2f( 0.0f, approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( venus ) );
  }
  {
    float mass = 5.97e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 12.756e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 29.8e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 149.6e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > earth( new Planet( "Earth", mass, radius * mult, Textures::ID::Earth, textureHolder_, csHandler_ ) );
    earth->setPosition( Eigen::Vector2f( -averageDistanceToSun, 0.0f ) );
    earth->setVelocity( Eigen::Vector2f( 0.0f, approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( earth ) );
  }
  {
    float mass = 0.642e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 6.792e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 24.1e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 227.9e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > mars( new Planet( "Mars", mass, radius * mult, Textures::ID::Mars, textureHolder_, csHandler_ ) );
    mars->setPosition( Eigen::Vector2f( -averageDistanceToSun, 0.0f ) );
    mars->setVelocity( Eigen::Vector2f( 0.0f, approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( mars ) );
  }
  {
    float mass = 1898e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 142.984e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 13.1e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 778.6e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > jupiter( new Planet( "Jupiter", mass, radius * mult, Textures::ID::Jupiter, textureHolder_, csHandler_ ) );
    jupiter->setPosition( Eigen::Vector2f( averageDistanceToSun, 0.0f ) );
    jupiter->setVelocity( Eigen::Vector2f( 0.0f, -approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( jupiter ) );
  }
  {
    float mass = 568e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 120.536e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 9.7e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 1433.5e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > saturn( new Planet( "Saturn", mass, radius * mult, Textures::ID::Saturn, textureHolder_, csHandler_ ) );
    saturn->setPosition( Eigen::Vector2f( 0.0f, averageDistanceToSun ) );
    saturn->setVelocity( Eigen::Vector2f( approxSpeedRelativeToSun, 0.0f ) ); 
    planets_.push_back( move( saturn ) );
  }
  {
    float mass = 86.8e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 51.118e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 6.8e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 2872.5e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > uranus( new Planet( "Uranus", mass, radius * mult, Textures::ID::Uranus, textureHolder_, csHandler_ ) );
    uranus->setPosition( Eigen::Vector2f( 0.0f, -averageDistanceToSun ) );
    uranus->setVelocity( Eigen::Vector2f( -approxSpeedRelativeToSun, 0.0f ) ); 
    planets_.push_back( move( uranus ) );
  }
  {
    float mass = 102.0e24f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 49.528e6f / 2.0f / Phys::PhysicalConstants::au2m; // au
    float approxSpeedRelativeToSun = 5.4e3f / Phys::PhysicalConstants::au2m * Phys::PhysicalConstants::D2s; // au / D
    float averageDistanceToSun = 4495.1e9f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > neptune( new Planet( "Neptune", mass, radius * mult, Textures::ID::Neptune, textureHolder_, csHandler_ ) );
    neptune->setPosition( Eigen::Vector2f( -averageDistanceToSun, 0.0f ) );
    neptune->setVelocity( Eigen::Vector2f( 0.0f, approxSpeedRelativeToSun ) ); 
    planets_.push_back( move( neptune ) );
  }
  {
    float mass = 1.989e30f / Phys::PhysicalConstants::S2kg; // Solar mass
    float radius = 6.955e8f / Phys::PhysicalConstants::au2m; // au
    unique_ptr< Planet > sun( new Planet( "Sun", mass, radius * 10.0f, Textures::ID::Sun, textureHolder_, csHandler_ ) );
    sun->setPosition( Eigen::Vector2f( 0.0f, 0.0f ) );
    sun->setVelocity( Eigen::Vector2f( 0.0f, 0.0f ) );
    planets_.push_back( move( sun ) );
  }
}