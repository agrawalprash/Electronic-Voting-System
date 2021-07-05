#ifndef _COMMON_H_
#define _COMMON_H

#include <vector>
#include <string>
#include "pairing/pairing.hpp"
#include "helper_functions.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"

extern Pairing* pg;
extern std::vector<std::string> Candidate_List;

extern void initialize();

namespace PairingGeneration
{
    void initialize();
}

// ================================= defines  ===============================
#define __DEBUG__
// #define __UNIT_TESTING__
// #define __INTERACTIVE__
#define PRINT_PROCEDURE     true
#define VVPR_ONLY           false
#define REMAINDER_FOR_PRINT (_VOTERS_/5)
// ================================= defines  ===============================


// =================================== a  ===================================
// type a (PBC)
#define RBITS      126
#define QBITS      512
#define ITERATIONS 100
// =================================== a  ===================================

// =================================== a1 ===================================
// type a1 (PBC)
#define P_BITS 256
#define Q_BITS 256
// =================================== a1 ===================================


// =================================== a1 ===================================
#define _VOTERS_    10
#define _CANDIDATES_ Candidate_List.size()
// =================================== a1 ===================================


#endif
