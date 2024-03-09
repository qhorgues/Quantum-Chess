enable_testing()

create_test_sourcelist (Tests
    QuantumChessTest.cxx
    test_move_promotion.cpp
    test_move_enpassant_with_capture.cpp
    test_move_pawn_two_step.cpp
    test_move_promotion_with_capture.cpp
    test_move_promotion_with_split_before.cpp
    test_move_split_with_mesure.cpp
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