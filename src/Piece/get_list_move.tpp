#include "Piece.hpp"
#include <Coord.hpp>
#include <check_path.hpp>
#include <cmath>
#include <forward_list>
#include <observer_ptr.hpp>
#include <math_utility.hpp>
#include <Constexpr.hpp>
#include <Move.hpp>

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_king(Board<N, M> const &board,
                          Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  std::array<int, 8> const
      move_king{{-P - 1, -P, -P + 1, -1, 1, P - 1, P, P + 1}};
  std::size_t current_pos{board.offset(pos.n, pos.m)};
  int posBox{board.m_S_mailbox[current_pos]};
  std::forward_list<Coord> list_move{};
  for (int const e : move_king)
  {
    int arv{board.m_L_mailbox[posBox + e]};
    if (arv >= 0)
    {
      std::size_t n{arv / M}, m{arv % M};
      Piece const &arvPiece{board(n, m)};
      bool eval{false};
      if CONSTEXPR (MOVE == Move_Mode::NORMAL)
      {
        eval = arvPiece.get_type() == TypePiece::EMPTY ||
               !same_color(arvPiece) ||
               board.get_proba(Coord(n, m)) < 1. - EPSILON;
      }
#if !defined(KING_NO_SPLIT)
      else
      {
        eval = arvPiece.get_type() == TypePiece::EMPTY;
      }
#endif
      if (eval)
      {
        list_move.push_front(Coord(n, m));
      }
    }
  }
  if CONSTEXPR (N == 8 && M == 8 && MOVE == Move_Mode::NORMAL)
  {
    if (board.m_q_castle[static_cast<int>(get_color())] &&
        check_path_straight(board, pos, Coord(pos.n, pos.m - 4)))
    {
      list_move.push_front(Coord(pos.n, pos.m - 2));
    }
    if (board.m_k_castle[static_cast<int>(get_color())] &&
        check_path_straight(board, pos, Coord(pos.n, pos.m + 3)))
    {
      list_move.push_front(Coord(pos.n, pos.m + 2));
    }
  }
  return list_move;
}

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_knight(Board<N, M> const &board,
                            Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  std::array<int, 8> const
      move_knight{{-2 * P - 1, -P - 2, -2 * P + 1, -P + 2,
                   P - 2, 2 * P - 1, 2 * P + 1, P + 2}};
  std::size_t current_pos{board.offset(pos.n, pos.m)};
  int posBox{board.m_S_mailbox[current_pos]};
  std::forward_list<Coord> list_move{};
  for (int const e : move_knight)
  {
    int arv{board.m_L_mailbox[posBox + e]};
    if (arv >= 0)
    {
      std::size_t n{arv / M}, m{arv % M};
      Piece const &arvPiece{board(n, m)};
      bool eval;
      if CONSTEXPR (MOVE == Move_Mode::NORMAL)
      {
        eval = arvPiece.get_type() == TypePiece::EMPTY ||
               !same_color(arvPiece) ||
               board.get_proba(Coord(n, m)) < 1. - EPSILON;
      }
      else
      {
        eval = arvPiece.get_type() == TypePiece::EMPTY;
      }
      if (eval)
      {
        list_move.push_front(Coord(n, m));
      }
    }
  }
  return list_move;
}

