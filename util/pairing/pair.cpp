#include "pair.hpp"

Pairing::Pairing(FILE *f)
{
    char s[8192];
    size_t count = fread(s, 1, 8192, *(FILE **) &f);
    if(count)
    {
        pairing_init_set_buf(pairing, s, count);
    }
    element_init_G1(gen_G1, pairing);
    element_init_G2(gen_G2, pairing);
    element_init_GT(gen_GT, pairing);
    element_init_Zr(gen_Zr, pairing);

    element_set(gen_Zr, field_get_nqr(pairing->Zr));
    element_random(gen_G1);
    element_random(gen_G2);
    element_random(gen_GT);
}

Pairing::Pairing(string type, mpz_t p, mpz_t q)
{
    if(type == "a1")
    {
        mpz_t n; // n = p*q
        mpz_init(n);
        mpz_mul(n, p, q);
        
        pbc_param_t param;
        pbc_param_init_a1_gen(param, n);

        FILE *fp = fopen("param", "w+");
        pbc_param_out_str(fp, param);
        fclose(fp);

        pairing_init_pbc_param(pairing, param);

        pbc_param_clear(param);
        mpz_clear(n);
    }
    element_init_G1(gen_G1, pairing);
    element_init_G2(gen_G2, pairing);
    element_init_GT(gen_GT, pairing);
    element_init_Zr(gen_Zr, pairing);

    element_set(gen_Zr, field_get_nqr(pairing->Zr));
    element_random(gen_G1);
    element_random(gen_G2);
    element_random(gen_GT);
}

Pairing::Pairing(string type, mpz_t p, unsigned long q_)
{
    if(type == "a1")
    {
        mpz_t n,q; // n = p*q
        mpz_init(n);
        mpz_init(q);
        mpz_set_ui(q,q_);
        mpz_mul(n, p, q);
        
        pbc_param_t param;
        pbc_param_init_a1_gen(param, n);

        FILE *fp = fopen("param", "w+");
        pbc_param_out_str(fp, param);
        fclose(fp);

        pairing_init_pbc_param(pairing, param);

        pbc_param_clear(param);
        mpz_clear(n);
        mpz_clear(q);
    }
    element_init_G1(gen_G1, pairing);
    element_init_G2(gen_G2, pairing);
    element_init_GT(gen_GT, pairing);
    element_init_Zr(gen_Zr, pairing);

    element_set(gen_Zr, field_get_nqr(pairing->Zr));
    element_random(gen_G1);
    element_random(gen_G2);
    element_random(gen_GT);
}

Pairing::Pairing(string type, unsigned long p_, mpz_t q)
{
    if(type == "a1")
    {
        mpz_t n,p; // n = p*q
        mpz_init(n);
        mpz_init(p);
        mpz_set_ui(p,p_);
        mpz_mul(n, p, q);
        
        pbc_param_t param;
        pbc_param_init_a1_gen(param, n);

        FILE *fp = fopen("param", "w+");
        pbc_param_out_str(fp, param);
        fclose(fp);

        pairing_init_pbc_param(pairing, param);

        pbc_param_clear(param);
        mpz_clear(n);
        mpz_clear(p);
    }
    element_init_G1(gen_G1, pairing);
    element_init_G2(gen_G2, pairing);
    element_init_GT(gen_GT, pairing);
    element_init_Zr(gen_Zr, pairing);

    element_set(gen_Zr, field_get_nqr(pairing->Zr));
    element_random(gen_G1);
    element_random(gen_G2);
    element_random(gen_GT);
}

Pairing::Pairing(string type, unsigned long pbits, unsigned long qbits)
{
    pbc_param_t param;
    

    if(type == "a" || type == "e")
    {
        if(type == "a")
        {
            pbc_param_init_a_gen(param, pbits, qbits);
        }
        else if(type == "e")
        {
            pbc_param_init_e_gen(param, pbits, qbits);
        }
    }
    else if(type == "a1")
    {
        mpz_t p,q,n;
        mpz_init(p);
        mpz_init(q);
        mpz_init(n);
        mpz_set_ui(p, pbits);
        mpz_set_ui(q, qbits);
        mpz_mul(n,p,q);

        pbc_param_init_a1_gen(param, n);

        mpz_clear(n);
        mpz_clear(p);
        mpz_clear(q);
    }

    FILE *fp = fopen("param", "w+");
    pbc_param_out_str(fp, param);
    fclose(fp);

    pairing_init_pbc_param(pairing, param);
    element_init_G1(gen_G1, pairing);
    element_init_G2(gen_G2, pairing);
    element_init_GT(gen_GT, pairing);
    element_init_Zr(gen_Zr, pairing);

    element_set(gen_Zr, field_get_nqr(pairing->Zr));
    element_random(gen_G1);
    element_random(gen_G2);
    element_random(gen_GT);
    
    pbc_param_clear(param);
}

