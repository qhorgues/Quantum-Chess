// Lib standard
#include <array>
#include <initializer_list>
#include <algorithm>
#include <cassert>
#include <utility>
#include <random>
#include <functional>
#include <cmath>
#include <optional>

// Inclusion projet
#include <Qubit.hpp>
#include <Piece.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <Piece.hpp>
#include <math_utility.hpp>
#include <Constexpr.hpp>
#include "Board.hpp"

bool operator==(Coord const &lhs, Coord const &rhs)
{
    return lhs.n == rhs.n && lhs.m == rhs.m;
}

template <std::size_t N, std::size_t M>
CONSTEXPR Board<N, M>::Board()
    : m_board(),
      m_piece_board(),
      m_S_mailbox(),
      m_L_mailbox(),
      m_couleur(Color::WHITE),
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
                                 observer_ptr<Piece const>>> const &board)
    : m_board(),
      m_piece_board(),
      m_S_mailbox(),
      m_L_mailbox(),
      m_couleur(Color::WHITE),
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
            observer_ptr<Piece const>>> const &board,
    std::array<bool, N * M> &first_instance,
    std::array<observer_ptr<Piece const>, N * M> &piece_board) noexcept
{

    auto it_tab{std::begin(first_instance)};
    auto it_piece_dst{std::begin(piece_board)};
    assert(std::size(board) <= N && "Value entry out of Board");
    for (std::initializer_list<
             observer_ptr<Piece const>> const &e : board)
    {
        std::size_t const size_sub_list{std::size(e)};
        auto const it_tab_begin_line{it_tab};
        assert(size_sub_list <= M && "Value entry out of Board");
        std::for_each(std::begin(e),
                      std::end(e),
                      [it_tab](observer_ptr<Piece const> piece) mutable
                      -> void
                      {
                        if (piece != nullptr)
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
    std::size_t size_array{std::size(m_board)};
    double acc{0.};
    for (auto const &e : m_board)
    {
        acc += pow(abs(e.second), 2.) * e.first[offset(pos.n, pos.m)];
    }
    return acc / static_cast<double>(size_array);
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Board<N, M>::get_list_normal_move(Coord const &pos) const
{
    if ((*this)(pos.n, pos.m) != nullptr)
    {
        return (*this)(pos.n, pos.m)->get_list_normal_move(*this, pos);
    }
    return std::forward_list<Coord>{};
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord>
Board<N, M>::get_list_split_move(Coord const &pos) const
{
    if ((*this)(pos.n, pos.m) != nullptr)
    {
        return (*this)(pos.n, pos.m)->get_list_split_move(*this, pos);
    }
    return std::forward_list<Coord>{};
}

template <std::size_t N, std::size_t M>
double Board<N, M>::get_random_number_0_1()
{
    std::random_device rd;
    std::uniform_real_distribution<> gen(0., 1.);
    return gen(rd);
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::mesure(Coord const &p)
{

    observer_ptr<Piece const> p_actuelle = (*this)(p.n, p.m);
    if (p_actuelle == nullptr)
    {
        return false;
    }
    else
    {
        double x = get_random_number_0_1();
        std::size_t indice_mes = 0;
        double pow_coef{
            std::pow(
                std::abs(m_board[indice_mes].second), 2)};
        while (x - pow_coef > 0)
        {
            x -= pow_coef;
            indice_mes++;
            pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);
        }
        bool mes = m_board[indice_mes].first[offset(p.n, p.m)];
        double proba_delete = 0;
        // std::size_t nbr_elt_suppr{0};
        for (std::size_t i{std::size(m_board)}; i > 0; i--)
        {
            if (m_board[i - 1].first[offset(p.n, p.m)] != mes) //?
            {
                proba_delete +=
                    std::pow(std::abs(m_board[i - 1].second), 2);

                m_board.erase(
                    std::begin(m_board) +
                    std::size(m_board) - i + 1);
            }
        }
        for (auto &e : m_board)
        {
            e.second /= std::sqrt(1. - proba_delete);
        }
        for (std::size_t i{0}; i < N * M; i++)
        {
            if (m_piece_board[i] != nullptr &&
                p_actuelle->get_type() == m_piece_board[i]->get_type())
            {
                for (auto &e : m_board)
                {
                    if (e.first[i])
                    {
                        break;
                    }
                    m_piece_board[i] = nullptr;
                }
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
                       std::size_t)>
        check_path)
{
    std::size_t position = offset(s.n, s.m);
    observer_ptr<Piece const> p_actuelle = (*this)(s.n, s.m);
    if (p_actuelle == nullptr)
    {
        return false;
    }
    else
    {
        double x = get_random_number_0_1();
        std::size_t indice_mes = 0;
        double pow_coef {
            std::pow(std::abs(m_board[0].second, 2))};
        
        while (x - pow_coef > 0)
        {
            x -= pow_coef;
            indice_mes++;
            pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);
            
            // indice_suppr++,
        }
        bool mes = m_board[indice_mes].first[position] &&
                   check_path(*this, s, t, indice_mes);
        double proba_delete = 0;
        // std::size_t nbr_elt_suppr{0};
        for (std::size_t i{std::size(m_board)}; i > 0; i--)
        {
            if ((m_board[i - 1].first[position] &&
                 check_path(*this, s, t, i-1)) != mes)
            {
                proba_delete +=
                    std::pow(std::abs(m_board[i - 1].second), 2);

                m_board.erase(
                    std::begin(m_board) +
                    std::size(m_board) - i + 1);
            }
        }
        for (auto &e : m_board)
        {
            e.second /= std::sqrt(1. - proba_delete);
        }
        for (std::size_t i{0}; i < N * M; i++)
        {
            if (m_piece_board[i] != nullptr &&
                p_actuelle->get_type() == m_piece_board[i]->get_type())
            {
                for (auto &e : m_board)
                {
                    if (e.first[i])
                    {
                        break;
                    }
                    m_piece_board[i] = nullptr;
                }
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
    std::function<bool(Board<N, M> const &,
                       Coord const &,
                       Coord const &,
                       std::size_t)>
        check_path)
{
    std::size_t position_king = offset(king.n, king.m);
    std::size_t position_rook = offset(rook.n, rook.m);
    /*observer_ptr<Piece const> p_actuelle = (*this)(s.n, s.m);
    if (p_actuelle == nullptr)
    {
        return false;
    }*/

    double x = get_random_number_0_1();
    std::size_t indice_mes = 0;
     double pow_coef {
            std::pow(std::abs(m_board[0].second), 2)};
        
    while (x - pow_coef > 0)
    {
        x -= pow_coef;
        indice_mes++;
        pow_coef = std::pow(std::abs(m_board[indice_mes].second), 2);
        
        // indice_suppr++,
    }
    bool mes = m_board[indice_mes].first[position_king] &&
               m_board[indice_mes].first[position_rook] &&
               check_path(*this, king, rook, indice_mes);
    double proba_delete = 0;
    // std::size_t nbr_elt_suppr{0};
    for (std::size_t i{std::size(m_board)}; i > 0; i--)
    {
        if ((m_board[i - 1].first[position_king] &&
             m_board[i - 1].first[position_rook] &&
             check_path(*this, king, rook, i-1)) != mes)
        {
            proba_delete +=
                std::pow(std::abs(m_board[i - 1].second), 2);

            m_board.erase(
                std::begin(m_board) +
                std::size(m_board) - i + 1);
        }
    }
    for (auto &e : m_board)
    {
        e.second /= std::sqrt(1. - proba_delete);
    }
    for (std::size_t i{0}; i < N * M; i++)
    {
        if (m_piece_board[i] != nullptr &&
            (m_piece_board[i]->get_type() == TypePiece::ROOK ||
             m_piece_board[i]->get_type() == TypePiece::KING))
        {
            for (auto &e : m_board)
            {
                if (e.first[i])
                {
                    break;
                }
                m_piece_board[i] = nullptr;
            }
        }
    }
    return mes;
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
                             std::array<std::size_t, Q> const &tab_positions)
{
    Qubit<Q> q{case_modif};
    auto x{qubitToArray(matrix * q)};
    modify(std::move(x), position, tab_positions);
}
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::king_side_castle(Coord const &k,
                                             Coord const &r)
{
    std::size_t king = offset(k.n, k.m);
    std::size_t new_king = offset(k.n , k.m +2);
    std::size_t rook = offset(r.n, r.m);
    std::size_t new_rook = offset(r.n , r.m -2);
    if (mesure_castle(k, r, &check_path_straight_1_instance<N, M>))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{king, new_king});
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{rook, new_rook});
        }
        m_piece_board[new_king] = std::move(m_piece_board[king]);
        m_piece_board[new_rook] = std::move(m_piece_board[rook]);
        m_piece_board[king] = nullptr;
        m_piece_board[rook] = nullptr;
    }
}
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::queen_side_castle(Coord const &k,
                                              Coord const &r)
{
    std::size_t king = offset(k.n, k.m);
    std::size_t new_king = offset(k.n , k.m -2);
    std::size_t rook = offset(r.n, r.m);
    std::size_t new_rook = offset(r.n , r.m +3);
    if (mesure_castle(k, r, &check_path_straight_1_instance<N, M>))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{king, new_king});
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{rook, new_rook});
        }
        m_piece_board[new_king] = std::move(m_piece_board[king]);
        m_piece_board[new_rook] = std::move(m_piece_board[rook]);
        m_piece_board[king] = nullptr;
        m_piece_board[rook] = nullptr;
    }
}
template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_classic_jump(Coord const &s, Coord const &t)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 2>{m_board[i].first[source], false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{source, target});
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = nullptr;
    }
    else
    {
        if (m_piece_board[source]->same_color(*m_piece_board[target]))
        {
            if (!mesure(t))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    move_1_instance(
                        std::array<bool, 2>{m_board[i].first[source], false},
                        i, MATRIX_ISWAP,
                        std::array<std::size_t, 2>{source, target});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
        else
        {
            if (mesure(s))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    auto const A{
                        MATRIX_JUMP
                            .tensoriel_product(
                                CMatrix<2>::identity()) *
                        CMatrix<2>::identity()
                            .tensoriel_product(MATRIX_JUMP)};

                    move_1_instance(
                        std::array<bool, 3>{m_board[i].first[source],
                                            m_board[i].first[target],
                                            false},
                        i,
                        A,
                        std::array<std::size_t, 3>{source,
                                                   target,
                                                   N * M + 1});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_pawn_one_step(Coord const &s, Coord const &t)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 2>{m_board[i].first[source], false},
                i, MATRIX_ISWAP,
                std::array<std::size_t, 2>{source, target});
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = nullptr;
    }
    else
    {
        if (!mesure(t))
        {
            for (std::size_t i{0}; i < std::size(m_board); i++)
            {
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[source], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{source, target});
            }
            m_piece_board[target] = m_piece_board[source];
            m_piece_board[source] = nullptr;
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_pawn_two_step(Coord const &s, Coord const &t)

{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 3>{m_board[i].first[source],
                                    false,
                                    check_path_straight_1_instance(
                                        *this, s, t, i)},
                i, MATRIX_SLIDE,
                std::array<std::size_t, 3>{source, target, N * M + 1});
        }
        m_piece_board[target] = m_piece_board[source];
        m_piece_board[source] = nullptr;
    }
    else
    {
        if (!mesure(t))
        {
            for (std::size_t i{0}; i < std::size(m_board); i++)
            {
                move_1_instance(
                    std::array<bool, 3>{m_board[i].first[source],
                                        false,
                                        check_path_straight_1_instance(
                                            *this, s, t, i)},
                    i, MATRIX_SLIDE,
                    std::array<std::size_t, 3>{source, target, N * M + 1});
            }
            m_piece_board[target] = m_piece_board[source];
            m_piece_board[source] = nullptr;
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::capture_pawn(Coord const &s, Coord const &t)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (mesure(s))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            if (m_board[i].first[target])
            {
                auto const A{
                    MATRIX_JUMP
                        .tensoriel_product(
                            CMatrix<2>::identity()) *
                    CMatrix<2>::identity()
                        .tensoriel_product(MATRIX_JUMP)};
                move_1_instance(
                    std::array<bool, 3>{
                        m_board[i].first[source],
                        m_board[i].first[target],
                        false},
                    i, A,
                    std::array<std::size_t, 3>{source, target, N * M + 1});
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_enpassant(Coord const &s, Coord const &t, Coord const &ep)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t enpassant = offset(ep.n, ep.m);

    if (m_piece_board[target] == nullptr)
    {

        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            if (m_board[i].first[enpassant])
            {
                // permet d'enlever le pion pris en passant
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[enpassant], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{enpassant, N * M + 1});
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[source], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{source, target});
            }
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = nullptr;
        m_piece_board[enpassant] = nullptr;
    }
    else
    {
        if (m_piece_board[source]->same_color(*m_piece_board[target]))
        {
            if (!mesure(t))
            {

                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    if (m_board[i].first[enpassant])
                    {
                        // permet d'enlever le pion pris en passant
                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[enpassant],
                                false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{enpassant, N * M + 1});

                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[source],
                                false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{source, target});
                    }
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = nullptr;
            }
        }
        else
        {
            if (mesure(s))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    if (m_board[i].first[enpassant] ||
                        m_board[i].first[target])
                    {
                        // permet d'enlever le pion pris en passant
                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[enpassant], false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{
                                enpassant, N * M + 1});
                    }
                    // ces deux instructions représentent un mouvement
                    // de capture jump
                    move_1_instance(
                        std::array<bool, 2>{
                            m_board[i].first[target], false},
                        i, MATRIX_ISWAP,
                        std::array<std::size_t, 2>{
                            target, N * M + 1});

                    move_1_instance(
                        std::array<bool, 2>{
                            m_board[i].first[source], false},
                        i, MATRIX_ISWAP,
                        std::array<std::size_t, 2>{
                            source, target});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
                m_piece_board[enpassant] = nullptr;
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_split_jump(Coord const &s, Coord const &t1, Coord const &t2)
{
    std::size_t const source = offset(s.n, s.m);
    std::size_t const target1 = offset(t1.n, t1.m);
    std::size_t const target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 3>{
                m_board[i].first[target2],
                m_board[i].first[target1],
                m_board[i].first[source]},
            i, MATRIX_SPLIT,
            std::array<std::size_t, 3>{target2, target1, source});
    }
    m_piece_board[target1] = m_piece_board[source];
    m_piece_board[target2] = m_piece_board[source];
    m_piece_board[source] = nullptr;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_merge_jump(Coord const &s1, Coord const &s2, Coord const &t)
{
    std::size_t source1 = offset(s1.n, s1.m);
    std::size_t source2 = offset(s2.n, s2.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 3>{
                m_board[i].first[source1],
                m_board[i].first[source2],
                m_board[i].first[target]},
            i, MATRIX_MERGE,
            std::array<std::size_t, 3>{source1, source2, target});
    }
    m_piece_board[target] = m_piece_board[source1];
    m_piece_board[source2] = nullptr;
    m_piece_board[source1] = nullptr;
    update_after_merge();
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_classic_slide(
    Coord const &s,
    Coord const &t,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t)>
        check_path)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 3>{
                    m_board[i].first[source],
                    false,
                    check_path(
                        *this, s, t, i)},
                i, MATRIX_SLIDE,
                std::array<std::size_t, 3>{source, target, N * M + 1});
        }
        m_piece_board[target] = m_piece_board[source];
        m_piece_board[source] = nullptr;
    }
    else
    {
        if (m_piece_board[source]->same_color(*m_piece_board[target]))
        {
            if (!mesure(t))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    move_1_instance(
                        std::array<bool, 3>{
                            m_board[i].first[source],
                            false,
                            check_path(*this, s, t, i)},
                        i, MATRIX_SLIDE,
                        std::array<std::size_t, 3>{source, target, N * M + 1});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
        else
        {
            if (mesure_capture_slide(s, t, check_path))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    auto const A{
                        MATRIX_ISWAP.tensoriel_product(
                            CMatrix<2>::identity()) *
                        CMatrix<2>::identity()
                            .tensoriel_product(MATRIX_ISWAP)};

                    move_1_instance(
                        std::array<bool, 3>{
                            m_board[i].first[source],
                            m_board[i].first[target], false},
                        i, A,
                        std::array<std::size_t, 3>{source, target, N * M + 1});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_split_slide(
    Coord const &s,
    Coord const &t1,
    Coord const &t2,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t)>
        check_path)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target1 = offset(t1.n, t1.m);
    std::size_t target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 5>{
                !check_path(*this, s, t2, i),
                !check_path(*this, s, t1, i),
                m_board[i].first[target2],
                m_board[i].first[target1],
                m_board[i].first[source]},
            i, MATRIX_SPLIT_SLIDE,
            std::array<std::size_t, 5>{
                N * M + 1,
                N * M + 1,
                target2,
                target1,
                source});
        /*La matrice split slide est créée pour
          être utlisé sans appliquer de porte cnot a
          un chemin, nos fonctions check_path renvoie
          un résultat où l'on a appliquer la porte cnot */
    }
    m_piece_board[target1] = m_piece_board[source];
    m_piece_board[target2] = m_piece_board[source];
    m_piece_board[source] = nullptr;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_merge_slide(
    Coord const &s1,
    Coord const &s2,
    Coord const &t,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t)>
        check_path)
{
    std::size_t const source1 = offset(s1.n, s1.m);
    std::size_t const source2 = offset(s2.n, s2.m);
    std::size_t const target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 5>{
                !check_path(*this, s2, t, i),
                !check_path(*this, s1, t, i),
                m_board[i].first[source1],
                m_board[i].first[source2],
                m_board[i].first[target]},
            i, MATRIX_MERGE_SLIDE,
            std::array<std::size_t, 5>{
                N * M + 1,
                N * M + 1,
                source1,
                source2,
                target});
        /* La matrice merge slide est créée pour
           être utlisé sans appliquer de porte cnot au chemin,
           nos fonctions check_path renvoie un résultat où l'on
           a appliquer la porte cnot */
    }
    m_piece_board[target] = m_piece_board[source1];
    m_piece_board[source2] = nullptr;
    m_piece_board[source1] = nullptr;
    update_after_merge();
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_classic(Coord const &s, Coord const &t)
{
    TypePiece piece{(*this)(s.n, s.m)->get_type()};
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
            if(diff_colum ==2)
            {
                if(s.m>t.m)
                {
                    queen_side_castle(s,Coord(s.n, t.m-2));
                }
                else
                {
                    king_side_castle(s, Coord(s.n, t.m +1));
                }
            }
            else
            {
                move_classic_jump(s, t);
            }

        }
        
    case TypePiece::KNIGHT:
        move_classic_jump(s, t);
        break;
    case TypePiece::BISHOP:
        move_classic_slide(s, t, &check_path_diagonal_1_instance<N, M>);
        break;
    case TypePiece::ROOK:
        move_classic_slide(s, t, &check_path_straight_1_instance<N, M>);
        break;
    case TypePiece::QUEEN:
        move_classic_slide(s, t, &check_path_queen_1_instance<N, M>);
        break;
    default:
        break;
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_split(Coord const &s,
                                       Coord const &t1,
                                       Coord const &t2)
{
    TypePiece piece{(*this)(s.n, s.m)->get_type()};
    switch (piece)
    {
    case TypePiece::KING:
    case TypePiece::KNIGHT:
        move_split_jump(s, t1, t2);
        break;
    case TypePiece::BISHOP:
        move_split_slide(s, t1, t2, &check_path_diagonal_1_instance<N, M>);
        break;
    case TypePiece::ROOK:
        move_split_slide(s, t1, t2, &check_path_straight_1_instance<N, M>);
        break;
    case TypePiece::QUEEN:
        move_split_slide(s, t1, t2, &check_path_queen_1_instance<N, M>);
        break;
    case TypePiece::PAWN:
    default:
        break;
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_merge(Coord const &s1,
                                       Coord const &s2,
                                       Coord const &t)
{
    TypePiece piece1{(*this)(s1.n, s1.m)->get_type()};
    TypePiece piece2{(*this)(s2.n, s2.m)->get_type()};
    if (piece1 != piece2)
    {
        return;
    }
    switch (piece1)
    {
    case TypePiece::KING:
    case TypePiece::KNIGHT:
        move_merge_jump(s1, s2, t);
        break;
    case TypePiece::BISHOP:
        move_merge_slide(s1, s2, t, &check_path_diagonal_1_instance<N, M>);
        break;
    case TypePiece::ROOK:
        move_merge_slide(s1, s2, t, &check_path_straight_1_instance<N, M>);
        break;
    case TypePiece::QUEEN:
        move_merge_slide(s1, s2, t, &check_path_queen_1_instance<N, M>);
        break;
    default:
        break;
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_pawn(Coord const &s, Coord const &t)
{
    auto abs_diff{[](std::size_t x, std::size_t y) -> std::size_t
                  {
                      return (x >= y) ? x - y : y - x;
                  }};
    std::size_t diff_line{abs_diff(s.n, t.n)};
    if (diff_line == 2)
    {
        move_pawn_two_step(s, t);
    }
    else if (diff_line == 1)
    {
        std::size_t diff_col{abs_diff(s.m, t.m)};
        if (diff_col == 1)
        {

            if (m_ep != std::nullopt && m_ep == t)
            {
                int step{
                    ((*this)(s.n, s.m)->get_color() ==
                     Color::WHITE)
                        ? -1
                        : 1};
                Coord ep;
                ep.m = m_ep->m;
                ep.n = m_ep->m + step;
                move_enpassant(s, t, ep);
            }
            else
            {
                capture_pawn(s, t);
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move(Move const &movement)
{
    switch (movement.type)
    {
    case TypeMove::NORMAL:
        move_classic(movement.normal.src, movement.normal.arv);
    case TypeMove::SPLIT:
        move_split(movement.split.src,
                   movement.split.arv1,
                   movement.split.arv2);
    case TypeMove::MERGE:
        move_merge(movement.merge.src1,
                   movement.merge.src1,
                   movement.merge.arv);
    default:
        return;
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR observer_ptr<Piece const>
Board<N, M>::operator()(std::size_t n, std::size_t m) const noexcept
{
    return m_piece_board[offset(n, m)];
}

template <std::size_t N>
CONSTEXPR bool
operator==(std::array<bool, N> const &t1,
           std::array<bool, N> const &t2) noexcept
{
    for (std::size_t i{0}; i < N; i++)
    {
        if (t1[i] != t2[i])
        {
            return false;
        }
    }
    return true;
}

template <std::size_t N, std::size_t M>
void Board<N, M>::update_after_merge() noexcept
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
                }
            }
        }
    }
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