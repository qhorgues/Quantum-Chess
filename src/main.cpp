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
    

    /* Board<> ChessBoard{
        {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
         {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
         {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
         {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}}}; */
    std::forward_list<Move> l = B4.get_list_promote(Coord(1, 1));
    Move m0 = Move_promote(Coord(1, 1), Coord(0, 1), TypePiece::QUEEN);
    Move m1 = Move_split(Coord(1,2), Coord(3,2), Coord(0,3));
    B4.move_promotion(m0);
    Move m2 = Move_merge(Coord(0, 3), Coord(1, 3), Coord(1, 2));
    Board B5 = B4;
    B5.move(m2);
    B5.move(m1);
    Move m3 = Move_classic(Coord(3,3), Coord(3,2));
    B5.move(m3);
     Move m4 = Move_classic(Coord(3,2), Coord(3,1));
    B5.move(m4);
     Move m5 = Move_promote(Coord(2,0), Coord(3,1), TypePiece::BISHOP);
    B5.move(m5);
     Move m6 = Move_split(Coord(3,1), Coord(3,2), Coord(2,1));
    B5.move(m6);
     Move m7 = Move_classic(Coord(2,3), Coord(3,2));
    B5.move(m7);
     Move m8 = Move_classic(Coord(2,1), Coord(3,2));
    B5.move(m8);

    Board<3> ChessBoard{
        {B_KING, B_ROOK, Piece()},
        {B_PAWN, Piece(), Piece()},
        {Piece(), W_QUEEN, W_KING}
    };

    int number_move{20};
    while (number_move > 0 && !ChessBoard.winning_position(ChessBoard.get_current_player()))
    {

        Move m{computer::get_best_move(ChessBoard, 10)};
        std::string data1{{static_cast<char>('a' + m.normal.src.m), static_cast<char>('0' + 3 - m.normal.src.n)}};
        std::string data2{{static_cast<char>('a' + m.normal.arv.m), static_cast<char>('0' + 3 - m.normal.arv.n)}};
        if (m.type == TypeMove::NORMAL)
        {
            log_file << "N " << data1 << data2 << '\n';
        }
        else if (m.type == TypeMove::PROMOTE)
        {
            log_file << "P " << data1 << data2 << " :: " << std::to_string(static_cast<int>(m.promote.piece)) << '\n';
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