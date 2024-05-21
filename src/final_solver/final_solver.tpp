#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <iostream>
#include <Color.hpp>
#include <unordered_map>
#include <functional>

/*std::size_t C_hash::operator()(std::pair<TypePiece, Coord> const &c) const
{
    std::size_t h1 = std::hash<std::size_t>()(c.second.n);
    std::size_t h2 = std::hash<std::size_t>()(c.second.m);

    return h1 ^ h2;
}

bool operator==(std::pair<TypePiece, Coord> const &lhs, std::pair<TypePiece, Coord> const &rhs)
{
    return lhs.second.n == rhs.second.n && lhs.second.m == rhs.second.m && lhs.first == rhs.first;
}*/
template <std::size_t N, std::size_t M>
CONSTEXPR bool Final::brut_force_classic_chess(Board<N, M> &board, std::size_t profondeur, Color c)
{
    if (board.winning_position(c))
    {
        return true;
    }
    else
    {
        Color col = Color::WHITE;
        if (c == Color::WHITE)
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
                // std::cout<<"profondeur pas assez élevé"<<std::endl;
                return false;
            }
            else
            {

                if (board.get_current_player() == c)
                {
                    for (std::size_t i{0}; i < N; i++)
                    {
                        for (std::size_t j{0}; j < M; j++)
                        {
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color() == board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                for (auto const &e : l)
                                {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if (brut_force_classic_chess(b, profondeur - 1, c))
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
                    for (std::size_t i{0}; i < N; i++)
                    {
                        for (std::size_t j{0}; j < M; j++)
                        {
                            if (board(i, j).get_type() != TypePiece::EMPTY && board(i, j).get_color() == board.get_current_player())
                            {
                                std::forward_list<Coord> l = board.get_list_normal_move(Coord(i, j));
                                for (auto const &e : l)
                                {
                                    Move m = Move_classic(Coord(i, j), e);
                                    Board<N, M> b = board;
                                    b.change_player();
                                    b.move(m);
                                    if (!brut_force_classic_chess(b, profondeur - 1, c))
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
    std::cout << "erreur" << std::endl;
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool Final::brut_force_quantum_chess(Board<N, M> &board, std::size_t profondeur, Color c)
{
    bool found_win_pos{false};
    if (board.winning_position(c))
    {
        return true;
    }
    else
    {
        if (profondeur == 0)
        {
            return false;
        }
        else
        {
            board.all_move(
                [&board,
                 profondeur,
                 c,
                 &found_win_pos](Move const &m) mutable -> bool
                {
                    Board<N, M> board_cpy = board;
                    board_cpy.move(m);
                    board_cpy.change_player();
                    if (c == board.get_current_player())
                    {
                        if (brut_force_quantum_chess(board_cpy, profondeur - 1, c))
                        {
                            found_win_pos = true;
                            return true;
                        }
                    }
                    else
                    {
                        if (!brut_force_quantum_chess(board_cpy, profondeur - 1, c))
                        {
                            return true;
                        }
                        found_win_pos = true;
                    }
                    return false;
                });
        }
    }
    return found_win_pos;
}

template <std::size_t N, std::size_t M>
double evaluer(const Board<N, M> &board, Color c)
{

    if (board.winning_position(c))
    {
        return 1.;
    }
    else
    {
        if (board.winning_position(other_color(c)))
        {
            return -1.;
        }
        else
        {
            return 0.;
        }
    }
}
// Fonction récursive pour l'élagage alpha-bêta
template <std::size_t N, std::size_t M>
CONSTEXPR double alphaBeta(Board<N, M> const &board, std::size_t profondeur, double alpha, double beta, bool estMax, Color c)
{
    if (profondeur == 0 || board.winning_position(c) || board.winning_position(other_color(c)))
    {
        return evaluer(board, c);
    }
    else
    {
        if (estMax)
        {
            double meilleurScore = {-2.};
            board.all_move(
                [&board,
                 profondeur,
                 c,
                 &beta,
                 &alpha,
                 &meilleurScore](Move const &m) mutable -> bool

                {
                    bool one_move{true};
                    double proba_move{board.get_proba_move(m)};
                    if (double_equal(proba_move, 1.))
                    {
                        Board<N, M> board_cpy = board;
                        board_cpy.change_player();
                        board_cpy.move(m, true);
                        meilleurScore = std::max(meilleurScore, alphaBeta(board_cpy, profondeur - 1, alpha, beta, false, c));
                    }
                    else
                    {
                        if (double_equal(proba_move, 0.))
                        {
                            Board<N, M> board_cpy = board;
                            board_cpy.change_player();
                            board_cpy.move(m, false);
                            meilleurScore = std::max(meilleurScore, alphaBeta(board_cpy, profondeur - 1, alpha, beta, false, c));
                        }
                        else
                        {
                            one_move = false;
                            Board<N, M> board_cpy1 = board;
                            board_cpy1.change_player();
                            board_cpy1.move(m, false);
                            Board<N, M> board_cpy2 = board;
                            board_cpy2.change_player();
                            board_cpy2.move(m, true);

                            meilleurScore = std::max(meilleurScore, alphaBeta(board_cpy1, profondeur - 1, alpha, beta, false, c)*proba_move + alphaBeta(board_cpy2, profondeur - 1, alpha, beta, false, c)*(1-proba_move));
                        }
                    }
                    if (one_move)
                    {
                        alpha = std::max(alpha, meilleurScore);
                        if (beta <= alpha)
                        {
                            return true; // Élagage bêta
                        }
                    }
                    return false;
                });
            return meilleurScore;
        }
        else
        {
            double meilleurScore{2.};
            board.all_move(
                [&board,
                 profondeur,
                 c,
                 &alpha,
                 &meilleurScore,
                 &beta](Move const &m) mutable -> bool

                {
                    bool one_move{true};
                    double proba_move{board.get_proba_move(m)};
                    if (double_equal(proba_move, 1.))
                    {
                        Board<N, M> board_cpy = board;
                        board_cpy.change_player();
                        board_cpy.move(m, true);
                        meilleurScore = std::min(meilleurScore, alphaBeta(board_cpy, profondeur - 1, alpha, beta, true, c));
                    }
                    else
                    {
                        if (double_equal(proba_move, 0.))
                        {
                            Board<N, M> board_cpy = board;
                            board_cpy.change_player();
                            board_cpy.move(m, false);
                            meilleurScore = std::min(meilleurScore, alphaBeta(board_cpy, profondeur - 1, alpha, beta, true, c));
                        }
                        else
                        {
                            one_move = false;
                            Board<N, M> board_cpy1 = board;
                            board_cpy1.change_player();
                            board_cpy1.move(m, false);
                            Board<N, M> board_cpy2 = board;
                            board_cpy2.change_player();
                            board_cpy2.move(m, true);

                            meilleurScore = std::min(meilleurScore, alphaBeta(board_cpy1, profondeur - 1, alpha, beta, true, c)*proba_move + alphaBeta(board_cpy2, profondeur - 1, alpha, beta, true, c)*(1-proba_move));
                        }
                    }
                    if (one_move)
                    {
                        beta = std::min(beta, meilleurScore);
                        if (beta <= alpha)
                        {
                            return true; // Élagage bêta
                        }
                    }
                    return false;
                });
            return meilleurScore;
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR double Final::res_pos(Board<N, M> &board, std::size_t profondeur)
{
    Color c{board.get_current_player()};
    return alphaBeta(board, profondeur, -2., 2., true, c);
}
