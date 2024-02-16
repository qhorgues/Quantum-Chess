#include "Qubit.hpp"
#include <Matrix.hpp>
#include <algorithm>

template <std::size_t N>
Qubit<N>::Qubit(std::array<std::complex<double>, _2POW(N)> &&init_list) : m_data(std::move(init_list))
{
}

template <std::size_t N>
Qubit<N>::Qubit(std::array<bool, N> const& data) : m_data()
{
    for (std::size_t i { 0 }; i < _2POW(N); i++)
    {
        
    }
}

template <std::size_t N>
Qubit<N> operator*(Matrix<std::complex<double>, _2POW(N)> const &lhs, Qubit<N> const &rhs)
{
    std::array<std::complex<double>, _2POW(N)> tab{};
    for (std::size_t i{0}; i < lhs.numberLines(); i++)
    {
        for (std::size_t j{0}; j < lhs.numberColumns(); j++)
        {
            tab[i] += lhs(i,j) * rhs.m_data[j];
        }
    }

    return Qubit<N>(std::move(tab));
}

template <std::size_t N>
std::ostream &operator<<(std::ostream &out, Qubit<N> const &qubit)
{
    for (std::size_t i {0}; i < _2POW(N); i++)
    {
        out << qubit.m_data[i].real() << "+" << qubit.m_data[i].imag() << "i  ";
    }
    return out;
}