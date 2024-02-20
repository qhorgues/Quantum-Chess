#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <Matrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>
#include <GameBoard.hpp>

int main()
{
    using enum Piece;
    Board<2> B {
        {
            {EMPTY, B_KING},
            {W_KING, W_QUEEN}
        }
    };
    B.move_1_instance(std::array<bool, 2>{true, true}, 0, MATRIX_ISWAP, std::array<std::size_t, 2>{0,2});

    return 0;
}