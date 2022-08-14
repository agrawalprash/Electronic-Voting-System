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

namespace ZKPSetMembership{

    void provers_message_D(element_t D, element_t g, element_t s, element_t h, element_t m, Pairing* pairing);

    void provers_message_a(element_t a, element_t sig, element_t g, element_t s, element_t t, Pairing* pairing);

    void helper_message_D(element_t D, element_t g, element_t s, element_t h, element_t m, Pairing* pairing);

    int helper_index(element_t *phi, element_t el, int n);

    void verifiers_generate_signature(element_t* signatures, element_t x, element_t g, element_t *phi, Pairing pairing, int n);

    void provers_obtain_signature(element_t Ap, element_t* signatures, element_t p, element_t *phi, Pairing pairing, int n);

    void verifiers_blind_signature(element_t blindsig, element_t sig, element_t v, Pairing* pairing);

    void verifiers_challenge(element_t t, Pairing* prg);

    void provers_response(element_t z, element_t s, element_t p,element_t c, Pairing* pairing);

    void verifiers_BilinearRHS(element_t t, element_t sig, element_t g, element_t x, element_t c, element_t zp, element_t zv, element_t y, Pairing* pairing);

    bool helper_comparison(element_t a, element_t b);

    bool verifiers_check(element_t LHS1, element_t RHS1, element_t LHS2, element_t RHS2);

    bool testing_verification(element_t p, element_t r, Pairing* pairing, element_t comm, element_t g, element_t h, element_t *phi, int n);

}