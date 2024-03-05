#ifndef COMPUTER_PLAYER_HPP
#define COMPUTER_PLAYER_HPP
#include <cstddef>
#include <Board.hpp>
#include <Constexpr.hpp>

namespace computer
{
    /**
     * @brief Renvoie une évaluation du plateau
     * @details L'évaluation du plateau est calculé
     * en sommant toutes les pièces d'un joueur
     * coéfficienté par leurs probabilités et en soustrayant
     * avec ce meme calcul les pièces de l'autre joueur
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param board Le plateau de jeu
     * @return double L'évaluation du plateau
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR double heuristic(Board<N, M> const& board) noexcept;
}

#include "ComputerPlayer.tpp"
#endif