#include <iostream>
#include <Board.hpp>

const char *getUnicodeChar(TypePiece piece, Color color)
{
  using enum TypePiece;
  if (color == Color::WHITE)
  {
#if defined(WIN32)
    switch (piece)
    {
    case KING:
      return "K";
    case QUEEN:
      return "Q";
    case ROOK:
      return "R";
    case BISHOP:
      return "B";
    case KNIGHT:
      return "N";
    case PAWN:
      return "P";
    case EMPTY:
    default:
      return " ";
    }
  }
  else
  {
    switch (piece)
    {
    case KING:
      return "k";
    case QUEEN:
      return "q";
    case ROOK:
      return "r";
    case BISHOP:
      return "b";
    case KNIGHT:
      return "n";
    case PAWN:
      return "p";
    case EMPTY:
    default:
      return " ";
    }
  }
#else
    switch (piece)
    {
    case KING:
      return "♚";
    case QUEEN:
      return "♛";
    case ROOK:
      return "♜";
    case BISHOP:
      return "♝";
    case KNIGHT:
      return "♞";
    case PAWN:
      return "♟";
    case EMPTY:
    default:
      return " ";
    }
  }
  else
  {
    switch (piece)
    {
    case KING:
      return "♔";
    case QUEEN:
      return "♕";
    case ROOK:
      return "♖";
    case BISHOP:
      return "♗";
    case KNIGHT:
      return "♘";
    case PAWN:
      return "♙";
    case EMPTY:
    default:
      return " ";
    }
  }
#endif
}

template <std::size_t N, std::size_t M>
std::ostream &operator<<(std::ostream &os, Board<N, M> const &board)
{
  for (std::size_t i{0}; i < M; i++)
  {
    os << "|-----";
  }
  os << "|\n";
  for (std::size_t i{0}; i < N; i++)
  {
    for (std::size_t j{0}; j < M; j++)
    {
      TypePiece piece{board(i, j).get_type()};
      Color color{board(i, j).get_color()};
      os << "|  " << getUnicodeChar(piece, color) << "  ";
    }
    os << "|\n";
    for (std::size_t j{0}; j < M; j++)
    {
      if (board(i, j).get_type() != TypePiece::EMPTY)
      {
        double proba{board.get_proba(Coord(i, j))};
        int p{static_cast<int>(std::round(100. * proba))};
        if (p != 100)
        {
          os << "| ";
          if (p < 10){
            os << '0';
          }
          os << p << "% ";
        }
        else
        {
          os << "|     ";
        }
      }
      else
      {
        os << "|     ";
      }
    }
    os << "|\n";
    for (std::size_t j{0}; j < M; j++)
    {
      os << "|-----";
    }
    os << "|\n";
  }
  (void)board;
  return os;
}