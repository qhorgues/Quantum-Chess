#ifndef CONSOLE_INTERFACE_HPP
#define CONSOLE_INTERFACE_HPP

#include <ostream>
#include <Board.hpp>

template <std::size_t N, std::size_t M>
std::ostream& operator<<(std::ostream& os, Board<N, M> const& board);

#include "ConsoleInterface.tpp"
#endif