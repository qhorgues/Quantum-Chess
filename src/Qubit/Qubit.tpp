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
    bool b{true};
    std::size_t compteur{N - 1};
    for (std::size_t i{0}; i < _2POW(N); i++)
    {
        using namespace std::complex_literals;
        if (qubit.m_data[i] == 0i)
        {
            if (b)
            {
                while (tab[1].first[compteur])
                {
                    tab[0].first[compteur] = false;
                    tab[1].first[compteur] = false;
                    compteur--;
                }
                tab[0].first[compteur] = true;
                tab[1].first[compteur] = true;
                compteur = N - 1;
            }
            else
            {
                while (tab[1].first[compteur])
                {
                    tab[1].first[compteur] = false;
                    compteur--;
                }
                tab[1].first[compteur] = true;
                compteur = N - 1;
            }
        }
        else
        {
            if (b)
            {
                tab[0].second = qubit.m_data[i];
                b = false;
                while (tab[1].first[compteur])
                {
                    tab[1].first[compteur] = false;
                    compteur--;
                }
                tab[1].first[compteur] = true;
                compteur = N - 1;
            }
            else
            {
                tab[1].second = qubit.m_data[i];
                return tab;
            }
        }
    }
    return tab;
}