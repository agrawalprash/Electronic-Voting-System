cmake_minimum_required(VERSION 3.14) # version can be different

project(e-voting_project) #name of your project

add_subdirectory(googletest) # add googletest subdirectory

include_directories(
    googletest/include
    /usr/local/include/pbc
) # this is so we can #include <gtest/gtest.h>

link_directories(~/.local/lib)

add_executable(
    test 
    test.cpp
    ../pairing/pair.cpp
    ../pairing/helper_functions.cpp
    ../commitment/commitment.cpp
    ../bilinear-mapping/bilinear_mapping.cpp
    ../evm/evm.cpp
    ../ballot-generation/ballot.cpp
    ../common/common.cpp
    ../protocol/protocol.cpp
) # add this executable

target_link_libraries(
    test PRIVATE gtest pbc gmp
) # link google test to this executable

# https://stackoverflow.com/questions/28597351/how-do-i-add-a-library-path-in-cmake
