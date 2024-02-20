#ifndef UNITARY_HPP
#define UNITARY_HPP
#include <Matrix.hpp>
#include <numbers>
#include <complex>

using namespace std::complex_literals;
using namespace std::numbers;

template <std::size_t N>
using CMatrix = Matrix<std::complex<double>, N>;

constexpr CMatrix<4> MATRIX_ISWAP {
    {
        {1,  0,  0, 0},
        {0,  0, 1i, 0},
        {0, 1i,  0, 0},
        {0,  0,  0, 1}
    }
};


constexpr CMatrix<4> MATRIX_SQRT_ISWAP {
    {
        {1,        0,         0, 0},
        {0, 1./sqrt2,  1i/sqrt2, 0},
        {0, 1i/sqrt2,  1./sqrt2, 0},
        {0,        0,         0, 1}
    }
};

constexpr CMatrix<4> MATRIX_JUMP { MATRIX_ISWAP };

constexpr CMatrix<8> MATRIX_SLIDE {
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

constexpr CMatrix<8> MATRIX_ISWAP_8 {
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

constexpr CMatrix<8> MATRIX_SQRT_ISWAP_8 {
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

constexpr CMatrix<8> MATRIX_SPLIT {
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
constexpr CMatrix<8> MATRIX_MERGE {
    {
        {1,   0,         0,         0,         0,         0,   0,  0},
        {0,   0, -1i/sqrt2,         0, -1i/sqrt2,         0,   0,  0},
        {0,   0,   1/sqrt2,         0,  -1/sqrt2,         0,   0,  0},
        {0,   0,         0,         0,         0,         0, -1i,  0},
        {0, -1i,         0,         0,         0,         0,    0, 0},
        {0,   0,         0,  -1/sqrt2,         0, -1i/sqrt2,    0, 0},
        {0,   0,         0, -1i/sqrt2,         0,  -1/sqrt2,    0, 0},
        {0,   0,         0,         0,         0,         0,    0, 1}
    }
};

constexpr CMatrix<32> MATRIX_SPLIT_SLIDE {
    CMatrix<16>{
        MATRIX_SPLIT, CMatrix<8>{},
        CMatrix<8>{}, MATRIX_ISWAP_8
    },                                  CMatrix<16>{},
    
    CMatrix<16>{},                      CMatrix<16>{
                                            MATRIX_ISWAP_8, CMatrix<8>{},
                                            CMatrix<8>{}, CMatrix<8>::identity()
                                        }
};

constexpr CMatrix<32> MATRIX_MERGE_SLIDE {
    CMatrix<16>{
        MATRIX_MERGE, CMatrix<8>{},
        CMatrix<8>{}, MATRIX_ISWAP_8.transposed()
    },                                                  CMatrix<16>{},

    CMatrix<16>{},                                      CMatrix<16>{
                                                            MATRIX_ISWAP_8.transposed(), CMatrix<8>{},
                                                            CMatrix<8>{},              CMatrix<8>::identity()
                                                        }
};



#endif