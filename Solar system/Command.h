#pragma once

class SpaceObject;

struct Command
{
public:
  Command(void);
  
  std::function< void( SpaceObject&, sf::Time ) > action;
};

