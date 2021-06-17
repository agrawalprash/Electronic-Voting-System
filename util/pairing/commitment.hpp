#pragma once
#include <iostream>
#include <fstream>
#include "pair.hpp"

void commit(element_t Commitment, element_t g, element_t Message, element_t h, element_t random);

void commit(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random);

void commit(element_t Commitment, mpz_t msg, mpz_t r, Pairing* pairing);

void commit(element_t Commitment, string msg, mpz_t r, Pairing* pairing);

void commit(element_t Commitment, string msg, string randomness, Pairing* pairing);

bool open(element_t Commitment, element_t g, element_t Message, element_t h, element_t random);

bool open(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random);

void BilinearMapping(element_t t, element_t g1, element_t g2, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, element_t b, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, mpz_t b, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, mpz_t b, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, element_t b, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, unsigned long b_, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, element_t b_, Pairing* pairing);

void BilinearMapping(element_t t, element_t g1, element_t a_, element_t g2, unsigned long b_, Pairing* pairing);

void RandomNumberGenerator(mpz_t t, int bits);

void PrimeGenerator(mpz_t t, int bits);

// Class for Commitment, Setup, commit, open
// virtual functions <- modularize
// UML diagrams
// Class diagrams
// activity, behaviour diagrams
// How these diagrams can be used to stitch together