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
#include <ConsoleInterface.hpp>
#include <final_solver.hpp>
#include <functional>


int main()
{
    Board<4> B{
        {{B_KING, Piece(), Piece(), Piece()},
         {Piece(), B_ROOK, Piece(), Piece()},
         {Piece(), Piece(),  Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};

         Board<4> Bt{
        {{B_KING, B_BISHOP, Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(),  Piece(), W_KNIGHT},
         {Piece(), Piece(), W_ROOK, W_KING}}};

         Board<3> Bt1{
        {{B_KING, Piece(), Piece()},
         {Piece(), W_ROOK, Piece()},
         {Piece(), Piece(),  W_KING}}
         };
         Board<3> Bt2{
        {{B_KING, Piece(), Piece()},
         {Piece(), W_KING, Piece()},
         {Piece(), Piece(), Piece()}}
         };


    Board<1,6> B2{
        {
            {W_KING, W_ROOK, Piece(), Piece(), B_KING, Piece()}
        }
    };
    /*bool b = Final::brut_force_quantum_chess(B2, 1, Color::WHITE);
    std::cout<<std::boolalpha<<b<<std::en
    dl;*/
    auto[res, res2] {Final::res_pos(Bt1, 12)};
    std::cout<<res<<std::endl;
    Final::print_stack<4>(res2);
   
}