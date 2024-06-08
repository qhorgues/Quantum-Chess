// Lib standard
#include <cassert>
#include <utility>
#include <functional>
#include <cmath>
#include <optional>
#include <stdexcept>
#include <iostream>

// Inclusion projet
#include <Piece.hpp>
#include <TypePiece.hpp>
#include <Constexpr.hpp>
#include "Board.hpp"

template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::mesure(Coord const &p,
                                   std::optional<bool> val_mes)
{

  Piece p_actuelle = (*this)(p.n, p.m);
  if (p_actuelle.get_type() == TypePiece::EMPTY)
  {
    return false;
  }
  else
  {
    bool mes;
    if (val_mes == std::nullopt)
    {
      double x = rnd::randreal(0., 1.);
      std::size_t indice_mes = 0;
      double pow_coef{
          std::pow(
              std::abs(m_board[indice_mes].second), 2)};
      std::size_t size_board{std::size(m_board)};
      while (x - pow_coef > 0 && indice_mes < size_board - 1)
      {
        x -= pow_coef;
        indice_mes++;
        if (indice_mes >= size_board)
        {
          throw std::runtime_error("Indice mesuré de mesure trop grand");
        }
        pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);
      }
      mes = m_board[indice_mes].first[offset(p.n, p.m)];
    }
    else
    {
      mes = val_mes.value();
    }
    double proba_delete = 0;
    // std::size_t nbr_elt_suppr{0};
    for (std::size_t i{std::size(m_board)}; i > 0; i--)
    {
      if (m_board[i - 1].first[offset(p.n, p.m)] != mes) //?
      {
        proba_delete +=
            std::pow(std::abs(m_board[i - 1].second), 2);

        m_board.erase(
            std::begin(m_board) + i - 1);
      }
    }
    for (auto &e : m_board)
    {
      e.second /= std::sqrt(1. - proba_delete);
    }
    for (std::size_t i{0}; i < N * M; i++)
    {
      if (p_actuelle.get_type() != TypePiece::EMPTY)
      {
        update_case(i);
      }
    }
    return mes;
  }
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::mesure_capture_slide(
    Coord const &s,
    Coord const &t,
    std::function<bool(Board<N, M> const &,
                       Coord const &,
                       Coord const &,
                       std::size_t,
                       std::optional<Coord>)>
        check_path,
    std::optional<bool> val_mes)
{
  std::size_t position = offset(s.n, s.m);
  Piece p_actuelle = (*this)(s.n, s.m);
  if (p_actuelle.get_type() == TypePiece::EMPTY)
  {
    return false;
  }
  else
  {
    bool mes;
    if (val_mes == std::nullopt)
    {
      double x = rnd::randreal(0., 1.);
      std::size_t indice_mes = 0;
      double pow_coef{
          std::pow(std::abs(m_board[0].second), 2)};
      std::size_t size_board{std::size(m_board)};
      while (x - pow_coef > 0 && indice_mes < size_board - 1)
      {
        x -= pow_coef;
        indice_mes++;
        if (indice_mes >= size_board)
        {
          throw std::runtime_error("Indice mesuré de mesure trop grand");
        }
        pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);

        // indice_suppr++,
      }
      mes = m_board[indice_mes].first[position] &&
            check_path(*this, s, t, indice_mes, std::nullopt);
    }
    else
    {
      mes = val_mes.value();
    }
    double proba_delete = 0;
    // std::size_t nbr_elt_suppr{0};
    for (std::size_t i{std::size(m_board)}; i > 0; i--)
    {
      if ((m_board[i - 1].first[position] &&
           check_path(*this, s, t, i - 1, std::nullopt)) != mes)
      {
        proba_delete +=
            std::pow(std::abs(m_board[i - 1].second), 2);

        m_board.erase(
            std::begin(m_board) + i - 1);
      }
    }
    for (auto &e : m_board)
    {
      e.second /= std::sqrt(1. - proba_delete);
    }
    for (std::size_t i{0}; i < N * M; i++)
    {
      if (p_actuelle.get_type() != TypePiece::EMPTY)
      {
        update_case(i);
      }
    }
    return mes;
  }
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::mesure_castle(
    Coord const &king,
    Coord const &rook,
    std::optional<bool> val_mes)
{
  std::size_t position_king = offset(king.n, king.m);
  std::size_t position_rook = offset(rook.n, rook.m);

  bool mes;
  if (val_mes == std::nullopt)
  {
    double x = rnd::randreal(0., 1.);
    std::size_t indice_mes = 0;
    std::size_t size_board{std::size(m_board)};
    double pow_coef{
        std::pow(std::abs(m_board[0].second), 2)};

    while (x - pow_coef > 0 && indice_mes < size_board - 1)
    {
      x -= pow_coef;
      indice_mes++;
      if (indice_mes >= size_board)
      {
        throw std::runtime_error("Indice mesuré de mesure trop grand");
      }
      pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);
    }
    mes = m_board[indice_mes].first[position_king] &&
          m_board[indice_mes].first[position_rook] &&
          check_path_straight_1_instance(*this, king, rook, indice_mes, std::nullopt);
  }
  else
  {
    mes = val_mes.value();
  }
  double proba_delete = 0;
  // std::size_t nbr_elt_suppr{0};
  for (std::size_t i{std::size(m_board)}; i > 0; i--)
  {
    if ((m_board[i - 1].first[position_king] &&
         m_board[i - 1].first[position_rook] &&
         check_path_straight_1_instance(*this, king, rook, i - 1, std::nullopt)) != mes)
    {
      proba_delete +=
          std::pow(std::abs(m_board[i - 1].second), 2);

      m_board.erase(
          std::begin(m_board) + i - 1);
    }
  }
  for (auto &e : m_board)
  {
    e.second /= std::sqrt(1. - proba_delete);
  }
  for (std::size_t i{0}; i < N * M; i++)
  {
    if (m_piece_board[i].get_type() != TypePiece::EMPTY)
    {
      update_case(i);
    }
  }
  return mes;
}
