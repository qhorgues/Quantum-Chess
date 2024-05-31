
list_file = [
  "./src/main.cpp",
  "./src/Board/Board.hpp",
  "./src/Board/Board.tpp",
  "./src/Board/mesure.tpp",
  "./src/Board/move.tpp",
  "./src/Board/get_proba_move.tpp",
  "./src/Piece/Piece.hpp",
  "./src/Piece/Piece.tpp",
  "./src/Piece/get_list_move.tpp",
  "./src/Piece/TypePiece.hpp",
  "./src/Coord/Coord.hpp",
  "./src/Coord/Coord.cpp",
  "./src/Move/Move.hpp",
  "./src/Move/Move.cpp",
  "./src/Qubit/Qubit.hpp",
  "./src/Qubit/Qubit.tpp",
  "./src/Random/Random.hpp",
  "./src/Random/Random.cpp",
  "./src/math_utility/math_utility.hpp",
  "./src/math_utility/math_utility.tpp",
  "./src/check_path/check_path.hpp",
  "./src/check_path/check_path.tpp",
  "./include/Unitary.hpp",
  "./include/CMatrix.hpp",
  "./include/Complex_printer.hpp",
  "./include/Constexpr.hpp",
  "./src/ComputerPlayer/ComputerPlayer.hpp",
  "./src/ComputerPlayer/ComputerPlayer.tpp",
  "./src/ConsoleInterface/ConsoleInterface.hpp",
  "./src/ConsoleInterface/ConsoleInterface.tpp"
]

output = open("code-latex-output.txt", 'w')

for path_file in list_file:
  output.write("\n\\begin{frame}[fragile]\n\\begin{minted}{cpp}\n")

  file = open(path_file, 'r')
  i = 0
  lines = file.readlines()
  for line in lines:
    if i == 30:
      output.write("\n\\end{minted}\n\\end{frame}\n\n\\begin{frame}[fragile]\n\\begin{minted}{cpp}\n")
      i = 0
    output.write(line)
    i+=1
  file.close()
  output.write("\n\\end{minted}\n\\end{frame}\n")
  