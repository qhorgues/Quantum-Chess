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
    END_MENU
};

struct Event
{
    Game_Statue statue;
    std::pair<Coord, std::optional<Coord>> coord;
};

class Interface
{
    Interface() = default;
    Interface(Interface const&) = delete;
    Interface& operator=(Interface const&) = delete;
    Interface(Interface &&) = default;
    Interface& operator=(Interface &&) = default;

    virtual Event getEvent(observer_ptr<Board<8> const> board) const = 0;

    virtual void update(Board<8> const&) = 0;

    Game_Statue getStatut() const noexcept
    {
        return statue;
    }

protected:
    Game_Statue statue;
};


#endif