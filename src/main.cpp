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
#include <functional>
int main()
{
    Board<3> B{
        {{B_KING, Piece(), Piece()},
         {Piece(), W_ROOK, Piece()},
         {Piece(), Piece(),W_KING}}};

    bool b = Final::brut_force_quantum_chess(B, 7, Color::WHITE);
    std::cout<<std::boolalpha<<b<<std::endl;

    return 0;
}