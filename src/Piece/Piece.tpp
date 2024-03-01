#include "Piece.hpp"
#include <Coord.hpp>
#include <check_path.hpp>
#include <cmath>
#include <forward_list>
#include <observer_ptr.hpp>

#define POW2(x) ((x) * (x))

constexpr Piece::Piece(TypePiece piece, Color color) noexcept
    : m_color(color),
      m_type(piece)
{
}

constexpr bool Piece::is_white() const noexcept
{
    return m_color == Color::WHITE;
}

constexpr bool Piece::is_black() const noexcept
{
    return m_color == Color::BLACK;
}

constexpr bool Piece::same_color(Piece const &other) const noexcept
{
    return m_color == other.m_color;
}

constexpr TypePiece Piece::get_type() const noexcept
{
    return m_type;
}

constexpr Color Piece::get_color() const noexcept
{
    return m_color;
}

constexpr std::size_t Piece::abs_substracte(std::size_t x, std::size_t y) noexcept
{
    if (x >= y)
    {
        return x - y;
    }
    return y - x;
}

constexpr double Piece::norm(Coord const &x, Coord const &y) noexcept
{
    return sqrt(static_cast<double>(POW2(abs_substracte(x.n, y.n)) + POW2(abs_substracte(x.m, y.m))));
}

/* template <std::size_t N, std::size_t M>
constexpr bool Coord_in_Board(Board<N, M> const& board, Coord const& pos) noexcept
{
    if (pos.n >= board.numberLines() || pos.m >= board.numberColumns())
    {
        return false;
    }
    return true;
} */

/* template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_pawn(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{
    if (arv.m - dpt.m == 2)
    {
        if (dpt.n == 1 && m_color == Color::WHITE && board(dpt.n + 1, dpt.m) == nullptr)
        {
            return true;
        }
        else if (dpt.n == board.numberLines() - 2 && m_color == Color::BLACK && board(dpt.n - 1, dpt.m) == nullptr)
        {
            return true;
        }
        return false;
    }
    return true;
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_knight(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{
    return true;
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_bishop(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{

    return check_path_diagonal(board, dpt, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_rook(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{

    return check_path_straight(board, dpt, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_queen(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{

    return check_path_straight(board, dpt, arv) || check_path_diagonal(board, dpt, arv);
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::check_path_king(Board<N, M> const &board, Coord const &dpt, Coord const &arv) noexcept
{
    bool path{true};
    if constexpr (N = 8 &&M = 8)
    {
        path = check_path_straight(board, dpt, arv);
    }
    return path;
} */

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_king(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    std::array<int, 8> const move_king{ {-P - 1, -P, -P + 1, -1, 1, P -1, P, P +1} };
    std::size_t current_pos{board.offset(pos.n, pos.m)};
    int posBox{board.m_S_mailbox[current_pos]};
    std::forward_list<Coord> list_move{};
    for (int const e : move_king)
    {
        int arv{board.m_L_mailbox[posBox + e]};
        if (arv >= 0)
        {
            std::size_t n{arv / N}, m{arv % M};
            observer_ptr<Piece const> arvPiece{board(n, m)};
            if (arvPiece == nullptr || !same_color(*arvPiece) || board.get_proba(Coord(n, m)) < 1.)
            {
                list_move.push_front(Coord(n, m));
            }
        }
    }
    if constexpr (N == 8 && M == 8)
    {
        if (board.m_q_castle[static_cast<int>(get_color())] && check_path_straight(board, pos, Coord(pos.n, pos.m - 4)))
        {
            list_move.push_front(Coord(pos.n, pos.m - 2));
        }
        if (board.m_k_castle[static_cast<int>(get_color())] && check_path_straight(board, pos, Coord(pos.n, pos.m + 3)))
        {
            list_move.push_front(Coord(pos.n, pos.m + 2));
        }
    }
    return list_move;
}

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_knight(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    std::array<int, 8> const move_knight{ {-2*P-1, -P-2, -2*P+1, -P+2, P-2, 2*P-1, 2*P+1, P+2} };
    std::size_t current_pos{board.offset(pos.n, pos.m)};
    int posBox{board.m_S_mailbox[current_pos]};
    std::forward_list<Coord> list_move{};
    for (int const e : move_knight)
    {
        int arv{board.m_L_mailbox[posBox + e]};
        if (arv >= 0)
        {
            std::size_t n{arv / N}, m{arv % N};
            observer_ptr<Piece const> arvPiece{board(n, m)};
            if (arvPiece == nullptr || !same_color(*arvPiece) || board.get_proba(Coord(n, m)) < 1.)
            {
                list_move.push_front(Coord(n, m));
            }
        }
    }
    return list_move;
}

