#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>
#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <observer_ptr.hpp>
#include <check_path.hpp>
#include <Constexpr.hpp>
#include <TypePiece.hpp>

int main()
{
    Board<4> B4 {
        {
            { Piece(), Piece(), Piece(), W_QUEEN },
            { Piece(), B_PAWN,  Piece(), W_QUEEN },
            {  W_PAWN, Piece(),  Piece(), W_BISHOP },
            {  W_ROOK, Piece(), Piece(), B_KING }
        }
    };
    B4.move_promotion(Coord(1,1), Coord(0,1), TypePiece::BISHOP);
    Move m2 = Move_merge(Coord(0,3), Coord(1,3), Coord(1, 2));
    Board B5 = B4;
    B5.move(m2);

    return 0;
}