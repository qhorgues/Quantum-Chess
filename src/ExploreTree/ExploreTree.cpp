#include "ExploreTree.hpp"
#include "Move.hpp"
#include <algorithm>
#include <stdexcept>

NodeTree::NodeTree(Move m, double eval, TypeMinMax min_max)
 : m_sub_node(),
   m_move(m), 
   m_eval(eval), 
   m_min_max(min_max),
   m_cut_alpha_beta(false)
{
}

void NodeTree::add_node(NodeTree const& node)
{
  m_sub_node.push(node);
}

bool NodeTree::empty() const
{
  return m_sub_node.empty();
}

void NodeTree::setCutAlphaBeta()
{
  m_cut_alpha_beta = true;
}

bool NodeTree::checkCutAlphaBeta() const
{
  return m_cut_alpha_beta;
}

ExploreTree::ExploreTree(TypeMinMax root_min_max)
: m_root(
  Move_classic(Coord(0, 0), Coord(0, 0)), 
  0.,
  root_min_max)
{
}

NodeTree& ExploreTree::getRoot()
{
  return m_root;
}

void ExploreTree::keep_one_branch(NodeTree& new_root)
{
  m_root = std::move(new_root);
}