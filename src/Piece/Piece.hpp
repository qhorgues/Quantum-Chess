#ifndef PIECE_HPP
#define PIECE_HPP

enum class Piece
{
    EMPTY = 0,

    W_KING,
    W_BISHOP,
    W_QUEEN,
    W_ROOK,
    W_KNIGHT,
    W_PAWN,

    B_KING,
    B_BISHOP,
    B_QUEEN,
    B_ROOK,
    B_KNIGHT,
    B_PAWN
};

bool is_white (Piece p);

bool is_black (Piece p);

bool same_color (Piece p1, Piece p2); // Renvoie un résultat indéterminé si une des deux pièces vaut EMPTY

#endif