cmake_minimum_required (VERSION 3.20)

project (Quantum-Chess LANGUAGES CXX VERSION 1.0)

include(CTest)
enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
    src/main.cpp
    src/GameBoard/GameBoard.tpp
    src/Piece/Piece.cpp
    src/Qubit/Qubit.tpp
    src/Interface_SDL2pp/Interface_SDL2pp.cpp
    
)

set (HEADERS
    include/Unitary.hpp
    include/CMatrix.hpp
    include/Complex_printer.hpp
    include/Interface.hpp
    include/observer_ptr.hpp

    src/GameBoard/GameBoard.hpp
    src/Piece/Piece.hpp
    src/Qubit/Qubit.hpp
    src/Interface_SDL2pp/Interface_SDL2pp.hpp
)

set (ASSETS
    assets/ChessBoard.png
    assets/Piece.png
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

find_package(SDL2pp REQUIRED)

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
target_link_libraries(${PROJECT_NAME} Matrix m SDL2pp::SDL2pp)
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${HEADERS})
set_target_warnings(${PROJECT_NAME})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(TARGETS ${PROJECT_NAME} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
install(DIRECTORY assets/ DESTINATION assets)

foreach(df ${ASSETS})
    file(COPY ${df} DESTINATION ${CMAKE_BINARY_DIR}/assets)
endforeach()


include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2PP_INCLUDE_DIRS})

# Win32 SDL2 dll
if (WIN32)
	# SDL2
    set_option(SDL2_DLL_DIR "SDL2_BINARY_DIR NOT FOUND" PATH "Path to SDL2.dll directory")
    configure_file(${SDL2_DLL_DIR}/SDL2.dll SDL2.dll COPYONLY)
	
	# SDL2 ttf
	set_option(SDL2_TTF_DLL_DIR "SDL2_TTF_BINARY_DIR NOT FOUND" PATH "Path to SDL2_ttf.dll directory")
    configure_file(${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll SDL2_ttf.dll COPYONLY)

    set_option(SDL2_IMAGE_DLL_DIR "SDL2_IMAGE_BINARY_DIR NOT FOUND" PATH "Path to SDL2_image.dll directory")
    configure_file(${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll SDL2_image.dll COPYONLY)

    set_option(SDL2_MIXER_DLL_DIR "SDL2_MIXER_BINARY_DIR NOT FOUND" PATH "Path to SDL2_mixer.dll directory")
    configure_file(${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll SDL2_mixer.dll COPYONLY)

    foreach(df ${ASSETS})
        configure_file(${CMAKE_SOURCE_DIR}/${df} ${CMAKE_BINARY_DIR}/${df} COPYONLY)
    endforeach()
    

    install(FILES ${SDL2_DLL_DIR}/SDL2.dll DESTINATION .)
    install(FILES ${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll DESTINATION .)
    install(FILES ${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll DESTINATION .)
    install(FILES ${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll DESTINATION .)

endif(WIN32)

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