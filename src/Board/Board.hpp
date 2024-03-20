#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <complex>
#include <cstddef>
#include <initializer_list>
#include <functional>
#include <CMatrix.hpp>
#include <Piece.hpp>
#include <TypePiece.hpp>
#include <Color.hpp>
#include <Coord.hpp>
#include <forward_list>
#include <observer_ptr.hpp>
#include <Move.hpp>
#include <Constexpr.hpp>
#include <check_path.hpp>

class Piece;

/**
 * @brief La classe représentant le plateau de jeu
 *
 * @tparam N Le nombre de ligne du plateau
 * @tparam M Le nombre de colonne du plateau
 */
template <std::size_t N = 8, std::size_t M = N>
class Board final
{
public:
    // Constructeur
    CONSTEXPR Board();
    /**
     * @brief Initialise le plateau à partir d'une liste de pièce,
     * les pièces ne sont pas divisées initialement.
     *
     * @param[in] board Double initializer_list sur des pointeurs sur Piece
     */
    CONSTEXPR Board(std::initializer_list<
                    std::initializer_list<
                        Piece>> const &
                        board);

    // Copie
    CONSTEXPR Board(Board const &) = default;
    CONSTEXPR Board &operator=(Board const &) = default;

    // Mouvement
    CONSTEXPR Board(Board &&) = default;
    CONSTEXPR Board &operator=(Board &&) = default;

    // Destructeur
    CONSTEXPR ~Board() = default;

    /**
     * @brief Retourne le nombre de ligne du plateau
     *
     * @return std::size_t Le nombre de ligne
     */
    CONSTEXPR static std::size_t numberLines() noexcept;

    /**
     * @brief Retourne le nombre de colonne
     *
     * @return std::size_t Le nombre de colonne
     */
    CONSTEXPR static std::size_t numberColumns() noexcept;

    /**
     * @brief Retourne un pointeur sur la piece à l'emplacement cible
     *
     * @param[in] n L'indice de la ligne
     * @param[in] m L'indice de la colonne
     * @return Un pointeur observateur sur une piece
     * ou Piece() si la case est vide
     */
    CONSTEXPR Piece const &
    operator()(std::size_t n,
               std::size_t m) const noexcept;

    /**
     * @brief Renvoie la liste dans tous les mouvements
     * classiques légaux d'une pièce
     *
     * @param pos Position de la pièce
     *
     * @return std::forward_list<Coord> La liste de coordonnées des positions
     * d'arrivées de chaque mouvement possible
     */
    CONSTEXPR std::forward_list<Coord>
    get_list_normal_move(Coord const &pos) const;

    /**
     * @brief Renvoie la liste de toutes les cases qu'une pièce
     * peut atteindre lors d'un mouvement split, il faut choisir
     * deux éléments de la liste pour créer un mouvement split.
     *
     * @param[in] pos Position de la pièce.
     *
     * @return std::forward_list<Coord> La liste de coordonnées
     * des positions d'arrivées possible sachant que chaque élément
     * représente une seule des deux cases d'arrivées d'un split move
     */
    CONSTEXPR std::forward_list<Coord>
    get_list_split_move(Coord const &pos) const;

    /**
     * @brief Teste si les mouvement de la piece sont si la
     * pièce est un pion un mouvement de promotion
     *
     * @note peut etre utiliser sans verifier si la pièce est un pion
     *
     * @param[in] pos La position de la pièce
     * @return true si le mouvement possible est un mouvement
     * de promotion
     * @return false sinon
     */
    CONSTEXPR bool
    check_if_use_move_promote(Coord const &pos) const noexcept;

    /**
     * @brief Renvoie la liste de toutes les promotions
     *
     * @warning Ne verifie pas la validité du mouvement,
     * de la position ou du type de la pièce
     *
     * @param[in] pos La position du pion
     * @return La liste de tout les mouvements de promotion
     */
    CONSTEXPR std::forward_list<Move>
    get_list_promote(Coord const &pos) const noexcept;

    /**
     * @brief Applique une fonction à l'entiéreté des mouvements possible
     *
     * @param[in, out] func La fonction à appliquer sur tout les mouvements
     * @param[in] color La couleur du joueur au qu'elle on 
     * récupère les mouvements
     */
    template <class UnitaryFunction>
    CONSTEXPR void
    all_move(
        UnitaryFunction func,
        std::optional<Color> color_player = std::nullopt) noexcept;

