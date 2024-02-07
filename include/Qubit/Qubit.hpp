#ifndef QUBIT_HPP
#define QUBIT_HPP

#include <cstddef>
#include <complex>
#include <array>
#include <Matrix.hpp>

#define _2POW(n) (1 << (n))

template <std::size_t N>
class Qubit final
{
public:
    // Constructeur
    Qubit() = default;
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

template <std::size_t N>
Qubit<N> operator*(Matrix<std::complex<double>, _2POW(N)> const &lhs, Qubit<N> const &rhs);

template <std::size_t N>
std::ostream &operator<<(std::ostream &out, Qubit<N> const &qubit);

#include "Qubit.tpp" 

#endif