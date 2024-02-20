#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <Matrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>
#include "GameBoard/GameBoard.hpp"

int main()
{
    using enum Piece;
    std::cout << static_cast<int>(EMPTY) << std::endl;
    Board<2> B {
        {
            {EMPTY, B_KING},
            {W_KING, W_QUEEN}
        }
    };
    /* using namespace std::complex_literals;

    Matrix<std::complex<double>, 4> A {
        {
            {1,     3, 1.+1i,  8},
            {8,    1i,     2,  0},
            {0,     8,     1, 4i},
            {2, 2.+5i,     0,  1}
        }
    };

    Qubit<2> q {std::array<std::complex<double>, 4>({0, 1, 2, 3})};
    auto q2 = A*q;
    std::cout << q2 << std::endl;
    std::cout << MATRIX_SPLIT_SLIDE << std::endl;*/
    std::cout << MATRIX_MERGE_SLIDE << std::endl;
    
    std::cout << Qubit<3>(std::array{true, false, true}) <<std::endl;
    

}