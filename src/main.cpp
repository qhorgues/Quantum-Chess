#include <iostream>
#include <fstream>
#include <complex>
#include <string>
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
    std::ofstream log_file{"Output.txt"};
    Board<4> B4{
        {{Piece(), Piece(), Piece(), W_QUEEN},
         {Piece(), B_PAWN, Piece(), W_QUEEN},
         {W_PAWN, Piece(), Piece(), W_BISHOP},
         {W_ROOK, W_KING, Piece(), B_KING}}};

    Board<> ChessBoard{
        {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
         {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
         {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}}};

    int number_move{20};
    while (number_move > 0 && !ChessBoard.winning_position(ChessBoard.get_current_player()))
    {

        Move m{computer::get_best_move(ChessBoard, 4)};
        std::string data1{{static_cast<char>('a' + m.normal.src.m), static_cast<char>('0' + 8 - m.normal.src.n)}};
        std::string data2{{static_cast<char>('a' + m.normal.arv.m), static_cast<char>('0' + 8 - m.normal.arv.n)}};
        if (m.type == TypeMove::NORMAL)
        {
            log_file << "N " << data1 << data2 << '\n';
        }
        else if (m.type == TypeMove::PROMOTE)
        {
            log_file << "P " << data1 << data2 << std::to_string(static_cast<int>(m.promote.piece)) << '\n';
        }
        else
        {
            std::string data3{{static_cast<char>('a' + m.split.arv2.m), static_cast<char>('0' + 7 - m.split.arv2.n)}};
            if (m.type == TypeMove::SPLIT)
            {
                log_file << "S " << data1 << '(' << data2 << data3 << ')' << '\n';
            }
            else
            {
                log_file << "M " << '(' << data1 << data2 << ')' << data3 << '\n';
            }
        }
        ChessBoard.move(m);
        ChessBoard.change_player();
        number_move--;
    }

    log_file.close();
    return 0;
}