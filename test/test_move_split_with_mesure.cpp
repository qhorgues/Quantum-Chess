#include <Board.hpp>
#include <Piece.hpp>
#include <Coord.hpp>
#include <TypePiece.hpp>
#include <Move.hpp>
#include <math_utility.hpp>

int test_move_split_with_mesure(int argc, char* argv[])
{
    Board<5> board{
        {W_ROOK, Piece(), Piece(),Piece(), Piece()},
        {Piece(), Piece(), Piece(), Piece(), Piece()},
        {Piece(), Piece(), Piece(),Piece(),Piece()},
        {Piece(), Piece(),  Piece(), Piece(),Piece()},
        {Piece(), Piece(), Piece(),B_ROOK,Piece()}};
    Move m = Move_split(Coord(4,3), Coord(4, 1), Coord(4,4));
    Move m1 = Move_split(Coord(0, 0), Coord(0, 1), Coord(0 ,4));
    Move m2 = Move_classic(Coord(0,1), Coord(4,1));
    Move m3 = Move_classic(Coord(4, 4), Coord(0, 4));
    board.move(m);
    board.move(m1);
    board.move(m2);
    board.move(m3);
    std::size_t acc{0};
    for(std::size_t i{0}; i<5; i++)
    {
        for(std::size_t j{0}; j<5; j++)
        {
        if(board(i, j).get_type() == TypePiece::ROOK)
        {
            acc++;
        }
        }
    }
    return acc != 2 && acc != 1;
}