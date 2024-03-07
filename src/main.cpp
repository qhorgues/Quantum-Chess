#include <iostream>
#include <fstream>
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
#include <ComputerPlayer.hpp>

int main()
{
    std::ofstream log_file {"Output.txt"};
    Board<4> B4 {
        {
            {                  nullptr,                  nullptr,                nullptr,   make_observer(&W_QUEEN) },
            {                  nullptr,   make_observer(&B_PAWN),                nullptr,    make_observer(&W_QUEEN) },
            {  make_observer (&W_PAWN),                  nullptr,                nullptr, make_observer (&W_BISHOP) },
            {  make_observer(&W_ROOK),    make_observer(&W_KING),                nullptr,    make_observer(&B_KING) }
        }
    };

    Board<> ChessBoard {
        {
            { make_observer(&B_ROOK), make_observer(&B_KNIGHT), make_observer(&B_BISHOP), make_observer(&B_QUEEN), make_observer(&B_KING), make_observer(&B_BISHOP), make_observer(&B_KNIGHT), make_observer(&B_ROOK)},
            { make_observer(&B_PAWN),   make_observer(&B_PAWN),   make_observer(&B_PAWN),  make_observer(&B_PAWN), make_observer(&B_PAWN),   make_observer(&B_PAWN),   make_observer(&B_PAWN), make_observer(&B_PAWN)},
            {                nullptr,                  nullptr,                  nullptr,                 nullptr,                nullptr,                  nullptr,                  nullptr,                nullptr},
            {                nullptr,                  nullptr,                  nullptr,                 nullptr,                nullptr,                  nullptr,                  nullptr,                nullptr},
            {                nullptr,                  nullptr,                  nullptr,                 nullptr,                nullptr,                  nullptr,                  nullptr,                nullptr},
            {                nullptr,                  nullptr,                  nullptr,                 nullptr,                nullptr,                  nullptr,                  nullptr,                nullptr},
            { make_observer(&W_PAWN),   make_observer(&W_PAWN),   make_observer(&W_PAWN),  make_observer(&W_PAWN), make_observer(&W_PAWN),   make_observer(&W_PAWN),   make_observer(&W_PAWN), make_observer(&W_PAWN)},
            { make_observer(&W_ROOK), make_observer(&W_KNIGHT), make_observer(&W_BISHOP), make_observer(&W_QUEEN), make_observer(&W_KING), make_observer(&W_BISHOP), make_observer(&W_KNIGHT), make_observer(&W_ROOK)}
        }
    };
    
    while (!ChessBoard.winning_position(ChessBoard.get_current_player()))
    {
        Move m {computer::get_best_move(ChessBoard, 5)};
        if (m.type == TypeMove::NORMAL)
        {
            log_file << "N " << m.normal.src.n << '/' << m.normal.src.m << " -> " << m.normal.arv.n << '/' << m.normal.arv.m;
        }
        else if (m.type == TypeMove::SPLIT)
        {
            log_file << "S " << m.split.src.n << '/' << m.split.src.m << " -> " 
                    << m.split.arv1.n << '/' << m.split.arv1.m << " | "
                     << m.split.arv2.n << '/' << m.split.arv2.m;
        }
        else
        {
            log_file << "M " << m.merge.src1.n << '/' << m.merge.src1.m << " | " 
                    << m.merge.src2.n << '/' << m.merge.src2.m << " | "
                     << m.merge.arv.n << '/' << m.merge.arv.m;
        }
        ChessBoard.move(m);
        ChessBoard.change_player();
    }
        
    log_file.close();
    return 0;
}