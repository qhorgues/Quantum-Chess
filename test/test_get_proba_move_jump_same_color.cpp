#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_get_proba_move_jump_same_color(int argc, char **argv)
{
  Board<3> board{
      {W_KING, Piece(), Piece()},
      {Piece(), Piece(), Piece()},
      {Piece(), Piece(), W_KNIGHT}};
  Move m1 = Move_split(Coord(2, 2), Coord(0, 1), Coord(1, 0));
  Move m5 = Move_classic(Coord(0, 0), Coord(0, 1));
  board.move(m1);
  bool res{double_equal(board.get_proba_move(m5), 1. / 2)};
  return !res;
}