#ifndef MOVE_HPP
#define MOVE_HPP

#include <Coord.hpp>

enum class TypeMove
{
    NORMAL,
    SPLIT,
    MERGE
};
struct Move 
{
    TypeMove type;
    union
    {
        struct
        {
            Coord src;
            Coord arv;
        } normal;
        struct
        {
            Coord src;
            Coord arv1;
            Coord arv2;
        } split;
        struct
        {
            Coord src1;
            Coord src2;
            Coord arv;
        } merge;
    };
};

#endif