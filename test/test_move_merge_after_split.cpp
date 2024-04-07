#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_merge_after_split(int argc, char **argv)
{
    Board<4> board{
        {B_KING, Piece(), Piece(), Piece()},
        {B_ROOK, Piece(),  Piece(), Piece()},
        {Piece(),Piece(),Piece(),  Piece()},
        {W_QUEEN, Piece(), Piece(), W_KING}};
    Move m1 = Move_split(Coord(3, 0), Coord(3, 1), Coord(3, 2));
    Move m2 = Move_merge(Coord(3, 1), Coord(3, 2), Coord(3, 0));
    board.move(m1);
    board.move(m2, true);
    bool res{double_equal(board.get_proba(Coord(3,0)), 1.)};
    return !res;
}