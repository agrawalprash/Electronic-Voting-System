#pragma once
#include <iostream>
#include <fstream>
#include "pair.hpp"

namespace Commitment
{
    void commit(element_t Commitment, element_t g, element_t Message, element_t h, element_t random);

    void commit(element_t Commitment, element_t Message, element_t random, Pairing *pairing);

    void commit(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random);

    void commit(element_t Commitment, mpz_t msg, mpz_t r, Pairing* pairing);

    void commit(element_t Commitment, string msg, mpz_t r, Pairing* pairing);

    void commit(element_t Commitment, string msg, string randomness, Pairing* pairing);

    bool open(element_t Commitment, element_t g, element_t Message, element_t h, element_t random);

    bool open(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random);
}

// Class for Commitment, Setup, commit, open
// virtual functions <- modularize
// UML diagrams
// Class diagrams
// activity, behaviour diagrams
// How these diagrams can be used to stitch together
