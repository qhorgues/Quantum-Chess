#include <math_utility.hpp>
#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>

int test_move_pawn_two_step(int argc, char* argv[])
{
    Board<5> board{
        {W_ROOK, Piece(), Piece(),Piece(), Piece()},
        {B_PAWN, Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(),Piece(),Piece()},
        {Piece(), Piece(), Piece(),Piece(),Piece()},
        {Piece(), Piece(), Piece(),Piece(),Piece()}};
    Move m = Move_classic(Coord(1,0), Coord(3,0));
    board.move(m);
    return board(3, 0).get_type() == TypePiece::PAWN &&
           double_equal(board.get_proba(Coord(1, 0)), 0.) &&
           board(1, 0).get_type() == TypePiece::EMPTY;
}