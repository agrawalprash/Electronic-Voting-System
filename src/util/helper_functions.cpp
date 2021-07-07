#include "helper_functions.hpp"
#include <iostream>

namespace HelperFunctions
{
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
}

std::ostream& operator<<(std::ostream& os, const element_t g)
{
    element_printf("%B",g);
    return os << "";
}

std::ostream& operator<<(std::ostream& os, const mpz_t g)
{
    gmp_printf("%Zd",g);
    return os << "";
}