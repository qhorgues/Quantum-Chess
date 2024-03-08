#ifndef MATH_UTILITY_HPP
#define MATH_UTILITY_HPP

#define EPSILON 0.00001

bool double_equal(double x, double y);
bool complex_equal(
    std::complex<double> const &z1,
    std::complex<double> const &z2);

#endif