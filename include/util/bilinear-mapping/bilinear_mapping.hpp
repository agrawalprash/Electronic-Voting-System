#include "pairing/pairing.hpp"

namespace BilinearMapping
{
    void BilinearMapping(element_t t, element_t g1, element_t g2, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, element_t b, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, mpz_t b, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, mpz_t b, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, element_t b, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, unsigned long b_, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, element_t b_, Pairing* pairing);

    void BilinearMapping(element_t t, element_t g1, element_t a_, element_t g2, unsigned long b_, Pairing* pairing);
}