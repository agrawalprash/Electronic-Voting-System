#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "zk/forwardZKPfolder/prover_f.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

class verifier_f {

    public:
        element_t g;
        element_t h;
        element_t comm;
        element_t *phi;
        prover_f *prover;
        int numOfCandidates;

    public:
        verifier_f (element_t g_p, element_t h_p, element_t comm_p , int n, prover_f *prover_p);

        void verifiers_generate_signature(element_t* signatures, element_t x);

        void verifiers_challenge(element_t t);

        void helper_message_D(element_t D, element_t zp, element_t zr);

        void verifiers_BilinearRHS(element_t t, element_t sig, element_t g, element_t x, element_t c, element_t zp, element_t zv, element_t y);

        bool helper_comparison(element_t a, element_t b);

        bool verifiers_check(element_t LHS1, element_t RHS1, element_t LHS2, element_t RHS2);

        //void generate_publickey(element_t y, element_t g);
        bool implementForwardZKP();

};