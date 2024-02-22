#include "Piece.hpp"

bool is_white(Piece p)
{
    return (p == Piece::W_BISHOP || p == Piece::W_KING || p == Piece::W_QUEEN || p == Piece::W_ROOK || p == Piece::W_KNIGHT);
}
bool is_black(Piece p)
{
    return (p == Piece::B_BISHOP || p == Piece::B_KING || p == Piece::B_QUEEN || p == Piece::B_ROOK || p == Piece::B_KNIGHT);
}

bool same_color(Piece p1, Piece p2)
{
    if (is_white p1)
        return is_white p2;
    else
        return is_black p2;
}