#ifndef PIECE_HPP
#define PIECE_HPP

#include <Color.hpp>
#include <Coord.hpp>
#include <concepts>
#include <vector>
#include <forward_list>
#include <Constexpr.hpp>
#include <Move.hpp>
#include "TypePiece.hpp"

template <std::size_t N, std::size_t M>
class Board;

/**
 * @brief Class conservant le type et la couleur d'une pièce.
 * Elle permet aussi de recuperer ses mouvement possible
 */
class Piece
{
public:
    constexpr inline Piece() noexcept;

    /**
     * @brief Construit une piece à l'aide de son type et de sa couleur
     *
     * @param[in] piece Le type de la piece
     * @param[in] color La couleur de la piece
     */
    constexpr inline Piece(TypePiece piece, Color color) noexcept;
    constexpr inline Piece(Piece const &) = default;
    constexpr inline Piece &operator=(Piece const &) = default;

    /**
     * @brief Deplacement d'un objet piece vers un autre objet piece
     */
    constexpr inline Piece(Piece &&) = default;
    constexpr inline Piece &operator=(Piece &&) = default;
    constexpr inline ~Piece() = default;

    /**
     * @brief Renvoie le type de la piece
     *
     * @return Une enumération TypePiece
     */
    constexpr inline TypePiece get_type() const noexcept;

    /**
     * @brief Renvoie la couleur d'un piece
     *
     * @return Color La couleur de la piece
     */
    constexpr inline Color get_color() const noexcept;

    /**
     * @brief Récupère la liste des mouvements classics pour une piece
     *
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos la position de la piece sur le plateau
     * @warning Le resultat n'est pas garanti si la position ne concorde pas
     * avec la position de la piece sur le plateau
     * @return std::forward_list<Coord> La liste de coordonnées des positions
     * d'arrivées de chaque mouvement possible
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_normal_move(Board<N, M> const &board,
                         Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste des mouvements split pour une piece
     *
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos la position de la piece sur le plateau
     * @warning Le resultat n'est pas garanti si la position ne concorde pas
     * avec la position de la piece sur le plateau
     * @warning Ne renvoie qu'une liste de coordonnées uniques car
     * l'ensemble des mouvements possibles est toutes les couples
     * de coordonnées de la liste
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible sachant que chaque élément
     * représente une seule des deux cases d'arrivées d'un split move
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_split_move(Board<N, M> const &board,
                        Coord const &pos) const noexcept;

    /**
     * @brief Teste si la piece est blanche
     *
     * @return bool true si la piece est blanche, false sinon
     */
    constexpr inline bool is_white() const noexcept;

    /**
     * @brief Teste si la piece est noire
     *
     * @return bool true si la piece est noire, false sinon
     */
    constexpr inline bool is_black() const noexcept;

    /**
     * @brief Teste si l'autre piece est de la meme couleur
     *
     * @param[in] other L'autre pièce à comparer
     *
     * @return bool true si la piece est de la meme couleur, false sinon
     */
    constexpr inline bool same_color(Piece const &other) const noexcept;

    /**
     * @brief Teste si les mouvement de la piece sont si la
     * pièce est un pion un mouvement de promotion
     * 
     * @note peut etre utiliser sans verifier si la pièce est un pion
     * 
     * @param[in] board Le plateau
     * @param[in] pos La position de la pièce
     * @return true si le mouvement possible est un mouvement
     * de promotion
     * @return false sinon
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR bool
    check_if_use_move_promote(Board<N, M> const &board,
                              Coord const &pos) const noexcept;

    /**
     * @brief Renvoie la liste de toutes les promotions 
     * 
     * @warning Ne verifie pas la validité du mouvement,
     * de la position ou du type de la pièce
     * 
     * @param[in] board Le plateau
     * @param[in] pos La position du pion
     * @return La liste de tout les mouvements de promotion
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Move>
    get_list_promote(Board<N, M> const &board,
                     Coord const &pos) const noexcept;

private:
    /**
     * @brief Enumération utilisé dans le template des fonctions
     * qui récupère les listes de mouvement pour modifier leurs
     * comportement lors de la prise de pièce.
     */
    enum class Move_Mode
    {
        /**
         * @brief Dans le cas du mouvement classique on
         * autorise la prise de pièce.
         */
        NORMAL,

        /**
         * @brief Dans le cas du split on interdit la
         * prise de pièce.
         */
        SPLIT
    };

    /**
     * @brief renvoie la valeur absolue d'un soustraction sur des types size_t
     *
     * @param[in] x Variable de type size_t
     * @param[in] y Variable de type size_t
     * @return std::size_t Retourne l'opération |x - y|
     */
    constexpr inline static std::size_t
    abs_substracte(std::size_t x, std::size_t y) noexcept;