    /**
     * @brief Test si un mouvement est réalisable
     *
     * @param[in] move Le mouvement à tester
     * @return true si le mouvement est légal
     */
    CONSTEXPR bool move_is_legal(Move const &move) const;

    /**
     * @brief Réalise un mouvement d'une pièce quelque soit le type
     * du mouvement (classic, split, merge)
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @param movement Le mouvement à raliser
     */
    CONSTEXPR void move(Move const &movement);

    /**
     * @brief Test si un plateau est gagnant pour une couleur
     *
     * @param[in] c Couleur
     * @return true si la position est gagnante pour la couleur c
     * @return false sinon
     */
    CONSTEXPR bool winning_position(Color c);

    /**
     * @brief Recupère la couleur du joueur au tour de jouer
     *
     * @return Color la couleur du joueur
     */
    CONSTEXPR Color get_current_player() const noexcept;

    /**
     * @brief Change le joueur actuel et passe la main à l'autre joueur
     */
    CONSTEXPR void change_player() noexcept;

    /**
     * @brief Renvoie la probabilité qu'il y ait une pièce à une position.
     *
     * @param pos La position
     */
    CONSTEXPR double get_proba(Coord const &pos) const noexcept;

    friend class Piece;

    template <std::size_t _N, std::size_t _M>
    friend CONSTEXPR bool
    check_path_straight_1_instance(
        Board<_N, _M> const &board,
        Coord const &dpt,
        Coord const &arv,
        std::size_t position);

    template <std::size_t _N, std::size_t _M>
    friend CONSTEXPR bool
    check_path_diagonal_1_instance(
        Board<_N, _M> const &board,
        Coord const &dpt,
        Coord const &arv,
        std::size_t position);

    template <std::size_t _N, std::size_t _M>
    friend CONSTEXPR bool
    check_path_queen_1_instance(
        Board<_N, _M> const &board,
        Coord const &dpt,
        Coord const &arv,
        std::size_t position);

    /**
     * @brief Fonction qui permet de faire un mouvement de pion quelconque avec une promotion
     *
     * @param s Coordonnées de la source
     * @param t Coordonnées de la cible
     * @param p Le type de la pièce de la promotion
     */
    CONSTEXPR void
    move_promotion(Move const &move);

private:
    /**
     * @brief Vérifie si la case à une possibilité de contenir une pièce,
     * et si elle n'en a pas modifie m_piece_board en Piece().
     *
     * @param pos Les coordonnées de a position de la case a vérifier.
     */
    void update_case(std::size_t pos) noexcept;
    /**
     * @brief Fonction qui permet de mattre à jour le plateau,
     * car ce mouvement entraine l'apparition de plusieurs instance de board
     * identique, il faut donc les concaténer en ajoutant les probas, si la
     * proba vaut 0, on supprime l'instance
     *
     * @warning Complexité en k² x N x M, avec k la taille de m_board
     * c'est à dire le nombre d'instance du plateau, N et M les dimensions
     * du plateau
     */
    void update_board() noexcept;

    /**
     * @brief Mouvement classique d'une pièce qui "saute"
     * (cavalier, roi)
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t Coordonnées de la cible du mouvement
     */
    CONSTEXPR void move_classic_jump(Coord const &s, Coord const &t);

    /**
     * @brief Mouvement "split jump"
     *
     * @warning Aucun tests sur la validité du mouvement (cible vide, ect)
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t1 Coordonnées de la cible 1 (qui doit être vide)
     * @param[in] t2 Coordonnées de la cible 2 (qui doit être vide)
     */
    CONSTEXPR void move_split_jump(
        Coord const &s,
        Coord const &t1,
        Coord const &t2);

