#ifndef CMATRIX_HPP
#define CMATRIX_HPP
#include <Matrix.hpp>

/**
 * @brief Objet représentant les matrices carrées complexe
 * de dimension N
 * 
 * @tparam N La dimension de la matrice
 */
template <std::size_t N>
using CMatrix = Matrix<std::complex<double>, N>;

/**
 * @brief Réalise l'opération 2^n
 * @warning Est valable uniquement sur les types entiers
 */
#define _2POW(n) (1 << (n))

#endif