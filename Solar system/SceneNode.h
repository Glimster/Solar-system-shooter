#pragma once

// TODO, ska jag använda Transformable?
class SceneNode : /*public sf::Transformable,*/ public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr< SceneNode > Ptr;

	SceneNode();

  void attachChild( Ptr child );
  Ptr detachChild( const SceneNode& node );

private:
  virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;
  virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

  std::vector< Ptr > children_;
  SceneNode* parent_;
};