    /**
     * @brief Mouvement de merge
     *
     * @warning Aucun test sur la validité du mouvement
     * (cible vide, pièce identique sur les sources, ect)
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s1 Coordonnées de la source 1
     * @param[in] s2 Coordonnées de la source 2
     * @param[in] t Coordonnées de la cible
     */
    CONSTEXPR void move_merge_jump(
        Coord const &s,
        Coord const &t1,
        Coord const &t2);
    /**
     * @brief Mouvement classique d'un pièce qui "glisse" (fou, dame, tour)
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @tparam N Le nombre de lignes
     * @tparam M Le nombre de colonnes
     * @param[in] s Coordonnées de la source
     * @param[in] t Coordonnées de la cible
     * @param[in] check_path  Fonction qui permet de vérifier la présence
     * d'une pièce entre la source et la cible sur une instance du plateau
     */
    CONSTEXPR void move_classic_slide(Coord const &s, Coord const &t,
                                      std::function<
                                          bool(
                                              Board<N, M> const &,
                                              Coord const &,
                                              Coord const &,
                                              std::size_t)>
                                          check_path);
    /**
     * @brief Mouvement "split slide"
     * @warning Aucun tests sur la validité du mouvement (cible vide, ect)
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t1 Coordonnées de la cible 1 (qui doit être vide)
     * @param[in] t2 Coordonnées de la cible 2 (qui doit être vide)
     * @param[in] check_path Fonction qui permet de vérifier la présence d'une
     * pièce entre la source et la cible sur une instance du plateau
     */
    CONSTEXPR void move_split_slide(
        Coord const &s,
        Coord const &t1,
        Coord const &t2,
        std::function<
            bool(
                Board<N, M> const &,
                Coord const &,
                Coord const &,
                std::size_t)>
            check_path);

    /**
     * @brief Mouvement de merge
     * @warning Aucun test sur la validité du mouvement
     * (cible vide, pièce identique sur les sources, ect)
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s1 Coordonnées de la source 1
     * @param[in] s2 Coordonnées de la source 2
     * @param[in] t Coordonnées de la cible
     * @param[in] check_path Fonction qui permet de vérifier la présence d'une
     * pièce entre la source et la cible sur une instance du plateau
     */
    CONSTEXPR void move_merge_slide(
        Coord const &s1,
        Coord const &s2,
        Coord const &t,
        std::function<
            bool(
                Board<N, M> const &,
                Coord const &,
                Coord const &,
                std::size_t)>
            check_path);

    /**
     * @brief Le mouvement de piond'une case fonctionne comme
     * un mouvementde jump classique, à la différence qu'un
     * pion ne peut pas capturer une pièce en avançant.
     * On mesure de la même façon qu'un jump classique en considérant
     * toutes les pièces comme des pièces alliées
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source
     * @param[in] t Coordonnées de la cible
     * @return true si le mouvement à etait effectué
     * @return false sinon
     */
    CONSTEXPR bool move_pawn_one_step(Coord const &s, Coord const &t);

    /**
     * @brief Le mouvement de pion de deux cases fonctionne
     * comme un mouvement de slide classique, à la différence
     * qu'un pion ne peut pas capturer une pièce en avançant,
     * on mesure de la même façon qu'un slide classique en considérant
     * toutes les pièces comme des pièces alliées.
     *
     * @warning Aucun test sur la possibilité de faire un mouvement de deux
     *  cases du pion, pas de mise a jour du board sur la prise en passant
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source
     * @param[in] t Coordonnées de la cible
     * @return true si le mouvement à etait effectué
     * @return false sinon
     */
    CONSTEXPR bool move_pawn_two_step(Coord const &s, Coord const &t);
    /**
     * @brief Mouvement de capture dun pion. A la différence
     * d'un mouvement de "capture jump", on mesure la présence
     * de la cible car le pion a besoin de la cible pour se déplacer
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @param s Coordonnées de la source
     * @param t Coordonnées de la cible
     * @return true si le mouvement à etait effectué
     * @return false sinon
     */
    CONSTEXPR bool capture_pawn(Coord const &s, Coord const &t);

    /**
     * @brief Permet d'effectuer un mouvement de prise en passant
     *
     * @warning Aucun test sur la validité de la cible et de la cible
     * en passant, ni sur le type de la pièce
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnnes du plateau
     * @param[in] s Les coordonnées de la source
     * @param[in] t Les coordonnées de la cible (l'endroit où arrive le pion)
     * @param[in] ep Les coordonnées du pion capturer "en passant"
     * @return true si le mouvement à etait effectué
     * @return false sinon
     */
    CONSTEXPR bool
    move_enpassant(Coord const &s, Coord const &t, Coord const &ep);