template <std::size_t N, std::size_t M, std::size_t SIZE>
constexpr std::forward_list<Coord> Piece::get_list_move_rec(Board<N, M> const &board, Coord const &pos, std::array<int, SIZE> const& list_move) const
{
    std::size_t current_pos{board.offset(pos.n, pos.m)};
    int const posBox{board.m_S_mailbox[current_pos]};
    std::forward_list<Coord> move{};
    for (int const e : list_move)
    {
        int arv {board.m_L_mailbox[posBox + e]};
        int posBox_tmp { posBox };
        while (arv >= 0)
        {
            std::size_t n{arv / N}, m{arv % N};
            observer_ptr<Piece const> arvPiece{board(n, m)};
            if (arvPiece == nullptr || board.get_proba(Coord(n, m)) < 1.)
            {
                move.push_front(Coord(n, m));
            }
            else if (!same_color(*arvPiece))
            {
                move.push_front(Coord(n, m));
                break;
            }
            else
            {
                break;
            }
            posBox_tmp = board.m_S_mailbox[arv];
            arv = board.m_L_mailbox[posBox_tmp + e];
        }
    }
    return move;
}

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_bishop(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    std::array<int, 4> const move_bishop{ {-P -1, -P +1, P -1, P +1} };
    return get_list_move_rec(board, pos, move_bishop);
}

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_rook(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    std::array<int, 4> const move_rook{ {-P , 1, P , -1} };
    return get_list_move_rec(board, pos, move_rook);
}

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_queen(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    std::array<int, 8> const move_queen{ {-P - 1, -P, -P + 1, -1, 1, P -1, P, P +1} };
    return get_list_move_rec(board, pos, move_queen);
}

template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move_pawn(Board<N, M> const &board, Coord const &pos) const
{
    constexpr int P {M+2};
    auto sign_color {
        [](Color color)
        {
            return (color == Color::WHITE) ? -1 : 1;
        }
    };
    int sign { sign_color(get_color()) };
    std::array<int, 3> move_pawn {sign*P, sign*P -1, sign*P +1};
    std::forward_list<Coord> list_move {};
    std::size_t current_pos{board.offset(pos.n, pos.m)};
    int posBox{board.m_S_mailbox[current_pos]};
    int arv{board.m_L_mailbox[posBox + move_pawn[0]]};
    if (arv >= 0)
    {
        std::size_t n{arv / N}, m{arv % N};
        if (board(n, m) == nullptr || board.get_proba(Coord(n, m)) < 1.)
        {
            list_move.push_front(Coord(n, m));
            if (((pos.n == 1 && get_color() == Color::BLACK) || (pos.n == N-2 && get_color() == Color::WHITE)) 
                && (board(n + sign, m) == nullptr || board.get_proba(Coord(n + sign, m)) < 1.))
            {
                list_move.push_front(Coord(n + sign, m));
            }
        }
    }
    for (std::size_t i {1}; i < 3; i++)
    {
        arv = board.m_L_mailbox[posBox + move_pawn[i]];
        if (arv >= 0)
        {
            std::size_t n{arv / N}, m{arv % N};
            if (board(n, m) != nullptr || (board.m_ep != std::nullopt && board.m_ep->n == pos.n && board.m_ep->m == pos.m))
            {
                list_move.push_front(Coord(n, m));
            }
        }
    }
    return list_move;
}



template <std::size_t N, std::size_t M>
constexpr std::forward_list<Coord> Piece::get_list_move(Board<N, M> const &board, Coord const &pos) const
{
    switch (get_type())
    {
    case TypePiece::PAWN:
        return get_list_move_pawn(board, pos);
    case TypePiece::KNIGHT:
        return get_list_move_knight(board, pos);
    case TypePiece::BISHOP:
        return get_list_move_bishop(board, pos);
    case TypePiece::ROOK:
        return get_list_move_rook(board, pos);
    case TypePiece::QUEEN:
        return get_list_move_queen(board, pos);
    case TypePiece::KING:
        return get_list_move_king(board, pos);
    default:
        return std::forward_list<Coord>{};
    }
}

template <std::size_t N, std::size_t M>
constexpr bool Piece::is_legal(Board<N, M> const &board, Coord const &dpt, Coord const &arv) const
{
    return true; // A faire
}