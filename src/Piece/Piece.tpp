#include "Piece.hpp"
#include <Coord.hpp>
#include <check_path.hpp>
#include <cmath>
#include <forward_list>
#include <observer_ptr.hpp>
#include <math_utility.hpp>
#include <Constexpr.hpp>
#include <Move.hpp>

#define POW2(x) ((x) * (x))

constexpr inline Piece::Piece() noexcept
    : m_color(Color::BLACK),
      m_type(TypePiece::EMPTY)
{
}

constexpr inline Piece::Piece(TypePiece piece, Color color) noexcept
    : m_color(color),
      m_type(piece)
{
}

constexpr inline bool Piece::is_white() const noexcept
{
  return m_color == Color::WHITE;
}

constexpr inline bool Piece::is_black() const noexcept
{
  return m_color == Color::BLACK;
}

constexpr inline bool
Piece::same_color(Piece const &other) const noexcept
{
  return m_color == other.m_color;
}

constexpr inline TypePiece Piece::get_type() const noexcept
{
  return m_type;
}

constexpr inline Color Piece::get_color() const noexcept
{
  return m_color;
}

constexpr inline std::size_t
Piece::abs_substracte(std::size_t x, std::size_t y) noexcept
{
  if (x >= y)
  {
    return x - y;
  }
  return y - x;
}

constexpr inline double
Piece::norm(Coord const &x, Coord const &y) noexcept
{
  return sqrt(
      static_cast<double>(POW2(abs_substracte(x.n, y.n)) +
                          POW2(abs_substracte(x.m, y.m))));
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Piece::check_if_use_move_promote(Board<N, M> const &board,
                                 Coord const &pos) const noexcept
{
  if constexpr (N >= 2)
  {
    if (board(pos.n, pos.m).get_type() == TypePiece::PAWN)
    {
      auto sign_color{
          [](Color color) -> int
          {
            return (color == Color::WHITE) ? -1 : 1;
          }};
      std::size_t required_line{(get_color() == Color::WHITE) ? 1 : N - 2};
      if (pos.n == required_line)
      {
        return true;
      }
    }
  }
  return false;
}
