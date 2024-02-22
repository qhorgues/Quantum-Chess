#ifndef PIECE_HPP
#define PIECE_HPP

enum class Piece
{
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
    B_PAWN,
    EMPTY
};

bool is_white (Piece const &p);

bool is_black (Piece const &p);

bool same_color (Piece const &p1, Piece const &p2); // Renvoie un résultat indéterminé si une des deux pièces vaut EMPTY

#endif