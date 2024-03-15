#ifndef FINAL_SOLVER_HPP
#define FINAL_SOLVER_HPP
#include <Color.hpp>
#include <Constexpr.hpp>
namespace Final
{
template <std::size_t N, std::size_t M>
CONSTEXPR bool brut_force_classic_chess(Board<N, M> const &board, std::size_t profondeur, Color c);

template <std::size_t N, std::size_t M>
CONSTEXPR bool brut_force_quantum_chess(Board<N, M> const &board, std::size_t profondeur, Color c);
};
struct C_hash{
std::size_t operator () (std::pair<TypePiece, Coord> const &c) const;
};
bool operator==(std::pair<TypePiece, Coord> const &lhs, std::pair<TypePiece, Coord> const &rhs);




#include "final_solver.tpp"

#endif