#pragma once

#include "pbc.h"
#include <string>
#include <iostream>

using namespace std;

class Pairing
{
    public:
        pairing_t pairing;
        element_t gen_G1, gen_G2, gen_GT, gen_Zr;

    public:

    // Initialization

        // Initializing using param file
        Pairing(FILE* paramFile);

        // type a1 (User can select prime p,q)
        Pairing(string type, mpz_t p, mpz_t q); 
        Pairing(string type, mpz_t p, unsigned long q); 
        Pairing(string type, unsigned long p, mpz_t q); 

        // type a, a1, e
        // type a, e -> p,q are bits of random prime to be generated
        // type a and type e differ in the curves they function on
        // type a1 -> p,q are prime
        Pairing(string type, unsigned long rbits, unsigned long qbits);

        // type d,f (Not included yet) <Good choice when elements are small>
        Pairing(string type, mpz_t Discriminant);
        Pairing(string type, unsigned long Discriminant); // Discriminant must be 3 mod 4
    
    // deconstructor
        ~Pairing()
        {
            pairing_clear(pairing);
        }

    // Group Generator
        // void generator   (element_t t);
        void generator_G1(element_t t);
        void generator_G2(element_t t);
        void generator_GT(element_t t);
        void generator_Zr(element_t t);
    
    // Random Number Generator
        // void random   (element_t t);
        void random_G1(element_t t);
        void random_G2(element_t t);
        void random_GT(element_t t);
        void random_Zr(element_t t);
    
    // operation
        void add(element_t t, element_t a, element_t b);
        void sub(element_t t, element_t a, element_t b);
        void mul(element_t t, element_t a, element_t b);
        void div(element_t t, element_t a, element_t b);
        void exp(element_t t, element_t a, element_t x);
        void negate(element_t t, element_t a);

        void identity(element_t t);
        void inverse(element_t t, element_t a);
        void order();

        void mul(element_t t, element_t a, mpz_t b_);
        void mul(element_t t, element_t a, long long b);
        void exp(element_t t, element_t a, mpz_t x);
        void exp(element_t t, element_t a, unsigned long x);    
};

// namespaces
// virtual functions
// friend functions