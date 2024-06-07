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
        {{B_KING, B_BISHOP, Piece(), Piece()},
         {B_KNIGHT, Piece(), Piece(), Piece()},
         {Piece(), Piece(),  Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};
Board<4> B1{
        {{B_KING, Piece(), Piece(), B_BISHOP},
         {Piece(), B_KNIGHT, Piece(), Piece()},
         {Piece(), Piece(),  Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};

         Board<4> B3{
        {{Piece(), Piece(), B_KNIGHT, B_BISHOP},
         {Piece(), B_KING, Piece(), Piece()},
         {Piece(), Piece(),  Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};


         Board<4> Bt{
        {{B_KING, B_BISHOP, Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(),  Piece(), W_KNIGHT},
         {Piece(), Piece(), W_KNIGHT, W_KING}}};

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
    
    //auto[res, res2] {Final::res_pos(B3, 4)};
    //std::cout<<res<<std::endl;
    //Final::print_stack<4>(res2);
    
    Board<4> board = Final::init_random_board<4,4>();
    int p {0};
    p++;

   
}