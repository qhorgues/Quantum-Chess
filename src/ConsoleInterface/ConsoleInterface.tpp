#include <iostream>
#include <Board.hpp>

const char *getUnicodeChar(TypePiece piece, Color color)
{
  using enum TypePiece;
  if (color == Color::WHITE)
  {
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
        int p{static_cast<int>(100. * proba)};
        if (p != 100)
        {
          os << "| " << p << "% ";
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