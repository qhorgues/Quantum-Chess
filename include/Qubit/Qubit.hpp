#ifndef QUBIT_HPP
#define QUBIT_HPP

#include <cstddef>
#include <complex>
#include <array>
#include <Matrix.hpp>

/**
 * @brief Réalise l'opération 2^n
 */
#define _2POW(n) (1 << (n))

template <std::size_t N>
class Qubit final
{
public:
    // Constructeur
    Qubit() = default;

    /**
     * @brief Construit un nouveau qubit à l'aide d'un tableau de booléen
     * 
     * @param data le tableau de n booléen utilisé pour construire un n-qubit ex : |10> 
     */
    Qubit(std::array<bool, N> const & data);
    Qubit(std::array<std::complex<double>, _2POW(N)> && init_list);

    // Copie
    Qubit(Qubit const &) = delete;
    Qubit& operator=(Qubit const &) = delete;

    // Mouvement
    Qubit(Qubit &&) = delete;
    Qubit& operator=(Qubit &&) = delete;

    // Destructeur
    ~Qubit() = default;

    template <std::size_t M>
    friend Qubit<M> operator*(Matrix<std::complex<double>, _2POW(M)> const& lhs, Qubit<M> const& rhs);

    template <std::size_t M>
    friend std::ostream &operator<<(std::ostream &out, Qubit<M> const &qubit);

private:
    std::array<std::complex<double>, _2POW(N)> m_data;
};

/**
 * @brief Opérateur du produit entre un qubit et une matrice
 * @warning le produit n'est pas associatif
 * @tparam M La taille du qubit, la matrice est de taille 2^M
 * @param lhs La matrice carré complexe  de taille 2^M
 * @param rhs Le M-qubit
 * @return Qubit<M> renvoie le qubit resultant du produit
 */
template <std::size_t N>
Qubit<N> operator*(Matrix<std::complex<double>, _2POW(N)> const &lhs, Qubit<N> const &rhs);

/**
 * @brief Fonction d'affichage des qubit sous forme de liste de nombre complexe
 * 
 * @tparam M La taille du qubit
 * @param out Le flux de destination
 * @param qubit Le qubit à afficher
 * @return std::ostream& le flux de destination
 */
template <std::size_t N>
std::ostream &operator<<(std::ostream &out, Qubit<N> const &qubit);

#include "Qubit.tpp" 

#endif