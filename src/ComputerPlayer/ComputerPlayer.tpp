#include <Constexpr.hpp>
#include <cstddef>
#include <forward_list>
#include <functional>
#include <limits>
#include <tuple>
#include <algorithm>
#include <thread>
#include <concepts>
#include <unordered_map>
#include <Board.hpp>
#include <TypePiece.hpp>
#include <Color.hpp>
#include <Coord.hpp>
#include <Move.hpp>

namespace computer
{
    namespace __utility
    {
        /**
         * @brief Renvoi la valeur d'une pièce
         *
         * @param piece Le type de la piece
         * @return La valeur de la pièce
         */
        CONSTEXPR int value_piece(TypePiece piece)
        {
            switch (piece)
            {
            case TypePiece::PAWN:
                return 1;
            case TypePiece::KNIGHT:
                return 3;
            case TypePiece::BISHOP:
                return 3;
            case TypePiece::ROOK:
                return 5;
            case TypePiece::QUEEN:
                return 9;
            case TypePiece::KING:
                return 5;
            case TypePiece::EMPTY:
            default:
                return 0;
            }
        }

        /**
         * @brief Renvoi le signe d'un joueur
         *
         * @param color La couleur du joueur
         * @return 1 si c'est le joueur blanc
         * @return 0 si c'est le joueur noir
         */
        CONSTEXPR int sign_color(Color color)
        {
            return (color == Color::WHITE) ? 1 : -1;
        }

        /**
         * @brief Teste si un plateau est gagnat et indique le gagnant
         *
         * @tparam N Le nombre de ligne du plateau
         * @tparam M Le nombre de colonne du plateau
         * @param[in] board Le plateau à tester
         * @param[out] winner_color prend la couleur du joueur gagnant.
         * Contenu non garanti si il n'y a pas de gagnant
         * @return true si il y a un gagnant
         * @return false sinon
         */
        template <std::size_t N, std::size_t M>
        CONSTEXPR bool winner(Board<N, M> const &board, Color &winner_color)
        {
            bool found_W_king{false}, found_B_king{false};
            for (std::size_t i{0}; i < board.numberLines(); i++)
            {
                for (std::size_t j{0}; j < board.numberColumns(); j++)
                {
                    auto piece = board(i, j);
                    if (piece.get_type() == TypePiece::KING)
                    {
                        if (piece.get_color() == Color::WHITE)
                        {
                            found_W_king = true;
                        }
                        else
                        {
                            found_B_king = true;
                        }
                        if (found_W_king == found_B_king)
                        {
                            return false;
                        }
                    }
                }
            }
            if (found_B_king)
            {
                winner_color = Color::BLACK;
            }
            else
            {
                winner_color = Color::WHITE;
            }
            return true;
        }

        /**
         * @brief Renvoi la fonction permettant de calculer le meilleur
         * score du joueur
         *
         * @param[in] board Le plateau de jeu
         * @param[in] x Le premier score
         * @param[in] y Le second score
         * @return true si le second score est plus
         * interessant pour le joueur
         * @return false sinon
         */
        CONSTEXPR bool
        get_player_calc_best_score(Color color,
                                   double x,
                                   double y)
        {
            if (color == Color::WHITE)
            {
                return (y > x) ? true : false;
            }
            return (y < x) ? true : false;
        }

        CONSTEXPR inline std::size_t factorial(std::size_t x)
        {
            if (x == 0)
            {
                return 1;
            }
            std::size_t rep{1};
            for (std::size_t i{2}; i <= x; i++)
            {
                rep *= i;
            }
            return rep;
        }

        /**
         * @brief Renvoie une évaluation du plateau
         * @details L'évaluation du plateau est calculé
         * en sommant toutes les pièces d'un joueur
         * coéfficienté par leurs probabilités et en soustrayant
         * avec ce meme calcul les pièces de l'autre joueur
         * @tparam N Le nombre de ligne du plateau
         * @tparam M Le nombre de colonne du plateau
         * @param board Le plateau de jeu
         * @return double L'évaluation du plateau
         */
        template <std::size_t N, std::size_t M>
        CONSTEXPR double heuristic(Board<N, M> const &board) noexcept
        {
            Color win;
            if (__utility::winner(board, win))
            {
                return __utility::sign_color(win) *
                       std::numeric_limits<double>::max();
            }
            double h{0};
            for (std::size_t i{0}; i < board.numberLines(); i++)
            {
                for (std::size_t j{0}; j < board.numberColumns(); j++)
                {
                    auto p = board(i, j);
                    if (p.get_type() != TypePiece::EMPTY)
                    {
                        h += sign_color(p.get_color()) *
                             __utility::value_piece(p.get_type()) *
                             board.get_proba(Coord(i, j));
                    }
                }
            }
            return h;
        }

