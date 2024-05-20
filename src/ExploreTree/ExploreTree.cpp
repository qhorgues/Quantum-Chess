#include "ExploreTree.hpp"
#include "Move.hpp"
#include <algorithm>
#include <stdexcept>

#if defined(UNUSED)
bool operator=(Move const& lhs, Move const& rhs) noexcept
{
  if (lhs.type == rhs.type) {
    switch (lhs.type) {
      case TypeMove::NORMAL:
        return lhs.normal.src == rhs.normal.src &&
        lhs.normal.arv == rhs.normal.arv;
      case TypeMove::SPLIT:
        return lhs.split.src == rhs.split.src &&
        lhs.split.arv1 == rhs.split.arv1 &&
        lhs.split.arv2 == rhs.split.arv2;
      case TypeMove::MERGE:
        return lhs.merge.src1 == rhs.merge.src1 &&
        lhs.merge.src2 == rhs.merge.src2 &&
        lhs.merge.arv == rhs.merge.arv;
      case TypeMove::PROMOTE:
        return lhs.promote.src == rhs.promote.src &&
        lhs.promote.arv == rhs.promote.arv &&
        lhs.promote.piece == rhs.promote.piece;
      default:
        return false;
    }
    return false;
}
#endif

bool NodeTree::cmpNodeTree(NodeTree const& lhs, NodeTree const& rhs) const
{
  if (lhs.m_min_max != rhs.m_min_max)
  {
    throw std::runtime_error("Invalid compare between Max-Node and Min-Node");
  }
}

#if defined(UNUSED)
void ExploreTree::keep_one_branch(Move const &move_play) {
  if (!m_root) {
    throw std::runtime_error("Explore tree is empty");
  }
  auto it_move{std::find_if(
      m_root.get().m_sub_node.begin(), m_root.get().m_sub_node.end(),
      [&move_play](Move const &m) -> bool {
          move_play == m
        }
      })};
  if (it_move == m_root.get().m_sub_node.end()) {
      throw std::runtime_error("Move not exist");
  }
  m_root = *it_move;
}
#endif
