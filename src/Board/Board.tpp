// Lib standard
#include <array>
#include <initializer_list>
#include <algorithm>
#include <cassert>
#include <utility>
#include <functional>
#include <cmath>
#include <optional>
#include <stdexcept>

// Inclusion projet
#include <Qubit.hpp>
#include <Piece.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <TypePiece.hpp>
#include <math_utility.hpp>
#include <Constexpr.hpp>
#include <Move.hpp>
#include <Random.hpp>
#include "Board.hpp"

template <std::size_t N, std::size_t M>
CONSTEXPR Board<N, M>::Board()
    : m_board(),
      m_piece_board(),
      m_S_mailbox(),
      m_L_mailbox(),
      m_color_current_player(Color::WHITE),
      m_k_castle({true, true}),
      m_q_castle({true, true}),
      m_ep()
{
    m_board.push_back(std::pair<std::array<bool, 64>,
                                std::complex<double>>{{}, 0});
    init_mailbox(m_S_mailbox, m_L_mailbox);
};

template <std::size_t N, std::size_t M>
CONSTEXPR Board<N, M>::Board(std::initializer_list<
                             std::initializer_list<
                                 Piece>> const &board)
    : m_board(),
      m_piece_board(),
      m_S_mailbox(),
      m_L_mailbox(),
      m_color_current_player(Color::WHITE),
      m_k_castle({true, true}),
      m_q_castle({true, true}),
      m_ep()
{
    init_mailbox(m_S_mailbox, m_L_mailbox);
    m_board.push_back(std::pair<std::array<bool, N * M>,
                                std::complex<double>>{{false}, 1.});
    initializer_list_to_2_array(board, m_board[0].first, m_piece_board);
};

template <std::size_t N, std::size_t M>
CONSTEXPR std::size_t
Board<N, M>::offset(std::size_t ligne, std::size_t colonne) noexcept
{
    return ligne * M + colonne;
};

template <std::size_t N, std::size_t M>
CONSTEXPR std::size_t Board<N, M>::numberLines() noexcept
{
    return N;
};

