// Reference -> https://www.codeproject.com/articles/22452/a-simple-c-implementation-of-elliptic-curve-crypto

#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include "constants.hpp"

// Use BN_value_one() for 1
// BN_cmp(BIGNUM* a, BIGNUM* b) returns 0 if a == b

// using namespace Constants;

// ostream& operator<<(ostream& os, BIGNUM* g);
// ostream& operator<<(ostream& os, const BIGNUM* g);

namespace Crypto
{
    namespace helper
    {
        // Constants::constants();
        string  bn_to_bin(BIGNUM* a)
        {
            a = BN_dup(a);
            string b = "";
            BIGNUM* rem = BN_new();
            BN_CTX *ctx = BN_CTX_new();
            while( BN_cmp(a, BN_ZERO) == 1)
            {
                BN_div(a, rem, a, BN_TWO, ctx);
                if(BN_cmp(rem, BN_ZERO) == 0)b = "0" + b;
                else b = "1" + b;
            }
            return b;
        }

        BIGNUM* ExtendedGCD(BIGNUM* a, BIGNUM* b, BIGNUM* &x, BIGNUM* &y)
        {
            // Reference -> https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
            // Base Case
            if( BN_cmp(a, BN_ZERO) == 0)
            {            
                x = BN_new();
                y = BN_dup(BN_ONE);
                return b;
            }

            BIGNUM* x1;
            BIGNUM* y1;        // To store results of recursive call
            BIGNUM* b_mod_a;
            BN_CTX *ctx = BN_CTX_new();

            b_mod_a = BN_dup(a);
            x1 = BN_dup(a);
            y1 = BN_dup(b);

            BN_mod(b_mod_a, b, a, ctx);
            BIGNUM* gcd = ExtendedGCD(b_mod_a, a, x1, y1);

            // Update x and y using results of
            // recursive call
            BN_div(x, NULL, b, a, ctx);
            BN_mul(x, x, x1, ctx);
            BN_sub(x, y1, x);
            BN_add(y, x1, BN_ZERO);
            // x = y1 - (b/a) * x1;
            // y = x1;

            return gcd;
        }

        BIGNUM* MultiplicativeInverse(BIGNUM* a, BIGNUM* m)
        {
            // a*b == 1 (mod m) Need to calculate b
            BN_CTX *ctx = BN_CTX_new();
            // a = a % m;
            BN_mod(a, a, m, ctx);
            BIGNUM* x = BN_new();
            BIGNUM* y = BN_new();
            
            BIGNUM* g = ExtendedGCD(a, m, x, y);

            // cout << a << " " << m << " " << x << " " << y << endl;

            if( BN_cmp(g, BN_ONE) != 0) // Returns Zero if a == b
            {
                std::cerr << "ERROR: For MulInverse, numbers should be Coprime\n";
                BN_set_word(x, -1);
                return x;
            }
            BN_mod(x, x, m, ctx);
            BN_add(x, x, m);
            BN_mod(x, x, m, ctx);
            // x = ((x % m) + m )%m;

            return x;
        }
    }

    /*
        An element in a Galois field FP
        Adapted for the specific behaviour of the "mod" function where (-n) mod m returns a negative number
        Allows basic arithmetic operations between elements:
            +,-,/,scalar multiply                
            
        The template argument T is the order of the field
    */
    
    // template<BIGNUM* PRIME>
    class   FiniteFieldElement
    {
        BIGNUM*     i_ = NULL;
        
        void    assign(BIGNUM* i)
        {
            i = BN_dup(i);
            if(i_ == NULL)i_ = BN_new();
            BN_CTX *ctx = BN_CTX_new();
            BN_mod(i, i, PRIME, ctx);
            BN_add(i, i, PRIME);
            BN_mod(i_, i, PRIME, ctx);
            // i_ = ((i%PRIME) + PRIME)%PRIME;
        }
        
        public:
            // ctor
            FiniteFieldElement()
                : i_(BN_new())
            {}
            // ctor
            explicit FiniteFieldElement(BIGNUM* i)
            {
                assign(i);
            }
            explicit FiniteFieldElement(int i)
            {
                BIGNUM* tmp = BN_new();
                BN_set_word(tmp, i);
                assign(tmp);
            }
            // copy ctor
            FiniteFieldElement(const FiniteFieldElement& rhs) 
                : i_(rhs.i_)               
            {}
            
