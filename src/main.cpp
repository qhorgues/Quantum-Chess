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
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};
    Board<4> B1{
        {{B_KING, Piece(), Piece(), B_BISHOP},
         {Piece(), B_KNIGHT, Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};

    Board<4> B3{
        {{Piece(), Piece(), B_KNIGHT, B_BISHOP},
         {Piece(), B_KING, Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), W_QUEEN, W_KING}}};

    Board<4> Bt{
        {{B_KING, B_BISHOP, Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), W_KNIGHT},
         {Piece(), Piece(), W_KNIGHT, W_KING}}};

    Board<3> Bt1{
        {{B_KING, Piece(), Piece()},
         {Piece(), W_ROOK, Piece()},
         {Piece(), Piece(), W_KING}}};
    Board<3> Bt2{
        {{B_KING, Piece(), Piece()},
         {Piece(), W_KING, Piece()},
         {Piece(), Piece(), Piece()}}};

    Board<1, 6> B2{
        {{W_KING, W_ROOK, Piece(), Piece(), B_KING, Piece()}}};

    // auto[res, res2] {Final::res_pos(B3, 4)};
    // std::cout<<res<<std::endl;
    // Final::print_stack<4>(res2);
    std::size_t compteur_skip{0};
    double res{0};
    std::size_t c1{0};
    std::size_t c2{0};
    std::size_t c3{0};
    std::size_t c4{0};
    std::size_t c5{0};
    std::size_t c6{0};
    std::size_t c7{0};
    std::size_t c8{0};
    std::size_t c0{0};
    for (std::size_t i{0}; i < 1000; i++)
    {
        Board<4> board = Final::init_random_board<4, 4>();
        if (double_equal(Final::res_pos<4, 4>(board, 1).first, 0.))
        {
            double inter{Final::res_pos<4, 4>(board, 4).first};
            res += inter;
            if (inter < -0.75)
            {
                c1++;
            }
            else
            {
                if (inter < -0.5)
                {
                    c2++;
                }
                else
                {
                    if (inter < -0.25)
                    {
                        c3++;
                    }
                    else
                    {
                        if (inter < 0)
                        {
                            c4++;
                        }
                        else
                        {
                            if (double_equal(inter, 0.))
                            {
                                c0++;
                            }
                            else
                            {
                                if (inter < 0.25)
                                {
                                    c5++;
                                }
                                else
                                {
                                    if (inter < 0.5)
                                    {
                                        c6++;
                                    }
                                    else
                                    {
                                        if (inter < 0.75)
                                        {
                                            c7++;
                                        }
                                        else
                                        {
                                            c8++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            compteur_skip++;
        }
        std::cout << i << std::endl;
    }
    res /= 1000. - static_cast<double>(compteur_skip);
    std::cout << res << std::endl
              << compteur_skip << std::endl;
    std::cout << c1 << std::endl;
    std::cout << c2 << std::endl;
    std::cout << c3 << std::endl;
    std::cout << c4 << std::endl;
     std::cout << c0 << std::endl;
    std::cout << c5 << std::endl;
    std::cout << c6 << std::endl;
    std::cout << c7 << std::endl;
    std::cout << c8 << std::endl;
}