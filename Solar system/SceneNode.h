#pragma once
#include "Category.h"

struct Command;
class CommandQueue;

class SceneNode : public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr< SceneNode > Ptr;

	SceneNode( Category::Type category = Category::None );

  void attachChild( Ptr child );
  Ptr detachChild( const SceneNode& node );

  void update( sf::Time dt, CommandQueue& commands );

  // Excecutes command if the right category and the forwards to children
  void onCommand( const Command& command, sf::Time dt );
  virtual unsigned int getCategory() const;

  void print( std::string& string ) const;

protected:
  void printDefaultData_( std::string& string ) const;

private:
  virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const final;
  
  virtual void drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const;  
  virtual void updateCurrent_( sf::Time dt, CommandQueue& commands );

  virtual void printCurrent_( std::string& string ) const;
protected:
  sf::Transformable transformable_;

private:
  std::vector< Ptr > children_;
  SceneNode* parent_;

  Category::Type defaultCategory_;
};