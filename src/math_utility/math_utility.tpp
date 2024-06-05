#include <cmath>
#include <complex>
#include <Constexpr.hpp>
#include "math_utility.hpp"

namespace
{
  CONSTEXPR bool double_equal(double x, double y)
  {
    return std::abs(x - y) <= EPSILON;
  }

  CONSTEXPR bool complex_equal(
      std::complex<double> const &z1,
      std::complex<double> const &z2)
  {
    return double_equal(z1.real(), z2.real()) &&
           double_equal(z1.imag(), z2.imag());
  }
}