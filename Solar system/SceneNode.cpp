#include "stdafx.h"
#include "SceneNode.h"

#include "Command.h"
#include "Category.h"

using namespace std;

SceneNode::SceneNode():
  children_(),
  parent_( nullptr )
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

void SceneNode::onCommand( const Command& command )
{
  // NB, bitwise and intentional!
  if( getCategory() & command.category )
  {
    command.action( *this );
  }

  for( Ptr& child : children_ )
  {
    child->onCommand( command );
  }
}

unsigned int SceneNode::getCategory() const
{
  return Category::Scene;
}

void SceneNode::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
  //states.transform *= getTransform();

  drawCurrent_( target, states );

  for( const Ptr& child : children_ )
  {
    child->draw( target, states );
  }
}

void SceneNode::updateGraphics()
{
  updateCurrentGraphics_();
  
  for (const Ptr& child : children_)
  {
    child->updateGraphics();
  }
}

void SceneNode::drawCurrent_( sf::RenderTarget& target, sf::RenderStates states ) const
{}

void SceneNode::updateCurrentGraphics_()
{}