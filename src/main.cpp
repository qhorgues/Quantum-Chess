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

int main()
{
    Move m1;
    m1.type = TypeMove::NORMAL;
    m1.normal.src = Coord(0, 1);
    m1.normal.arv = Coord(3, 2);

    /*Board<> B3 {};
    Board<2> B {
        {
            { make_observer(&W_QUEEN),  make_observer(&B_KING)},
            {                 nullptr, make_observer(&B_QUEEN)}
        }
    };
    Qubit<2> q {std::array<bool, 2> {false, true}};
    auto x {qubitToArray (q)};
    (void)x;
    
    B.move_1_instance(std::array<bool, 2>{true, false}, 0, MATRIX_ISWAP, std::array<std::size_t, 2>{0,2});*/

    Board<4> B2 {
        {
            {  make_observer(&W_QUEEN),                  nullptr,                nullptr,   make_observer(&B_QUEEN) },
            {                  nullptr, make_observer(&B_BISHOP), make_observer(&B_PAWN),    make_observer(&W_KING) },
            {  make_observer (&W_PAWN),                  nullptr,                nullptr, make_observer (&W_BISHOP) },
            {                  nullptr,   make_observer(&W_ROOK),                nullptr,    make_observer(&B_KING) }
        }
    };
   /* std::forward_list<Coord> list = B2.get_list_move(Coord(2, 0));
    for (Coord const& e : list)
    {
        std::cout << e.n << " / " << e.m << std::endl;
    }*/
    /*B2.capture_pawn(Coord(2, 0), Coord(1,1));
    B2.move_pawn_one_step(Coord(1,1), Coord(0,1));

    B2.move_split_jump(Coord(0, 0), Coord(0,1), Coord(2,2));
    std::cout << std::boolalpha << check_path_straight(B2, Coord(0, 0), Coord(3, 0)) << std::endl;
    B2.move_merge_jump(Coord(0,1), Coord(2, 2), Coord(0, 0));
    B2.update_after_merge();
    std::cout << std::boolalpha << check_path_diagonal(B2, Coord(0, 0), Coord(3, 3)) << std::endl;
    B2.move_classic_slide(Coord(0,3), Coord(3, 3), &check_path_straight_1_instance<4, 4>);*/
    bool a  {check_path_straight_1_instance(B2, Coord(0, 3), Coord(0, 1), 0)} ;
    (void)a;
    bool b {check_path_straight_1_instance(B2, Coord(0, 3), Coord(0, 2), 0)};
    (void)b;
    B2.move_split_slide(Coord(0,3), Coord(0,1), Coord(0,2), &check_path_straight_1_instance<4, 4>);
   /* B2.move_classic_slide(Coord(0,0), Coord(0,3), &check_path_straight_1_instance<4, 4>);*/


    return 0;
}