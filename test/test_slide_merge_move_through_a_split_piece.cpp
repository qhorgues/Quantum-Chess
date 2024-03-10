#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_slide_merge_move_through_a_split_piece(int argc, char **argv)
{
    Board<2, 5> board{
        {W_ROOK, Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), B_ROOK, Piece(), Piece()}};
    Move m = Move_split(Coord(1, 2), Coord(0, 2), Coord(1, 4));
    Move m1 = Move_split(Coord(0, 0), Coord(0, 1), Coord(0, 4));
    Move m2 = Move_merge(Coord(0, 1), Coord(0, 4), Coord(0, 3));
    board.move(m);
    board.move(m1);
    board.move(m2);
    bool res{board(0, 2).get_type() == TypePiece::ROOK &&
             board(0, 2).get_color() == Color::BLACK &&
             board(0, 1).get_type() == TypePiece::ROOK &&
             board(0, 1).get_color() == Color::WHITE &&
             double_equal(board.get_proba(Coord(0, 1)), 0.5) &&
             board(0, 3).get_type() == TypePiece::ROOK &&
             board(0, 3).get_color() == Color::WHITE &&
             double_equal(board.get_proba(Coord(0, 3)), 0.5) &&
             board(0, 4).get_type() == TypePiece::EMPTY};
    return !res;
}