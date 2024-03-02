#ifndef PIECE_HPP
#define PIECE_HPP

#include <Color.hpp>
#include <Coord.hpp>
#include <concepts>
#include <vector>
#include <forward_list>
#include <constexpr.hpp>
#include "TypePiece.hpp"
#include <Constexpr.hpp>

enum class Move_Mode {
    NORMAL,
    SPLIT
};

template <std::size_t N, std::size_t M>
class Board;

class Piece
{
public:
    CONSTEXPR Piece() = delete;
    CONSTEXPR Piece(TypePiece piece, Color color) noexcept;
    CONSTEXPR Piece(Piece const &) = delete;
    CONSTEXPR Piece &operator=(Piece const &) = delete;
    CONSTEXPR Piece(Piece &&) = default;
    CONSTEXPR Piece &operator=(Piece &&) = default;
    CONSTEXPR ~Piece() = default;

    CONSTEXPR TypePiece get_type() const noexcept;
    CONSTEXPR Color get_color() const noexcept;

    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_normal_move(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_split_move(Board<N, M> const &board, Coord const &pos) const;

    CONSTEXPR bool is_white() const noexcept;
    CONSTEXPR bool is_black() const noexcept;
    CONSTEXPR bool same_color(Piece const &other) const noexcept;

private:
    CONSTEXPR static std::size_t abs_substracte(std::size_t x, std::size_t y) noexcept;
    CONSTEXPR static double norm(Coord const &x, Coord const &y) noexcept;

    template <std::size_t N, std::size_t M>
    CONSTEXPR static bool Coord_in_Board(Board<N, M> const &board, Coord const &pos) noexcept;

    template <Move_Mode MOVE, std::size_t N, std::size_t M, std::size_t SIZE>
    CONSTEXPR std::forward_list<Coord> get_list_move_rec(Board<N, M> const &board, Coord const &pos, std::array<int, SIZE> const &list_move) const;

    /*     template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path(Board<N, M> const &board, Coord const &dpt, Coord const &arv) const noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_pawn(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_knight(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_bishop(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_rook(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_queen(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        CONSTEXPR bool check_path_king(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;
    */

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_king(Board<N, M> const &board, Coord const &pos) const;

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_knight(Board<N, M> const &board, Coord const &pos) const;

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_bishop(Board<N, M> const &board, Coord const &pos) const;

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_rook(Board<N, M> const &board, Coord const &pos) const;

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_queen(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move_pawn(Board<N, M> const &board, Coord const &pos) const;

    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord> get_list_move(Board<N, M> const &board, Coord const &pos) const;

    Color m_color;
    TypePiece m_type;
};

#include "Piece.tpp"

CONSTEXPR Piece W_KING{TypePiece::KING, Color::WHITE};
CONSTEXPR Piece B_KING{TypePiece::KING, Color::BLACK};

CONSTEXPR Piece W_QUEEN{TypePiece::QUEEN, Color::WHITE};
CONSTEXPR Piece B_QUEEN{TypePiece::KING, Color::BLACK};

CONSTEXPR Piece W_BISHOP{TypePiece::BISHOP, Color::WHITE};
CONSTEXPR Piece B_BISHOP{TypePiece::BISHOP, Color::BLACK};

CONSTEXPR Piece W_ROOK{TypePiece::ROOK, Color::WHITE};
CONSTEXPR Piece B_ROOK{TypePiece::ROOK, Color::BLACK};

CONSTEXPR Piece W_KNIGHT{TypePiece::KNIGHT, Color::WHITE};
CONSTEXPR Piece B_KNIGHT{TypePiece::KNIGHT, Color::BLACK};

CONSTEXPR Piece W_PAWN{TypePiece::PAWN, Color::WHITE};
CONSTEXPR Piece B_PAWN{TypePiece::PAWN, Color::BLACK};

#endif