import ntpath
import os

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
  "./src/ConsoleInterface/ConsoleInterface.hpp",
  "./src/ConsoleInterface/ConsoleInterface.tpp",
  
  #----------Computer Player-------------
  "./src/ComputerPlayer/ComputerPlayer.hpp",
  "./src/ComputerPlayer/ComputerPlayer.tpp",
  #--------------------------------------

  "./csv-to-graph.py",

  "./build/Matrix/include/Matrix.hpp",
  "./build/Matrix/include/Matrix.tpp",

  "./test/test_move_promotion.cpp",
  "./test/test_move_enpassant_with_capture.cpp",
  "./test/test_move_pawn_two_step.cpp",
  "./test/test_move_promotion_with_capture.cpp",
  "./test/test_move_promotion_with_split_before.cpp",
  "./test/test_move_split_with_mesure.cpp",
  "./test/test_split_in_non_empty_case.cpp",
  "./test/test_slide_merge_move_through_a_split_piece.cpp",
  "./test/test_get_proba_move_slide_capture.cpp",
  "./test/test_get_proba_move_slide_on_same_color.cpp",
  "./test/test_get_proba_move_slide_without_target.cpp",
  "./test/test_get_proba_move_jump_same_color.cpp",
  "./test/test_get_proba_move_jump_capture.cpp",
  "./test/test_capture_slide_move_true.cpp",
  "./test/test_capture_slide_move_false.cpp",
  "./test/test_move_merge_after_split.cpp",
  "./test/test_split_after_split.cpp",
  "./test/test_split_after_split2.cpp",
  "./test/test_multiple_split.cpp", 
  "./test/test_split_takes.cpp"
]

INDEX_LINES_PER_PAGE = 15
LINES_PER_PAGE = 30

output = open("code-latex-output.tex", 'w')

output.write("\n\\begin{frame}\n\\frametitle{{Index}}\n")
nb_line = 0
for path_file in list_file:
  file = ntpath.basename(path_file).replace('_', '-')
  label_name = ntpath.basename(path_file).replace('_', '')
  nb_line+=1
  if nb_line >= INDEX_LINES_PER_PAGE:
    output.write("\n\\end{frame}\n\\begin{frame}\n")
    nb_line = 0
  output.write(f"$\\bullet$ \\hyperlink{{{label_name}}}{{{file}}}\\newline\n")
output.write("\n\\end{frame}")

output.write("{\\tiny") # Pour afficher le code en petit


for path_file in list_file:
  file_name = ntpath.basename(path_file).replace('_', '-')
  label_name = ntpath.basename(path_file).replace('_', '')
  file_n, file_extension = os.path.splitext(path_file)

  if file_extension == '.py':
    lang = 'python'
  else:
    lang = 'cpp'

  output.write(f"\n\\begin{{frame}}[fragile]\n\\label{{{label_name}}}\n\\frametitle{{{file_name}}}\n")
  output.write("\\hyperlink{conclusion}{Retour au début}\\newline\n")
  output.write(f"\\begin{{minted}}[obeytabs=true,tabsize=2,linenos=true, breaklines]{{{lang}}}\n")

  file = open(path_file, 'r')
  i = 0
  nb_line = 0
  lines = file.readlines()
  maxline = LINES_PER_PAGE-4
  lines.pop()
  for line in lines:
    nb_line += 1
    if i >= maxline:
      maxline = LINES_PER_PAGE
      output.write("\\end{minted}\n\\end{frame}\n\n\\begin{frame}[fragile]\n")
      output.write("\\hyperlink{conclusion}{Retour au début}\\newline\n")
      output.write(f"\\begin{{minted}}[obeytabs=true,tabsize=2,linenos=true,breaklines,firstnumber={nb_line}]{{{lang}}}\n")
      i = 0
    output.write(line)
    i+=1
  file.close()
  output.write("\n\\end{minted}\n\\end{frame}\n")
output.write("}") # Fermant de la balise tiny