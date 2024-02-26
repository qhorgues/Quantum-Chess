#ifndef INTERFACE_SDL2PP_HPP
#define INTERFACE_SDL2PP_HPP

#include <Interface.hpp>
#include <SDL2pp/SDL2pp.hh>

class Interface_SDL2pp : public Interface
{
public:
    Interface_SDL2pp();
    Interface_SDL2pp(Interface_SDL2pp const&) = delete;
    Interface_SDL2pp& operator=(Interface_SDL2pp const&) = delete;
    Interface_SDL2pp(Interface_SDL2pp &&) = default;
    Interface_SDL2pp& operator=(Interface_SDL2pp &&) = default;

    Event getEvent(observer_ptr<Board<8> const> board) override;
    void update(Board<8> const&) override;

private:
    Type_Move m_tmp_move;
    Event::Coord_Move m_tmp_coord_move;
    SDL2pp::SDL m_sdl;
    SDL2pp::Window m_window;
    SDL2pp::Renderer m_renderer;
    SDL2pp::Texture m_board;
    SDL2pp::Texture m_piece;
};


#endif