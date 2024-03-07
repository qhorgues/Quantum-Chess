#ifndef COMPUTER_PLAYER_HPP
#define COMPUTER_PLAYER_HPP
#include <cstddef>
#include <Board.hpp>
#include <Constexpr.hpp>

namespace computer
{
    template <std::size_t N, std::size_t M>
    CONSTEXPR Move 
    get_best_move(Board<N, M> const &board, int profondeur);
}

#include "ComputerPlayer.tpp"
#endif