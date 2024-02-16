#ifndef COMPLEX_PRINTER_HPP
#define COMPLEX_PRINTER_HPP

#include <complex>
#include <concepts>
#include <string>

namespace std
{
    using namespace std::literals;
    template <std::floating_point T>
    std::string to_string(std::complex<T> value)
    {
        return std::string{std::to_string(std::real(value)) 
                            + ((std::imag(value) >= 0) ? '+' : '\0') 
                            + std::to_string(std::imag(value)) + 'i'};
    }

    template <std::floating_point T>
    std::ostream& operator<<(std::ostream& os, std::complex<T> value)
    {
        os << std::real(value) << ((std::imag(value) >= 0) ? '+' : '\0' ) << std::imag(value) << 'i';
        return os;
    }
}

#endif