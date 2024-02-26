#include <array>
#include <initializer_list>
#include <algorithm>
#include <cassert>
#include <utility>
#include <CMatrix.hpp>
#include <Qubit.hpp>
#include <random>
#include <Piece.hpp>
#include <Unitary.hpp>
#include "GameBoard.hpp"

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

template <std::size_t N, std::size_t M>
double Board<N, M>::get_random_number_0_1()
{
    std::random_device rd;
    std::uniform_real_distribution<> gen(0., 1.);
    return gen(rd);
}

template <std::size_t N, std::size_t M>
bool Board<N, M>::mesure(std::size_t position)
{
    Piece p_actuelle = m_piece_board[position];
    double x = get_random_number_0_1();
    std::size_t indice_mes = 0;
    while (x - std::pow(std::abs(m_board[indice_mes].second), 2) > 0)
    {
        x -= std::pow(std::abs(m_board[indice_mes].second), 2);
        // indice_suppr++,
    }
    bool mes = m_board[indice_mes].first[position];
    double proba_delete = 0;
    //std::size_t nbr_elt_suppr{0};
    for (std::size_t i {std::size(m_board)}; i > 0; i--)
    {
        if (m_board[i-1].first[position] != mes) //?
        {
            proba_delete += std::pow(std::abs(m_board[i-1].second), 2);
            m_board.erase(std::begin(m_board) +std::size(m_board)- i -1 );
        }

    }
    for (auto &e : m_board)
    {
        e.second /= std::sqrt(1.-proba_delete);
    }
    for (std::size_t i {0}; i<N*M; i++)
    {
        if(m_piece_board[i] == p_actuelle )
        {
            for(auto &e : m_board)
            {
                if(e.first[i])
                {
                    break;
                }
                m_piece_board[i] = Piece::EMPTY;
            }
        }
    }
    return mes;
}

/**
 * @brief fonction auxiliaire qui permet de modifier un plateau à l'aide d'un array
 * de la forme du type de retour de la fonction qubitToArray,
 * le deuxième éléments du tableau à une probalité non nulle lors d'un mouv split
 *
 * @tparam Q La taille du qubit
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de collone du plateau
 * @param arrayQubit Type de retour de la fonction quibitToArray, représente la facon dont va être modifier m_board
 * @param position_board  L'indice du plateau dans le tableau de toutes les instances du plateau
 * @param tab_positions Tableau des indices des cases modifiées,
  on utilise N*M+1 pour signifier que le qubit en question est un qubit auxiliaire qui ne modifie pas le board
 */
template <std::size_t N, std::size_t M>
template <std::size_t Q>
constexpr void Board<N, M>::modify(std::array<std::pair<std::array<bool, Q>, std::complex<double>>, 2> const &arrayQubit,
                                   std::size_t position_board, std::array<std::size_t, Q> const &tab_positions)

{
    if (arrayQubit[1].second != 0i)
    {
        std::pair<std::array<bool, N * M>, std::complex<double>> new_b{};
        std::copy(std::begin(m_board[position_board].first),
                  std::end(m_board[position_board].first),
                  std::begin(new_b.first));
        new_b.second = m_board[position_board].second;
        for (std::size_t i{0}; i < Q; i++)
        {
            if (tab_positions[i] < N * M + 1)
            { /*Ca nous permet de mettre des variables dans les qubits
                qui ne sont pas prises en compte lors de la modif du plateau */
                new_b.first[tab_positions[i]] = arrayQubit[1].first[i];
            }
        }
        new_b.second *= arrayQubit[1].second;
        m_board.push_back(new_b);
    }
    for (std::size_t i{0}; i < Q; i++)
    {
        m_board[position_board].first[tab_positions[i]] = arrayQubit[0].first[i];
    }
    m_board[position_board].second *= arrayQubit[0].second;
}

template <std::size_t N, std::size_t M>
template <std::size_t Q>
constexpr void Board<N, M>::move_1_instance(std::array<bool, Q> const &case_modif,
                                            std::size_t position, CMatrix<_2POW(Q)> const &matrix,
                                            std::array<std::size_t, Q> const &tab_positions)
{
    Qubit<Q> q{case_modif};
    auto x{qubitToArray(matrix * q)};
    modify(std::move(x), position, tab_positions);
}

#include <iostream>

template <std::size_t N, std::size_t M>
constexpr void Board<N, M>::move_classic_jump(std::size_t source, std::size_t target)
{
    if (m_piece_board[target] == Piece::EMPTY)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                            MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
        }
        m_piece_board[target] = m_piece_board[source];
        m_piece_board[source] = Piece::EMPTY;
    }
    else
    {
        if (same_color(m_piece_board[source], m_piece_board[target]))
        {
            if (!mesure(target))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                    MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = Piece::EMPTY;
            }
        }
        else
        {
            if (mesure(source))
            {
                for (std::size_t i {0}; i < std::size(m_board); i++)
                {
                    auto const A { MATRIX_JUMP.tensoriel_product(CMatrix<2>::identity()) * CMatrix<2>::identity().tensoriel_product(MATRIX_JUMP) };
                    std::cout << A << std::endl;
                    move_1_instance(std::array<bool, 3>{ m_board[i].first[source], m_board[i].first[target], false}, i,
                                    A , std::array<std::size_t, 3>{source, target, N * M + 1});
                    
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = Piece::EMPTY;
            }
        }
    }
}

template <std::size_t N, std::size_t M>
constexpr void Board<N, M>::move_split_jump(std::size_t source, std::size_t target1, std::size_t target2)
{
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i {0}; i < size_board; i++)
    {
        move_1_instance(std::array<bool, 3>{ m_board[i].first[target2], m_board[i].first[target1], m_board[i].first[source]}, i,
                        MATRIX_SPLIT, std::array<std::size_t, 3>{target2, target1, source });
    }
    m_piece_board[target1] = m_piece_board[source];
    m_piece_board[target2] = m_piece_board[source];
    m_piece_board[source] = Piece::EMPTY;
}

template <std::size_t N, std::size_t M>
constexpr bool Board<N, M>::check_path_straight(Coord const &dpt, Coord const &arv)
{
    if (dpt.n == arv.n)
    {
        for (std::size_t i{std::min(dpt.m, arv.m) + 1}; i < std::max(dpt.m, arv.m); i++)
        {
            if (m_piece_board[offset(dpt.n, i)] != Piece::EMPTY)
            {
                return false;
            }
        }
        return true;
    }
    else if (dpt.m == dpt.m)
    {
        for (std::size_t i{std::min(dpt.n, arv.n) + 1}; i < std::max(dpt.n, arv.n); i++)
        {
            if (m_piece_board[offset(i, dpt.m)] != Piece::EMPTY)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
constexpr bool Board<N, M>::check_path_diagonal(Coord const &dpt, Coord const &arv)
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
            if (m_piece_board[offset(min_lines + i, min_columns + i)] != Piece::EMPTY)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}