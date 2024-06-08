#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <iostream>
#include <Color.hpp>
#include <unordered_map>
#include <functional>
#include <stack>

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
        if (board.winning_position(opponent_color(c)))
        {
            return -1.;
        }
        else
        {
            return 0.;
        }
    }
}

template <std::size_t N, std::size_t M>
double evaluer_v2(const Board<N, M> &board, Color c)
{

    if (board.winning_position(c))
    {
        return 1.;
    }
    else
    {
        if (board.winning_position(opponent_color(c)) || (board.no_queen(c) && board.BishopAndKnight(opponent_color(c))))
        {
            return -1.;
        }
        else
        {
            return 0.;
        }
    }
}
template<std::size_t N>
void print_move(std::ostream &output, Move m)
{
    std::string data1{{static_cast<char>('a' + m.normal.src.m), static_cast<char>('0' + N - m.normal.src.n)}};
    std::string data2{{static_cast<char>('a' + m.normal.arv.m), static_cast<char>('0' + N - m.normal.arv.n)}};
    if (m.type == TypeMove::NORMAL)
    {
        output << "N " << data1 << data2 ;
    }
    else if (m.type == TypeMove::PROMOTE)
    {
        output << "P " << data1 << data2 << " :: " << std::to_string(static_cast<int>(m.promote.piece)) ;
    }
    else
    {
        std::string data3{{static_cast<char>('a' + m.split.arv2.m), static_cast<char>('0' + N - m.split.arv2.n)}};
        if (m.type == TypeMove::SPLIT)
        {
            output << "S " << data1 << '(' << data2 << data3 << ')' ;
        }
        else
        {
            output << "M " << '(' << data1 << data2 << ')' << data3 ;
        }
    }
}
// Fonction récursive pour l'élagage alpha-bêta
template <std::size_t N, std::size_t M>
CONSTEXPR double alphaBeta(Board<N, M> const &board, std::size_t profondeur, double alpha, double beta, bool estMax, Color c, std::stack<Node> &stack)
{
    if (profondeur == 0 || board.winning_position(c) || board.winning_position(opponent_color(c)))
    {
        Move m = Move_classic(Coord(0, 0), Coord(0, 0));
        stack.push(m); // Cette information n'a pas d'intérêt dans la pile mais évite les erreurs de segmentation lorsqu'on dépile
        return evaluer_v2(board, c); //Seul endroit on on modifie la fonction d'évaluation
    }
    else
    {
        Move meilleur_move;
        if (estMax)
        {
            double meilleurScore = {-2.};
            board.all_move(
                [&board,
                 profondeur,
                 c,
                 &beta,
                 &alpha,
                 &meilleur_move,
                 &meilleurScore,
                 &stack](Move const &m) mutable -> bool

                {
                    double res;
                    bool one_move{true};
                    double proba_move{board.get_proba_move(m)};
                    if (double_equal(proba_move, 1.))
                    {
                        Board<N, M> board_cpy = board;
                        board_cpy.change_player();
                        board_cpy.move(m, true);
                        double eval{evaluer(board_cpy, c)};
                        if (double_equal(eval, 1.))
                        {
                            meilleur_move = m;
                            meilleurScore = 1.;
                            return true;
                        }
                        else
                        {
                            if (double_equal(eval, -1.))
                            {
                                if (-1. > meilleurScore)
                                {
                                    meilleurScore = -1.;
                                    meilleur_move = m;
                                }
                            }
                            else
                            {
                                std::stack<Node> new_stack{};
                                res = alphaBeta(board_cpy, profondeur - 1, alpha, beta, false, c, new_stack);
                                if (res > meilleurScore)
                                {
                                    meilleurScore = res;
                                    meilleur_move = m;
                                    stack = std::move(new_stack);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (double_equal(proba_move, 0.))
                        {
                            Board<N, M> board_cpy = board;
                            board_cpy.change_player();
                            board_cpy.move(m, false);
                            double eval{evaluer(board_cpy, c)};
                            if (double_equal(eval, 1.))
                            {
                                meilleur_move = m;
                                meilleurScore = 1.;
                                return true;
                            }
                            else
                            {
                                if (double_equal(eval, -1.))
                                {
                                    if (-1. > meilleurScore)
                                    {
                                        meilleurScore = -1.;
                                        meilleur_move = m;
                                    }
                                }
                                else
                                {
                                    std::stack<Node> new_stack{};
                                    res = alphaBeta(board_cpy, profondeur - 1, alpha, beta, false, c, new_stack);
                                    if (res > meilleurScore)
                                    {
                                        meilleurScore = res;
                                        meilleur_move = m;
                                        stack = std::move(new_stack);
                                    }
                                }
                            }
                        }
                        else
                        {
                            one_move = false;
                            Board<N, M> board_cpy1 = board;
                            board_cpy1.change_player();
                            board_cpy1.move(m, true);
                            Board<N, M> board_cpy2 = board;
                            board_cpy2.change_player();
                            board_cpy2.move(m, false);
                            double eval1{evaluer(board_cpy1, c)};
                            std::stack<Node> new_stack1{};
                            std::stack<Node> new_stack2{};
                            if (double_equal(eval1, 0.))
                            {
                                eval1 = alphaBeta(board_cpy1, profondeur - 1, -2., 2., false, c, new_stack1);
                            }
                            double eval2{evaluer(board_cpy2, c)};
                            if (double_equal(eval2, 0.))
                            {
                                eval2 = alphaBeta(board_cpy2, profondeur - 1, -2., 2., false, c, new_stack2);
                            }
                            res = eval1 * proba_move + eval2 * (1 - proba_move);
                            if (res > meilleurScore)
                            {
                                meilleurScore = res;
                                meilleur_move = m;
                                stack = std::move(new_stack1);
                                if (stack.empty())
                                {
                                    stack = std::move(new_stack2);
                                }
                                else
                                {
                                    Node inter = stack.top();
                                    stack.pop();
                                    inter.other_way = std::move(new_stack2);
                                }
                            }
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
            stack.push(Node(meilleur_move));
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
                 &meilleur_move,
                 &beta,
                 &stack](Move const &m) mutable -> bool

                {
                    double res;
                    bool one_move{true};
                    double proba_move{board.get_proba_move(m)};
                    if (double_equal(proba_move, 1.))
                    {
                        Board<N, M> board_cpy = board;
                        board_cpy.change_player();
                        board_cpy.move(m, true);
                        double eval{evaluer(board_cpy, c)};
                        if (double_equal(eval, -1.))
                        {
                            meilleur_move = m;
                            meilleurScore = -1.;
                            return true;
                        }
                        else
                        {
                            if (double_equal(eval, 1.))
                            {
                                if (1. < meilleurScore)
                                {
                                    meilleurScore = 1.;
                                    meilleur_move = m;
                                }
                            }
                            else
                            {
                                std::stack<Node> new_stack{};
                                res = alphaBeta(board_cpy, profondeur - 1, alpha, beta, true, c, new_stack);
                                if (res < meilleurScore)
                                {
                                    meilleurScore = res;
                                    meilleur_move = m;
                                    stack = std::move(new_stack);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (double_equal(proba_move, 0.))
                        {
                            Board<N, M> board_cpy = board;
                            board_cpy.change_player();
                            board_cpy.move(m, false);
                            double eval{evaluer(board_cpy, c)};
                            if (double_equal(eval, -1.))
                            {
                                meilleur_move = m;
                                meilleurScore = -1.;
                                return true;
                            }
                            else
                            {
                                if (double_equal(eval, 1.))
                                {
                                    if (1. < meilleurScore)
                                    {
                                        meilleurScore = 1.;
                                        meilleur_move = m;
                                    }
                                }
                                else
                                {
                                    std::stack<Node> new_stack{};
                                    res = alphaBeta(board_cpy, profondeur - 1, alpha, beta, true, c, new_stack);
                                    if (res < meilleurScore)
                                    {
                                        meilleurScore = res;
                                        meilleur_move = m;
                                    }
                                }
                            }
                        }
                        else
                        {
                            one_move = false;
                            Board<N, M> board_cpy1 = board;
                            board_cpy1.change_player();
                            board_cpy1.move(m, true);
                            Board<N, M> board_cpy2 = board;
                            board_cpy2.change_player();
                            board_cpy2.move(m, false);
                            double eval1{evaluer(board_cpy1, c)};
                            std::stack<Node> new_stack1{};
                            std::stack<Node> new_stack2{};
                            if (double_equal(eval1, 0.))
                            {
                                eval1 = alphaBeta(board_cpy1, profondeur - 1, -2., 2., true, c, new_stack1);
                            }
                            double eval2{evaluer(board_cpy2, c)};
                            if (double_equal(eval2, 0.))
                            {
                                eval2 = alphaBeta(board_cpy2, profondeur - 1, -2., 2., true, c, new_stack2);
                            }
                            res = eval1 * proba_move + eval2 * (1 - proba_move);
                            if (res < meilleurScore)
                            {
                                meilleurScore = res;
                                meilleur_move = m;

                                stack = std::move(new_stack1);
                                if (stack.empty())
                                {
                                    stack = std::move(new_stack2);
                                }
                                else
                                {
                                    Node inter = stack.top();
                                    stack.pop();
                                    inter.other_way = std::move(new_stack2);
                                    stack.push(inter);
                                }
                            }
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
            stack.push(Node(meilleur_move));
            return meilleurScore;
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR std::pair<double, std::stack<Node>> Final::res_pos(Board<N, M> &board, std::size_t profondeur)
{
    Color c{board.get_current_player()};
    std::stack<Node> stack{};
    return std::make_pair(alphaBeta(board, profondeur, -2., 2., true, c, stack), stack);
}
template <std::size_t N>
CONSTEXPR void Final::print_stack( std::stack<Node> & stack)
{
    std::vector<std::stack<Node>> tab{};
    tab.push_back(stack);
    std::size_t compteur{0};
    while (compteur < std::size(tab))
    {
        compteur = 0;
        for (auto &e : tab)
        {
            if (e.empty())
            {
                compteur++;
                std::cout << "      ";
            }
            else
            {
                Node elt = e.top();
                e.pop();
                if(!elt.other_way.empty())
                {
                    tab.push_back(elt.other_way);
                }
                print_move<N>(std::cout, elt.m);
            }
        }
        std::cout<<std::endl;
    }
}

template<std::size_t N, std::size_t M>
CONSTEXPR Board<N, M> Final::init_random_board()
{
    int queen = rnd::randint(0,N*M-1);
    int bishop = rnd::randint(0,N*M-1);
    while(bishop == queen)
    {
        bishop = rnd::randint(0,N*M-1);
    }
    int knight = rnd::randint(0, N*M-1);
      while(knight == queen || knight == bishop)
    {
        knight = rnd::randint(0,N*M-1);
    }
    int b_king = rnd::randint(0, N*M-1);
      while(b_king ==knight || b_king == queen || b_king == bishop)
    {
        b_king = rnd::randint(0,N*M-1);
    }
     int w_king = rnd::randint(0, N*M-1);
      while(w_king==b_king || w_king ==knight || w_king == queen || w_king == bishop)
    {
        w_king = rnd::randint(0,N*M-1);
    }
    std::array<std::array<Piece,M>, N> init {};
    init[queen/M][queen%M] = W_QUEEN;
    init[bishop/M][bishop%M] = B_BISHOP;
    init[knight/M][knight%M] = B_KNIGHT;
    init[b_king/M][b_king%M] = B_KING;
    init[w_king/M][w_king%M] = W_KING;
    Board <N,M> B {init};
    return B;
}

template<std::size_t N, std::size_t M>
CONSTEXPR Board<N, M> Final::init_random_board_v2()
{
    int b_king = rnd::randint(N*(M-1), N*M-1);
    int w_king = rnd::randint(0,M-1);
    int knight = rnd::randint(0, N*M-1);
      while(knight == b_king || knight == w_king)
    {
        knight = rnd::randint(0,N*M-1);
    }
    int bishop = rnd::randint(0, N*M-1);
      while(bishop ==knight || bishop == b_king || bishop == w_king)
    {
        bishop = rnd::randint(0,N*M-1);
    }
     int queen = rnd::randint(0, N*M-1);
      while(queen==b_king || queen ==knight || queen == w_king || queen == bishop)
    {
        queen = rnd::randint(0,N*M-1);
    }
    std::array<std::array<Piece,M>, N> init {};
    init[queen/M][queen%M] = W_QUEEN;
    init[bishop/M][bishop%M] = B_BISHOP;
    init[knight/M][knight%M] = B_KNIGHT;
    init[b_king/M][b_king%M] = B_KING;
    init[w_king/M][w_king%M] = W_KING;
    Board <N,M> B {init};
    //std::cout << B << std::endl;
    return B;
}