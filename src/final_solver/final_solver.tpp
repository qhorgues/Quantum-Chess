#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>


template <std::size_t N, std::size_t M>
Final::brut_force(Board<N, M> const &board, std::size_t profondeur, Color c)
{
    if (board.winning_position(c))
    {
        return true;
    }
    else
    {
        if (board.winning_position(!c))
        {
            return false;
        }
        else
        {
            if (profondeur == 0)
            {
                return false;
            }
            else
            {
                if (board.m_color_current_player == c)
                {
                    for(std::size_t i{0}; i<N; i++)
                    {
                        for(std::size_t j{0}; j<M; j++)
                        {
                            if (board.m_piece_board[offset(i, j)] != nullptr)
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    b = board;
                                    if(brut_force(b.move(m), profondeur-1, c))
                                    {
                                        return true;
                                    }
                                 }
                                
                            }
                        }
                    }
                    return false;
                }
                else
                {
                    for(std::size_t i{0}; i<N; i++)
                    {
                        for(std::size_t j{0}; j<M; j++)
                        {
                            if (board.m_piece_board[offset(i, j)] != nullptr)
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    b = board;
                                    if(!brut_force(b.move(m), profondeur-1, c))
                                    {
                                        return false;
                                    }
                                 }
                                
                            }
                        }
                    } 
                    return true;
                }

            }
        }
    }
}