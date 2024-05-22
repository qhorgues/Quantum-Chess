#include "ExploreTree.hpp"
#include "Move.hpp"
#include <algorithm>
#include <stdexcept>

bool NodeTree::cmpNodeTree(NodeTree const &lhs, NodeTree const &rhs) const
{
  if (lhs.m_min_max != rhs.m_min_max)
  {
    throw std::runtime_error("Invalid compare between Max-Node and Min-Node");
  }
  if (lhs.m_min_max == TypeMinMax::MAX)
  {
    return lhs.m_eval < rhs.m_eval;
  }
  else
  {
    return lhs.m_eval > rhs.m_eval;
  }
}

void NodeTree::add_node(std::unique_ptr<NodeTree>&& node)
{
  m_sub_node.push(node);
}

ExploreTree::ExploreTree()
: m_root(std::move(std::make_unique<NodeTree>{}));
{
}

void ExploreTree::keep_one_branch(std::unique_ptr<NodeTree>&& new_root)
{
  m_root = std::move(new_root);
}
