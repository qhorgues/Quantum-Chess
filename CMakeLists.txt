cmake_minimum_required (VERSION 3.20)

project (Quantum-Chess LANGUAGES CXX VERSION 0.5)

enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)


set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
    ${CMAKE_SOURCE_DIR}/src/math_utility/math_utility.cpp
    ${CMAKE_SOURCE_DIR}/src/Coord/Coord.cpp
    ${CMAKE_SOURCE_DIR}/src/Board/Board.tpp
    ${CMAKE_SOURCE_DIR}/src/Piece/Piece.tpp
    ${CMAKE_SOURCE_DIR}/src/Qubit/Qubit.tpp
    ${CMAKE_SOURCE_DIR}/src/check_path/check_path.tpp
)

set (HEADERS
    ${CMAKE_SOURCE_DIR}/include/Unitary.hpp
    ${CMAKE_SOURCE_DIR}/include/CMatrix.hpp
    ${CMAKE_SOURCE_DIR}/include/Complex_printer.hpp
    ${CMAKE_SOURCE_DIR}/include/Move.hpp
    ${CMAKE_SOURCE_DIR}/include/Constexpr.hpp
    ${CMAKE_SOURCE_DIR}/src/Coord/Coord.hpp
    ${CMAKE_SOURCE_DIR}/src/Piece/TypePiece.hpp
    ${CMAKE_SOURCE_DIR}/src/Piece/Piece.hpp
    ${CMAKE_SOURCE_DIR}/src/Board/Board.hpp
    ${CMAKE_SOURCE_DIR}/src/Qubit/Qubit.hpp
    ${CMAKE_SOURCE_DIR}/src/check_path/check_path.hpp
    ${CMAKE_SOURCE_DIR}/src/math_utility/math_utility.hpp
)

include(FetchContent)

FetchContent_Declare(Matrix
    GIT_REPOSITORY "https://github.com/qhorgues/Matrix.git"
    GIT_TAG "origin/master"
    SOURCE_DIR "Matrix"
)

FetchContent_MakeAvailable(Matrix)

include_directories(${CMAKE_SOURCE_DIR}/include)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_list)
include_directories(${header_list})

option(BUILD_DOC "Build documentation" ON)
if (BUILD_DOC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/docs)
endif()

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

set_sanitizer()

if (MINGW)
    set (CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    # if (${CMAKE_BUILD_TYPE} MATCHES "Debug" OR ${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
    #     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    # endif()
endif (MINGW)

add_executable (${PROJECT_NAME} ${SRCS} src/main.cpp ${HEADERS})
target_link_libraries(${PROJECT_NAME} Matrix m)
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${HEADERS})
set_target_warnings(${PROJECT_NAME})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} ARCHIVE)
install(TARGETS ${PROJECT_NAME} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

SET(CPACK_BINARY_DEB TRUE)
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SET_DESTDIR TRUE)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "quentin.horgues@outllok.fr")

SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
SET(CPACK_PACKAGE_VENDOR "Horgues")
# make postinst run after install

include(CPack)