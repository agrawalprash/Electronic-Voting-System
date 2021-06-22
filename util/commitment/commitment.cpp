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

void BilinearMapping(element_t t, element_t g1, element_t g2, Pairing* pairing)
{   
    element_init_GT(t, pairing->pairing);
    pairing_apply(t, g1, g2, pairing->pairing);
}

void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, element_t b, Pairing* pairing)
{   
    element_t temp1, temp2;
    element_init_same_as(temp1, g1);
    element_init_same_as(temp2, g2);
    element_init_GT(t, pairing->pairing);

    element_pow_zn(temp1, g1, a);
    element_pow_zn(temp2, g2, b);

    pairing_apply(t, temp1, temp2, pairing->pairing);

    element_clear(temp1);
    element_clear(temp2);
}

void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, mpz_t b, Pairing* pairing)
{
    
    element_t temp1, temp2;
    
    element_init_same_as(temp1, g1);
    
    element_init_same_as(temp2, g2);
    
    // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
    element_init_GT(t, pairing->pairing);
    // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);

    
    element_pow_mpz(temp1, g1, a);
    element_pow_mpz(temp2, g2, b);

    // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
    // element_printf("%B %B",temp1,temp2);
    

    pairing_apply(t, temp1, temp2, pairing->pairing);

    element_clear(temp1);
    element_clear(temp2);
}

void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, unsigned long b_, Pairing* pairing)
{
    
    mpz_t a,b;
    mpz_init(a);
    mpz_init(b);
    mpz_set_ui(a,a_);
    mpz_set_ui(b,b_);
    // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
    BilinearMapping(t, g1, a, g2, b, pairing);
}

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
