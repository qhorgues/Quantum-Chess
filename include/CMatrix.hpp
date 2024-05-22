#ifndef CMATRIX_HPP
#define CMATRIX_HPP
#include <Matrix.hpp>
#include <complex>
#include <Constexpr.hpp>
#include <cstddef>

/**
 * @brief Objet représentant les matrices carrées complexe
 * de dimension N
 * 
 * @tparam N La dimension de la matrice
 */
template <std::size_t N>
using CMatrix = Matrix<std::complex<double>, N>;

namespace
{
    template <std::size_t N>
    CONSTEXPR CMatrix<N> conj(CMatrix<N> const & matrix)
    {
        CMatrix<N> matrix_conj{};
        for(std::size_t i = 0; i<N; i++)
        {
            for(std::size_t j = 0; j<N; j++)
            {
                matrix_conj(i, j) = std::conj(matrix(i, j));
            }
        }
        return matrix_conj;
    }
}

/**
 * @brief Réalise l'opération 2^n
 * @warning Est valable uniquement sur les types entiers
 */
#define _2POW(n) (1 << (n))

#endif