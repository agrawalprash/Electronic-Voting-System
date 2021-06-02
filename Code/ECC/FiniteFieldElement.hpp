// Reference -> https://www.codeproject.com/articles/22452/a-simple-c-implementation-of-elliptic-curve-crypto

#pragma once
#include <iostream>
#include <ostream>
#include <openssl/crypto.h>
#include <openssl/bn.h>

namespace Crypto
{
    namespace helper
    {
        int ExtendedGCD(int a, int b, int &x, int &y)
        {
            // Reference -> https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
            // Base Case
            if (a == 0)
            {
                x = 0;
                y = 1;
                return b;
            }

            int x1, y1; // To store results of recursive call
            int gcd = ExtendedGCD(b%a, a, x1, y1);

            // Update x and y using results of
            // recursive call
            x = y1 - (b/a) * x1;
            y = x1;

            return gcd;
        }

        int MultiplicativeInverse(int a, int m)  // a*b == 1 (mod m) Need to calculate b
        {
            a = a % m;
            int x,y;
            int g = ExtendedGCD(a, m, x, y);
            if( g!= 1)
            {
                std::cout << "ERROR: For MulInverse, numbers should ne Coprime\n";                
                return -1;
            }
            x = x % m;
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
    
    template<int P>
    class   FiniteFieldElement
    {
        int     i_;
        
        void    assign(int i)
        {
            i_ = ((i%P) + P)%P;
        }
        
        public:
            // ctor
            FiniteFieldElement()
                : i_(0)
            {}
            // ctor
            explicit FiniteFieldElement(int i)
            {
                assign(i);
            }
            // copy ctor
            FiniteFieldElement(const FiniteFieldElement<P>& rhs) 
                : i_(rhs.i_)               
            {}
            
            // access "raw" integer
            int i() const { return i_; }                
            // negate
            FiniteFieldElement  operator-() const
            {
                return FiniteFieldElement(-i_);
            }                                
            // assign from integer
            FiniteFieldElement& operator=(int i)
            {
                assign(i);
                return *this;
            }
            // assign from field element
            FiniteFieldElement<P>& operator=(const FiniteFieldElement<P>& rhs)
            {
                i_ = rhs.i_;
                return *this;
            }
            // *=
            FiniteFieldElement<P>& operator*=(const FiniteFieldElement<P>& rhs)
            {
                i_ = (i_*rhs.i_) % P;
                return *this;           
            }
            // ==
            friend bool    operator==(const FiniteFieldElement<P>& lhs, const FiniteFieldElement<P>& rhs)
            {
                return (lhs.i_ == rhs.i_);
            }
            // == int
            friend bool    operator==(const FiniteFieldElement<P>& lhs, int rhs)
            {
                return (lhs.i_ == rhs);
            }
            // !=
            friend bool    operator!=(const FiniteFieldElement<P>& lhs, int rhs)
            {
                return (lhs.i_ != rhs);
            }                
            // a / b
            friend FiniteFieldElement<P> operator/(const FiniteFieldElement<P>& lhs, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( lhs.i_ * helper::MultiplicativeInverse(rhs.i_,P));
            }
            // a + b
            friend FiniteFieldElement<P> operator+(const FiniteFieldElement<P>& lhs, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( lhs.i_ + rhs.i_);
            }
            // a - b
            friend FiniteFieldElement<P> operator-(const FiniteFieldElement<P>& lhs, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( lhs.i_ - rhs.i_);
            }
            // a + int
            friend FiniteFieldElement<P> operator+(const FiniteFieldElement<P>& lhs, int i)
            {
                return FiniteFieldElement<P>( lhs.i_+i);
            }
            // int + a
            friend FiniteFieldElement<P> operator+(int i, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( rhs.i_+i);
            }
            // int * a
            friend FiniteFieldElement<P> operator*(int n, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( n*rhs.i_);
            }                
            // a * b
            friend FiniteFieldElement<P> operator*(const FiniteFieldElement<P>& lhs, const FiniteFieldElement<P>& rhs)
            {
                return FiniteFieldElement<P>( lhs.i_ * rhs.i_);
            }
            // ostream handler
            template<int T>
            friend  ostream&    operator<<(ostream& os, const FiniteFieldElement<T>& g)
            {
                return os << g.i_;
            }
    };
}

    
