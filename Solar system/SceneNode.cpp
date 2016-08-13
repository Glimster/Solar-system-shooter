#include "stdafx.h"
#include "SceneNode.h"

#include "Command.h"
#include "Category.h"

using namespace std;

SceneNode::SceneNode( Category::Type category ):
  children_(),
  parent_( nullptr ),
  defaultCategory_( category )
{}

void SceneNode::attachChild( Ptr child )
{
  child->parent_ = this;
  children_.push_back( std::move( child ) );
}

SceneNode::Ptr SceneNode::detachChild( const SceneNode& node )
{
  auto found = find_if( children_.begin(), children_.end(), 
    [&]( Ptr& p ) { return p.get() == &node; } );
  
  assert( found != children_.end() );

  Ptr result = std::move( *found );
  result->parent_ = nullptr;
  children_.erase( found );

  return result;
}

void SceneNode::update( sf::Time dt, CommandQueue& commands )
{
  updateCurrent_( dt, commands );

  for( const Ptr& child : children_ )
  {
    child->update( dt, commands );
  }
}

void SceneNode::onCommand( const Command& command, sf::Time dt )
{
  // NB, bitwise and intentional!
  if( getCategory() & command.category )
  {
    command.action( *this, dt );
  }

  for( Ptr& child : children_ )
  {
    child->onCommand( command, dt );
  }
}

unsigned int SceneNode::getCategory() const
{
  return defaultCategory_;
}

void SceneNode::print( string& string ) const
{
  this->printCurrent_( string );

  for( const Ptr& child : children_ )
  {
    child->print( string );
  }
}

void SceneNode::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
  states.transform *= transformable_.getTransform();

  drawCurrent_( target, states );

  for( const Ptr& child : children_ )
  {
    child->draw( target, states );
  }
}

void SceneNode::drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const
{}

void SceneNode::updateCurrent_( sf::Time dt, CommandQueue& commands )
{}

void SceneNode::printCurrent_( string& string ) const
{
  string.append( "SceneNode: category = " + to_string( getCategory() ) );
  printDefaultData_( string );
  string.append( "\n" );
}

void SceneNode::printDefaultData_( string& string ) const
{
  string.append( ", #c = " + to_string( children_.size() ) );
  if( parent_ == nullptr )
    string.append( ". Top node!" );
}