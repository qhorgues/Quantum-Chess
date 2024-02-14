#include <iostream>
#include "GameBoard/GameBoard.hpp"

int main()
{
    using enum Piece;
    std::cout << static_cast<int>(EMPTY) << std::endl;
    Board<2> B {
        {
            {EMPTY, B_KING},
            {W_KING, W_QUEEN}
        }
    };
    

}