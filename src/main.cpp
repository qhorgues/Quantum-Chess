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
    Board<4> B4{
        {{W_ROOK, Piece(), Piece(), W_QUEEN},
         {Piece(), W_PAWN, Piece(), W_QUEEN},
         {B_PAWN, Piece(), Piece(), W_BISHOP},
         {Piece(), Piece(), Piece(), B_KING}}};

    std::forward_list<Move> l = B4.get_list_promote(Coord(1, 1));
    Move m = Move_promote(Coord(1, 1), Coord(0, 1), TypePiece::QUEEN);
    Move m1 = Move_split(Coord(1,2), Coord(3,2), Coord(0,3));
    B4.move_promotion(m);
    Move m2 = Move_merge(Coord(0, 3), Coord(1, 3), Coord(1, 2));
    Board B5 = B4;
    B5.move(m2);
    B5.move(m1);
    Move m3 = Move_classic(Coord(3,3), Coord(3,2));
    B5.move(m3);
     Move m4 = Move_classic(Coord(3,2), Coord(3,1));
    B5.move(m4);
     Move m5 = Move_promote(Coord(2,0), Coord(3,1), TypePiece::BISHOP);
    B5.move(m5);
     Move m6 = Move_split(Coord(3,1), Coord(3,2), Coord(2,1));
    B5.move(m6);
     Move m7 = Move_classic(Coord(2,3), Coord(3,2));
    B5.move(m7);
     Move m8 = Move_classic(Coord(2,1), Coord(3,2));
    B5.move(m8);

    return 0;
}