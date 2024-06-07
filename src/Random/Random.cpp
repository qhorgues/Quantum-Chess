#include <random>
#include "Random.hpp"

namespace
{
  std::random_device rd;
}

namespace rnd
{
  int randint(int a, int b)
  {
    std::uniform_int_distribution<> distrib(a, b);
    return distrib(rd);
  }

  double randreal(double a, double b)
  {
    std::uniform_real_distribution<> distrib(a, b);
    return distrib(rd);
  }
}