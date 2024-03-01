#ifndef PIECE_HPP
#define PIECE_HPP

#include <Color.hpp>
#include <Coord.hpp>
#include <concepts>
#include <vector>
#include <forward_list>
#include "TypePiece.hpp"

template <std::size_t N, std::size_t M>
class Board;

class Piece
{
public:
    constexpr Piece() = delete;
    constexpr Piece(TypePiece piece, Color color) noexcept;
    constexpr Piece(Piece const &) = delete;
    constexpr Piece &operator=(Piece const &) = delete;
    constexpr Piece(Piece &&) = default;
    constexpr Piece &operator=(Piece &&) = default;
    constexpr virtual ~Piece() = default;

    constexpr TypePiece get_type() const noexcept;
    constexpr Color get_color() const noexcept;

    template <std::size_t N, std::size_t M>
    constexpr bool is_legal(Board<N, M> const &board, Coord const &dpt, Coord const &arv) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move(Board<N, M> const &board, Coord const &pos) const;

    constexpr bool is_white() const noexcept;
    constexpr bool is_black() const noexcept;
    constexpr bool same_color(Piece const &other) const noexcept;

private:
    constexpr static std::size_t abs_substracte(std::size_t x, std::size_t y) noexcept;
    constexpr static double norm(Coord const &x, Coord const &y) noexcept;

    template <std::size_t N, std::size_t M>
    constexpr static bool Coord_in_Board(Board<N, M> const &board, Coord const &pos) noexcept;

    template <std::size_t N, std::size_t M, std::size_t SIZE>
    constexpr std::forward_list<Coord> get_list_move_rec(Board<N, M> const &board, Coord const &pos, std::array<int, SIZE> const &list_move) const;

    /*     template <std::size_t N, std::size_t M>
        constexpr bool check_path(Board<N, M> const &board, Coord const &dpt, Coord const &arv) const noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_pawn(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_knight(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_bishop(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_rook(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_queen(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept;

        template <std::size_t N, std::size_t M>
        constexpr bool check_path_king(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept; */

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_king(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_knight(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_bishop(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_rook(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_queen(Board<N, M> const &board, Coord const &pos) const;

    template <std::size_t N, std::size_t M>
    constexpr std::forward_list<Coord> get_list_move_pawn(Board<N, M> const &board, Coord const &pos) const;

    Color m_color;
    TypePiece m_type;
};

#include "Piece.tpp"

constexpr Piece W_KING{TypePiece::KING, Color::WHITE};
constexpr Piece B_KING{TypePiece::KING, Color::BLACK};

constexpr Piece W_QUEEN{TypePiece::QUEEN, Color::WHITE};
constexpr Piece B_QUEEN{TypePiece::KING, Color::BLACK};

constexpr Piece W_BISHOP{TypePiece::BISHOP, Color::WHITE};
constexpr Piece B_BISHOP{TypePiece::BISHOP, Color::BLACK};

constexpr Piece W_ROOK{TypePiece::ROOK, Color::WHITE};
constexpr Piece B_ROOK{TypePiece::ROOK, Color::BLACK};

constexpr Piece W_KNIGHT{TypePiece::KNIGHT, Color::WHITE};
constexpr Piece B_KNIGHT{TypePiece::KNIGHT, Color::BLACK};

constexpr Piece W_PAWN{TypePiece::PAWN, Color::WHITE};
constexpr Piece B_PAWN{TypePiece::PAWN, Color::BLACK};

#endif