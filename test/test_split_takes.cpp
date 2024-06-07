#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_split_takes(int argc, char **argv)
{
  Board<3> board{
      {Piece(), Piece(), W_BISHOP},
      {Piece(), B_KING, Piece()},
      {Piece(), Piece(), Piece()}};
  Move m1 = Move_split(Coord(1, 1), Coord(2, 1), Coord(2, 0));
  Move m2 = Move_split(Coord(2, 1), Coord(1, 1), Coord(2, 2));
  Move m3 = Move_classic(Coord(0, 2), Coord(1, 1));
  Move m4 = Move_classic(Coord(1, 1), Coord(2, 0));
  board.move(m1);
  board.move(m2);
  board.move(m3);
  board.move(m4);
  bool res{double_equal(board.get_proba(Coord(2, 0)), 1.)};
  return !res;
}
