#include "stdafx.h"
#include "Game.h"

#include "Planet.h"
#include "StarShip.h"

#include "PhysicalConstants.h"

using namespace std;

Game::Game(void):
  mainWindow_( sf::VideoMode::getDesktopMode(), "Solar System" ),
  GUIView_( mainWindow_.getDefaultView() ),
  space_( mainWindow_ ),
  clock_(),
  totalTime_( sf::Time::Zero ),
  //maxTimeStep_( sf::seconds( 1.0f / 60.0f ) ), // TODO: how high can this value be set?
  maxTimeStep_( sf::seconds( 1.0f / 60.0f ) / 100.0f ),
  //maxTimeStep_( sf::seconds( 1.0f / 60.0f ) / 10.0f )
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
    sf::Time elapsedAbsoluteTime = clock_.restart();
    timeSinceLastUpdate += elapsedAbsoluteTime * Phys::PhysicalConstants::timeScale;
    
    while( timeSinceLastUpdate >= dt ) {
      //processEvents(); // Beh�ver denna verkligen g�ras s� ofta? Det h�r har ju att g�ra med physik juh!
      update( dt );

      timeSinceLastUpdate -= dt;
      totalTime_ += dt;
    }
    
    processEvents(); // �r det ok att den h�r ligger utanf�r innersta loopen?


    // Debug
    updateFPSStats_( elapsedAbsoluteTime );

    // TODO, add interpolation so that the rendered state always matches the current time!

    render();
  }
}

void Game::processEvents()
{
  // Ska inte Game sk�ta detta, inte Space!?
  space_.processEvents();
}

void Game::update( const sf::Time& timeStep )
{
  space_.update( timeStep );
}

void Game::render()
{
  mainWindow_.clear( sf::Color::Black );
  space_.render();
  renderDebugPrintouts_();
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

void Game::renderDebugPrintouts_()
{
  mainWindow_.setView( GUIView_ );

  { 
    // Time
    float days = totalTime_.asSeconds();
    string theString = "Elapsed time = " + to_string( days ) + " days\n";

    // Object data
    const vector< SpaceObject* >& spaceObjects = space_.getSpaceObjects();
    for( size_t i = 0; i < spaceObjects.size() - 1; ++i ) {
      Planet p = *dynamic_cast< Planet* >( spaceObjects[i] );
      Eigen::Vector2f pos = p.getPosition();

      float speed = sqrt( p.getVelocity()(0) * p.getVelocity()(0) + p.getVelocity()(1) * p.getVelocity()(1) );
      speed *= Phys::PhysicalConstants::au2m / 1.0e3f / Phys::PhysicalConstants::D2s;
      theString.append( p.getName() + " v = " + to_string( speed ) + " km/s" );

      float position = sqrt( p.getPosition()(0) * p.getPosition()(0) + p.getPosition()(1) * p.getPosition()(1) );
      theString.append( ",  pos = " + to_string( position ) + " au" );

      theString.append( "\n" );
    }

    StarShip ship = *dynamic_cast< StarShip* >( spaceObjects[spaceObjects.size() - 1] );
    Eigen::Vector2f pos = ship.getPosition();

    float speed = sqrt( ship.getVelocity()(0) * ship.getVelocity()(0) + ship.getVelocity()(1) * ship.getVelocity()(1) );
    speed *= Phys::PhysicalConstants::au2m / 1.0e3f / Phys::PhysicalConstants::D2s;
    theString.append( "Player 1: v = " + to_string( speed ) + " km/s" );

    float position = sqrt( ship.getPosition()(0) * ship.getPosition()(0) + ship.getPosition()(1) * ship.getPosition()(1) );
    theString.append( ",  pos = " + to_string( position ) + " au" );

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