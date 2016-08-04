#include "stdafx.h"
#include "Game.h"

#include "PhysicalObject.h"
#include "Planet.h"
#include "StarShip.h"

#include "PhysicalConstants.h"

using namespace std;

Game::Game(void):
  mainWindow_( sf::VideoMode::getDesktopMode(), "Solar System" ),
  GUIView_( mainWindow_.getDefaultView() ),
  space_( mainWindow_ ),
  isPaused_( false ),
  clock_(),
  totalTime_( sf::Time::Zero ),
  maxTimeStep_( sf::seconds( 1.0f / 60.0f ) ), // TODO: how high can this value be set?
  //maxTimeStep_( sf::seconds( 1.0f / 60.0f ) / 100.0f ),
  //maxTimeStep_( sf::seconds( 1.0f / 60.0f ) / 10.0f ),
  // Debug
  font_(),
  debugText_(),
  fpsText_(),
  fpsUpdateTime_(),
  fpsNumFrames_( 0 )
{
  Phys::PhysicalConstants::timeScale = 10.0f;

  // Debug
  setupDebug_();
}

Game::~Game(void)
{}

void Game::run()
{
  clock_ = sf::Clock(); // Start the clock

  // Game logic (and physics simulation) time step
  sf::Time dt = sf::seconds( 1.0f / 60.0f ) * Phys::PhysicalConstants::timeScale;
  dt = dt > maxTimeStep_ ? maxTimeStep_ : dt; // Ensures that physics simulation will not explode
  
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while( mainWindow_.isOpen() )
  {
    const sf::Time elapsedAbsoluteTime = clock_.restart();
    timeSinceLastUpdate += elapsedAbsoluteTime * Phys::PhysicalConstants::timeScale;
    
    while( timeSinceLastUpdate >= dt ) {
      //processEvents(); // Behöver denna verkligen göras så ofta? Det här har ju att göra med physik juh!
      if( !isPaused_ ) // Ska man inte göra så att tiden också stannar när det är pausat?
        update( dt ); 

      timeSinceLastUpdate -= dt;
      totalTime_ += dt;
    }
    
    processEvents(); // Är det ok att den här ligger utanför innersta loopen?


    // Debug
    updateFPSStats_( elapsedAbsoluteTime );

    // TODO, add interpolation so that the rendered state always matches the current time!

    render();
  }
}

void Game::processEvents()
{
  CommandQueue& commands = space_.getCommandQueue();

  sf::Event event;
  while( mainWindow_.pollEvent( event ) )
  {
    player_.handleEvent( event, commands );

    switch( event.type )
    {
      case sf::Event::LostFocus:
      {
        isPaused_ = true;
        break;
      }
      case sf::Event::GainedFocus:
      {
        isPaused_ = false;
        break;
      }
      case sf::Event::Closed:
      {
        mainWindow_.close();
        break;
      }
    }
  }

  player_.handleRealtimeInput( commands );
}

void Game::update( const sf::Time& timeStep )
{
  space_.update( timeStep );
}

void Game::render()
{
  mainWindow_.clear( sf::Color::Black );
  space_.render();
  renderDebugPrintouts_( true );
  mainWindow_.display();
}

void Game::setupDebug_()
{
  bool result = font_.loadFromFile( "Resources/garreg.ttf" );
  if( !result )
  {
    throw runtime_error( "Failed to load font" );
  }

  debugText_.setFont( font_ );
  debugText_.setCharacterSize( 20 ); // in pixels, not points!
  debugText_.setColor( sf::Color::White );

  fpsText_.setFont( font_ );
  fpsText_.setPosition( float( mainWindow_.getSize().y - 100 ), 0.0f );
  fpsText_.setCharacterSize( 20 );
}

void Game::renderDebugPrintouts_( bool onlyFPS )
{
  mainWindow_.setView( GUIView_ );

  if( !onlyFPS )
  { 
    // Time
    float days = totalTime_.asSeconds();
    string theString = "Elapsed time = " + to_string( days ) + " days\n";

    // Object data
    const vector< PhysicalObject* >& physicalObjects = space_.getPhysicalObjects();
    for( size_t i = 0; i < physicalObjects.size() - 1; ++i ) {
      Eigen::Vector2f pos = physicalObjects[i]->getPosition();

      float speed = sqrt( physicalObjects[i]->getVelocity()( 0 ) * physicalObjects[i]->getVelocity()( 0 ) + physicalObjects[i]->getVelocity()( 1 ) * physicalObjects[i]->getVelocity()( 1 ) );
      speed *= Phys::PhysicalConstants::au2m / 1.0e3f / Phys::PhysicalConstants::D2s;
      theString.append( dynamic_cast<Planet*>(physicalObjects[i] )->getName() + " v = " + to_string( speed ) + " km/s" );

      float position = sqrt( physicalObjects[i]->getPosition()( 0 ) * physicalObjects[i]->getPosition()( 0 ) + physicalObjects[i]->getPosition()( 1 ) * physicalObjects[i]->getPosition()( 1 ) );
      theString.append( ",  pos = " + to_string( position ) + " au" );

      theString.append( "\n" );
    }

    //StarShip ship = *dynamic_cast< StarShip* >( physicalObjects[physicalObjects.size() - 1] );
    Eigen::Vector2f pos = physicalObjects[physicalObjects.size() - 1]->getPosition();

    float speed = sqrt( physicalObjects[physicalObjects.size() - 1]->getVelocity()( 0 ) * physicalObjects[physicalObjects.size() - 1]->getVelocity()( 0 ) + physicalObjects[physicalObjects.size() - 1]->getVelocity()( 1 ) * physicalObjects[physicalObjects.size() - 1]->getVelocity()( 1 ) );
    speed *= Phys::PhysicalConstants::au2m / 1.0e3f / Phys::PhysicalConstants::D2s;
    theString.append( "Player 1: v = " + to_string( speed ) + " km/s" );

    float position = sqrt( physicalObjects[physicalObjects.size() - 1]->getPosition()( 0 ) * physicalObjects[physicalObjects.size() - 1]->getPosition()( 0 ) + physicalObjects[physicalObjects.size() - 1]->getPosition()( 1 ) * physicalObjects[physicalObjects.size() - 1]->getPosition()( 1 ) );
    theString.append( ",  pos = " + to_string( position ) + " au" );

    theString.append( ", ft = " + boolToString_( dynamic_cast<StarShip*>(physicalObjects[physicalObjects.size() - 1] )->aftThrusters_ ) );
    theString.append( ", lt = " + boolToString_( dynamic_cast<StarShip*>(physicalObjects[physicalObjects.size() - 1] )->leftRotationThrusters_ ) );
    theString.append( ", rt = " + boolToString_( dynamic_cast<StarShip*>(physicalObjects[physicalObjects.size() - 1] )->rightRotationThrusters_ ) );

    theString.append( "\n" );

    debugText_.setString( theString );
    mainWindow_.draw( debugText_ );
  }

  mainWindow_.draw( fpsText_ );
}

void Game::updateFPSStats_( const sf::Time& elapsedTime )
{
	fpsUpdateTime_ += elapsedTime;
	fpsNumFrames_ += 1;

	if( fpsUpdateTime_ >= sf::seconds( 1.0f ) )
	{
		fpsText_.setString(
			"Frames / Second = " + to_string( fpsNumFrames_ ) + "\n" +
			"Time / Update = " + to_string( fpsUpdateTime_.asMicroseconds() / fpsNumFrames_ ) + "us");
							 
		fpsUpdateTime_ -= sf::seconds( 1.0f );
		fpsNumFrames_ = 0;
	}
}