    /**
     * @brief Mesure la présence d'une pièce
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] position La position de la pièce mesurée
     * @return true Si la pièce est présente sur la case position
     * @return false Sinon
     */
    CONSTEXPR bool mesure(Coord const &p);

    /**
     * @brief Fonction qui permet de faire une mesure dans le cas
     d'un mouvement "capture slide"
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t Coordonnées de la cible du mouvement
     * @param[in] check_path Fonction qui permet de vérifier si il y a une pièce
     * entre la source et la cible sur une instance du plateau
     * @return true Si la mesure indique de faire le mouvement
     * @return false Sinon
     */
    CONSTEXPR bool
    mesure_capture_slide(Coord const &s, Coord const &t,
                         std::function<bool(Board<N, M> const &,
                                            Coord const &, Coord const &,
                                            std::size_t)>
                             check_path);

    /**
     * @brief Mouvement classique d'une pièce (pion exclu)
     * @warning Ne procède aucune vérification sur la validité du mouvement
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t Coordonnées de la cible du mouvement
     */
    CONSTEXPR void move_classic(Coord const &s, Coord const &t);

    /**
     * @brief Mouvement split d'une pièce (pion exclu)
     * @warning Ne procède aucune vérification sur la validité du mouvement
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source du mouvement
     * @param[in] t1 Coordonnées de la cible 1 (qui doit être vide)
     * @param[in] t2 Coordonnées de la cible 2 (qui doit être vide)
     */
    CONSTEXPR void move_split(Coord const &s,
                              Coord const &t1,
                              Coord const &t2);

    /**
     * @brief Mouvement de merge de pièce (pion exclu)
     *
     * @warning Aucun test sur la validité du mouvement
     * (cible vide, pièce identique sur les sources, ect)
     *
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s1 Coordonnées de la source 1
     * @param[in] s2 Coordonnées de la source 2
     * @param[in] t Coordonnées de la cible
     */
    CONSTEXPR void move_merge(Coord const &s1,
                              Coord const &s2,
                              Coord const &t);

    /**
     * @brief Gère tout les mouvements d'un pion
     * @warning Ne procède aucune vérification sur la validité du mouvement
     * @tparam N Le nombre de lignes du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @param[in] s Coordonnées de la source
     * @param[in] t Coordonnées de la cible
     * @return true si le mouvement à etait effectué
     * @return false sinon
     */
    CONSTEXPR bool move_pawn(Coord const &s, Coord const &t) noexcept;

    /**
     * @brief Renvoie une position 1D d'une coordonnée 2D
     *
     * @param[in] ligne L'indice de ligne
     * @param[in] colonne L'indice de colonne
     * @return std::size_t La position en 1D
     */
    CONSTEXPR static std::size_t
    offset(std::size_t ligne,
           std::size_t colonne) noexcept;

    /**
     * @brief Initialise un plateau à l'aide des listes d'initialisations
     *
     * @param[in] board La liste d'initialisation en 2D
     * @param[out] first_instance Le tableau de la première
     * instance du plateau
     * @param[out] piece_board Le plateau contenant les pièces
     */
    CONSTEXPR static void
    initializer_list_to_2_array(
        std::initializer_list<
            std::initializer_list<
                Piece>> const &board,
        std::array<bool, N * M> &first_instance,
        std::array<Piece, N * M> &piece_board) noexcept;

    /**
     * @brief Construit les mailbox en fonction
     * des dimenssions du plateau
     *
     * @param[out] S_mailbox La petite mailbox de la taille du plateau
     * @param[out] L_mailbox La grande mailbox comportant 4 ligne de plus
     * et 2 colonne de plus
     * @return CONSTEXPR
     */
    CONSTEXPR static void init_mailbox(std::array<int, N * M>
                                           &S_mailbox,
                                       std::array<int, (N + 4) * (M + 2)>
                                           &L_mailbox) noexcept;

    /**
     * @brief Renvoie un réel pseudo aléatoirement entre [O, 1)
     *
     * @return double Un nombre aléatoire entre 0 et 1
     */
    static double get_random_number_0_1();

