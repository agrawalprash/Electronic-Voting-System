#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "prover_r.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

class verifier_r {

    public:
        element_t g;
        element_t h;
        element_t p;
        element_t *commitments;
        prover_r *prover;
        int numOfCandidates;
        int numOfCommitments;

    public:
        verifier_r(element_t g_p, element_t h_p, element_t p_p, element_t *commitments_p, int n, int m, prover_r *prover_p);
        bool pok1_pie_com_verifier();
        bool implementReverseZKP();
        void compute_RHS1(element_t t, element_t delta1, element_t delta2);
        void compute_RHS_pieSig(element_t t, element_t B2, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r);


};