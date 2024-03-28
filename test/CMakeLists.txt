enable_testing()

create_test_sourcelist (Tests
    QuantumChessTest.cxx
    test_move_promotion.cpp
    test_move_enpassant_with_capture.cpp
    test_move_pawn_two_step.cpp
    test_move_promotion_with_capture.cpp
    test_move_promotion_with_split_before.cpp
    test_move_split_with_mesure.cpp
    test_split_in_non_empty_case.cpp
    test_slide_merge_move_through_a_split_piece.cpp
    test_get_proba_move_slide_capture.cpp
    test_get_proba_move_slide_on_same_color.cpp
    test_get_proba_move_slide_without_target.cpp
)

# add the executable
add_executable (QuantumChessTest ${Tests} ${SRCS})
#${CMAKE_SOURCE_DIR}/src/math_utility/math_utility.cpp ${CMAKE_SOURCE_DIR}/src/Coord/Coord.cpp)
add_dependencies(QuantumChessTest Quantum-Chess)

# remove the test driver source file
set (TestsToRun ${Tests})
remove (TestsToRun QuantumChessTest.cxx)

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
    get_filename_component (TName ${test} NAME_WE)
    add_test (NAME ${TName} COMMAND QuantumChessTest ${TName})
endforeach ()

include(CTest)