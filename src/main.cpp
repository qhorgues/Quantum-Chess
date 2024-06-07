#include <iostream>
#include <complex>
#include <Complex_printer.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <Qubit.hpp>
#include <Board.hpp>
#include <Piece.hpp>
#include <Move.hpp>
#include <observer_ptr.hpp>
#include <check_path.hpp>
#include <Constexpr.hpp>
#include <TypePiece.hpp>
#include <ConsoleInterface.hpp>

int main()
{
  Board<4> board{
      {B_KING, B_PAWN, Piece(), Piece()},
      {Piece(), Piece(), W_PAWN, Piece()},
      {Piece(), Piece(), Piece(), Piece()},
      {Piece(), Piece(), Piece(), W_KING}};

  Board<> ChessBoard{
      {B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
      {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
      {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
      {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
      {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
      {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
      {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
      {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}};
  /* Move m1 = Move_split(Coord(3, 0), Coord(2, 1), Coord(3, 2));
   Move m2 = Move_split(Coord(3, 2), Coord(1, 0), Coord(2, 1));
   board.move(m1);
   board.move(m2, true);
   bool res{double_equal(board.get_proba(Coord(3,3)), 1.)};
   std::cout<<res<<std::endl;*/

  Board<6, 4> smallBoard{
      {{B_KNIGHT, B_QUEEN, B_KING, B_BISHOP},
       {B_PAWN, B_PAWN, B_PAWN, B_PAWN},
       {Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece()},
       {W_PAWN, W_PAWN, W_PAWN, W_PAWN},
       {W_KNIGHT, W_QUEEN, W_KING, W_BISHOP}}};

  std::cout << ChessBoard << std::endl;

  return 0;
}