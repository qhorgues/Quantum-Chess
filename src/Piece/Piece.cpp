#include "Piece.hpp"

bool is_white(Piece p)
{
    using enum Piece;
    return p == W_BISHOP || p == W_KING || p == W_QUEEN || p == W_ROOK || p == W_KNIGHT;
}
bool is_black(Piece p)
{
    using enum Piece;
    return p == B_BISHOP || p == B_KING || p == B_QUEEN || p == B_ROOK || p == B_KNIGHT;
}

bool same_color(Piece p1, Piece p2)
{
    if (is_white(p1))
    {
        return is_white(p2);
    }
    else
    {
        return is_black(p2);
    }
}