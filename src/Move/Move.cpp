#include <Coord.hpp>
#include <TypePiece.hpp>
#include "Move.hpp"

bool Move::operator==(Move const &m) const noexcept
{
  if (type == m.type)
  {
    switch (type)
    {
    case TypeMove::NORMAL:
      return normal.src == m.normal.src &&
             normal.arv == m.normal.arv;
    case TypeMove::SPLIT:
      return split.src == m.split.src &&
             split.arv1 == m.split.arv1 &&
             split.arv2 == m.split.arv2;
    case TypeMove::MERGE:
      return merge.src1 == m.merge.src1 &&
             merge.src2 == m.merge.src2 &&
             merge.arv == m.merge.arv;
    case TypeMove::PROMOTE:
      return promote.src == m.promote.src &&
             promote.arv == m.promote.arv &&
             promote.piece == m.promote.piece;
    default:
      return false;
    }
  }
  return false;
}