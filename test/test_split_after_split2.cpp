#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_split_after_split2(int argc, char **argv)
{
    Board<4> board{
        {Piece(), Piece(), Piece(), Piece()},
        {Piece(), B_KING,  B_ROOK, Piece()},
        {Piece(),Piece(),Piece(),  Piece()},
        {W_QUEEN, Piece(), Piece(), W_KING}};
    Move m1 = Move_split(Coord(3, 0), Coord(2, 1), Coord(3, 2));
    Move m2 = Move_split(Coord(3, 2), Coord(1, 0), Coord(2, 1));
    board.move(m1);
    board.move(m2, true);
    bool res{double_equal(board.get_proba(Coord(3,3)), 1.)&&
    double_equal(board.get_proba(Coord(2,1)), 1./4) &&
    double_equal(board.get_proba(Coord(1,0)), 1./4) &&
    double_equal(board.get_proba(Coord(3,2)), 1./2) };
    return !res;
}