#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace rnd
{
  /**
   * @brief Renvoie un entier dans l'intervale [a, b]
   * 
   * @param a, b Entier pour spécifier la plage
   * @return int Un entier entre a et b inclu
   */
  int randint(int a, int b);

  /**
   * @brief Renvoie un réel dans l'intervale [a, b)
   * 
   * @param a, b Réel pour spécifier la plage
   * @return double Un réel entre a et b inclu
   */
  double randreal(double a, double b);
}


#endif