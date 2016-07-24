#pragma once

class SceneNode;
class CommandQueue;

class Player
{
public:
  void handleEvent( const sf::Event& event, CommandQueue& commands );
  void handleRealtimeInput( CommandQueue& commands );

private:

  // TODO, inte s� snygg l�sning
  void createThrusterCommand_( CommandQueue& commands, sf::Keyboard::Key key, bool isPressed );

};