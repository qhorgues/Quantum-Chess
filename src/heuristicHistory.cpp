#include <iostream>
#include <fstream>
#include <Board.hpp>
#include <Move.hpp>

#include <Piece.hpp>
#include <string>
#include <observer_ptr.hpp>
#include <ConsoleInterface.hpp>


template<std::size_t N>
Move readMove(std::istream& is, observer_ptr<Piece> piece = nullptr)
{
  (void)piece;
  char type_piece;
  char type_move;
  char promote_piece;
  std::stringstream str_move;

  Move move;
  is >> type_piece >> type_move;
  move.type = chr_to_TypeMove(type_move);
  if (move.type == TypeMove::PROMOTE)
  {
    is >> promote_piece;
    move.promote.piece = chr_to_Piece(promote_piece).get_type();
  }

  char d1, d2, d3, d4, d5, d6;
  switch (move.type)
  {
  case TypeMove::NORMAL:
    is >> d1 >> d2 >> d3 >> d4;
    move.normal.src.m = d1 - 'a';
    move.normal.src.n = '0' + N - d2;
    move.normal.arv.m = d3 - 'a';
    move.normal.arv.n = '0' + N - d4;
    break;
  case TypeMove::SPLIT:
    is >> d1 >> d2 >> d3 >> d4 >> d5 >> d6;
    move.split.src.m = d1 - 'a';
    move.split.src.n = '0' + N - d2;
    move.split.arv1.m = d3 - 'a';
    move.split.arv1.n = '0' + N - d4;
    move.split.arv2.m = d5 - 'a';
    move.split.arv2.n = '0' + N - d6;
    break;
  case TypeMove::MERGE:
    is >> d1 >> d2  >> d3 >> d4 >>  d5 >> d6;
    move.merge.src1.m = d1 - 'a';
    move.merge.src1.n = '0' + N - d2;
    move.merge.src2.m = d3 - 'a';
    move.merge.src2.n = '0' + N - d4;
    move.merge.arv.m = d5 - 'a';
    move.merge.arv.n = '0' + N - d6;
    break;
  case TypeMove::PROMOTE:
    is >> d1 >> d2 >> d3 >> d4;
    move.promote.src.m = d1 - 'a';
    move.promote.src.n = '0' + N - d2;
    move.promote.arv.m = d3 - 'a';
    move.promote.arv.n = '0' + N - d4;
    break;
  default:
    break;
  }

  if (piece != nullptr)
  {
    *piece = chr_to_Piece(type_piece);
  }

  return move;
}

int main(int argc, char** argv)
{
  if (argc <= 1)

  { 
    std::cout << "Invalid argument" << std::endl;
    return EXIT_FAILURE;
  }
  std::string path { argv[1] };
  std::ifstream gameHistory {path};
  if (!gameHistory.is_open())
  {
    std::cout << "failed to open " << path << '\n';
    return EXIT_FAILURE;
  }

  Piece piece;
  readMove<6>(gameHistory, make_observer(&piece));

  gameHistory.close();
  return EXIT_SUCCESS;
}