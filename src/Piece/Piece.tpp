#include "Piece.hpp"
#include <Coord.hpp>
#include <check_path.hpp>
#include <cmath>

#define POW2(x) ((x) * (x))

constexpr Piece::Piece(TypePiece piece, Color color) noexcept
 : m_color(color),
   m_type(piece)
{
} 


constexpr bool Piece::is_white() const noexcept
{
    return m_color == Color::WHITE;
}

constexpr bool Piece::is_black() const noexcept
{
    return m_color == Color::BLACK;
}

constexpr bool Piece::same_color(Piece const& other) const noexcept
{
    return m_color == other.m_color;
}

constexpr TypePiece Piece::get_type() const noexcept
{
    return m_type;
}

namespace
{
    constexpr std::size_t abs_substracte(std::size_t x, std::size_t y)
    {
        if (x >= y)
        {
            return x - y;
        }
        return y - x;
    }
}; // namespace


constexpr double Piece::norm(Coord const& x, Coord const& y) noexcept
{
    return sqrt(static_cast<double>(POW2(abs_substracte(x.n, y.n)) + POW2(abs_substracte(x.m, y.m))));
}


template <std::size_t N, std::size_t M>
constexpr bool Coord_in_Board(Board<N, M> const& board, Coord const& pos) noexcept
{
    if (pos.n >= board.numberLines() || pos.m >= board.numberColumns())
    {
        return false;
    }
    return true;
}


template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_pawn(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    if (arv.m - dpt.m == 2)
    {
        if (dpt.n == 1 && m_color == Color::WHITE && board(dpt.n +1, dpt.m) == std::nullopt)
        {
            return true;
        }
        else if (dpt.n == board.numberLines() -2 && m_color == Color::BLACK && board(dpt.n -1, dpt.m) == std::nullopt)
        {
            return true;
        }
        return false;
    }
    return true;

}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_knight(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    return Coord_in_Board(board, dpt) && Coord_in_Board(board, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_bishop(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    
    return Coord_in_Board(board, dpt) && Coord_in_Board(board, arv) && check_path_diagonal(board, dpt, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_rook(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    
    return Coord_in_Board(board, dpt) && Coord_in_Board(board, arv) && check_path_straight(board, dpt, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_queen(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    
    return Coord_in_Board(board, dpt) && Coord_in_Board(board, arv) && (check_path_straight(board, dpt, arv) || check_path_diagonal(board, dpt, arv));
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_king(Board<N, M> const& board, Coord const &dpt, Coord const &arv) noexcept
{
    bool path { true }; 
    if constexpr (N = 8 && M = 8)
    {
        path = check_path_straight(board, dpt, arv);
    }
    return path && Coord_in_Board(board, dpt) && Coord_in_Board(board, arv);
}