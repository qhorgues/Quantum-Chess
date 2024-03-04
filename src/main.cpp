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

int main()
{
    Board<4> B4 {
        {
            {                  nullptr,                  nullptr,                nullptr,   make_observer(&W_QUEEN) },
            {                  nullptr,   make_observer(&B_PAWN),                nullptr,    make_observer(&W_QUEEN) },
            {  make_observer (&W_PAWN),                  nullptr,                nullptr, make_observer (&W_BISHOP) },
            {  make_observer(&W_ROOK),                   nullptr,                nullptr,    make_observer(&B_KING) }
        }
    };
    Move m {.type=TypeMove::NORMAL, .normal={.src=Coord(3, 0), .arv=Coord(3, 3)}};
    B4.move(m);
    Move m2 = Move_merge(Coord(0,3), Coord(1,3), Coord(1, 2));
    Board B5 = B4;
    B5.move(m2);

    return 0;
}