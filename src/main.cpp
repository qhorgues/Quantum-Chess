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

template <std::size_t N, std::size_t M>
void auto_playing(Board<N, M> &board, std::ostream &output, int nb_moves, int search_depth)
{
    while (nb_moves > 0 && !board.winning_position(opponent_color(board.get_current_player())))
    {

        Move m{computer::get_best_move(board, search_depth)};
        std::string data1{{static_cast<char>('a' + m.normal.src.m), static_cast<char>('0' + N - m.normal.src.n)}};
        std::string data2{{static_cast<char>('a' + m.normal.arv.m), static_cast<char>('0' + N - m.normal.arv.n)}};
        if (m.type == TypeMove::NORMAL)
        {
            output << "N " << data1 << data2 << std::endl;
        }
        else if (m.type == TypeMove::PROMOTE)
        {
            output << "P " << data1 << data2 << " :: " << std::to_string(static_cast<int>(m.promote.piece)) << std::endl;
        }
        else
        {
            std::string data3{{static_cast<char>('a' + m.split.arv2.m), static_cast<char>('0' + N - m.split.arv2.n)}};
            if (m.type == TypeMove::SPLIT)
            {
                output << "S " << data1 << '(' << data2 << data3 << ')' << std::endl;
            }
            else
            {
                output << "M " << '(' << data1 << data2 << ')' << data3 << std::endl;
            }
        }
        board.move(m);
        board.change_player();
        nb_moves--;
    }
}

int main()
{
    std::ofstream log_file{"Output.txt"};

    Board<> ChessBoard{
        {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
         {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
         {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}}};

    Board<3> B2{
        {
            {B_KING, Piece(), B_QUEEN},
            {W_PAWN,  B_PAWN, W_QUEEN},
            {Piece(), Piece(), W_KING}
        } 
    };

    Board<3> B3{
        {
            {B_KING, Piece(), Piece()},
            {Piece(), W_ROOK, Piece()},
            {Piece(), Piece(), W_KING}
        } 
    };
    /* Board<3> B1{
        {B_KING, B_ROOK, Piece()},
        {B_PAWN, Piece(), Piece()},
        {Piece(), W_QUEEN, W_KING}}; */

    auto_playing(B3, log_file, 10, 5);

    log_file.close();
    return 0;
}