#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <utility>
#include <optional>
#include <observer_ptr.hpp>
#include <GameBoard.hpp>

enum class Game_Statue
{
    IN_GAME,
    MAIN_MENU,
    END_MENU,
    QUIT
};

enum class Type_Move
{
    NONE,
    NORMAL_MOVE,
    SPLIT_MOVE,
    REUNION_MOVE
};

struct Event
{
    Game_Statue statue;
    Type_Move type_move;
    union Coord_Move
    {
        struct Coord_Normal_Move {
            Coord dpt;
            Coord arv;
        } normal;
        struct Coord_Split_Move {
            Coord dpt;
            Coord arv1;
            Coord arv2;
        } split;
        struct Coord_Reunion_Move {
            Coord dpt1;
            Coord dpt2;
            Coord arv;
        } reunion;
    }; 
};

class Interface
{
public:
    Interface() = default;
    Interface(Interface const&) = delete;
    Interface& operator=(Interface const&) = delete;
    Interface(Interface &&) = default;
    Interface& operator=(Interface &&) = default;
    virtual ~Interface() = default;

    virtual Event getEvent(observer_ptr<Board<8> const> board) = 0;

    virtual void update(Board<8> const&) = 0;

    Game_Statue getStatut() const noexcept
    {
        return m_statue;
    }

protected:
    void setStatut(Game_Statue statue) noexcept
    {
        m_statue = statue;
    }

private:
    Game_Statue m_statue;
};


#endif