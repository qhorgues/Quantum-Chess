#ifndef MOVE_HPP
#define MOVE_HPP

#include <Coord.hpp>

/**
 * @brief Enumération représentant tout les types de mouvements
 */
enum class TypeMove
{
    /**
     * @brief Représente le mouvement classic
     */
    NORMAL,

    /**
     * @brief Réprésente le mouvement de split de deux pièces
     */
    SPLIT,

    /**
     * @brief Représente le mouvement de fusion de deux pièces
     */
    MERGE
};

/**
 * @brief Stoque tout les mouvements possibles
 */
struct Move
{
    Move() = default;
    Move(Move const&) = default;
    Move& operator=(Move const&) = default;
    Move(Move &&) = default;
    Move& operator=(Move &&) = default;
    /**
     * @brief Indique quel est le mouvement stoqué par l'union
     */
    TypeMove type;

    /**
     * @brief Stoque au choix l'un des trois mouvements possible
     */
    union
    {
        /**
         * @brief Stoque les coordonnées pour un mouvement classic
         */
        struct
        {
            /**
             * @brief La coordonnée de depart
             */
            Coord src;

            /**
             * @brief La coordonnées d'arrivée
             */
            Coord arv;
        } normal;

        /**
         * @brief stoque l'ensemble des coordonnées pour un
         * mouvement splité
         */
        struct
        {
            /**
             * @brief La coordonnée de depart
             */
            Coord src;

            /**
             * @brief Une coordonnée d'arrivée
             */
            Coord arv1;

            /**
             * @brief Une seconde coordonnée d'arrivée
             *
             */
            Coord arv2;
        } split;

        /**
         * @brief  stoque l'ensemble des coordonnées pour un
         * mouvement de fusion
         */
        struct
        {
            /**
             * @brief La coordonnée de la première pièce à fusioner
             */
            Coord src1;

            /**
             * @brief La coordonnée de la seconde pièce à fusioner
             */
            Coord src2;

            /**
             * @brief La coordonnée de la position d'arrivée
             */
            Coord arv;
        } merge;
    };
};

CONSTEXPR Move Move_classic(Coord const &src, Coord const &arv)
{
    Move move;
    move.type = TypeMove::NORMAL;
    move.normal.src = src;
    move.normal.arv = arv;
    return move;
}

CONSTEXPR Move Move_split(Coord const &src, Coord const &arv1, Coord const &arv2)
{
    Move move;
    move.type = TypeMove::SPLIT;
    move.split.src = src;
    move.split.arv1 = arv1;
    move.split.arv2 = arv2;
    return move;
}

CONSTEXPR Move Move_merge(Coord const &src1, Coord const &src2, Coord const &arv)
{
    Move move;
    move.type = TypeMove::MERGE;
    move.merge.src1 = src1;
    move.merge.src2 = src2;
    move.merge.arv = arv;
    return move;
}
#endif