#include <pbc/pbc.h>
#include <iostream>
#include "constants.hpp"
// #include <bits/stdc++.h>

using namespace std;

pairing_t pairing;
element_t g, h; // Generators of Gq

void initialize_a1()
{
    mpz_t p, q, N;

    mpz_init(p);
    mpz_init(q);
    mpz_init(N);

    // In a real application, p and q must be stored somewhere safe.
    pbc_mpz_randomb(p, 512);
    pbc_mpz_randomb(q, 512);

    mpz_nextprime(p, p);
    mpz_nextprime(q, q);
    mpz_mul(N, p, q);

    pbc_param_t param;
    pbc_param_init_a1_gen(param, N);

    FILE* fp = fopen("Parameter.txt", "w+");
    pbc_param_out_str(fp, param);
    
    pairing_init_pbc_param(pairing, param);

    element_init_G1(g, pairing);
    element_init_G2(h, pairing);

    element_random(g);
    element_random(h);
    
    pbc_param_clear(param);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(N);

    return;
}

void initialize_a()
{
    pbc_param_t param;
    pbc_param_init_a_gen(param, RBITS, QBITS);

    FILE* fp = fopen("Parameter.txt", "w+");
    pbc_param_out_str(fp, param);

    pairing_init_pbc_param(pairing, param);
    element_init_G1(g, pairing);
    element_init_G2(h, pairing);

    element_random(g);
    element_random(h);

    pbc_param_clear(param);
    return;
}

void commitment(element_t &Commitment, element_t Message)
{
    element_t r;

    element_init_Zr(r, pairing); // "r" is the random key used to generate commitment for "Messgae". Belongs to Zr    
    element_init_G1(Commitment, pairing);

    element_random(r);

    element_pow2_zn(Commitment, g, Message, h, r);
    element_clear(r);
    return;
}

void commitment(element_t &Commitment, unsigned long long Message_)
{
    element_t r, Message;

    element_init_Zr(r, pairing); // "r" is the random key used to generate commitment for "Messgae". Belongs to Zr    
    element_init_G1(Commitment, pairing);
    element_init_Zr(Message, pairing);

    element_set_si(Message, Message_);

    element_random(r);

    element_pow2_zn(Commitment, g, Message, h, r);
    element_clear(r);
    return;
}

int main(int argc, char* argv[])
{
    clock_t begin, end;
    int    Iterations = 1000;
    double TotalTime = 0;
    double PairingTime = 0;
    unsigned long long Message = 100;

    begin = clock();
    
    initialize_a();
    
    end = clock();
    PairingTime = double(end - begin)/CLOCKS_PER_SEC;
    // cout << "Time taken to generate the Pairing: " << (double)(end - begin)*1000/CLOCKS_PER_SEC << " ms\n";

    begin = clock();
    
    for(int i=0;i < Iterations; i++)
    {    
        element_t c;
        
        commitment(c, Message);
        
        end = clock();
        TotalTime+=(double)(end-begin);
        cout << "Processed: " << i+1 << "\n";

        element_clear(c);
        // cout << "Time taken to generate the Commitment of Message \""<< Message << "\": " << (double)(end - begin)*1000/CLOCKS_PER_SEC << " ms\n";
        // element_printf("%B\n", c);
    }

    cout << "Time taken to generate the Pairing: " << PairingTime*1000 << " ms\n";
    cout << "Avg.( over "<< Iterations << " iterations ) Time taken to generate the Pederson Commitment of Message \""<< Message << "\": " << (double)(end - begin)*1000/(CLOCKS_PER_SEC*Iterations) << " ms\n";

    return 0;
}