#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_promotion_with_split_before(int argc, char **argv)
{
    Board<3> board{
        {W_ROOK, Piece(), Piece()},
        {B_PAWN, Piece(), Piece()},
        {Piece(), W_QUEEN, Piece()}};
    Move m1 = Move_split(Coord(2, 1), Coord(2, 0), Coord(2, 2));
    Move m2 = Move_promote(Coord(1, 0), Coord(2, 0), TypePiece::BISHOP);
    board.move(m1);
    board.move(m2);
    bool res = !((board(2, 0).get_type() == TypePiece::BISHOP &&
                  board(2, 0).get_color() == Color::BLACK &&
                  board(1, 0).get_type() == TypePiece::EMPTY &&
                  double_equal(board.get_proba(Coord(2, 2)), 1.)) ||
                 (board(2, 0).get_type() == TypePiece::QUEEN &&
                  board(2, 0).get_color() == Color::WHITE &&
                  board(1, 0).get_type() == TypePiece::PAWN &&
                  double_equal(board.get_proba(Coord(2, 2)), 0.)));
    return res;
}