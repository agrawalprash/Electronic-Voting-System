#include <iostream>
#include <cstring>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "hash/sha256.h"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

class prover_r {

    public:
        element_t g;
        element_t h;
        element_t p;
        element_t C;
        element_t rval;
        element_t *r;
        element_t *commitments;
        element_t *pvals;
        int numOfCandidates;
        int numOfCommitments;


    public:
        prover_r(element_t g, element_t h, element_t p, element_t c, element_t rval, element_t *r, element_t *commitments, element_t *pvals, int n, int m);
        void pok_piecom_prover(element_t challenge, element_t z1, element_t z2,  int i);
        void prover_gives_r_double_dash_array(element_t B1,element_t B2, element_t challenge, element_t z1,element_t z2,element_t z3,element_t z4,element_t z5,element_t z6,element_t  B1_c,element_t  LHS_piesig_val,element_t *A_array, element_t *c_array, element_t *r_dash_array, element_t y);
        int find_index();
        void compute_B1(element_t b1, element_t s1, element_t s2);
        void compute_B2(element_t b2, element_t Ai ,element_t s2);
        void compute_LHS1(element_t t, element_t b1, element_t c);
        void compute_RHS1(element_t t, element_t delta1, element_t delta2);
        bool helper_comparison(element_t a, element_t b);
        void compute_LHS_pieSig(element_t t, element_t B2, element_t y) ;
        void compute_RHS_pieSig(element_t t, element_t B2, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r);
        void pok2(element_t challenge, element_t z1, element_t z2,element_t z3, element_t z4, element_t z5, element_t z6,  element_t s1, element_t s2, element_t c_val, element_t r_val, element_t delta1, element_t delta2, element_t b1, element_t b2, element_t y, element_t B1_c, element_t LHS_piesig_val) ;

};