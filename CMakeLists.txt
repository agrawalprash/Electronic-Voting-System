cmake_minimum_required(VERSION 3.14) # version can be different

project(e-voting_project) #name of your project

add_subdirectory(
    # build/googletest
    pybind11
) # add googletest subdirectory

include_directories(
    googletest/include
    /usr/local/include/pbc
    include/pybind11/include/pybind11
    include/util
    include/util/pairing
    include/util/bilinear-mapping
    include/util/commitment
    include/util/encryption
    include/util/signature
    include/util/hash
    include/util/zk
    # include/QR_Code_generator
    # include/util/io/qr
    include/protocol
    include/protocol/ea
    include/protocol/ea/ballot
    include/protocol/evm
    include/protocol/po
    include/protocol/verif
    # include/pypbc/
    
) # this is so we can #include <gtest/gtest.h>


pybind11_add_module(
    voting SHARED 
    # src/QR_Code_generator/QrCode.cpp
    # src/util/io/qr/qrcode.cpp
    main.cpp 
    src/util/pairing/pairing.cpp 
    src/util/helper_functions.cpp 
    src/util/commitment/commitment.cpp 
    src/util/bilinear-mapping/bilinear_mapping.cpp 
    src/util/signature/bls.cpp 
    src/util/hash/sha256.cpp
    src/util/zk/set_membership_zkp.cpp
    src/util/zk/reverse_set_membership.cpp
    src/util/zk/forwardZKPfolder/prover_f.cpp
    src/util/zk/forwardZKPfolder/verifier_f.cpp
    src/util/zk/reverseZKPfolder/prover_r.cpp
    src/util/zk/reverseZKPfolder/verifier_r.cpp
    src/protocol/evm/evm.cpp 
    src/protocol/ea/ballot/ballot.cpp 
    src/util/common.cpp 
    src/protocol/evm/receipt.cpp 
    src/protocol/verif/generate_ballot.cpp 
    # pypbc/pypbc.c
    src/util/encryption/encryption.cpp
)

target_link_libraries(voting PRIVATE pbc gmp python3.8)

# MACRO(SUBDIRLIST result firstdir curdir)
#     file(GLOB ENDF6_SRC_TOP RELATIVE
#         ${curdir} ${curdir}/*)
#     file(GLOB_RECURSE ENDF6_SRC_NESTED ${curdir}/*)
#     set(children ${ENDF6_SRC_TOP} ${ENDF6_SRC_NESTED})

#     SET(dirlist "${firstdir}")
#     FOREACH(child ${children})
#         IF(IS_DIRECTORY ${curdir}/${child})
#             LIST(APPEND dirlist ${curdir}/${child})
#         ENDIF()
#     ENDFOREACH()
#     SET(${result} ${dirlist})
# ENDMACRO()

# SUBDIRLIST(result . .)
# message("Value: ${result}")


# link_directories(~/.local/lib)


# add_executable(
#     test
#     main.cpp
#     # testing/test.cpp
#     src/util/pairing/pairing.cpp
#     src/util/helper_functions.cpp
#     src/util/commitment/commitment.cpp
#     src/util/bilinear-mapping/bilinear_mapping.cpp
#     src/util/signature/bls.cpp
#     src/util/hash/sha256.cpp
#     src/protocol/evm/evm.cpp    
#     src/protocol/ea/ballot/ballot.cpp
#     src/util/common.cpp
#     src/protocol/evm/receipt.cpp
#     src/protocol/verif/generate_ballot.cpp    
#     src/util/encryption/encryption.cpp
# ) # add this executable

# pybind11_add_module(
#     protocol 
    
# )

# target_link_libraries(
#     test PRIVATE gtest pbc gmp
# ) # link google test to this executable

# # https://stackoverflow.com/questions/28597351/how-do-i-add-a-library-path-in-cmake
