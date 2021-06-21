#include "commitment.hpp"
// #include "constant.hpp"

void commit(element_t Commitment, element_t g, element_t Message, element_t h, element_t random)
{
    element_init_same_as(Commitment, g);
    element_pow2_zn(Commitment, g, Message, h, random); // c = g^msg * h^random
}

void commit(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random)
{
    element_init_same_as(Commitment, g);
    element_pow2_mpz(Commitment, g, Message, h, random); // c = g^msg * h^random
}

void commit(element_t Commitment, mpz_t msg, mpz_t r, Pairing* pairing)
{
    element_t t1,t2;
    pairing->generator_G1(t1);
    pairing->generator_G2(t2);
    commit(Commitment, t1, msg, t2, r);

    element_clear(t1);
    element_clear(t2);
}

void commit(element_t Commitment, string msg, string random, Pairing* pairing)
{
    mpz_t m,r;
    mpz_init(m);
    mpz_init(r);
    mpz_set_str(m,msg.c_str(),10);
    mpz_set_str(r,random.c_str(),10);
    commit(Commitment,m,r,pairing);

    mpz_clear(m);
    mpz_clear(r);
}

void commit(element_t Commitment, string msg, mpz_t r, Pairing* pairing)
{
    mpz_t m;
    mpz_init(m);
    mpz_set_str(m,msg.c_str(),10);
    commit(Commitment,m,r,pairing);
    mpz_clear(m);
}

bool open(element_t Commitment, element_t g, element_t Message, element_t h, element_t random)
{
    element_t t;
    element_init_same_as(t, g);
    element_pow2_zn(t, g, Message, h, random); // t = g^msg * h^random
    if(element_cmp(Commitment, t) == 0)
    {
        element_clear(t);
        return true;
    }
    element_clear(t);
    return false;
}

bool open(element_t Commitment, element_t g, mpz_t Message, element_t h, mpz_t random)
{
    element_t t;
    element_init_same_as(t, g);
    element_pow2_mpz(Commitment, g, Message, h, random); // t = g^msg * h^random
    if(element_cmp(Commitment, t) == 0)
    {
        element_clear(t);
        return true;
    }
    element_clear(t);
    return false;
}


