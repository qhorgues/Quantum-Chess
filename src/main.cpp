#include <iostream>
#include <complex>

#include <string>
#include <concepts>
namespace std
{
    using namespace std::literals;
    template <std::floating_point T>
    std::string to_string(std::complex<T> value)
    {
        return std::string{std::to_string(std::real(value)) 
                            + ((std::imag(value) > 0) ? '+' : '-') 
                            + std::to_string(std::imag(value)) + 'i'};
    }

    template <std::floating_point T>
    std::ostream& operator<<(std::ostream& os, std::complex<T> value)
    {
        os << std::real(value) << ((std::imag(value) > 0) ? '+' : '-') << std::imag(value) << 'i';
        return os;
    }
}
#include <Matrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>

int main()
{
    using namespace std::complex_literals;

    Matrix<std::complex<double>, 4> A {
        {
            {1,     3, 1.+1i,  8},
            {8,    1i,     2,  0},
            {0,     8,     1, 4i},
            {2, 2.+5i,     0,  1}
        }
    };

    Qubit<2> q {std::array<std::complex<double>, 4>({0, 1, 2, 3})};
    auto q2 = A*q;
    std::cout << q2 << std::endl;
    std::cout << MATRIX_SPLIT_SLIDE << std::endl;
}