#pragma once

struct Command;

// TODO, ska jag använda Transformable? Skulle isf vara för att uppdatera tillstånd (positioner) för barnen,
// som är beroende av föräldern...
class SceneNode : /*public sf::Transformable,*/ public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr< SceneNode > Ptr;

	SceneNode();

  void attachChild( Ptr child );
  Ptr detachChild( const SceneNode& node );

  virtual void updateGraphics();

  // Excecutes command if the right category and the forwards to children
  void onCommand( const Command& command );
  virtual unsigned int getCategory() const;

private:
  virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const final;
  
  // TODO: currently separated, but right now it would be possible to always update the
  // graphics (sprites etc.) in drawCurrent_()
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;
  virtual void updateCurrentGraphics_();

  std::vector< Ptr > children_;
  SceneNode* parent_;
};