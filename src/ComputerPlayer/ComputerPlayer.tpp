#include <Constexpr.hpp>
#include <functional>
#include <limits>

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
         * @param[in] board Le plateau à tester
         * @param[out] winner_color prend la couleur du joueur gagnant.
         * Contenu non garanti si il n'y a pas de gagnant 
         * @return true si il y a un gagnant
         * @return false sinon
         */
        CONSTEXPR bool winner(Board<N, M> const& board , Color &winner_color)
        {
            bool found_W_king {false}, found_B_king {false};
            for (std::size_t i {0}; i < board.numberLines(); i++)
            {
                for (std::size_t j{0}; j < board.numberColumns(); j++)
                {
                    auto piece = board(i, j);
                    if (piece != nullptr)
                    {
                        if (piece->get_type() == TypePiece::KING)
                        {
                            if (piece->get_color() == Color::WHITE)
                            {
                                found_W_king = true;
                            }
                            else
                            {
                                found_B_king = true;
                            }
                            if (found_W_king == found_B_king)
                            {
                                goto END_FOR_WINNER_FUNC;
                            }
                        }
                    }
                }
            }
            END_FOR_WINNER_FUNC:
            if (found_B_king)
            {
                if (found_W_king)
                {
                    return false;
                }
                else
                {
                    color = Color::BLACK;
                }
            }
            else
            {
                color = Color::WHITE;
            }
            return true;
        }
    }



    template <std::size_t N, std::size_t M>
    CONSTEXPR double heuristic(Board<N, M> const &board) noexcept
    {
        Color win;
        if (__utility::winner(board, color))
        {
            return sign_color(win) * std::numeric_limits<double>::max();
        }
        double h {0};
        for (std::size_t i {0}; i < board.numberLines(); i++)
        {
            for (std::size_t j{0}; j < board.numberColumns(); j++)
            {
                auto p = board(i, j);
                if (p != nullptr)
                {
                    h += sign_color(p->get_color()) * 
                        __utility::value_piece(p->get_type()) *
                        board.get_proba(Coord(i, j));
                }
            }
        }
        return h;
    }
}