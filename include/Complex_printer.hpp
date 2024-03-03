#ifndef COMPLEX_PRINTER_HPP
#define COMPLEX_PRINTER_HPP

#include <complex>
#include <concepts>
#include <string>

namespace std
{
    /**
     * @brief convertit un nombre complexe en chaine de caractère
     */
    using namespace std::literals;
    template <std::floating_point T>
    std::string to_string(std::complex<T> value)
    {
        return std::string{std::to_string(std::real(value)) 
                            + ((std::imag(value) >= 0) ? '+' : '\0') 
                            + std::to_string(std::imag(value)) + 'i'};
    }

    /**
     * @brief Implémente l'opérateur de flux sortant
     * sur les nombres complexes
     * 
     * @tparam T le type de représentation floatante du complexe
     * @param os Le flux de sorti
     * @param value Le nombre complexe a transférer
     * @return std::ostream& retourne le flux passé en paramètre
     */
    template <std::floating_point T>
    std::ostream& operator<<(std::ostream& os, std::complex<T> value)
    {
        os << std::real(value) << ((std::imag(value) >= 0) ? '+' : '\0' ) << std::imag(value) << 'i';
        return os;
    }
}

#endif