#ifndef CHECK_PATH_HPP
#define CHECK_PATH_HPP

#include <Coord.hpp>
#include <Board.hpp>
#include <constexpr.hpp>

template <std::size_t N, std::size_t M>
CONSTEXPR static bool check_path_straight(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

template <std::size_t N, std::size_t M>
CONSTEXPR static bool check_path_diagonal(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;
/**
 * @brief Vérififie si il y a une pièce entre deux cases sur une instance du plateau pour un mouvement orthogonale 
 * 
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param board Le plateau
 * @param dpt Les coordonnées de la case de départ
 * @param arv Les coordonnées de la case d'arrivé
 * @param position L'instance du plateau
 * @return true si le mouvement est possible
 * @return false si le mouvement est bloqué, on si ce n'est pas un mouvement orthogonal
 */
template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_straight_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);
/**
 * @brief Vérififie si il y a une pièce entre deux cases sur une instance du plateau pour un mouvement diagonale.
 * 
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param board Le plateau
 * @param dpt Les coordonnées de la case de départ
 * @param arv Les coordonnées de la case d'arrivé
 * @param position L'instance du plateau
 * @return true si le mouvement est possible
 * @return false si le mouvement est bloqué, on si ce n'est pas un mouvement diagonale.
 */
template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_diagonal_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);

/**
 * @brief Vérififie si il y a une pièce entre deux cases sur une instance du plateau pour un mouvement orthogonale ou diagonale.
 * 
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonnes du plateau
 * @param board Le plateau
 * @param dpt Les coordonnées de la case de départ
 * @param arv Les coordonnées de la case d'arrivé
 * @param position L'instance du plateau
 * @return true si le mouvement est possible
 * @return false si le mouvement est bloqué, on si ce n'est pas un mouvement orthogonal ou diagonale.
 */
template <std::size_t N, std::size_t M>
CONSTEXPR bool check_path_queen_1_instance(Board<N, M> const &board, Coord const &dpt, Coord const &arv, std::size_t position);

#include "check_path.tpp"

#endif