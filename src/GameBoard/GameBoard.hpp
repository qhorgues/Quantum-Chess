#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <complex>
#include <cstddef> 
#include <initializer_list>
#include "Piece.hpp"

struct Coord
{
    std::size_t n;
    std::size_t m;
};


template <std::size_t N = 8, std::size_t M = N>
class Board final
{
public:
    // Constructeur
    constexpr Board() = default;
    constexpr Board(std::initializer_list<std::initializer_list<Piece>> const& board);

    // Copie
    constexpr Board(Board const &) = delete;
    constexpr Board &operator=(Board const &) = delete;

    // Mouvement
    constexpr Board(Board &&) = default;
    constexpr Board &operator=(Board &&) = default;

    // Destructeur
    constexpr ~Board() = default;
    
    constexpr static std::size_t numberLines() noexcept;
    constexpr static std::size_t numberColumns() noexcept;

    

private:
    constexpr std::size_t offset(std::size_t ligne, std::size_t colonne);
    constexpr static std::pair<std::array<bool, N * M>, std::array<Piece, N * M>>
        initializer_list_to_2_array(std::initializer_list<std::initializer_list<Piece>> const &board) noexcept;

    template <std::size_t P>
    void Board<N, M>::modify(std::array<std::pair<std::array<bool, P>, std::complex<double>>, 2> arrayQubit, 
                             std::size_t position_board, 
                             std::array<std::size_t, P> tab_positions);

    bool check_path_straight(Coord const& dpt, Coord const& arv);


    std::vector<std::pair<std::array<bool, N * M>, std::complex<double>>> m_board;
    std::array<Piece, N * M> m_piece_board;

    enum class Color : bool
    {
        BLACK = false,
        WHITE = true
    };

    Color m_couleur;    // Vrai si c'est aux blanc de jouer
    bool m_w_k_castle; // Vrai si les blancs peuvent faire le petit roque
    bool m_w_q_castle; // Vrai si les blancs peuvent faire le grand roque
    bool m_b_k_castle; // Vrai si les noirs peuvent faire le petit roque
    bool m_b_q_castle; // Vrai si les noirs peuvent faire le grand roque
    std::optional<Coord> m_ep; // Contient la case sur laquelle il est possible de faire une prise en passant
};
#include "GameBoard.tpp"
#endif