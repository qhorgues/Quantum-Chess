#ifndef COORD_HPP
#define COORD_HPP

#include <cstddef>

/**
 * @brief La structure qui représente une coordonnée
 * sur le plateau
 */
struct Coord
{
    Coord() = default;
    Coord(std::size_t i, std::size_t j) : n(i), m(j) {};
    Coord(Coord const&) = default;
    Coord& operator=(Coord const&) = default;
    Coord(Coord &&) = default;
    Coord& operator=(Coord &&) = default;


    /**
     * @brief l'indice sur les lignes
     */
    std::size_t n;

    /**
     * @brief l'indice sur les colonnes
     */
    std::size_t m;
};

struct Coord_hash
{
    std::size_t operator () (Coord const &coord) const
    {
        std::size_t h1 = std::hash<std::size_t>()(coord.n);
        std::size_t h2 = std::hash<std::size_t>()(coord.m);
 
        return h1 ^ h2;
    }
};
#endif