#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <Color.hpp>
#include <math_utility.hpp>

int test_capture_slide_move_true(int argc, char **argv)
{
    Board<3, 5> board{
        {W_ROOK, Piece(), Piece(), Piece(), Piece()},
        {Piece(), W_ROOK,  W_ROOK,  W_ROOK, Piece()},
        {Piece(),Piece(),Piece(),  Piece(), B_ROOK}};
    Move m1 = Move_split(Coord(1, 1), Coord(0, 1), Coord(2, 1));
    Move m2 = Move_split(Coord(1, 2), Coord(0, 2), Coord(2, 2));
    Move m3 = Move_split(Coord(1, 3), Coord(0, 3), Coord(2, 3));
    Move m4 = Move_split(Coord(2, 4), Coord(0, 4), Coord(1, 4));
    Move m5 = Move_classic(Coord(0, 4), Coord(0, 0));
    board.move(m1);
    board.move(m2);
    board.move(m3);
    board.move(m4);
    board.move(m5, true);
    bool res{board(0, 0).get_type() == TypePiece::ROOK && board(0, 0).get_color() == Color::BLACK };
    return !res;
}