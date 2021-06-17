#pragma once

// ================================ a ==============================
#define RBITS 16
#define QBITS 52
// ================================ a ==============================

// ================================ a1 ==============================
// Prime Will be generated
#define P_BITS 256
#define Q_BITS 256
// ================================ a1 ==============================


    
// ==========================================================================
    // mpz_t p,q;
    // PrimeGenerator(p, P_BITS);
    // PrimeGenerator(q, Q_BITS);
    // Pairing *pg = new Pairing("a1", p, q);
// ==========================================================================
ostream& operator<<(ostream& os, const element_t g)
{
    element_printf("%B",g);
    return os << "";
}

ostream& operator<<(ostream& os, const mpz_t g)
{
    gmp_printf("%Zd",g);
    return os << "";
}
