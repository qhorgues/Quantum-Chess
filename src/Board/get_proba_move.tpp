// Lib standard
#include <cassert>
#include <utility>
#include <functional>
#include <cmath>
#include <optional>
#include <stdexcept>

// Inclusion projet
#include <Piece.hpp>
#include <TypePiece.hpp>
#include <Constexpr.hpp>
#include "Board.hpp"
#include <math_utility.hpp>

template <std::size_t N, std::size_t M>
CONSTEXPR double
Board<N, M>::get_proba_mesure(Coord const &p) const noexcept
{
    double res{0};
    std::size_t size_board = std::size(m_board);
    for (std::size_t i{0}; i < size_board; i++)
    {
        if (m_board[i].first[offset(p.n, p.m)])
        {
            res += std::pow(std::abs(m_board[i].second), 2);
        }
    }
    return res;
}

template <std::size_t N, std::size_t M>
CONSTEXPR double
Board<N, M>::get_proba_mesure_capture_slide(
    Coord const &s,
    Coord const &t,
    std::function<bool(Board<N, M> const &,
                       Coord const &,
                       Coord const &,
                       std::size_t)>
        check_path) const noexcept
{
    double res{0};
    std::size_t position = offset(s.n, s.m);
    std::size_t size_board = std::size(m_board);
    for (std::size_t i{0}; i < size_board; i++)
    {
        if (m_board[i].first[position] &&
            check_path(*this, s, t, i))
        {
            res += std::pow(std::abs(m_board[i].second), 2);
        }
    }
    return res;
}

template <std::size_t N, std::size_t M>
CONSTEXPR double
Board<N, M>::get_proba_mesure_castle(
    Coord const &king,
    Coord const &rook) const noexcept
{
    double res{0};
    std::size_t size_board = std::size(m_board);
    std::size_t position_king = offset(king.n, king.m);
    std::size_t position_rook = offset(rook.n, rook.m);
    for (std::size_t i{0}; i < size_board; i++)
    {
        if (m_board[i].first[position_king] &&
            m_board[i].first[position_rook] &&
            check_path_straight_1_instance(*this, king, rook, i))
        {
            res += std::pow(std::abs(m_board[i].second), 2);
        }
    }
    return res;
}

template <std::size_t N, std::size_t M>
CONSTEXPR double Board<N, M>::get_proba_move(Move const &move) const noexcept
{
    Coord s;
    Coord t;
    switch (move.type)
    {
    case TypeMove::NORMAL:
        s = move.normal.src;
        t = move.normal.arv;
        break;
    case TypeMove::PROMOTE:
        s = move.promote.src;
        t = move.promote.arv;
        break;
    case TypeMove::MERGE:
        return 1.;
    case TypeMove::SPLIT:
        return 1.;
    default:
        return 1.;
    }
    TypePiece piece{(*this)(s.n, s.m).get_type()};
    TypePiece piece_target{(*this)(t.n, t.m).get_type()};
    if((piece == piece_target) && (*this)(s.n, s.m).same_color((*this)(t.n, t.m)))
    {
        return 1.;
    }
    switch (piece)
    {
    case TypePiece::KING:
        if constexpr (N == 8 && M == 8)
        {
            auto abs_diff{[](std::size_t x, std::size_t y) -> std::size_t
                          {
                              return (x >= y) ? x - y : y - x;
                          }};
            std::size_t diff_colum{abs_diff(s.m, t.m)};
            if (diff_colum == 2)
            {
                if (s.m > t.m)
                {
                    return get_proba_mesure_castle(s, Coord(s.n, t.m - 2));
                }
                else
                {
                    return get_proba_mesure_castle(s, Coord(s.n, t.m + 1));
                }
            }
            else
            {
                if (piece_target != TypePiece::EMPTY)
                {
                    if ((*this)(s.n, s.m).same_color((*this)(t.n, t.m)))
                    {
                        return 1. - get_proba_mesure(t);
                    }
                    else
                    {
                        return get_proba_mesure(s);
                    }
                }
                else
                {
                    return 1.;
                }
            }
        }
        else
        {
            if (piece_target != TypePiece::EMPTY)
            {
                if ((*this)(s.n, s.m).same_color((*this)(t.n, t.m)))
                {
                    return 1. - get_proba_mesure(t);
                }
                else
                {
                    return get_proba_mesure(s);
                }
            }
            else
            {
                return 1.;
            }
        }
        break;
    case TypePiece::KNIGHT:
        if (piece_target != TypePiece::EMPTY)
        {
            if ((*this)(s.n, s.m).same_color((*this)(t.n, t.m)))
            {
                return 1. - get_proba_mesure(t);
            }
            else
            {
                return get_proba_mesure(s);
            }
        }
        else
        {
            return 1.;
        }

        break;
    case TypePiece::BISHOP:
        if ((*this)(t.n, t.m).get_type() == TypePiece::EMPTY)
        {
            return 1.;
        }
        else
        {
            if (!(*this)(t.n, t.m).same_color((*this)(s.n, s.m)))
            {
                return get_proba_mesure_capture_slide(
                    s, t,
                    &check_path_diagonal_1_instance<N, M>);
            }
            else
            {
                return 1. - get_proba_mesure(t);
            }
        }
        break;
    case TypePiece::ROOK:
        if ((*this)(t.n, t.m).get_type() == TypePiece::EMPTY)
        {
            return 1.;
        }
        else
        {
            if (!(*this)(t.n, t.m).same_color((*this)(s.n, s.m)))
            {
                return get_proba_mesure_capture_slide(
                    s, t,
                    &check_path_straight_1_instance<N, M>);
            }
            else
            {
                return 1. - get_proba_mesure(t);
            }
        }
        break;
    case TypePiece::QUEEN:
       if ((*this)(t.n, t.m).get_type() == TypePiece::EMPTY)
        {
            return 1.;
        }
        else
        {
            if (!(*this)(t.n, t.m).same_color((*this)(s.n, s.m)))
            {
                return get_proba_mesure_capture_slide(
                    s, t,
                    &check_path_queen_1_instance<N, M>);
            }
            else
            {
                return 1. - get_proba_mesure(t);
            }
        }
        break;
    case TypePiece::PAWN:
        if ((*this)(t.n, t.m).get_type() == TypePiece::EMPTY ||
            ((*this)(t.n, t.m).get_type() == (*this)(s.n, s.m).get_type() &&
             (*this)(t.n, t.m).get_color() == (*this)(s.n, s.m).get_color()))
        {
            return 1.;
        }
        else
        {
            return 1. - get_proba_mesure(t);
        }
    case TypePiece::EMPTY:
    default:
        break;
    }
    return 1.;
}