        template <std::size_t N, std::size_t M>
        CONSTEXPR double rec_get_best_move(
            Board<N, M> &board,
            std::forward_list<double> best_score_alpha_beta,
            int profondeur)
        {
            Color win;
            if (profondeur <= 0)
            {
                return heuristic(board);
            }
            else if (winner(board, win))
            {
                return __utility::sign_color(win) *
                       std::numeric_limits<double>::max();
            }
            else
            {
                double best_score{-1 * sign_color(board.get_current_player()) *
                                  std::numeric_limits<double>::max()};
                auto check_best_score{
                    [&board,
                     &best_score_alpha_beta,
                     best_score](double score) mutable -> bool
                    {
                        if (get_player_calc_best_score(
                                board.get_current_player(),
                                best_score, score))
                        {
                            best_score = score;
                            Color current{board.get_current_player()};
                            for (double const alpha : best_score_alpha_beta)
                            {
                                if (get_player_calc_best_score(
                                        current,
                                        alpha,
                                        best_score))
                                {
                                    return true;
                                }
                                if (current == Color::WHITE)
                                {
                                    current = Color::BLACK;
                                }
                                else
                                {
                                    current = Color::WHITE;
                                }
                            }
                        }
                        return false;
                    }};

                board.all_move(
                    [&board,
                     &best_score_alpha_beta,
                     profondeur,
                     check_best_score,
                     &best_score](Move const &m) mutable -> bool
                    {
                        double score;
                        Board<N, M> board_cpy = board;
                        try
                        {
                            board_cpy.move(m);
                        }
                        catch (std::runtime_error const &e)
                        {
                            return false;
                        }
                        board_cpy.change_player();
                        best_score_alpha_beta
                            .push_front(best_score);
                        score =
                            rec_get_best_move(
                                board_cpy,
                                best_score_alpha_beta,
                                profondeur -
                                    1);
                        best_score_alpha_beta.pop_front();
                        return check_best_score(score);
                    });
                return best_score;
            }
        }

        template <std::size_t N, std::size_t M>
        struct Param_get_best_move
        {
            Board<N, M> board;
            std::forward_list<Move> const &move;
            double &best_eval;
            Move &best_move;
            int profondeur;
        };
        
        template <std::size_t N, std::size_t M>
        CONSTEXPR void
        th_get_best_move(Param_get_best_move<N, M> &&param)
        {
            param.best_eval = -1 * sign_color(param.board.get_current_player()) *
                              std::numeric_limits<double>::max();
            for (Move const &move : param.move)
            {
                double score;
                Board<N, M> board_cpy = param.board;
                if (move.type == TypeMove::PROMOTE)
                {
                    board_cpy.move_promotion(move);
                }
                else
                {
                    board_cpy.move(move);
                }
                board_cpy.change_player();
                score = rec_get_best_move(board_cpy,
                                          std::forward_list<double>{
                                              param.best_eval},
                                          param.profondeur - 1);
                if (get_player_calc_best_score(
                        param.board.get_current_player(),
                        param.best_eval, score))
                {
                    param.best_eval = score;
                    param.best_move = move;
                }
            }
        }
    }
    template <std::size_t N, std::size_t M>
    CONSTEXPR Move get_best_move(Board<N, M> const &board, int profondeur)
    {
        unsigned int number_thread{std::thread::hardware_concurrency()};
        std::vector<std::forward_list<Move>> list_move(number_thread);
        std::size_t last_th_view{0};

        board.all_move(
            [&list_move,
             &last_th_view,
             number_thread](Move const &m) mutable -> bool
            {
                list_move[last_th_view].push_front(std::move(m));
                last_th_view = (last_th_view + 1) % number_thread;
                return false;
            });

        std::vector<std::thread> threads(number_thread);

        std::vector<double> best_eval(number_thread);

        std::vector<Move> best_move(number_thread);

        for (std::size_t i{0}; i < number_thread; i++)
        {

            __utility::Param_get_best_move param{board,
                                                 std::move(list_move[i]),
                                                 best_eval[i],
                                                 best_move[i],
                                                 profondeur};
            // param.board = board;
            threads[i] = std::thread(__utility::th_get_best_move<N, M>, param);
        }
        double better{0};
        Move better_move;
        for (std::size_t i{0}; i < number_thread; i++)
        {
            threads[i].join();
            if (__utility::get_player_calc_best_score(
                    board.get_current_player(),
                    better,
                    best_eval[i]))
            {
                better = best_eval[i];
                better_move = best_move[i];
            }
        }
        return better_move;
    }
}