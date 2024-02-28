cmake_minimum_required (VERSION 3.20)

project (Quantum-Chess LANGUAGES CXX VERSION 1.0)

include(CTest)
enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
    src/main.cpp
    src/Board/Board.tpp
    src/Piece/Piece.tpp
    src/Qubit/Qubit.tpp
    src/check_path/check_path.tpp
)

set (HEADERS
    include/Unitary.hpp
    include/CMatrix.hpp
    include/Complex_printer.hpp
    include/Move.hpp
    src/Piece/TypePiece.hpp
    src/Piece/Piece.hpp
    src/Board/Board.hpp
    src/Qubit/Qubit.hpp
    src/check_path/check_path.hpp
)

#if (BUILD_TESTING)
#    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
#endif()
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

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_sanitizer()

if (MINGW)
    set (CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    if (${CMAKE_BUILD_TYPE} MATCHES "Debug" OR ${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif()
endif (MINGW)

add_executable (${PROJECT_NAME} ${SRCS} ${HEADERS})
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
# make postinst run after install                                                #

include(CPack)