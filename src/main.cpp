#include <iostream>
#include <fstream>
#include <complex>
#include <string>
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
#include <ComputerPlayer.hpp>
#include <ConsoleInterface.hpp>

char piece_to_char(TypePiece piece, Color color = Color::WHITE)
{
  int const offset{(color == Color::BLACK) ? 32 : 0};
  using enum TypePiece;
  switch (piece)
  {
  case KING:
    return static_cast<char>('K' + offset);
  case QUEEN:
    return static_cast<char>('Q' + offset);
  case ROOK:
    return static_cast<char>('R' + offset);
  case BISHOP:
    return static_cast<char>('B' + offset);
  case KNIGHT:
    return static_cast<char>('N' + offset);
  case PAWN:
    return static_cast<char>('P' + offset);
  case EMPTY:
    return ' ';
  default:
    return '\0';
  }
}

template <std::size_t N, std::size_t M>
char print_piece(Board<N, M> &board, Coord const &position)
{
  TypePiece piece{board(position.n, position.m).get_type()};
  Color color{board(position.n, position.m).get_color()};
  return piece_to_char(piece, color);
}

template <std::size_t N, std::size_t M>
void auto_playing(
    Board<N, M> &board,
    std::ostream &output,
    std::ostream &csv,
    int nb_moves,
    int search_depth)
{
  std::cout << board << std::endl;
  csv << "Heuristique; Type-Move" << std::endl;
  //print_board_light(output, board);
  //output << "\n";
  while (
      nb_moves > 0 &&
      !board.winning_position(opponent_color(board.get_current_player())))
  {

    Move m{computer::get_best_move(board, search_depth)};

    std::string data1{{static_cast<char>('a' + m.normal.src.m),
                       static_cast<char>('0' + N - m.normal.src.n)}};

    std::string data2{{static_cast<char>('a' + m.normal.arv.m),
                       static_cast<char>('0' + N - m.normal.arv.n)}};

    output << print_piece(board, m.normal.src) << ' ';
    if (m.type == TypeMove::NORMAL)
    {
      output << "N " << data1 << data2 << std::endl;
    }
    else if (m.type == TypeMove::PROMOTE)
    {
      output << 'P' << piece_to_char(m.promote.piece) << ' '
             << data1 << data2
             << std::endl;
    }
    else
    {
      std::string data3{{static_cast<char>('a' + m.split.arv2.m),
                         static_cast<char>('0' + N - m.split.arv2.n)}};

      if (m.type == TypeMove::SPLIT)
      {
        output << "S " << data1 << '(' << data2 << ':'
               << data3 << ')' << std::endl;
      }
      else
      {
        output << "M " << '(' << data1 << ':' << data2 << ')'
               << data3 << std::endl;
      }
    }
    board.move(m);
    std::string Tmove;
    switch (m.type)
    {
    case TypeMove::NORMAL:
      Tmove = "Normal";
      break;
    case TypeMove::SPLIT:
      Tmove = "Split";
      break;
    case TypeMove::MERGE:
      Tmove = "Merge";
      break;
    case TypeMove::PROMOTE:
      Tmove = "Promote";
      break;
    default:
      return;
    }
    csv << computer::__utility::heuristic(board) << ';' << Tmove << std::endl;
    /*
    #if defined(WIN32)
        int ret = system("cls");
    #else
        int ret = system("clear");
    #endif
        (void)ret;*/
    std::cout << board << std::endl;
    board.change_player();
    nb_moves--;
  }
}

int main()
{

  Board<> const ChessBoard{
      {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
       {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
       {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
       {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
       {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}}};

  /* Board<3> B1{
      {B_KING, B_ROOK, Piece()},
      {B_PAWN, Piece(), Piece()},
      {Piece(), W_QUEEN, W_KING}};

  Board<3> B2{
      {
          {B_KING, Piece(), B_QUEEN},
          {W_PAWN,  B_PAWN, W_QUEEN},
          {Piece(), Piece(), W_KING}
      }
  };

  Board<3> B3{
      {
          {B_KING, Piece(), Piece()},
          {Piece(), W_ROOK, Piece()},
          {Piece(), Piece(), W_KING}
      }
  };
  */

  Board<6, 4> smallBoard{
      {{B_KNIGHT, B_QUEEN, B_KING, B_BISHOP},
       {B_PAWN, B_PAWN, B_PAWN, B_PAWN},
       {Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece()},
       {W_PAWN, W_PAWN, W_PAWN, W_PAWN},
       {W_KNIGHT, W_QUEEN, W_KING, W_BISHOP}}};

  Board<4> miniBoard{
      {{B_KING, Piece(), Piece(), B_BISHOP},
       {Piece(), B_KNIGHT, Piece(), Piece()},
       {Piece(), Piece(), Piece(), Piece()},
       {Piece(), Piece(), W_QUEEN, W_KING}}};

  for (int i = 2; i <= 2; i++)
  {
    Board board{smallBoard};
    std::ofstream log_file{"partie" + std::to_string(i) + ".txt"};
    std::ofstream csv{"partie" + std::to_string(i) + ".csv"};
    auto_playing(board, log_file, csv, 80, 6);
    log_file.close();
  }

  return 0;
}
