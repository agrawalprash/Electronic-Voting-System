cmake_minimum_required(VERSION 3.14) # version can be different

project(e-voting_project) #name of your project

add_subdirectory(
    googletest
) # add googletest subdirectory

include_directories(
    googletest/include
    /usr/local/include/pbc
    ../include/util
    ../include/util/pairing
    ../include/util/bilinear-mapping
    ../include/util/commitment
    ../include/protocol
    ../include/protocol/ea
    ../include/protocol/ea/ballot
    ../include/protocol/evm
    ../include/protocol/po
    ../include/protocol/verif
) # this is so we can #include <gtest/gtest.h>

link_directories(~/.local/lib)


add_executable(
    test
    ../src/util/pairing/pairing.cpp
    ../src/util/helper_functions.cpp
    ../src/util/commitment/commitment.cpp
    ../src/util/bilinear-mapping/bilinear_mapping.cpp    
    ../src/protocol/evm/evm.cpp    
    ../src/protocol/ea/ballot/ballot.cpp
    ../src/util/common.cpp
    ../src/protocol/evm/receipt.cpp
    ../src/protocol/verif/generate_ballot.cpp
    test.cpp
) # add this executable

target_link_libraries(
    test PRIVATE gtest pbc gmp
) # link google test to this executable

# https://stackoverflow.com/questions/28597351/how-do-i-add-a-library-path-in-cmake
