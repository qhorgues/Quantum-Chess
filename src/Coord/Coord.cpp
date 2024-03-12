#include "Coord.hpp"
#include <functional>

std::size_t Coord_hash::operator () (Coord const &coord) const
{
    std::size_t h1 = std::hash<std::size_t>()(coord.n);
    std::size_t h2 = std::hash<std::size_t>()(coord.m);

    return h1 ^ h2;
}


bool operator==(Coord const &lhs, Coord const &rhs)
{
    return lhs.n == rhs.n && lhs.m == rhs.m;
}