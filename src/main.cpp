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
            {W_KING, B_KING},
            {EMPTY, W_QUEEN}
        }
    };
    Qubit<2> q {std::array<bool, 2> {false, true}};
    auto x {qubitToArray (q)};
    (void)x;
    
    B.move_1_instance(std::array<bool, 2>{true, false}, 0, MATRIX_ISWAP, std::array<std::size_t, 2>{0,2});

    Board<4, 1> B2 {
        {
            {W_QUEEN},
            {EMPTY},
            {EMPTY},
            {B_BISHOP}
        }
    };
    std::cout << std::boolalpha << B2.check_path_straight(Coord(0, 0), Coord(0, 3)) << std::endl;
    return 0;
}