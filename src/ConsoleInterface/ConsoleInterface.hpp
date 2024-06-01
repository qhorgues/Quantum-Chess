#ifndef CONSOLE_INTERFACE_HPP
#define CONSOLE_INTERFACE_HPP

#include <ostream>
#include <Board.hpp>

const char *getUnicodeChar(TypePiece piece, Color color);
TypeMove chr_to_TypeMove(char type_move);
char Piece_to_chr(TypePiece piece, Color color);
Piece chr_to_Piece(char piece);

template <std::size_t N, std::size_t M>
void print_board_light(std::ostream &os, Board<N, M> const &board);

template <std::size_t N, std::size_t M>
std::ostream& operator<<(std::ostream& os, Board<N, M> const& board);

#include "ConsoleInterface.tpp"
#endif