#include "helper_functions.hpp"

void RandomNumberGenerator(mpz_t t, int bits)
{
    mpz_init(t);
    pbc_mpz_randomb(t, bits);
}

void PrimeGenerator(mpz_t t, int bits)
{
    mpz_init(t);
    pbc_mpz_randomb(t, bits);
    mpz_nextprime(t, t);
}
