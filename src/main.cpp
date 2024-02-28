#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>
#include <Board.hpp>
#include <Piece.hpp>
#include <observer_ptr.hpp>
#include <check_path.hpp>

int main()
{
    Board<2> B {
        {
            { make_observer(&W_QUEEN),  make_observer(&B_KING)},
            {                 nullptr, make_observer(&B_QUEEN)}
        }
    };
    Qubit<2> q {std::array<bool, 2> {false, true}};
    auto x {qubitToArray (q)};
    (void)x;
    
    B.move_1_instance(std::array<bool, 2>{true, false}, 0, MATRIX_ISWAP, std::array<std::size_t, 2>{0,2});

    Board<4> B2 {
        {
            {  make_observer(&W_QUEEN),                nullptr,                nullptr,   make_observer(&B_QUEEN) },
            {                  nullptr,                nullptr, make_observer(&B_PAWN),    make_observer(&W_KING) },
            {  make_observer (&W_PAWN),                nullptr,                nullptr, make_observer (&W_BISHOP) },
            { make_observer(&B_BISHOP), make_observer(&W_ROOK),                nullptr,    make_observer(&B_KING) }
        }
    };
    B2.move_split_jump(Coord(0, 0), Coord(0,1), Coord(2,2));
    std::cout << std::boolalpha << check_path_straight(B2, Coord(0, 0), Coord(3, 0)) << std::endl;
    B2.move_merge_jump(Coord(0,1), Coord(2, 2), Coord(0, 0));
    std::cout << std::boolalpha << check_path_diagonal(B2, Coord(0, 0), Coord(3, 3)) << std::endl;
    B2.move_classic_slide(Coord(0,3), Coord(3, 3), &check_path_straight_1_instance<4, 4>);

    return 0;
}