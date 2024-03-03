#ifndef CHECK_PATH_HPP
#define CHECK_PATH_HPP

#include <Coord.hpp>
#include <Board.hpp>
#include <constexpr.hpp>

template <std::size_t N, std::size_t M>
CONSTEXPR static bool check_path_straight(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

template <std::size_t N, std::size_t M>
CONSTEXPR static bool check_path_diagonal(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_straight_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);

template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_diagonal_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);


template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_queen_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);

#include "check_path.tpp"

#endif