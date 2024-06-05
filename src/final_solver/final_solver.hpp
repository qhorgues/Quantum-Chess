#ifndef FINAL_SOLVER_HPP
#define FINAL_SOLVER_HPP
#include <Color.hpp>
#include <Constexpr.hpp>
#include <memory>
#include <stack>

struct Node
{
    Node(Move const &y, std::stack<Node> &&o = std::stack<Node>{});
    Move m;
    std::stack<Node> other_way;
};

Node::Node(Move const &y, std::stack<Node> &&o) : m(y), other_way(o)
{
}

namespace Final
{
    template <std::size_t N, std::size_t M>
    CONSTEXPR bool brut_force_classic_chess(Board<N, M> const &board, std::size_t profondeur, Color c);

    template <std::size_t N, std::size_t M>
    CONSTEXPR bool brut_force_quantum_chess(Board<N, M> const &board, std::size_t profondeur, Color c);

    template <std::size_t N, std::size_t M>
    CONSTEXPR std::pair<double, std::stack<Node>> res_pos(Board<N, M> &board, std::size_t profondeur);
    template<std::size_t N>
    CONSTEXPR void print_stack(std::stack<Node> &stack);

    template<std::size_t N, std::size_t M>
    CONSTEXPR Board<N, M> init_random_board();
};

/*struct C_hash{
std::size_t operator () (std::pair<TypePiece, Coord> const &c) const;
};
bool operator==(std::pair<TypePiece, Coord> const &lhs, std::pair<TypePiece, Coord> const &rhs);
*/

#include "final_solver.tpp"

#endif