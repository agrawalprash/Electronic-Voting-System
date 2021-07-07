#pragma once

#include "pbc.h"

namespace HelperFunctions
{
    void RandomNumberGenerator(mpz_t t, int bits);

    void PrimeGenerator(mpz_t t, int bits);
}

std::ostream& operator<<(std::ostream& os, const element_t g);

std::ostream& operator<<(std::ostream& os, const mpz_t g);