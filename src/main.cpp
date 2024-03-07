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
#include <final_solver.hpp>

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
    Board<3> B {
        {
            {make_observer(&B_KING),                nullptr,                nullptr},
            {               nullptr, make_observer(&W_ROOK),                nullptr},
            {               nullptr, make_observer(&W_ROOK), make_observer(&W_KING)}
        }
    };
    Move m = Move_classic(Coord(2,1), Coord(0,1));
    B.move(m);
    bool b = Final::brut_force(B, 4, Color::WHITE);
    std::cout<<std::boolalpha<<b<<std::endl;

    return 0;
}