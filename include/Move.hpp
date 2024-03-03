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
    /**
     * @brief Indique qu'elle est le mouvement stoqué par l'union
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

#endif