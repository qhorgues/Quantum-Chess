#include "Interface_SDL2pp.hpp"

Interface_SDL2pp::Interface_SDL2pp() :
    Interface(),
    m_tmp_move(Type_Move::NONE),
    m_tmp_coord_move(),
    m_sdl(SDL_INIT_VIDEO),
    m_window("Quantum Chess", 
                SDL_WINDOWPOS_CENTERED, 
                SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN),
    m_renderer(m_window, -1, SDL_RENDERER_ACCELERATED),
    m_board(m_renderer, "./assets/ChessBoard.png"),
    m_piece(m_renderer, "./assets/Piece.png")
{
    setStatut(Game_Statue::IN_GAME);
}

void Interface_SDL2pp::update(Board<8> const& board)
{
    (void)board;
    m_renderer.Copy(m_board);
    m_renderer.Present();
}

Event Interface_SDL2pp::getEvent(observer_ptr<Board<8> const> board)
{
    (void)board;
    return Event();
}
