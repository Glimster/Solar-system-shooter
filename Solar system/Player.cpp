#include "stdafx.h"
#include "Player.h"

#include "Command.h"
#include "Category.h"
#include "CommandQueue.h"

#include "StarShip.h"

void Player::handleEvent( const sf::Event& event, CommandQueue& commands )
{
  switch( event.type )
  {
    case sf::Event::KeyPressed:
    {
      createThrusterCommand_( commands, event.key.code, true );
      break;
    }
    case sf::Event::KeyReleased:
    {
      createThrusterCommand_( commands, event.key.code, false );
      break;
    }
  }
}

void Player::handleRealtimeInput( CommandQueue& commands )
{

}

void Player::createThrusterCommand_( CommandQueue& commands, sf::Keyboard::Key key, bool isPressed )
{
  if( key == sf::Keyboard::Up )
  {
    Command forwardThrusters;
    forwardThrusters.category = Category::Player;
    forwardThrusters.action = derivedAction< StarShip >( [isPressed]( StarShip& ship ){ ship.setAftThrusters( isPressed ); } );
    commands.push( forwardThrusters );
  }
  else if( key == sf::Keyboard::Left )
  {
    Command forwardThrusters;
    forwardThrusters.category = Category::Player;
    forwardThrusters.action = derivedAction< StarShip >( [isPressed]( StarShip& ship ){ ship.setLeftRotationThrusters( isPressed ); } );
    commands.push( forwardThrusters );
  }
  else if( key == sf::Keyboard::Right )
  {
    Command forwardThrusters;
    forwardThrusters.category = Category::Player;
    forwardThrusters.action = derivedAction< StarShip >( [isPressed]( StarShip& ship ){ ship.setRightRotationThrusters( isPressed ); } );
    commands.push( forwardThrusters );
  }
}