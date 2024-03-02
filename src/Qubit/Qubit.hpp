#ifndef QUBIT_HPP
#define QUBIT_HPP

#include <cstddef>
#include <complex>
#include <array>
#include <CMatrix.hpp>
#include <Constexpr.hpp>

template <std::size_t N>
class Qubit final
{
public:
    // Constructeur
    CONSTEXPR Qubit() = default;

    /**
     * @brief Construit un nouveau qubit à l'aide d'un tableau de booléen
     * 
     * @param data le tableau de n booléen utilisé pour construire un n-qubit ex : |10> 
     */
    CONSTEXPR Qubit(std::array<bool, N> const & data);
    CONSTEXPR Qubit(std::array<std::complex<double>, _2POW(N)> && init_list);

    // Copie
    CONSTEXPR Qubit(Qubit const &) = delete;
    CONSTEXPR Qubit& operator=(Qubit const &) = delete;

    // Mouvement
    CONSTEXPR Qubit(Qubit &&) = delete;
    CONSTEXPR Qubit& operator=(Qubit &&) = delete;

    // Destructeur
    CONSTEXPR ~Qubit() = default;

    template <std::size_t M>
    friend CONSTEXPR Qubit<M> operator*(CMatrix<_2POW(M)> const& lhs, Qubit<M> const& rhs);

    template <std::size_t M>
    friend std::ostream &operator<<(std::ostream &out, Qubit<M> const &qubit);

    template <std::size_t M>
    friend CONSTEXPR std::array<std::pair<std::array<bool , M>, std::complex<double>>, 2> qubitToArray (Qubit<M> const & qubit); 

private:
    std::array<std::complex<double>, _2POW(N)> m_data;
};

template <std::size_t N>
CONSTEXPR std::array<std::pair<std::array<bool , N>, std::complex<double>>, 2> qubitToArray (Qubit<N> const & qubit); 

/**
 * @brief Opérateur du produit entre un qubit et une matrice
 * @warning le produit n'est pas associatif
 * @tparam M La taille du qubit, la matrice est de taille 2^M
 * @param lhs La matrice carré complexe  de taille 2^M
 * @param rhs Le M-qubit
 * @return Qubit<M> renvoie le qubit resultant du produit
 */
template <std::size_t N>
CONSTEXPR Qubit<N> operator*(Matrix<std::complex<double>, _2POW(N)> const &lhs, Qubit<N> const &rhs);

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