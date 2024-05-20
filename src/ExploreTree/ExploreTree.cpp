#include "ExploreTree.hpp"
#include "Move.hpp"
#include <algorithm>
#include <stdexcept>

bool operator=(Move const& lhs, Move const& rhs) noexcept
{
  if (m.type == move_play.type) {
    switch (m.type) {
      case TypeMove::NORMAL:
        return m.normal.src == move_play.normal.src &&
        m.normal.arv == move_play.normal.arv;
      case TypeMove::SPLIT:
        return m.split.src == move_play.split.src &&
        m.split.arv1 == move_play.split.arv1 &&
        m.split.arv2 == move_play.split.arv2;
      case TypeMove::MERGE:
        return m.merge.src1 == move_play.merge.src1 &&
        m.merge.src2 == move_play.merge.src2 &&
        m.merge.arv == move_play.merge.arv;
      case TypeMove::PROMOTE:
        return m.promote.src == move_play.promote.src &&
        m.promote.arv == move_play.promote.arv &&
        m.promote.piece == move_play.promote.piece;
      default:
        return false;
    }
    return false;
}

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

