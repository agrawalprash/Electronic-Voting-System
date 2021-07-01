#include "bilinear_mapping.hpp"

namespace BilinearMapping
{
        
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

    void BilinearMapping(element_t t, element_t g1, element_t a, element_t g2, mpz_t b, Pairing* pairing)
    {
        element_t temp1, temp2;
        
        element_init_same_as(temp1, g1);
        
        element_init_same_as(temp2, g2);
        
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        element_init_GT(t, pairing->pairing);
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);

        
        element_pow_zn(temp1, g1, a);
        element_pow_mpz(temp2, g2, b);

        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        // element_printf("%B %B",temp1,temp2);
        

        pairing_apply(t, temp1, temp2, pairing->pairing);

        element_clear(temp1);
        element_clear(temp2);
    }

    void BilinearMapping(element_t t, element_t g1, mpz_t a, element_t g2, element_t b, Pairing* pairing)
    {
        element_t temp1, temp2;
        
        element_init_same_as(temp1, g1);
        
        element_init_same_as(temp2, g2);
        
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        element_init_GT(t, pairing->pairing);
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);

        
        element_pow_mpz(temp1, g1, a);
        element_pow_zn(temp2, g2, b);

        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        // element_printf("%B %B",temp1,temp2);
        

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

    void BilinearMapping(element_t t, element_t g1, unsigned long a_, element_t g2, element_t b_, Pairing* pairing)
    {
        mpz_t a;
        mpz_init(a);
        mpz_set_ui(a,a_);
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        BilinearMapping(t, g1, a, g2, b_, pairing);
    }

    void BilinearMapping(element_t t, element_t g1, element_t a_, element_t g2, unsigned long b_, Pairing* pairing)
    {
        mpz_t b;
        mpz_init(b);
        mpz_set_ui(b,b_);
        // element_printf("%B %Zd %B %Zd\n",g1,a,g2,b);
        BilinearMapping(t, g1, a_, g2, b, pairing);
    }
}