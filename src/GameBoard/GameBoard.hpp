#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <complex>
#include <cstddef> 
#include "Piece.hpp"

template <std::size_t N = 8, std::size_t M = N>
class Board final
{
public:
    // Constructeur
    Board() = default;
    Board(std::array<<std::array<Piece, N>,M> const& board);

    // Copie
    Board(Board const &) = delete;
    Board &operator=(Board const &) = delete;

    // Mouvement
    Board(Board &&) = default;
    Board &operator=(Board &&) = default;

    // Destructeur
    ~Board() = default;

private:
    std::vector<std::pair<std::array<bool, N * M>, std::complex<double>>> m_board;
    std::array<Piece, N * M> m_piece_board;
    bool m_couleur;    // Vrai si c'est aux blanc de jouer
    bool m_w_k_castle; // Vrai si les blancs peuvent faire le petit roque
    bool m_w_q_castle; // Vrai si les blancs peuvent faire le grand roque
    bool m_b_k_castle; // Vrai si les noirs peuvent faire le petit roque
    bool m_b_q_castle; // Vrai si les noirs peuvent faire le grand roque
    using Coord = std::pair<std::size_t, std::size_t>;
    std::optional<Coord> m_ep; // Contient la case sur laquelle il est possible de faire une prise en passant
};
#include "GameBoard.tpp"
#endif