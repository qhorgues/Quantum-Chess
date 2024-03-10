#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <Coord.hpp>
#include <math_utility.hpp>

int test_split_in_non_empty_case(int argc, char** argv)
{
    Board<1, 4> board {
        {W_ROOK, Piece(), Piece(), Piece()}
    };

    Move m1 {Move_split(Coord(0, 0), Coord(0, 1), Coord(0, 3))};
    Move m2 {Move_split(Coord(0, 1), Coord(0, 2), Coord(0, 3))};

    board.move(m1);
    board.move(m2);
    
    bool ret {
        board(0, 1).get_type() == TypePiece::ROOK &&
        board(0, 2).get_type() == TypePiece::ROOK &&
        board(0, 3).get_type() == TypePiece::ROOK &&
        double_equal(board.get_proba(Coord(0, 1)), 0.5) &&
        double_equal(board.get_proba(Coord(0, 2)), 0.25) &&
        double_equal(board.get_proba(Coord(0, 3)), 0.25)
    };
    return !ret;
}