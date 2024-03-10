#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_promotion_with_capture(int argc, char **argv)
{
    Board<3> board{
        {W_ROOK, Piece(), Piece()},
        {B_PAWN, Piece(), Piece()},
        {Piece(), W_QUEEN, Piece()}};
    Move m = Move_promote(Coord(1, 0), Coord(2, 1), TypePiece::BISHOP);
    board.move(m);
    return !(board(2, 1).get_type() == TypePiece::BISHOP &&
             board(2, 1).get_color() == Color::BLACK &&
             double_equal(board.get_proba(Coord(1, 0)), 0.) &&
             board(1, 0).get_type() == TypePiece::EMPTY);
}