#ifndef FINAL_SOLVER_HPP
#define FINAL_SOLVER_HPP
#include <Color.hpp>
namespace Final
{
template <std::size_t N, std::size_t M>
brut_force(Board<N, M> const &board, std::size_t profondeur, Color c);
}







#include "final_solver.tpp"

#endif