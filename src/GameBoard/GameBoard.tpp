#include <array>
#include <initializer_list>
#include <algorithm>
#include <cassert>
#include <utility>
#include "GameBoard.hpp"
#include <CMatrix.hpp>

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
    std::pair p{initializer_list_to_2_array(board)};
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

template <std::size_t N, std::size_t M1, std::size_t M2>
void Board<M1, M2>::modify(std::array<std::pair<std::array<bool, N>, std::complex<double>>, 2> arrayQubit,
                           std::size_t position_board, std::array<std::size_t, N> tab_positions)

{
    if (arrayQubit[1].first != 0i)
    {
        std::pair<std::array<bool, M1 * M2>, std::complex<double>> new_b{};
        std::copy(std::begin(m_board[position_board].first),
                  std::end(m_board[position_board].first),
                  std::begin(new_b.first));
        new_b.second = m_board[position_board].second;
        for (std::size_t i{0}; i < N; i++)
        {
            if (tab_positions[i] < M1 * M2 + 1)
            { /*Ca nous permet de mettre des variables dans les qubits
                qui ne sont pas prises en compte lors de la modif du plateau */
                new_b.first[tab_positions[i]] = arrayQubit[1].first[i];
            }
        }
        new_b.second *= arrayQubit[1].second;
        m_board.push_back(new_b)
    }
    for (std::size_t i{0}; i < N; i++)
    {
        m_board[position_board].first[tab_positions[i]] = arrayQubit[0].first[i];
    }
    m_board[position_board].second *= arrayQubit[0].second;
}

template <std::size_t N, std::size_t M1, std::size_t M2>
void Board<M1, M2>::mouvement1board(std::array<bool, N> case_modif,
                     std::size_t position, Matrix<_2POW(N)> matrix, std::array<std::size_t, N> tab_positions)
{
    Qubit<N> q{case_modif};
    modify_board(board, qubitToArray(matrix * q), position, tab_positions);
    return;
}


template <std::size_t N, std::size_t M>
bool Board<N, M>::check_path_straight(Coord const& dpt, Coord const& arv)
{
    if (dpt.n == arv.n)
    {
        for (std::size_t i {std::min(dpt.m) +1}; i < std::max(dpt.m); i++)
        {
            if (m_piece_board[offset(dpt.n, i)] != Piece::EMPTY)
            {
                return false;
            }
        }
    }
    else if (dpt.m == dpt.m)
    {
        for (std::size_t i {std::min(dpt.n) +1}; i < std::max(dpt.n); i++)
        {
            if (m_piece_board[offset(i, dpt.m)] != Piece::EMPTY)
            {
                return false;
            }
        }
    }
    else 
        return false;
    return true;
}

template <std::size_t N, std::size_t M>
bool Board<N, M>::check_path_diagonal(Coord const& dpt, Coord const& arv)
{
    auto size_t_abs = []()
    if (std::abs(dpt.n) == arv.n)
    {
        
    }
    else if (dpt.m == dpt.m)
    {
        for (std::size_t i {std::min(dpt.n) +1}; i < std::max(dpt.n); i++)
        {
            if (m_piece_board[offset(i, dpt.m)] != Piece::EMPTY)
            {
                return false;
            }
        }
    }
    else 
        return false;
    return true;
}