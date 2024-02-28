#ifndef MOVE_HPP
#define MOVE_HPP

#include <Coord.hpp>

enum class TypeMove
{
    NORMAL,
    SPLIT,
    MERGE
};

class Move
{   
public:
    Move() = delete;
    Move(TypeMove move) : m_tmove(move) {};
    Move(Move const&) = default;
    Move& operator=(Move const&) = default;
    Move(Move&&) = default;
    Move& operator=(Move&&) = default;


private:
    TypeMove m_tmove;
    union Move_u
    {
        struct Normal
        {
            Coord src;
            Coord arv;
        } normal;
        struct Split
        {
            Coord src;
            Coord arv1;
            Coord arv2;
        } split;
        struct Merge
        {
            Coord src1;
            Coord src2;
            Coord arv;
        } merge;
    };
    
};

#endif