template <std::size_t N, std::size_t M>
CONSTEXPR std::size_t Board<N, M>::numberColumns() noexcept
{
    return M;
};

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::initializer_list_to_2_array(
    std::initializer_list<
        std::initializer_list<
            Piece>> const &board,
    std::array<bool, N * M> &first_instance,
    std::array<Piece, N * M> &piece_board) noexcept
{

    auto it_tab{std::begin(first_instance)};
    auto it_piece_dst{std::begin(piece_board)};
    assert(std::size(board) <= N && "Value entry out of Board");
    for (std::initializer_list<Piece> const &e : board)
    {
        auto const it_tab_begin_line{it_tab};
        assert(std::size(e) <= M && "Value entry out of Board");
        std::for_each(std::begin(e),
                      std::end(e),
                      [it_tab](Piece piece) mutable
                      -> void
                      {
                        if (piece.get_type() != TypePiece::EMPTY)
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
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::init_mailbox(
    std::array<int, N * M> &S_mailbox,
    std::array<int, (N + 4) * (M + 2)> &L_mailbox) noexcept
{
    std::fill(std::begin(L_mailbox),
              std::begin(L_mailbox) + (2 * (M + 2) + 1), -1);

    std::fill(std::end(L_mailbox) - (2 * (M + 2) + 1),
              std::end(L_mailbox), -1);
    auto offset_L_mailboard{
        [](std::size_t n, std::size_t m)
            -> std::size_t
        {
            return n * (M + 2) + m;
        }};
    for (std::size_t i{0}; i < N; i++)
    {
        L_mailbox[offset_L_mailboard(i + 2, 0)] = -1;
        L_mailbox[offset_L_mailboard(i + 2, M + 1)] = -1;
        for (std::size_t j{0}; j < M; j++)
        {
            L_mailbox[offset_L_mailboard(i + 2, j + 1)] =
                static_cast<int>(Board<N, M>::offset(i, j));
            S_mailbox[Board<N, M>::offset(i, j)] =
                static_cast<int>(offset_L_mailboard(i + 2, j + 1));
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR double Board<N, M>::get_proba(Coord const &pos) const noexcept
{
    double acc{0.};
    for (auto const &e : m_board)
    {
        acc += pow(abs(e.second), 2.) * e.first[offset(pos.n, pos.m)];
    }
    return acc;
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Board<N, M>::get_list_normal_move(Coord const &pos) const
{
    if ((*this)(pos.n, pos.m).get_type() != TypePiece::EMPTY)
    {
        return (*this)(pos.n, pos.m).get_list_normal_move(*this, pos);
    }
    return std::forward_list<Coord>{};
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Board<N, M>::get_list_split_move(Coord const &pos) const
{
    if ((*this)(pos.n, pos.m).get_type() != TypePiece::EMPTY)
    {
        return (*this)(pos.n, pos.m).get_list_split_move(*this, pos);
    }
    return std::forward_list<Coord>{};
}

template <std::size_t N, std::size_t M>
CONSTEXPR Color Board<N, M>::get_current_player() const noexcept
{
    return m_color_current_player;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::change_player() noexcept
{
    if (get_current_player() == Color::WHITE)
    {
        m_color_current_player = Color::BLACK;
    }
    else
    {
        m_color_current_player = Color::WHITE;
    }
}

template <std::size_t N, std::size_t M>
template <std::size_t Q>
CONSTEXPR void Board<N, M>::modify(
    std::array<
        std::pair<
            std::array<bool, Q>,
            std::complex<double>>,
        2> const &arrayQubit,
    std::size_t position_board,
    std::array<std::size_t, Q> const &tab_positions)

{
    if (!complex_equal(arrayQubit[1].second, 0i))
    {
        std::pair<std::array<bool, N * M>, std::complex<double>> new_b{};
        std::copy(std::begin(m_board[position_board].first),
                  std::end(m_board[position_board].first),
                  std::begin(new_b.first));
        new_b.second = m_board[position_board].second;
        for (std::size_t i{0}; i < Q; i++)
        {
            if (tab_positions[i] < N * M + 1)
            { /*Ca nous permet de mettre des variables
                dans les qubits qui ne sont pas prises
                en compte lors de la modif du plateau */
                new_b.first[tab_positions[i]] = arrayQubit[1].first[i];
            }
        }
        new_b.second *= arrayQubit[1].second;
        m_board.push_back(new_b);
    }
    for (std::size_t i{0}; i < Q; i++)
    {
        if (tab_positions[i] < N * M + 1)
        {
            m_board[position_board].first[tab_positions[i]] =
                arrayQubit[0].first[i];
        }
    }
    m_board[position_board].second *= arrayQubit[0].second;
}

template <std::size_t N, std::size_t M>
template <std::size_t Q>
CONSTEXPR void
Board<N, M>::move_1_instance(std::array<bool, Q> const &case_modif,
                             std::size_t position,
                             CMatrix<_2POW(Q)> const &matrix,
                             std::array<std::size_t, Q> const
                                 &tab_positions) noexcept
{
    Qubit<Q> q{case_modif};
    auto x{qubitToArray(matrix * q)};
    modify(std::move(x), position, tab_positions);
}



template <std::size_t N, std::size_t M>
CONSTEXPR Piece const &
Board<N, M>::operator()(std::size_t n, std::size_t m) const noexcept
{
    return m_piece_board[offset(n, m)];
}

template <std::size_t N, std::size_t M>
void Board<N, M>::update_board() noexcept
{
    std::size_t size_board = std::size(m_board);
    for (std::size_t i{size_board}; i > 0; i--)
    {
        using namespace std::complex_literals;
        if (complex_equal(m_board[i - 1].second, 0i))
        {
            m_board.erase(std::begin(m_board) + i - 1);
        }
        else
        {
            for (std::size_t j{i - 1}; j > 0; j--)
            {
                if (m_board[i - 1].first == m_board[j - 1].first)
                {
                    m_board[j - 1].second += m_board[i - 1].second;
                    m_board.erase(std::begin(m_board) + i - 1);
                    break;
                }
            }
        }
    }
}
template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::winning_position(Color c) const noexcept
{
    for (std::size_t i{0}; i < N * M; i++)
    {
        if (m_piece_board[i].get_type() == TypePiece::KING &&
            m_piece_board[i].get_color() != c)
        {
            return false;
        }
    }
    return true;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::move_is_legal(Move const &move) const
{
    if (move.type == TypeMove::NORMAL)
    {
        auto list{get_list_normal_move(move.normal.src)};
        return std::find(
                   std::begin(list),
                   std::end(list),
                   move.normal.arv) !=
               std::end(list);
    }
    else if (move.type == TypeMove::SPLIT)
    {
        std::forward_list<Coord> list{get_list_split_move(move.split.src)};
        std::array<bool, 2> found{};
        std::array<Coord, 2> arv{{move.split.arv1, move.split.arv2}};
        for (Coord const &e : list)
        {
            for (std::size_t i{0}; i < std::size(arv); i++)
            {
                if (e == arv[i])
                {
                    found[i] = true;
                }
            }
            if (std::all_of(
                    std::begin(found),
                    std::end(found),
                    [](bool v) -> bool
                    { return v; }))
            {
                break;
            }
        }
    }
    else if (move.type == TypeMove::MERGE)
    {
        std::forward_list<Coord> list1{
            get_list_split_move(move.merge.src1)};
        std::forward_list<Coord> list2{
            get_list_split_move(move.merge.src2)};

        return std::find(
                   std::begin(list1),
                   std::end(list1),
                   move.merge.arv) !=
                   std::end(list1) &&
               std::find(
                   std::begin(list2),
                   std::end(list2),
                   move.merge.arv) !=
                   std::end(list2);
    }
}

template <std::size_t N, std::size_t M>
void Board<N, M>::update_case(std::size_t pos) noexcept
{
    std::size_t size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        if (m_board[i].first[pos])
        {
            return;
        }
    }
    m_piece_board[pos] = Piece();
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::check_if_use_move_promote(Coord const &pos) const noexcept
{
    return (*this)(pos.n, pos.m).check_if_use_move_promote(*this, pos);
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Move>
Board<N, M>::get_list_promote(Coord const &pos) const noexcept
{
    return (*this)(pos.n, pos.m).get_list_promote(*this, pos);
}

template <std::size_t N, std::size_t M>
template <class UnitaryFunction>
CONSTEXPR void
Board<N, M>::all_move(
    UnitaryFunction func,
    std::optional<Color> color_player) const noexcept
{
    std::unordered_map<TypePiece,
                       std::unordered_map<
                           Coord,
                           std::vector<Coord>,
                           Coord_hash>>
        move_merge{};

    for (std::size_t i{0}; i < numberLines(); i++)
    {
        for (std::size_t j{0}; j < numberColumns(); j++)
        {
            Piece const &piece{(*this)(i, j)};
            double p_proba{get_proba(Coord(i, j))};
            if (piece.get_type() != TypePiece::EMPTY &&
                piece.get_color() == color_player
                                         .value_or(get_current_player()))
            {
                if (!check_if_use_move_promote(Coord(i, j)))
                {
                    std::forward_list<Coord> move_normal{
                        get_list_normal_move(Coord(i, j))};

                    for (Coord const &c : move_normal)
                    {
                        if (func(Move_classic(Coord(i, j), c)))
                        {
                            return;
                        }
                    }

                    std::forward_list<Coord> move_split{
                        get_list_split_move(Coord(i, j))};

                    std::forward_list<Coord>::
                        const_iterator it_move{
                            std::cbegin(move_split)};
                    for (; it_move != std::cend(move_split);
                         it_move++)
                    {
                        for (std::forward_list<Coord>::
                                 const_iterator it2{
                                     std::cbegin(move_split)};
                             it2 != cend(move_split); it2++)
                        {
                            if (it_move == it2)
                            {
                                continue;
                            }
                            if (func(Move_split(Coord(i, j), *it_move, *it2)))
                            {
                                return;
                            }
                        }
                        if (piece.get_type() != TypePiece::PAWN)
                        {
                            if (move_merge
                                    .contains(piece.get_type()))
                            {
                                if (move_merge
                                        .at(piece.get_type())
                                        .contains(*it_move))
                                {
                                    for (Coord const &c :
                                         move_merge
                                             .at(piece.get_type())
                                             .at(*it_move))
                                    {
                                        if (p_proba < 1. ||
                                            get_proba(
                                                Coord(
                                                    c.n,
                                                    c.m)) < 1.)
                                        {
                                            if (func(Move_merge(
                                                    Coord(i, j),
                                                    c,
                                                    *it_move)))
                                            {
                                                return;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    move_merge
                                        .at(piece.get_type())
                                        .insert({*it_move,
                                                 std::vector{
                                                     Coord(i, j)}});
                                }
                            }
                            else
                            {
                                move_merge
                                    .insert(
                                        std::make_pair(
                                            piece
                                                .get_type(),
                                            std::unordered_map<
                                                Coord,
                                                std::vector<Coord>,
                                                Coord_hash>{
                                                std::make_pair(
                                                    *it_move,
                                                    std::vector{
                                                        Coord(i, j)})}));
                            }
                        }
                    }
                }
                else
                {
                    std::forward_list<Move> list{
                        get_list_promote(Coord(i, j))};

                    for (Move const &move : list)
                    {
                        if (func(move))
                        {
                            return;
                        }
                    }
                }
            }
        }
    }
}