template <Piece::Move_Mode MOVE,
          std::size_t N,
          std::size_t M,
          std::size_t SIZE>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_rec(
    Board<N, M> const &board,
    Coord const &pos,
    std::array<int, SIZE> const &list_move) const noexcept
{
  std::size_t current_pos{board.offset(pos.n, pos.m)};
  int const posBox{board.m_S_mailbox[current_pos]};
  std::forward_list<Coord> move{};
  for (int const e : list_move)
  {
    int arv{board.m_L_mailbox[posBox + e]};
    int posBox_tmp{posBox};
    while (arv >= 0)
    {
      std::size_t n{arv / M}, m{arv % M};
      Piece const &arvPiece{board(n, m)};
      if CONSTEXPR (MOVE == Move_Mode::NORMAL)
      {
        if (arvPiece.get_type() == TypePiece::EMPTY ||
            board.get_proba(Coord(n, m)) < 1. - EPSILON)
        {
          move.push_front(Coord(n, m));
        }
        else if (!same_color(arvPiece))
        {
          move.push_front(Coord(n, m));
          break;
        }
        else
        {
          break;
        }
      }
      else
      {
        if (arvPiece.get_type() == TypePiece::EMPTY ||
            (arvPiece.get_type() == board(pos.n, pos.m).get_type() &&
             arvPiece.get_color() == board(pos.n, pos.m).get_color()))
        {
          move.push_front(Coord(n, m));
        }
        else
        {
          break;
        }
      }
      posBox_tmp = board.m_S_mailbox[arv];
      arv = board.m_L_mailbox[posBox_tmp + e];
    }
  }
  return move;
}

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_bishop(Board<N, M> const &board,
                            Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  std::array<int, 4> const move_bishop{{-P - 1, -P + 1, P - 1, P + 1}};
  return get_list_move_rec<MOVE>(board, pos, move_bishop);
}

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_rook(Board<N, M> const &board,
                          Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  std::array<int, 4> const move_rook{{-P, 1, P, -1}};
  return get_list_move_rec<MOVE>(board, pos, move_rook);
}

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_queen(Board<N, M> const &board,
                           Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  std::array<int, 8> const
      move_queen{{-P - 1, -P, -P + 1, -1, 1, P - 1, P, P + 1}};
  return get_list_move_rec<MOVE>(board, pos, move_queen);
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move_pawn(Board<N, M> const &board,
                          Coord const &pos) const noexcept
{
  CONSTEXPR int P{M + 2};
  auto sign_color{
      [](Color color)
      {
        return (color == Color::WHITE) ? -1 : 1;
      }};
  int sign{sign_color(get_color())};
  std::array<int, 3> move_pawn{sign * P, sign * P - 1, sign * P + 1};
  std::forward_list<Coord> list_move{};
  std::size_t current_pos{board.offset(pos.n, pos.m)};
  int posBox{board.m_S_mailbox[current_pos]};
  int arv{board.m_L_mailbox[posBox + move_pawn[0]]};
  if (arv >= 0)
  {
    std::size_t n{arv / M}, m{arv % M};
    if (board(n, m).get_type() == TypePiece::EMPTY ||
        board.get_proba(Coord(n, m)) < 1. - EPSILON)
    {
      list_move.push_front(Coord(n, m));
      if (((pos.n == 1 && get_color() == Color::BLACK) ||
           (pos.n == N - 2 && get_color() == Color::WHITE)) &&
          (board(n + sign, m).get_type() == TypePiece::EMPTY ||
           board.get_proba(Coord(n + sign, m)) < (1. - EPSILON)))
      {
        list_move.push_front(Coord(n + sign, m));
      }
    }
  }
  for (std::size_t i{1}; i < 3; i++)
  {
    arv = board.m_L_mailbox[posBox + move_pawn[i]];
    if (arv >= 0)
    {
      std::size_t n{arv / M}, m{arv % M};
      if ((board(n, m).get_type() != TypePiece::EMPTY &&
           !same_color(board(n, m))) ||
          (board.m_ep != std::nullopt &&
           board.m_ep->n == pos.n &&
           board.m_ep->m == pos.m))
      {
        list_move.push_front(Coord(n, m));
      }
    }
  }
  return list_move;
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Move>
Piece::get_list_promote(Board<N, M> const &board, Coord const &pos) const noexcept
{
  std::forward_list<Coord> list_move{get_list_move_pawn(board, pos)};
  std::forward_list<Move> list_promote;
  std::array<TypePiece, 4> promote_choice{TypePiece::QUEEN,
                                          TypePiece::ROOK,
                                          TypePiece::BISHOP,
                                          TypePiece::KNIGHT};
  for (Coord const &e : list_move)
  {
    for (TypePiece p : promote_choice)
    {
      list_promote.push_front(Move_promote(pos, e, p));
    }
  }
  return list_promote;
}

template <Piece::Move_Mode MOVE, std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_move(Board<N, M> const &board, Coord const &pos) const noexcept
{
  switch (get_type())
  {
  case TypePiece::PAWN:
    if CONSTEXPR (MOVE == Move_Mode::NORMAL)
    {
      return get_list_move_pawn(board, pos);
    }
    else
    {
      return std::forward_list<Coord>{};
    }
  case TypePiece::KNIGHT:
    return get_list_move_knight<MOVE>(board, pos);
  case TypePiece::BISHOP:
    return get_list_move_bishop<MOVE>(board, pos);
  case TypePiece::ROOK:
    return get_list_move_rook<MOVE>(board, pos);
  case TypePiece::QUEEN:
    return get_list_move_queen<MOVE>(board, pos);
  case TypePiece::KING:
    return get_list_move_king<MOVE>(board, pos);
  case TypePiece::EMPTY:
  default:
    return std::forward_list<Coord>{};
  }
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_normal_move(Board<N, M> const &board,
                            Coord const &pos) const noexcept
{
  return get_list_move<Move_Mode::NORMAL>(board, pos);
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Piece::get_list_split_move(Board<N, M> const &board,
                           Coord const &pos) const noexcept
{
  return get_list_move<Move_Mode::SPLIT>(board, pos);
}