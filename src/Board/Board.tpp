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
#include <constexpr.hpp>
#include "Board.hpp"

bool operator==(Coord const& lhs, Coord const& rhs)
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
    m_board.push_back(std::pair<std::array<bool, 64>, std::complex<double>>{{}, 0});
    init_mailbox(m_S_mailbox, m_L_mailbox);
};

template <std::size_t N, std::size_t M>
CONSTEXPR Board<N, M>::Board(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board)
    : m_board(),
      m_piece_board(),
      m_S_mailbox(),
      m_L_mailbox(),
      m_couleur(Color::WHITE),
      m_k_castle({true, true}),
      m_q_castle({true, true}),
      m_ep()
{
    std::pair p{initializer_list_to_2_array(board)};
    std::move(std::begin(p.second), std::end(p.second), std::begin(m_piece_board));
    m_board.push_back(std::move(std::pair{std::move(p.first), 1.}));
    init_mailbox(m_S_mailbox, m_L_mailbox);
};

template <std::size_t N, std::size_t M>
CONSTEXPR std::size_t Board<N, M>::offset(std::size_t ligne, std::size_t colonne) noexcept
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
CONSTEXPR std::pair<std::array<bool, N * M>, std::array<observer_ptr<Piece const>, N * M>>
Board<N, M>::initializer_list_to_2_array(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board) noexcept
{
    std::pair<std::array<bool, N * M>, std::array<observer_ptr<Piece const>, N * M>> p{};
    auto it_tab{std::begin(p.first)};
    auto it_piece_dst{std::begin(p.second)};
    assert(std::size(board) <= N && "Value entry out of Board");
    for (std::initializer_list<observer_ptr<Piece const>> const &e : board)
    {
        std::size_t const size_sub_list{std::size(e)};
        auto const it_tab_begin_line{it_tab};
        assert(size_sub_list <= M && "Value entry out of Board");
        std::for_each(std::begin(e), std::end(e), [it_tab](observer_ptr<Piece const> piece) mutable -> void
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
    return p;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::init_mailbox(std::array<int, N * M> &S_mailbox, std::array<int, (N + 4) * (M + 2)> &L_mailbox) noexcept
{
    std::fill(std::begin(L_mailbox), std::begin(L_mailbox) + (2 * (M + 2) + 1), -1);
    std::fill(std::end(L_mailbox) - (2 * (M + 2) + 1), std::end(L_mailbox), -1);
    auto offset_L_mailboard{[](std::size_t n, std::size_t m) -> std::size_t
                            {
                                return n * (M + 2) + m;
                            }};
    for (std::size_t i{0}; i < N; i++)
    {
        L_mailbox[offset_L_mailboard(i + 2, 0)] = -1;
        L_mailbox[offset_L_mailboard(i + 2, M + 1)] = -1;
        for (std::size_t j{0}; j < M; j++)
        {
            L_mailbox[offset_L_mailboard(i + 2, j + 1)] = static_cast<int>(Board<N, M>::offset(i, j));
            S_mailbox[Board<N, M>::offset(i, j)] = static_cast<int>(offset_L_mailboard(i + 2, j + 1));
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
CONSTEXPR std::forward_list<Coord> Board<N, M>::get_list_normal_move(Coord const& pos) const
{
    if ((*this)(pos.n, pos.m) != nullptr)
    {
        return (*this)(pos.n, pos.m)->get_list_normal_move(*this, pos);
    }
    return std::forward_list<Coord>{};
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::forward_list<Coord> Board<N, M>::get_list_split_move(Coord const& pos) const
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

/**
 * @brief Mesure la présence d'une pièce
 *
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param position La position de la pièce mesurée
 * @return true Si la pièce est présente sur la case position
 * @return false Sinon
 */
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
        double pow_coef { std::pow(std::abs(m_board[indice_mes].second), 2)};
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
                proba_delete += std::pow(std::abs(m_board[i - 1].second), 2);
                m_board.erase(std::begin(m_board) + std::size(m_board) - i - 1);
            }
        }
        for (auto &e : m_board)        {
            e.second /= std::sqrt(1. - proba_delete);
        }
        for (std::size_t i{0}; i < N * M; i++)
        {
            if (m_piece_board[i] != nullptr && p_actuelle->get_type() == m_piece_board[i]->get_type())
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
/**
 * @brief Fonction qui permet de faire une mesure dans le cas d'un mouvement "capture slide"
 *
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source du mouvement
 * @param t Coordonnées de la cible du mouvement
 * @param check_path Fonction qui permet de vérifier si il y a une pièce entre la source et la cible sur une instance du plateau
 * @return true Si la mesure indique de faire le mouvement
 * @return false Sinon
 */
template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::mesure_capture_slide(Coord const &s, Coord const &t,
                                       std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path)
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
        while (x - std::pow(std::abs(m_board[indice_mes].second), 2) > 0)
        {
            x -= std::pow(std::abs(m_board[indice_mes].second), 2);
            // indice_suppr++,
        }
        bool mes = m_board[indice_mes].first[position] && check_path(*this, s, t, indice_mes);
        double proba_delete = 0;
        // std::size_t nbr_elt_suppr{0};
        for (std::size_t i{std::size(m_board)}; i > 0; i--)
        {
            if ((m_board[i - 1].first[position] && check_path(*this, s, t, i)) != mes)
            {
                proba_delete += std::pow(std::abs(m_board[i - 1].second), 2);
                m_board.erase(std::begin(m_board) + std::size(m_board) - i - 1);
            }
        }
        for (auto &e : m_board)
        {
            e.second /= std::sqrt(1. - proba_delete);
        }
        for (std::size_t i{0}; i < N * M; i++)
        {
            if (m_piece_board[i] != nullptr && p_actuelle->get_type() == m_piece_board[i]->get_type())
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
CONSTEXPR void Board<N, M>::modify(std::array<std::pair<std::array<bool, Q>, std::complex<double>>, 2> const &arrayQubit,
                                   std::size_t position_board, std::array<std::size_t, Q> const &tab_positions)

{
    if (!complex_equal(arrayQubit[1].second , 0i))
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
        if (tab_positions[i] < N*M+1)
        {
        m_board[position_board].first[tab_positions[i]] = arrayQubit[0].first[i];
        }
    }
    m_board[position_board].second *= arrayQubit[0].second;
}
/**
 * @brief Fonction qui permet d'effectuer un mouvement sur une instance du plateau
 *
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @tparam Q La taille du qubit
 * @param case_modif Permet d'initialiser le qubit
 * @param position L'instance du plateau modifiée
 * @param matrix La matrice du mouvement que l'on veut effectuer
 * @param tab_positions La position sur le plateau des variables utilisées dans le qubit,
  que l'on met à N*M+1 si les variables ne représentent pas des pièces
 */
template <std::size_t N, std::size_t M>
template <std::size_t Q>
CONSTEXPR void Board<N, M>::move_1_instance(std::array<bool, Q> const &case_modif,
                                            std::size_t position, CMatrix<_2POW(Q)> const &matrix,
                                            std::array<std::size_t, Q> const &tab_positions)
{
    Qubit<Q> q{case_modif};
    auto x{qubitToArray(matrix * q)};
    modify(std::move(x), position, tab_positions);
}

/**
 * @brief Mouvement classique d'une pièce qui "saute" (cavalier, roi ou pion)
 *
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source du mouvement
 * @param t Coordonnées de la cible du mouvement
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_classic_jump(Coord const &s, Coord const &t)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                            MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
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
                    move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                    MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
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
                    auto const A{MATRIX_JUMP.tensoriel_product(CMatrix<2>::identity()) * CMatrix<2>::identity().tensoriel_product(MATRIX_JUMP)};
                    move_1_instance(std::array<bool, 3>{m_board[i].first[source], m_board[i].first[target], false}, i,
                                    A, std::array<std::size_t, 3>{source, target, N * M + 1});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
    }
}
/**
 * @brief Le mouvement de piond'une case fonctionne comme un mouvement de jump classique, à la différence qu'un pion ne peut pas capturer une pièce en avançant,
 on mesure de la même façon qu'un jump classique en considérant toutes les pièces comme des pièces alliées
 *
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source
 * @param t Coordonnées de la cible
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_pawn_one_step(Coord const &s, Coord const &t)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                            MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
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
                move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
            }
            m_piece_board[target] = m_piece_board[source];
            m_piece_board[source] = nullptr;
        }
    }
}
/**
 * @brief Le mouvement de piond'une case fonctionne comme un mouvement de slide classique, à la différence qu'un pion ne peut pas capturer une pièce en avançant,
 on mesure de la même façon qu'un slide classique en considérant toutes les pièces comme des pièces alliées
 * @warning Aucun test sur la possibilité de faire un mouvement de deux cases du pion, pas de mise a jour du board sur la prise en passant
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source
 * @param t Coordonnées de la cible
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_pawn_two_step(Coord const &s, Coord const &t)

{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(std::array<bool, 3>{m_board[i].first[source], false, check_path_straight_1_instance(*this, s, t, i)}, i,
                            MATRIX_SLIDE, std::array<std::size_t, 3>{source, target, N * M + 1});
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
                move_1_instance(std::array<bool, 3>{m_board[i].first[source], false, check_path_straight_1_instance(*this, s, t, i)}, i,
                                MATRIX_SLIDE, std::array<std::size_t, 3>{source, target, N * M + 1});
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
                auto const A {MATRIX_JUMP.tensoriel_product(CMatrix<2>::identity()) * CMatrix<2>::identity().tensoriel_product(MATRIX_JUMP)};
                move_1_instance(std::array<bool, 3>{m_board[i].first[source], m_board[i].first[target], false}, i,
                                A, std::array<std::size_t, 3>{source, target, N * M + 1});
            }
        }
    }
}
/**
 * @brief Permet d'effectuer un mouvement de prise en passant
 * @warning Aucun test sur la validité de la cible et de la cible en passant, ni sur le type de la pièce
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnnes du plateau
 * @param s Les coordonnées de la source
 * @param t Les coordonnées de la cible (l'endroit où arrive le pion)
 * @param ep Les coordonnées du pion capturer "en passant"
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_enpassant(Coord const &s, Coord const &t, Coord const &ep)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t enpassant = offset(ep.n, ep.m);

    if (m_piece_board[target] == nullptr)
    {
        
            std::size_t const size_board{std::size(m_board)};
            for (std::size_t i{0}; i < size_board; i++)
            {
                if(m_board[i].first[enpassant]) {
                move_1_instance(std::array<bool, 2>{m_board[i].first[enpassant], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{enpassant, N*M+1}); //permet d'enlever le pion pris en passant
                move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
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
            if(!mesure(t))
            {
            
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    if(m_board[i].first[enpassant])
                    {
                    move_1_instance(std::array<bool, 2>{m_board[i].first[enpassant], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{enpassant, N*M+1}); //permet d'enlever le pion pris en passant
                move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
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
                    if(m_board[i].first[enpassant] || m_board[i].first[target])
                    move_1_instance(std::array<bool, 2>{m_board[i].first[enpassant], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{enpassant, N*M+1}); //permet d'enlever le pion pris en passant
                       move_1_instance(std::array<bool, 2>{m_board[i].first[target], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{target, N*M+1});   //ces deux instructions représentent un mouvement de capture jump        
                move_1_instance(std::array<bool, 2>{m_board[i].first[source], false}, i,
                                MATRIX_ISWAP, std::array<std::size_t, 2>{source, target});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
                m_piece_board[enpassant] = nullptr;
            }
        }
    }
}

/**
 * @brief Mouvement "split jump"
 * @warning Aucun tests sur la validité du mouvement (cible vide, ect)
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source du mouvement
 * @param t1 Coordonnées de la cible 1 (qui doit être vide)
 * @param t2 Coordonnées de la cible 2 (qui doit être vide)
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_split_jump(Coord const &s, Coord const &t1, Coord const &t2)
{
    std::size_t const source = offset(s.n, s.m);
    std::size_t const target1 = offset(t1.n, t1.m);
    std::size_t const target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(std::array<bool, 3>{m_board[i].first[target2], m_board[i].first[target1], m_board[i].first[source]}, i,
                        MATRIX_SPLIT, std::array<std::size_t, 3>{target2, target1, source});
    }
    m_piece_board[target1] = m_piece_board[source];
    m_piece_board[target2] = m_piece_board[source];
    m_piece_board[source] = nullptr;
}

/**
 * @brief Mouvement de merge
 * @warning Aucun test sur la validité du mouvement (cible vide, pièce identique sur les sources, ect)
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s1 Coordonnées de la source 1
 * @param s2 Coordonnées de la source 2
 * @param t Coordonnées de la cible
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_merge_jump(Coord const &s1, Coord const &s2, Coord const &t)
{
    std::size_t source1 = offset(s1.n, s1.m);
    std::size_t source2 = offset(s2.n, s2.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(std::array<bool, 3>{m_board[i].first[source1], m_board[i].first[source2], m_board[i].first[target]}, i,
                        MATRIX_MERGE, std::array<std::size_t, 3>{source1, source2, target});
    }
    m_piece_board[target] = m_piece_board[source1];
    m_piece_board[source2] = nullptr;
    m_piece_board[source1] = nullptr;
}

/**
 * @brief Mouvement classique d'un pièce qui "glisse" (fou, dame, tour)
 *
 * @tparam N Le nombre de lignes
 * @tparam M Le nombre de colonnes
 * @param s Coordonnées de la source
 * @param t Coordonnées de la cible
 * @param check_path  Fonction qui permet de vérifier la présence d'une pièce entre la source et la cible sur une instance du plateau
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_classic_slide(Coord const &s, Coord const &t,
                                               std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target] == nullptr)
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(std::array<bool, 3>{m_board[i].first[source], false, check_path(*this, s, t, i)}, i,
                            MATRIX_SLIDE, std::array<std::size_t, 3>{source, target, N * M + 1});
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
                    move_1_instance(std::array<bool, 3>{m_board[i].first[source], false, check_path(*this, s, t, i)}, i,
                                    MATRIX_SLIDE, std::array<std::size_t, 3>{source, target, N * M + 1});
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
                    auto const A{MATRIX_ISWAP.tensoriel_product(CMatrix<2>::identity()) * CMatrix<2>::identity().tensoriel_product(MATRIX_ISWAP)};

                    move_1_instance(std::array<bool, 3>{m_board[i].first[source], m_board[i].first[target], false}, i,
                                    A, std::array<std::size_t, 3>{source, target, N * M + 1});
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = nullptr;
            }
        }
    }
}

/**
 * @brief Mouvement "split slide"
 * @warning Aucun tests sur la validité du mouvement (cible vide, ect)
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s Coordonnées de la source du mouvement
 * @param t1 Coordonnées de la cible 1 (qui doit être vide)
 * @param t2 Coordonnées de la cible 2 (qui doit être vide)
 * @param check_path Fonction qui permet de vérifier la présence d'une pièce entre la source et la cible sur une instance du plateau
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_split_slide(Coord const &s, Coord const &t1, Coord const &t2,
                                             std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target1 = offset(t1.n, t1.m);
    std::size_t target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(std::array<bool, 5>{!check_path(*this, s, t2, i), !check_path(*this, s, t1, i), m_board[i].first[target2], m_board[i].first[target1], m_board[i].first[source]}, i,
                        MATRIX_SPLIT_SLIDE, std::array<std::size_t, 5>{N * M + 1, N * M + 1, target2, target1, source});
                        //La matrice split slide est créée pour pour être utlisé sans appliquer de porte cnot au chemin, nos fonctions check_path renvoie un résultat où l'on a appliquer la porte cnot
    }
    m_piece_board[target1] = m_piece_board[source];
    m_piece_board[target2] = m_piece_board[source];
    m_piece_board[source] = nullptr;
}

/**
 * @brief Mouvement de merge
 * @warning Aucun test sur la validité du mouvement (cible vide, pièce identique sur les sources, ect)
 * @tparam N Le nombre de lignes du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param s1 Coordonnées de la source 1
 * @param s2 Coordonnées de la source 2
 * @param t Coordonnées de la cible
 * @param check_path Fonction qui permet de vérifier la présence d'une pièce entre la source et la cible sur une instance du plateau
 */
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_merge_slide(Coord const &s1, Coord const &s2, Coord const &t,
                                             std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path)
{
    std::size_t const source1 = offset(s1.n, s1.m);
    std::size_t const source2 = offset(s2.n, s2.m);
    std::size_t const target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(std::array<bool, 5>{!check_path(*this, s2, t, i), !check_path(*this, s1, t, i), m_board[i].first[source1], m_board[i].first[source2], m_board[i].first[target]}, i,
                        MATRIX_MERGE_SLIDE, std::array<std::size_t, 5>{N * M + 1, N * M + 1, source1, source2, target});
                        //La matrice merge slide est créée pour pour être utlisé sans appliquer de porte cnot au chemin, nos fonctions check_path renvoie un résultat où l'on a appliquer la porte cnot
    }
    m_piece_board[target] = m_piece_board[source1];
    m_piece_board[source2] = nullptr;
    m_piece_board[source1] = nullptr;
}

template <std::size_t N, std::size_t M>
CONSTEXPR observer_ptr<Piece const> Board<N, M>::operator()(std::size_t n, std::size_t m) const noexcept
{
    return m_piece_board[offset(n, m)];
}

template <std::size_t N>
CONSTEXPR bool operator==(std::array<bool, N> t1, std::array<bool, N> t2) noexcept
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
CONSTEXPR bool Board<N, M>::move_is_legal(Move const &move) const
{
    if (move.type == TypeMove::NORMAL)
    {
        auto list { get_list_normal_move(move.normal.src) };
        return std::find(std::begin(list), std::end(list), move.normal.arv) != std::end(list);
    }
    else if (move.type == TypeMove::SPLIT)
    {
        std::forward_list<Coord> list { get_list_split_move(move.split.src) };
        std::array<bool, 2> found {};
        std::array<Coord, 2> arv { {move.split.arv1, move.split.arv2} };
        for (Coord const& e : list)
        {
            for (std::size_t i {0}; i < std::size(arv); i++)
            {
                if (e == arv[i])
                {
                    found[i] = true;
                }
            }
            if (std::all_of(std::begin(found), std::end(found), [](bool v) -> bool { return v; }))
            {
                break;
            }
        }
    }
    else if (move.type == TypeMove::MERGE)
    {
        std::forward_list<Coord> list1 { get_list_split_move(move.merge.src1) };
        std::forward_list<Coord> list2 { get_list_split_move(move.merge.src2) };

    }

}