    /**
     * @brief renvoie la distance entre deux coordonnées
     * à l'aide de la norme 2
     *
     * @param[in] x Première coordonnée
     * @param[in] y Seconde coordonnée
     * @return double le résultat de la norme 1
     */
    constexpr inline static double
    norm(Coord const &x, Coord const &y) noexcept;

    /**
     * @brief Récupère la liste de mouvement d'une pièce pour
     * les déplacements indiqué de manière récursive
     * c'est à dire tout les mouvements infini (ex : diagonale du fou)
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @tparam SIZE La taille de la liste des mouvements possible
     * @param[in] board Le plateau
     * @param[in] pos La position de la pièce
     * @param[in] list_move La liste de tout les mouvements possible
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M, std::size_t SIZE>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_rec(Board<N, M> const &board, Coord const &pos,
                      std::array<int, SIZE> const &list_move) const noexcept;

    /**
     * @brief Récupère la liste de mouvement du roi
     *
     * @warning Ne récupère les mouvements du roque que pour un tableau
     * de taille 8 x 8
     *
     * @warning ne verifie pas si c'est bien un roi
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position du roi
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour le roi
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_king(Board<N, M> const &board,
                       Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement du cavalier
     *
     * @warning ne verifie pas si c'est bien un cavalier
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position du cavalier
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour le cavalier
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_knight(Board<N, M> const &board,
                         Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement du fou
     *
     * @warning ne verifie pas si c'est bien un fou
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position du fou
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour le fou
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_bishop(Board<N, M> const &board,
                         Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement de la tour
     *
     * @warning ne verifie pas si c'est bien une tour
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position de la tour
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour la tour
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_rook(Board<N, M> const &board,
                       Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement de la dame
     *
     * @warning ne verifie pas si c'est bien une dame
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position de la dame
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour la dame
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_queen(Board<N, M> const &board,
                        Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement du pion
     *
     * @warning ne verifie pas si c'est bien un pion
     *
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position du pion
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible pour le pion
     */
    template <std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move_pawn(Board<N, M> const &board,
                       Coord const &pos) const noexcept;

    /**
     * @brief Récupère la liste de mouvement pour
     * n'importe qu'elle pièce
     *
     * @tparam MOVE Le type de mouvement utiliser entre normal
     * et split (split ne prend pas les mouvements de capture de pièce)
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonne du plateau
     * @param[in] board Le plateau
     * @param[in] pos La position de la pièce
     * @param[in] list_move La liste de tout les mouvements possible
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible
     */
    template <Move_Mode MOVE, std::size_t N, std::size_t M>
    CONSTEXPR std::forward_list<Coord>
    get_list_move(Board<N, M> const &board,
                  Coord const &pos) const noexcept;

    /**
     * @brief La couleur de la pièce (WHITE/BLACK)
     */
    Color m_color;

    /**
     * @brief Le type de la piece
     * ( KING / QUEEN / ROOK / BISHOP / KNIGHT / PAWN )
     */
    TypePiece m_type;
};

#include "Piece.tpp"

/**
 * @brief L'objet représentant le roi blanc
 */
constexpr Piece W_KING{TypePiece::KING, Color::WHITE};

/**
 * @brief L'objet représentant le roi noir
 */
constexpr Piece B_KING{TypePiece::KING, Color::BLACK};

/**
 * @brief L'objet représentant la renne blanche
 */
constexpr Piece W_QUEEN{TypePiece::QUEEN, Color::WHITE};

/**
 * @brief L'objet représentant la renne noire
 */
constexpr Piece B_QUEEN{TypePiece::KING, Color::BLACK};

/**
 * @brief L'objet représentant le fou blanc
 */
constexpr Piece W_BISHOP{TypePiece::BISHOP, Color::WHITE};

/**
 * @brief L'objet représentant le fou noir
 */
constexpr Piece B_BISHOP{TypePiece::BISHOP, Color::BLACK};

/**
 * @brief L'objet représentant la tour blanche
 */
constexpr Piece W_ROOK{TypePiece::ROOK, Color::WHITE};

/**
 * @brief L'objet représentant la tour noire
 */
constexpr Piece B_ROOK{TypePiece::ROOK, Color::BLACK};

/**
 * @brief L'objet représentant le cavalier blanc
 */
constexpr Piece W_KNIGHT{TypePiece::KNIGHT, Color::WHITE};

/**
 * @brief L'objet représentant le cavalier noir
 */
constexpr Piece B_KNIGHT{TypePiece::KNIGHT, Color::BLACK};

/**
 * @brief L'objet représentant le pion blanc
 */
constexpr Piece W_PAWN{TypePiece::PAWN, Color::WHITE};

/**
 * @brief L'objet représentant le pion noir
 */
constexpr Piece B_PAWN{TypePiece::PAWN, Color::BLACK};

#endif