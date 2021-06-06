#pragma once
#include <ostream>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <string>

#define BN_TWO bignum_two
#define BN_ONE bignum_one
#define BN_ZERO bignum_zero
#define BN_MINUS_ONE bignum_minus_one
#define PRIME Prime
#define PRIME_LENGTH 15
#define SAFE true

BIGNUM* bignum_two = NULL;
BIGNUM *bignum_one = NULL;
BIGNUM *bignum_zero = NULL;
BIGNUM *bignum_minus_one = NULL;
BIGNUM *Prime = BN_new();

using namespace std;

ostream& operator<<(ostream& os, BIGNUM* g);
ostream& operator<<(ostream& os, const BIGNUM* g);

void generate()
{
    int p_small = -1;
    srand(time(0));
    if(PRIME_LENGTH == 5)
    {
        int a[] = {17, 19, 23, 29, 31};
        p_small = a[rand() % 5];
    }
    else if(PRIME_LENGTH == 4)
    {
        int a[] = {11, 13};
        p_small = a[rand() % 2];
    }
    else if(PRIME_LENGTH == 3)
    {
        int a[] = {5, 7};
        p_small = a[rand() % 2];
    }
    else if(PRIME_LENGTH == 2)
    {
        int a[] = {2, 3};
        p_small = a[rand() % 2];
    }

    if(p_small != -1)
    {
        BN_set_word(Prime, p_small);
        return;
    }

    // BN_set_word(Prime, 839);
    // return;

    BN_generate_prime_ex(Prime, PRIME_LENGTH, SAFE, NULL, NULL, NULL);
    
}

void function()
{
    string s = "-1";
    BN_dec2bn(&bignum_minus_one, s.c_str());
    s = "0";
    BN_dec2bn(&bignum_zero, s.c_str());
    s = "1";
    BN_dec2bn(&bignum_one, s.c_str());
    s = "2";
    BN_dec2bn(&bignum_two, s.c_str());
    generate();    
}



ostream& operator<<(ostream& os, BIGNUM* g)
{
    string ans = BN_bn2dec(g);
    return os << ans;
}

ostream& operator<<(ostream& os, const BIGNUM* g)
{
    string ans = BN_bn2dec(g);
    return os << ans;
}

// namespace Constants
// {
//     void constants()
//     {
//         string s = "-1";
//         BN_dec2bn(&bignum_minus_one, s.c_str());
//         s = "0";
//         BN_dec2bn(&bignum_zero, s.c_str());
//         s = "1";
//         BN_dec2bn(&bignum_one, s.c_str());
//     }
// }
