#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <iostream>
#include <Color.hpp>
#include <unordered_map>
#include <functional>

std::size_t C_hash::operator () (std::pair<TypePiece, Coord> const &c) const
{
    std::size_t h1 = std::hash<std::size_t>()(c.second.n);
    std::size_t h2 = std::hash<std::size_t>()(c.second.m);

    return h1 ^ h2;
}


bool operator==(std::pair<TypePiece, Coord> const &lhs, std::pair<TypePiece, Coord> const &rhs)
{
    return lhs.second.n == rhs.second.n && lhs.second.m == rhs.second.m && lhs.first==rhs.first;
}
template <std::size_t N, std::size_t M>
CONSTEXPR bool Final::brut_force_classic_chess(Board<N, M>  &board, std::size_t profondeur, Color c)
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
                //std::cout<<"profondeur pas assez élevé"<<std::endl;
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
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color ()== board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(brut_force_classic_chess(b, profondeur-1, c))
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
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color()== board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(!brut_force_classic_chess(b, profondeur-1, c))
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


template <std::size_t N, std::size_t M>
CONSTEXPR bool Final::brut_force_quantum_chess(Board<N, M>  &board, std::size_t profondeur, Color c)
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
                //std::cout<<"profondeur pas assez élevé"<<std::endl;
                return false;
            }
            else
            {
                std::unordered_map<std::pair<TypePiece, Coord>, std::vector<Coord>, C_hash> move_merge_table {};
                if (board.get_current_player() == c)
                {
                    for(std::size_t i{0}; i<N; i++)
                    {
                        for(std::size_t j{0}; j<M; j++)
                        {
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color ()== board.get_current_player())
                            {
                                //ne gère pas les promotions
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(brut_force_quantum_chess(b, profondeur-1, c))
                                    {
                                        return true;
                                    }
                                    TypePiece type_p_actuelle {board(i, j).get_type()};
                                    if(type_p_actuelle != TypePiece::PAWN)
                                    {
                                        std::forward_list<Coord> split_list = board.get_list_split_move(Coord(i, j));
                                        for (Coord const &c1 : split_list)
                                        {
                                            if (move_merge_table.contains(std::pair(type_p_actuelle, c1)))
                                            {
                                                for(Coord const &coord : move_merge_table[std::pair(type_p_actuelle, c1)])
                                                {
                                                    Move m_merge1 = Move_merge(Coord(i,j), coord, c1);
                                                    Board<N, M> copy_board = board;
                                                    copy_board.move(m_merge1);
                                                    copy_board.change_player();
                                                    if(brut_force_quantum_chess(copy_board, profondeur -1, c))
                                                    {
                                                        return true;
                                                    }
                                                     Move m_merge2 = Move_merge(coord, Coord(i,j), c1);
                                                    Board<N, M> copy_board2 = board;
                                                    copy_board.move(m_merge2);
                                                    copy_board.change_player();
                                                    if(brut_force_quantum_chess(copy_board2, profondeur -1, c))
                                                    {
                                                        return true;
                                                    }


                                                }
                                                move_merge_table[std::pair(type_p_actuelle, c1)].push_back(Coord(i, j));
                                            }
                                            else
                                            {
                                                std::vector<Coord> tab {};
                                                tab.push_back(Coord(i, j));
                                                move_merge_table.insert({std::pair(type_p_actuelle, c1), tab});
                                            }
                                            for (Coord const &c2 : split_list)
                                            {
                                                if (c1 != c2)
                                                {
                                                    Move m_split = Move_split(Coord(i, j), c1, c2);
                                                    Board<N, M> copy_board = board;
                                                    copy_board.change_player();
                                                    copy_board.move(m_split);
                                                    if(brut_force_quantum_chess(b, profondeur-1, c))
                                                    {
                                                        return true;
                                                    }
                                                }
                                            }
                                        }
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
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color()== board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                 for (auto const &e : l)
                                 {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if(!brut_force_quantum_chess(b, profondeur-1, c))
                                    {
                                        return false;
                                    }
                                      TypePiece type_p_actuelle {board(i, j).get_type()};
                                    if(type_p_actuelle != TypePiece::PAWN)
                                    {
                                        std::forward_list<Coord> split_list = board.get_list_split_move(Coord(i, j));
                                        for (Coord const &c1 : split_list)
                                        {
                                            if (move_merge_table.contains(std::pair(type_p_actuelle, c1)))
                                            {
                                                for(Coord const &coord : move_merge_table[std::pair(type_p_actuelle, c1)])
                                                {
                                                    Move m_merge1 = Move_merge(Coord(i,j), coord, c1);
                                                    Board<N, M> copy_board = board;
                                                    copy_board.move(m_merge1);
                                                    copy_board.change_player();
                                                    if(!brut_force_quantum_chess(copy_board, profondeur -1, c))
                                                    {
                                                        return false;
                                                    }
                                                     Move m_merge2 = Move_merge(coord, Coord(i,j), c1);
                                                    Board<N, M> copy_board2 = board;
                                                    copy_board.move(m_merge2);
                                                    copy_board.change_player();
                                                    if(!brut_force_quantum_chess(copy_board2, profondeur -1, c))
                                                    {
                                                        return false;
                                                    }


                                                }
                                                move_merge_table[std::pair(type_p_actuelle, c1)].push_back(Coord(i, j));
                                            }
                                            else
                                            {
                                                std::vector<Coord> tab {};
                                                tab.push_back(Coord(i, j));
                                                move_merge_table.insert({std::pair(type_p_actuelle, c1), tab});
                                            }
                                            for (Coord const &c2 : split_list)
                                            {
                                                if (c1 != c2)
                                                {
                                                    Move m_split = Move_split(Coord(i, j), c1, c2);
                                                    Board<N, M> copy_board = board;
                                                    copy_board.change_player();
                                                    copy_board.move(m_split);
                                                    if(!brut_force_quantum_chess(b, profondeur-1, c))
                                                    {
                                                        return false;
                                                    }
                                                }
                                            }
                                        }
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
