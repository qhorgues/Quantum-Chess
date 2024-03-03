#ifndef COORD_HPP
#define COORD_HPP

#include <cstddef>

/**
 * @brief La structure qui représente une coordonnée
 * sur le plateau
 */
struct Coord
{
    /**
     * @brief l'indice sur les lignes
     */
    std::size_t n;

    /**
     * @brief l'indice sur les colonnes
     */
    std::size_t m;
};


#endif