    /**
     * @brief fonction auxiliaire qui permet de modifier un plateau
     à l'aide d'un array de la forme du type de retour de la fonction
     qubitToArray, le deuxième éléments du tableau à une probalité non nulle
     lors d'un mouvement split
     *
     * @tparam Q La taille du qubit
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de collone du plateau
     * @param[in] arrayQubit Type de retour de la fonction quibitToArray,
     * représente la facon dont va être modifier m_board
     * @param[in] position_board  L'indice du plateau dans le tableau de
     * toutes les instances du plateau
     * @param[in] tab_positions Tableau des indices des cases modifiées,
     * on utilise N*M+1 pour signifier que le qubit en question
     * est un qubit auxiliaire qui ne modifie pas le board
     */
    template <std::size_t Q>
    CONSTEXPR void modify(std::array<std::pair<std::array<bool, Q>,
                                               std::complex<double>>,
                                     2> const &arrayQubit,
                          std::size_t position_board,
                          std::array<std::size_t, Q> const &tab_positions);

    /**
     * @brief Fonction qui permet d'effectuer un mouvement
     * sur une instance du plateau
     *
     * @tparam N Le nombre de ligne du plateau
     * @tparam M Le nombre de colonnes du plateau
     * @tparam Q La taille du qubit
     * @param[in] case_modif Permet d'initialiser le qubit
     * @param[in] position L'instance du plateau modifiée
     * @param[in] matrix La matrice du mouvement que l'on veut effectuer
     * @param[in] tab_positions La position sur le plateau des variables
     * utilisées dans le qubit, que l'on met à N*M+1 si les variables
     * ne représentent pas des pièces
     */
    template <std::size_t Q>
    CONSTEXPR void move_1_instance(std::array<bool, Q> const &case_modif,
                                   std::size_t position,
                                   CMatrix<_2POW(Q)> const &matrix,
                                   std::array<std::size_t, Q> const
                                       &tab_positions) noexcept;

    /**
     * @brief Mouvement du petit roque.
     * @warning Auncun test sur la validité du mouvement
     * @param[in] king Coordonnées du roi
     * @param[in] rook Coordonnées de la tour
     */
    CONSTEXPR void king_side_castle(Coord const &king, Coord const &rook);

    /**
     * @brief Fonction qui permet de mesurer la possibilité de faire le roque
     *
     * @param[in] king Coordonnées du roi
     * @param[in] rook Coordonnées de la tour
     * @return true si le roque est possible
     * @return false sinon
     */
    CONSTEXPR bool mesure_castle(
        Coord const &king,
        Coord const &rook);

    /**
     * @brief Mouvement du grand roque.
     *
     * @warning Ne procède aucune vérification sur la validité du mouvement
     *
     * @param[in] king Coordonnées du roi
     * @param[in] rook Coordonnées de la tour
     */
    CONSTEXPR void queen_side_castle(Coord const &king, Coord const &rook);

    /**
     * @brief Le tableau de toutes les instances possibles
     * du plateau
     */
    std::vector<std::pair<std::array<bool, N * M>,
                          std::complex<double>>>
        m_board;

    /**
     * @brief Le plateau indiquant le type des pièces
     */
    std::array<Piece, N * M> m_piece_board;

    /**
     * @brief La petite mailbox
     */
    std::array<int, N * M> m_S_mailbox;

    /**
     * @brief La grande mailbox
     */
    std::array<int, (N + 4) * (M + 2)> m_L_mailbox;

    /**
     * @brief Color::WHITE si c'est aux blanc de jouer aux noirs sinon
     */
    Color m_color_current_player;

    /**
     * @brief Vrai si les noirs[0] / blancs[1] peuvent faire le petit roque
     */
    std::array<bool, 2> m_k_castle;

    /**
     * @brief Vrai si les noirs[0] / blancs[1] peuvent faire le grand roque
     */
    std::array<bool, 2> m_q_castle;

    /**
     * @brief Contient la case sur laquelle il est
     * possible de faire une prise en passant qui
     * est vide (la case occupé si le pion avait avancé
     * d'une seule case)
     */
    std::optional<Coord> m_ep;
};

#include "Board.tpp"
#endif