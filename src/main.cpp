#include <iostream>
#include <Matrix.hpp>

int main()
{
    Matrix<int, 3> A {
        {
            {1, 3, 8},
            {8, 3, 2},
            {0, 8, 1}
        }
    };
    std::cout << A << std::endl;
}