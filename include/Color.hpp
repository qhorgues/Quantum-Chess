#ifndef COLOR_HPP
#define COLOR_HPP

/**
 * @brief Représente la couleur d'un joueur
 */
enum class Color : bool
{
    /**
     * @brief Le joueur noir dont la valeur vaut 0
     */
    BLACK = false,

    /**
     * @brief Le joueur blanc dont la valeur vaut 1
     */
    WHITE = true
};

constexpr inline Color opponent_color(Color color) noexcept
{
    return (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

#endif