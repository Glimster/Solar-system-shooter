#pragma once
#include "Space.h"

class Game : sf::NonCopyable
{
public:
  Game(void);
  ~Game(void);

  void run();

private:
  void processEvents();
  void update( const sf::Time& timeStep );
  void render();

private:

  // Debug
  void setupDebug_();
  void updateFPSStats_( const sf::Time& elapsedTime );
  void renderDebugPrintouts_();

  sf::RenderWindow mainWindow_;
  sf::View GUIView_;

  Space space_;

  //bool initialized_;

  sf::Clock clock_;
  sf::Time totalTime_;

  // Maximum time (s) step for the physics simulations to not go bananas.
  // 
  // If too large, the physics will explode.
  // If too small, the rendering will be slow, especially if timeScale is large.
  sf::Time maxTimeStep_;

  // Debug
  sf::Font font_;
  sf::Text debugText_;
  sf::Text fpsText_;
  sf::Time fpsUpdateTime_;
  std::size_t fpsNumFrames_;
};