            // access "raw" integer
            BIGNUM* i() const { return i_; }
            // negate
            FiniteFieldElement  operator-() const
            {
                BIGNUM* tmp = BN_new();
                BN_sub(tmp, BN_ZERO, i_);
                return FiniteFieldElement(tmp);
            }                    

            // assign from field element
            FiniteFieldElement& operator=(const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp = BN_dup(rhs.i_);
                i_ = tmp;
                return *this;
            }            
            // assign from Bignum
            FiniteFieldElement& operator=(BIGNUM* i)
            {
                assign(i);
                return *this;
            }
            // assign from int
            FiniteFieldElement& operator=(int i)
            {
                BIGNUM* tmp = BN_new();
                BN_set_word(tmp, i);
                assign(tmp);
                return *this;
            }
            

            // *= Field
            FiniteFieldElement& operator*=(const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, i_, rhs.i_, ctx);
                BN_mod(i_, tmp, PRIME, ctx);
                // i_ = (i_*rhs.i_) % PRIME;
                return *this;           
            }
            // *= Bignum
            FiniteFieldElement& operator*=(BIGNUM* rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, i_, rhs, ctx);
                BN_mod(i_, tmp, PRIME, ctx);
                return *this;           
            }
            // *= int
            FiniteFieldElement& operator*=(int rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BIGNUM* tmp2 = BN_new();
                BN_set_word(tmp2, rhs); 
                BN_mul(tmp, i_, tmp2, ctx);
                BN_mod(i_, tmp, PRIME, ctx);
                // i_ = (i_*rhs.i_) % PRIME;
                return *this;           
            }

            // == Field
            friend bool    operator==(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                if(BN_cmp(lhs.i_, rhs.i_) == 0)return true;
                return false;
                // return (lhs.i_ == rhs.i_);
            }
            // == BIGNUM
            friend bool    operator==(const FiniteFieldElement& lhs, BIGNUM* rhs)
            {
                if( BN_cmp(lhs.i_, rhs) == 0 )return true;
                return false;
                // return (lhs.i_ == rhs);
            }
            // == int
            friend bool    operator==(const FiniteFieldElement& lhs, int rhs)
            {
                BIGNUM* tmp = BN_new();
                BN_set_word(tmp, rhs); 
                if( BN_cmp(lhs.i_, tmp) == 0 )return true;
                return false;
                // return (lhs.i_ == rhs);
            }

            // != Field
            friend bool    operator!=(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                if( BN_cmp(lhs.i_, rhs.i_) == 0 )return false;
                return true;
                // return (lhs.i_ != rhs.i_);
            }
            // != BIGNUM
            friend bool    operator!=(const FiniteFieldElement& lhs, BIGNUM* rhs)
            {   
                if( BN_cmp(lhs.i_, rhs) == 0 )return false;
                return true;
                // return (lhs.i_ != rhs);
            }
            // != int
            friend bool    operator!=(const FiniteFieldElement& lhs, int rhs)
            {   
                BIGNUM* tmp = BN_new();
                BN_set_word(tmp, rhs); 
                if( BN_cmp(lhs.i_, tmp) == 0 )return false;
                return true;
                // return (lhs.i_ != rhs);
            }

            // a / b
            friend FiniteFieldElement operator/(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();

                // cout << lhs << " " << rhs << " " << PRIME << endl;
                // cout << lhs.i_ << " " << rhs.i_ << " " << PRIME << endl;
                // cout << rhs.i_ << "  " << PRIME << endl;
                BN_mul(tmp, lhs.i_, helper::MultiplicativeInverse(rhs.i_,PRIME), ctx);
                // cout << rhs.i_ << "  " << PRIME << endl;
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,PRIME));
            }
            // a % b
            friend FiniteFieldElement operator%(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mod(tmp, lhs.i_, rhs.i_, ctx);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,PRIME));
            }
            // BIGNUM % b
            friend FiniteFieldElement operator%(BIGNUM* lhs, const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mod(tmp, lhs, rhs.i_, ctx);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,PRIME));
            }
            // b % BIGNUM
            friend FiniteFieldElement operator%(const FiniteFieldElement& lhs, BIGNUM* rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mod(tmp, lhs.i_, rhs, ctx);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,PRIME));
            }
            // int % b
            friend FiniteFieldElement operator%(int n, const FiniteFieldElement& rhs)
            {
                BIGNUM *lhs = BN_new();
                BN_set_word(lhs, n);

                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mod(tmp, lhs, rhs.i_, ctx);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,PRIME));
            }
            

            // a + b
            friend FiniteFieldElement operator+(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp = BN_new();
                BN_add(tmp, lhs.i_, rhs.i_);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ + rhs.i_);
            }
            // a - b
            friend FiniteFieldElement operator-(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp = BN_new();
                BN_sub(tmp, lhs.i_, rhs.i_);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_ - rhs.i_);
            }
            // a + BIGNUM
            friend FiniteFieldElement operator+(const FiniteFieldElement& lhs, BIGNUM* i)
            {
                BIGNUM* tmp = BN_new();
                BN_add(tmp, lhs.i_, i);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_+i);
            }
            // BIGNUM + a
            friend FiniteFieldElement operator+(BIGNUM* i, const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp = BN_new();
                BN_add(tmp, i, rhs.i_);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( rhs.i_+i);
            }
            // a + int
            friend FiniteFieldElement operator+(const FiniteFieldElement& lhs, int n)
            {
                BIGNUM* tmp2 = BN_new();
                BN_set_word(tmp2, n); 

                BIGNUM* tmp = BN_new();
                BN_add(tmp, lhs.i_, tmp2);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( lhs.i_+i);
            }
            // int + a
            friend FiniteFieldElement operator+(int n, const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp2 = BN_new();
                BN_set_word(tmp2, n); 

                BIGNUM* tmp = BN_new();
                BN_add(tmp, tmp2, rhs.i_);
                return FiniteFieldElement(tmp);
                // return FiniteFieldElement( rhs.i_+i);
            }

            // BIGNUM * a
            friend FiniteFieldElement operator*(BIGNUM* n, const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, n, rhs.i_, ctx);
                return FiniteFieldElement( tmp );
            }
            // a * BIGNUM
            friend FiniteFieldElement operator*(const FiniteFieldElement& lhs, BIGNUM* n)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, n, lhs.i_, ctx);
                return FiniteFieldElement( tmp );
            }
            // int * a
            friend FiniteFieldElement operator*(int n, const FiniteFieldElement& rhs)
            {
                BIGNUM* tmp2 = BN_new();
                BN_set_word(tmp2, n); 

                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, tmp2, rhs.i_, ctx);
                return FiniteFieldElement( tmp );
            }
            // a * int
            friend FiniteFieldElement operator*(const FiniteFieldElement& lhs, int n)
            {
                BIGNUM* tmp2 = BN_new();
                BN_set_word(tmp2, n); 

                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, lhs.i_, tmp2, ctx);
                return FiniteFieldElement( tmp );
            }
            // // BIGNUM * int
            // friend FiniteFieldElement operator*(BIGNUM* lhs, int n)
            // {
            //     BIGNUM* tmp2 = BN_new();
            //     BN_set_word(tmp2, n); 

            //     BN_CTX *ctx = BN_CTX_new();
            //     BIGNUM* tmp = BN_new();
            //     BN_mul(tmp, lhs, tmp2, ctx);
            //     return FiniteFieldElement( tmp );
            // }
            // // int * BIGNUM
            // friend FiniteFieldElement operator*(int n, BIGNUM* rhs)
            // {
            //     BIGNUM* tmp2 = BN_new();
            //     BN_set_word(tmp2, n); 

            //     BN_CTX *ctx = BN_CTX_new();
            //     BIGNUM* tmp = BN_new();
            //     BN_mul(tmp, tmp2, rhs, ctx);
            //     return FiniteFieldElement( tmp );
            // }

            // a * b
            friend FiniteFieldElement operator*(const FiniteFieldElement& lhs, const FiniteFieldElement& rhs)
            {
                BN_CTX *ctx = BN_CTX_new();
                BIGNUM* tmp = BN_new();
                BN_mul(tmp, lhs.i_, rhs.i_, ctx);
                return FiniteFieldElement( tmp );
            }
            // ostream handler
            // template<BIGNUM* T>
            friend  std::ostream&    operator<<(std::ostream& os, const FiniteFieldElement& g)
            {
                string ans = BN_bn2dec(g.i_);
                return os << ans;
            }
    };
}

