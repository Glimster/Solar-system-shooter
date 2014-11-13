#include "stdafx.h"
#include "Space.h"

#include "Planet.h"
#include "StarShip.h"

#include "PhysicalData.h"
#include "PhysicalConstants.h"
#include "MathUtil.h"

#include "Integrator.h"

using namespace std;

Space::Space( sf::RenderWindow& mainWindow ):
  mainWindow_( mainWindow ),
  csHandler_(),
  textureHolder_(),
  playerView_(),
  commandQueue_(),
  sceneLayers_(),
  sceneGraph_(),
  spaceObjects_(),
  player_()
{
  {
    const float earthToSun = 1.0; // au
    const float neptuneToSun = 30.0; // au
    const float screenWidth = mainWindow_.getDefaultView().getSize().x;
    const float screenHeight = mainWindow_.getDefaultView().getSize().y;
    csHandler_ = CoordinateSystemHandler( min( screenWidth, screenHeight ) / ( neptuneToSun * 2.0f ), 
                                          Eigen::Vector2f( screenWidth / 2.0f, screenHeight / 2.0f ) );

    Eigen::Vector2f origo( 0.0f, 0.0f );
    csHandler_.convertToDisplayCS( origo );
    playerView_.setCenter( origo(0), origo(1) );

    const float height = earthToSun * 3.0f * csHandler_.unitOfLength2Pixel();
    //const float height = neptuneToSun * csHandler_.unitOfLength2Pixel();
    const float width = height * screenWidth / screenHeight;
    playerView_.setSize( width, height );
  }

  loadTextures_();
  buildScene_();
}

Space::~Space()
{}

void Space::update( const sf::Time& timeStep )
{
  const float dt = timeStep.asSeconds();

  // TODO, frågan är om jag kan stoppa den här nån annanstans, t.ex. utanför inre fysik-loopen,
  // eftersom jag inte använder dt!?!?!?
  while( !commandQueue_.isEmpty() )
  {
    sceneGraph_.onCommand( commandQueue_.pop() );
  }

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

  // TODO, perhaps not so nice, consider alternative
  for( SpaceObject* obj : spaceObjects_ )
  {
    obj->updateGraphics();
  }
  mainWindow_.draw( sceneGraph_ );
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

void Space::buildScene_()
{
  // Add one empty SceneNode for each scene layer
  for( size_t i = 0; i < LayerCount; ++i )
  {
    SceneNode::Ptr layer( new SceneNode() );
    sceneLayers_[i] = layer.get();
    sceneGraph_.attachChild( std::move( layer ) );
  }

  vector< PhysicalData::PlanetData > planetData;
  //PhysicalData::setupRealisticSolarSystem( planetData );
  PhysicalData::setupSolarSystem( planetData );
  
  for( size_t i = 0; i < planetData.size(); ++i )
  {
    const PhysicalData::PlanetData& data = planetData[i];
    unique_ptr< Planet > planet( new Planet( data.name, data.mass, data.radius, textureHolder_, csHandler_ ) );
    planet->setPosition( data.position );
    planet->setVelocity( data.velocity );
    
    spaceObjects_.push_back( planet.get() );

    sceneLayers_[ActionLayer]->attachChild( std::move( planet ) );
  }

  { // Create starship
    const float mass = 1e-10f; // Solar mass
    unique_ptr< StarShip > player( new StarShip( mass, textureHolder_, csHandler_ ) );
    player->setPosition( Eigen::Vector2f( 1.0f, 1.0f ) );
    player->setVelocity( Eigen::Vector2f( 0.0f, 0.0f ) );
    player->setOrientation( Eigen::Vector2f( 1.0f, 0.0f ) );

    player_ = player.get();

    spaceObjects_.push_back( player.get() );
    sceneLayers_[ActionLayer]->attachChild( std::move( player ) );
  }
}