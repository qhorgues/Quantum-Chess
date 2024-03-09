#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_promotion(int argc, char* argv[])
{
    Board<3> board{
        {W_ROOK, Piece(), Piece()},
        {B_PAWN, Piece(), Piece()},
        {Piece(), Piece(), Piece()}};
    Move m = Move_promote(Coord(1, 0), Coord(2, 0), TypePiece::BISHOP);
    board.move_promotion(m);
    return board(2, 0).get_type() == TypePiece::BISHOP &&
           double_equal(board.get_proba(Coord(1, 0)), 0.) &&
           board(1, 0).get_type() == TypePiece::EMPTY;
}

