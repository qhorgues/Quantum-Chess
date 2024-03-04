#include <algorithm>
#include <Board.hpp>
#include <Coord.hpp>
#include <Constexpr.hpp>
#include "check_path.hpp"

template <std::size_t N, std::size_t M>
CONSTEXPR bool
check_path_straight(
    Board<N, M> const &board,
    Coord const &dpt,
    Coord const &arv) noexcept
{
    if (dpt.n == arv.n)
    {
        for (std::size_t i{std::min(dpt.m, arv.m) + 1};
             i < std::max(dpt.m, arv.m);
             i++)
        {
            if (board(dpt.n, i) != nullptr ||
                board.get_proba(Coord(dpt.n, i)) < 1.)
            {
                return false;
            }
        }
        return true;
    }
    else if (dpt.m == arv.m)
    {
        for (std::size_t i{std::min(dpt.n, arv.n) + 1};
             i < std::max(dpt.n, arv.n);
             i++)
        {
            if (board(i, dpt.m) != nullptr ||
                board.get_proba(Coord(i, dpt.m)) < 1.)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
check_path_diagonal(
    Board<N, M> const &board,
    Coord const &dpt,
    Coord const &arv) noexcept
{
    std::size_t const max_lines{std::max(dpt.n, arv.n)};
    std::size_t const min_lines{std::min(dpt.n, arv.n)};
    std::size_t const max_columns{std::max(dpt.m, arv.m)};
    std::size_t const min_columns{std::min(dpt.m, arv.m)};

    std::size_t const dist{max_lines - min_lines};

    if (dist == max_columns - min_columns)
    {
        for (std::size_t i{1}; i < dist; i++)
        {
            if (board(min_lines + i, min_columns + i) != nullptr)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
check_path_straight_1_instance(
    Board<N, M> const &board,
    Coord const &dpt,
    Coord const &arv,
    std::size_t position)
{
    if (dpt.n == arv.n)
    {
        for (std::size_t i{std::min(dpt.m, arv.m) + 1};
             i < std::max(dpt.m, arv.m);
             i++)
        {
            if (board.m_board[position].first[board.offset(dpt.n, i)])
            {
                return false;
            }
        }
        return true;
    }
    else if (dpt.m == arv.m)
    {
        for (std::size_t i{std::min(dpt.n, arv.n) + 1};
             i < std::max(dpt.n, arv.n);
             i++)
        {
            if (board.m_board[position].first[board.offset(i, dpt.m)])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
check_path_diagonal_1_instance(
    Board<N, M> const &board,
    Coord const &dpt,
    Coord const &arv,
    std::size_t position)
{
    std::size_t const max_lines{std::max(dpt.n, arv.n)};
    std::size_t const min_lines{std::min(dpt.n, arv.n)};
    std::size_t const max_columns{std::max(dpt.m, arv.m)};
    std::size_t const min_columns{std::min(dpt.m, arv.m)};

    std::size_t const dist{max_lines - min_lines};

    if (dist == max_columns - min_columns)
    {
        for (std::size_t i{1}; i < dist; i++)
        {
            if (board.m_board[position]
                     .first[board.offset(min_lines + i, min_columns + i)])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_queen_1_instance(
    Board<N, M> const &board,
    Coord const &dpt,
    Coord const &arv,
    std::size_t position)
{
    return check_path_straight_1_instance<N, M>(board, dpt, arv, position) ||
           check_path_diagonal_1_instance<N, M>(board, dpt, arv, position);
}