void Pairing::generator_G1(element_t t)
{
    element_init_G1(t, pairing);
    element_set(t, gen_G1);
}

void Pairing::generator_G2(element_t t)
{
    element_init_G2(t, pairing);
    element_set(t, gen_G2);
}

void Pairing::generator_GT(element_t t)
{
    element_init_GT(t, pairing);
    element_set(t, gen_GT);
}

void Pairing::generator_Zr(element_t t)
{
    element_init_Zr(t, pairing);
    element_set(t, gen_Zr);
}

void Pairing::random_G1(element_t t)
{
    element_init_G1(t, pairing);
    element_random(t);
}

void Pairing::random_G2(element_t t)
{
    element_init_G2(t, pairing);
    element_random(t);
}

void Pairing::random_GT(element_t t)
{
    element_init_GT(t, pairing);
    element_random(t);
}

void Pairing::random_Zr(element_t t)
{
    element_init_Zr(t, pairing);
    element_random(t);
}

void Pairing::add(element_t t, element_t a, element_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    if(a->field != b->field)
    {
        cerr << "Exception: add operation is valid on elements of same field\n";
        exit(0);
    }
    element_add(temp, a, b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::sub(element_t t, element_t a, element_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    if(a->field != b->field)
    {
        cerr << "Exception: add operation is valid on elements of same field\n";
        exit(0);
    }
    element_sub(temp, a, b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::mul(element_t t, element_t a, element_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    if(b->field == gen_Zr->field)
    {
        element_mul_zn(temp, a, b);
        element_init_same_as(t, a);
        element_set(t, temp);
        element_clear(temp);
        return;
    }
    if(a->field != b->field)
    {
        cerr << "Exception: add operation is valid on elements of same field\n";
        exit(0);
    }
    element_mul(temp, a, b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::mul(element_t t, element_t a, mpz_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    element_mul_mpz(temp,a,b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::mul(element_t t, element_t a, long long b)
{
    element_t temp;
    element_init_same_as(temp, a);
    element_mul_si(temp,a,b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::div(element_t t, element_t a, element_t b_)
{
    element_t temp;
    element_init_same_as(temp, a);
    element_t b;
    element_init_same_as(b, b_);
    element_invert(b, b_);
    if(b->field == gen_Zr->field)
    {
        element_mul_zn(temp, a, b);
        element_init_same_as(t, a);
        element_set(t, temp);
        element_clear(temp);
        return;
    }
    if(a->field != b->field)
    {
        cerr << "Exception: add operation is valid on elements of same field\n";
        exit(0);
    }
    element_mul(temp, a, b);
    element_init_same_as(t, a);
    element_set(t, temp);

    element_clear(temp);
    element_clear(b);
}

void Pairing::exp(element_t t, element_t a, element_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    if(b->field == gen_Zr->field)
    {
        element_pow_zn(temp, a, b);
        element_init_same_as(t, a);
        element_set(t, temp);
        element_clear(temp);
        return;
    }
    else
    {
        cerr << "Exception: add operation is valid on elements of same field\n";
        exit(0);
    }
}

void Pairing::exp(element_t t, element_t a, mpz_t b)
{
    element_t temp;
    element_init_same_as(temp, a);
    element_pow_mpz(temp, a, b);    
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::exp(element_t t, element_t a, unsigned long x)
{
    element_t temp;
    element_init_same_as(temp, a);
    mpz_t b;
    mpz_init(b);
    mpz_set_ui(b,x);

    element_pow_mpz(temp, a, b);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
    mpz_clear(b);    
}

void Pairing::negate(element_t t, element_t a)
{
    element_t temp;
    element_init_same_as(temp, a);

    element_neg(temp, a);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}

void Pairing::inverse(element_t t, element_t a)
{
    element_t temp;
    element_init_same_as(temp, a);

    element_invert(temp, a);
    element_init_same_as(t, a);
    element_set(t, temp);
    element_clear(temp);
}
