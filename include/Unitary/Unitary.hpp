#ifndef UNITARY_HPP
#define UNITARY_HPP
#include <Matrix.hpp>
#include <numbers>
#include <complex>

using namespace std::complex_literals;
using namespace std::numbers;

constexpr Matrix<std::complex<double>, 4> MATRIX_ISWAP {
    {
        {1,  0,  0, 0},
        {0,  0, 1i, 0},
        {0, 1i,  0, 0},
        {0,  0,  0, 1}
    }
};


constexpr Matrix<std::complex<double>, 4> MATRIX_SQRT_ISWAP {
    {
        {1,        0,         0, 0},
        {0, 1./sqrt2,  1i/sqrt2, 0},
        {0, 1i/sqrt2,  1./sqrt2, 0},
        {0,        0,         0, 1}
    }
};

constexpr Matrix<std::complex<double>, 4> MATRIX_JUMP { MATRIX_ISWAP };

constexpr Matrix<std::complex<double>, 8> MATRIX_SLIDE {
    {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1i, 0, 0, 0, 0, 0 },
        {0, 1i, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0 },
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1}
    }
};

constexpr Matrix<std::complex<double>, 8> MATRIX_SLIT_ISWAP {
    {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1i, 0, 0, 0 },
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1i, 0},
        {0, 1i, 0, 0, 1, 0, 0, 0 },
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1i, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1}
    }
};

constexpr Matrix<std::complex<double>, 8> MATRIX_SLIT_SQRT_ISWAP {
    {
        {1,        0,        0, 0,  0,        0,        0, 0},
        {0, 1./sqrt2, 1i/sqrt2, 0, 1i,        0,        0, 0},
        {0, 1i/sqrt2, 1./sqrt2, 0,  0,        0,        0, 0},
        {0,        0,        0, 1,  0,        0,        0, 0},
        {0,        0,        0, 0,  1,        0,        0, 0},
        {0,        0,        0, 0,  0, 1./sqrt2, 1i/sqrt2, 0},
        {0,        0,        0, 0,  0, 1i/sqrt2, 1./sqrt2, 0},
        {0,        0,        0, 0,  0,        0,        0, 1}
    }
};

constexpr Matrix<std::complex<double>, 8> MATRIX_SLIT {
    {
        {1,        0,         0,   0,  0,         0,         0, 0},
        {0,        0,         0,   0, 1i,         0,         0, 0},
        {0, 1i/sqrt2,  1./sqrt2,   0,  0,         0,         0, 0},
        {0,        0,         0,   1,  0, -1./sqrt2,  1i/sqrt2, 0},
        {0, 1i/sqrt2, -1./sqrt2,   0,  1,         0,         0, 0},
        {0,        0,         0,   0,  0,  1./sqrt2, -1i/sqrt2, 0},
        {0,        0,         0,   0,  0,         0,         0, 0},
        {0,        0,         0,   0,  0,         0,         0, 1}
    }
};
constexpr Matrix<std::complex<double>, 8> MATRIX_MERGE {
{
{1, 0, 0, 0, 0, 0, 0, 0},
{0, 0, -1i/sqrt2, 0, -1i/sqrt2, 0, 0, 0 },
{0, 0, 1/sqrt2, 0, -1/sqrt2, 0, 0, 0},
{0, 0, 0, 0, 0, 0, -1i, 0},
{0, -1i, 0, 0, 0, 0, 0, 0 },
{0, 0, 0, -1/sqrt2, 0, -1i/sqrt2, 0, 0},
{0, 0, 0, -1i/sqrt2, 0, -1/sqrt2, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1}
}
};


#endif