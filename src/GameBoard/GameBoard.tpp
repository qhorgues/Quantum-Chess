#include "GameBoard.hpp"
#include <array>
#include <initializer_list>
#include <algorithm>
#include <cassert>
#include <utility>

template <std::size_t N, std::size_t M>
constexpr Board<N, M>::Board(std::initializer_list<std::initializer_list<Piece>> const &board)
    : m_board(),
      m_piece_board(),
      m_couleur(Color::WHITE),
      m_w_k_castle(true),
      m_w_q_castle(true),
      m_b_k_castle(true),
      m_b_q_castle(true),
      m_ep()
{
    std::pair p { initializer_list_to_2_array(board) };
    m_piece_board = std::move(p.second);
    m_board.push_back(std::move(std::pair{std::move(p.first), 1.}));
};

template <std::size_t N, std::size_t M>
constexpr std::size_t Board<N, M>::offset(std::size_t ligne, std::size_t colonne)
{
    return ligne * M + colonne;
};

template <std::size_t N, std::size_t M>
constexpr std::size_t Board<N, M>::numberLines() noexcept
{
    return N;
};

template <std::size_t N, std::size_t M>
constexpr std::size_t Board<N, M>::numberColumns() noexcept
{
    return M;
};

template <std::size_t N, std::size_t M>
constexpr std::pair<std::array<bool, N * M>, std::array<Piece, N * M>>
Board<N, M>::initializer_list_to_2_array(std::initializer_list<std::initializer_list<Piece>> const &board) noexcept
{
    std::pair<std::array<bool, N * M>, std::array<Piece, N * M>> p{};
    auto it_tab{std::begin(p.first)};
    auto it_piece_dst{std::begin(p.second)};
    assert(std::size(board) <= N && "Value entry out of Board");
    for (std::initializer_list<Piece> const &e : board)
    {
        std::size_t const size_sub_list{std::size(e)};
        auto const it_tab_begin_line{it_tab};
        assert(size_sub_list <= M && "Value entry out of Board");
        std::for_each(std::begin(e), std::end(e), [it_tab](Piece piece) mutable -> void
                      {
                        if (piece != Piece::EMPTY)
                        {
                            *it_tab = true;
                        }
                        else
                        {
                            *it_tab = false;
                        }
                        it_tab++; });
        std::move(std::begin(e), std::end(e), it_piece_dst);
        it_tab = it_tab_begin_line + M;
        it_piece_dst += M;
    }
    return p;
}