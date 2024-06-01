#ifndef MATH_UTILITY_HPP
#define MATH_UTILITY_HPP

#define EPSILON 10e-3

#include <complex>
#include <Constexpr.hpp>

namespace
{
  CONSTEXPR bool double_equal(double x, double y);
  CONSTEXPR bool complex_equal(
      std::complex<double> const &z1,
      std::complex<double> const &z2);
}

#include "math_utility.tpp"
#endif