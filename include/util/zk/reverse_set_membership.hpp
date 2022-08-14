#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

namespace reverseZKPSetMembership {

    bool helper_comparison(element_t a, element_t b);

    void compute_B1(element_t b1, element_t g , element_t h , element_t s1, element_t s2);

    void compute_B2(element_t b2, element_t Ai , element_t g , element_t s2);

    void compute_LHS_pieSig(element_t t, element_t B2, element_t g, element_t y, Pairing* pg);
    
    void compute_RHS_pieSig(element_t t, element_t B2, element_t g, element_t h, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r, Pairing* pg) ;
 
    void compute_LHS1(element_t t, element_t b1, element_t c);

    void compute_RHS1(element_t t, element_t g, element_t h, element_t delta1, element_t delta2);

    bool testing_verification(element_t p, element_t r, Pairing* pairing, element_t comm, element_t g, element_t h, element_t *Commitments, int numOfCandidates, int numOfCommitments, int ind);

}