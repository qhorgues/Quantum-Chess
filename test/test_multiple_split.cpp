#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_multiple_split(int argc, char **argv)
{
  Board<3> board{
      {Piece(), Piece(), B_BISHOP},
      {Piece(), B_KING, Piece()},
      {Piece(), Piece(), Piece()}};
  Move m1 = Move_split(Coord(1, 1), Coord(2, 1), Coord(2, 2));
  Move m2 = Move_split(Coord(2, 2), Coord(1, 1), Coord(1, 2));
  Move m3 = Move_classic(Coord(1, 2), Coord(2, 2));
  Move m4 = Move_split(Coord(1, 1), Coord(0, 0), Coord(0, 1));
  board.move(m1);
  board.move(m2);
  board.move(m3);
  board.move(m4);
  bool res{double_equal(board.get_proba(Coord(0, 2)), 1.)};
  return !res;
}
