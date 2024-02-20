#include "Qubit.hpp"
#include <Matrix.hpp>
#include <algorithm>
#include <numeric>

template <std::size_t N>
Qubit<N>::Qubit(std::array<std::complex<double>, _2POW(N)> &&init_list)
    : m_data(std::move(init_list))
{
}

template <std::size_t N>
Qubit<N>::Qubit(std::array<bool, N> const &data) : m_data()
{

    auto sum{[](std::array<bool, N> const &tab) -> int
             {
                 int acc{0};
                 for (std::size_t i{0}; i < std::size(tab); i++)
                 {
                     acc += _2POW(i) * tab[std::size(tab) - i - 1];
                 }
                 return acc;
             }};
    m_data[sum(data)] = 1;
}

template <std::size_t N>
Qubit<N> operator*(CMatrix<_2POW(N)> const &lhs, Qubit<N> const &rhs)
{
    std::array<std::complex<double>, _2POW(N)> tab{};
    for (std::size_t i{0}; i < lhs.numberLines(); i++)
    {
        for (std::size_t j{0}; j < lhs.numberColumns(); j++)
        {
            tab[i] += lhs(i, j) * rhs.m_data[j];
        }
    }

    return Qubit<N>(std::move(tab));
}

template <std::size_t N>
std::ostream &operator<<(std::ostream &out, Qubit<N> const &qubit)
{
    for (std::size_t i{0}; i < _2POW(N); i++)
    {
        out << qubit.m_data[i].real() << "+" << qubit.m_data[i].imag() << "i  ";
    }
    return out;
}

template <std::size_t N>
std::array<std::pair<std::array<bool, N>, std::complex<double>>, 2> qubitToArray(Qubit<N> const &qubit)
{
    std::array<std::pair<std::array<bool, N>, std::complex<double>>, 2> tab{};
    std::size_t c{0};
    std::size_t compteur{0};
    for (std::size_t i{0}; i < _2POW(N); i++)
    {
        if (qubit.m_data[i] == 0i)
        {
            while (tab[c].first[compteur])
            {
                tab[c].first[compteur] = false;
                compteur++;
            }
            tab[c].first[compteur] = true;
            compteur = 0;
        }
        else
        {
            tab[c].second = qubit.m_data[i];
            if (c == 1)
            {
                return tab;
            }
            else
            {
                c = 1;
                compteur = 0;
            }
        }
    }
    return tab;
}