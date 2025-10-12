# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/quentin/Programmes/C++/Quantum-Chess/build/Matrix")
  file(MAKE_DIRECTORY "/home/quentin/Programmes/C++/Quantum-Chess/build/Matrix")
endif()
file(MAKE_DIRECTORY
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-build"
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix"
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/tmp"
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/src/matrix-populate-stamp"
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/src"
  "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/src/matrix-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/src/matrix-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/quentin/Programmes/C++/Quantum-Chess/build/_deps/matrix-subbuild/matrix-populate-prefix/src/matrix-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
