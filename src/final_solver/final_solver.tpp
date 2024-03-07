#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <iostream>
#include <Color.hpp>


template <std::size_t N, std::size_t M>
CONSTEXPR bool Final::brut_force(Board<N, M>  &board, std::size_t profondeur, Color c)
{
    if (board.winning_position(c))
    {
        return true;
    }
    else
    {
        Color col = Color::WHITE;
        if(c == Color::WHITE)
        {
            col = Color::BLACK;
        }
        if (board.winning_position(col))
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
                if (board.get_current_player() == c)
                {
                    for(std::size_t i{0}; i<N; i++)
                    {
                        for(std::size_t j{0}; j<M; j++)
                        {
                            if (board(i, j) != nullptr && board(i, j)->get_color ()== board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(brut_force(b, profondeur-1, c))
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
                            if (board(i, j) != nullptr && board(i, j)->get_color()== board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(!brut_force(b, profondeur-1, c))
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
    std::cout<<"erreur"<<std::endl;
    return false;
}