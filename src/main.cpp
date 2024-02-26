#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <CMatrix.hpp>
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

    Board<4> B2 {
        {
            {W_QUEEN, EMPTY, EMPTY, B_QUEEN},
            {EMPTY, EMPTY, B_PAWN, W_KING},
            {W_PAWN, EMPTY, EMPTY, W_BISHOP},
            {B_BISHOP, W_ROOK, EMPTY, B_KING}
        }
    };
    B2.move_split_jump(B2.offset(0, 0), B2.offset(0,1), B2.offset(2,3));
    std::cout << std::boolalpha << B2.mesure(B2.offset(2,3)) << std::endl;
    std::cout << std::boolalpha << B2.check_path_straight(Coord(0, 0), Coord(3, 0)) << std::endl;
    std::cout << std::boolalpha << B2.check_path_diagonal(Coord(0, 0), Coord(3, 3)) << std::endl;

    return 0;
}