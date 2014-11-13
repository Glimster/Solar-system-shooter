#pragma once

class SceneNode;

struct Command
{
public:
  Command();
  
  // What to do
  std::function< void( SceneNode&/*, sf::Time*/ ) > action;

  // Who should recieve the command
  unsigned int category;
};

// Adapter for passing any derived (from SceneNode) object into a command.action
template< typename GameObject, typename Function >
std::function< void( SceneNode&/*, sf::Time*/ ) > derivedAction( Function fn )
{
  return [=]( SceneNode& node/*, sf::Time*/ )
  {
    assert( dynamic_cast< GameObject* >( &node ) != nullptr );

    fn( static_cast< GameObject& >( node ) );
  };
}