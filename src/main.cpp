#include <iostream>
#include <Matrix.hpp>
#include <complex>
#include <Qubit.hpp>
#include <Unitary.hpp>

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
}