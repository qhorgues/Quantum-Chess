#ifndef UNITARY_HPP
#define UNITARY_HPP
#include <CMatrix.hpp>
#include <numbers>
#include <complex>
#include <Constexpr.hpp>

using namespace std::complex_literals;
using namespace std::numbers;

constexpr inline
CMatrix<4> MATRIX_ISWAP {
    {
        {1,  0,  0, 0},
        {0,  0, 1i, 0},
        {0, 1i,  0, 0},
        {0,  0,  0, 1}
    }
};


constexpr inline
CMatrix<4> MATRIX_SQRT_ISWAP {
    {
        {1,        0,         0, 0},
        {0, 1./sqrt2,  1i/sqrt2, 0},
        {0, 1i/sqrt2,  1./sqrt2, 0},
        {0,        0,         0, 1}
    }
};

constexpr inline
CMatrix<4> MATRIX_JUMP { MATRIX_ISWAP };

constexpr inline
CMatrix<8> MATRIX_SLIDE {
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

constexpr inline
CMatrix<8> MATRIX_ISWAP_8 {
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

constexpr inline
CMatrix<8> MATRIX_SQRT_ISWAP_8 {
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

constexpr inline
CMatrix<8> MATRIX_SPLIT {
    {
        {1,        0,         0,   0,  0,         0,         0, 0},
        {0,        0,         0,   0, 1i,         0,         0, 0},
        {0, 1i/sqrt2,  1./sqrt2,   0,  0,         0,         0, 0},
        {0,        0,         0,   0,  0, -1./sqrt2,  1i/sqrt2, 0},
        {0, 1i/sqrt2, -1./sqrt2,   0,  0,         0,         0, 0},
        {0,        0,         0,   0,  0,  1i/sqrt2, -1/sqrt2, 0},
        {0,        0,         0,   1i,  0,         0,         0, 0},
        {0,        0,         0,   0,  0,         0,         0, 1}
    }
};

constexpr inline
CMatrix<8> MATRIX_MERGE {
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

constexpr inline
CMatrix<32> MATRIX_SPLIT_SLIDE {
    CMatrix<16>{
        MATRIX_SPLIT, CMatrix<8>{},
        CMatrix<8>{}, MATRIX_ISWAP_8
    },                                  CMatrix<16>{},
    CMatrix<16>{},                      CMatrix<16>{
                                            CMatrix<2>::identity()
                                                .tensoriel_product(MATRIX_ISWAP), CMatrix<8>{},
                                            CMatrix<8>{}, CMatrix<8>::identity()
                                        }
};

constexpr inline
CMatrix<32> MATRIX_MERGE_SLIDE {
    CMatrix<16>{
        MATRIX_MERGE, CMatrix<8>{},
        CMatrix<8>{}, CMatrix<2>::identity().tensoriel_product(MATRIX_ISWAP).transposed()
    },                                                  
    CMatrix<16>{},
    CMatrix<16>{},                                      
    CMatrix<16>{
        MATRIX_ISWAP_8.transposed(), CMatrix<8>{},
        CMatrix<8>{},                CMatrix<8>::identity()
    }
};



#endif