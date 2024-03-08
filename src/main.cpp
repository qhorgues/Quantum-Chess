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
#include <final_solver.hpp>
int main()
{
    Board<4> B4{
        {{W_ROOK, Piece(), Piece(), W_QUEEN},
         {Piece(), W_PAWN, Piece(), W_QUEEN},
         {B_PAWN, Piece(), Piece(), W_BISHOP},
         {Piece(), Piece(), Piece(), B_KING}}};

    std::forward_list<Move> l = B4.get_list_promote(Coord(1, 1));
    Move m = Move_promote(Coord(1, 1), Coord(0, 1), TypePiece::QUEEN);
    B4.move_promotion(m);
    Move m2 = Move_merge(Coord(0, 3), Coord(1, 3), Coord(1, 2));
    Board B5 = B4;
    B5.move(m2);

    return 0;
}