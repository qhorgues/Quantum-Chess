#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_enpassant_with_capture(int argc, char **argv)
{
    Board<5> board{
        {W_ROOK, Piece(), Piece(), Piece(), Piece()},
        {B_PAWN, Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), W_PAWN, Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(), W_BISHOP, Piece()}};
    Move m = Move_split(Coord(4, 3), Coord(3, 2), Coord(2, 1));
    Move m1 = Move_classic(Coord(3, 1), Coord(1, 1));
    Move m2 = Move_classic(Coord(1, 0), Coord(2, 1));
    board.move(m);
    board.move(m1);
    board.move(m2);
    bool equal = double_equal(board.get_proba(Coord(3, 2)), 0.5);
    return !(board(2, 1).get_type() == TypePiece::PAWN &&
             equal &&
             board(1, 1).get_type() == TypePiece::EMPTY &&
             board(1, 0).get_type() == TypePiece::EMPTY);
}