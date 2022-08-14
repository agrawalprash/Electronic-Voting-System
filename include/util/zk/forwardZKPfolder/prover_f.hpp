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

class prover_f {
    element_t g;
    element_t h;
    element_t r;
    element_t p;
    element_t comm;
    element_t *phi;
    int numOfCandidates;

    element_t s,t,m,v;

    public:
        prover_f (element_t g_p, element_t h_p, element_t r_p, element_t comm_p , element_t p_p, int n);

        void provers_message_D(element_t D);

        void provers_blind_signature(element_t blindsig, element_t sig);

        void provers_message_a(element_t a, element_t sig);

        int helper_index(element_t *phi, element_t el, int n); 

        void provers_obtain_signature(element_t Ap, element_t* signatures);

        void provers_response_1(element_t z, element_t c);

        void provers_response_2(element_t z, element_t c);

        void provers_response_3(element_t z, element_t c);
    

};