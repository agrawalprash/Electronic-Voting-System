#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <pbc/pbc.h>

#include "../pairing/helper_functions.hpp"
#include "../pairing/pair.hpp"
#include "../bilinear-mapping/bilinear_mapping.hpp"
#include "../commitment/commitment.hpp"
#include "../common/common.hpp"

using namespace HelperFunctions;
using namespace BilinearMapping;
using namespace Commitment;
 
TEST(Prime_Generator_Validity, check_prime)
{
    mpz_t t;
    mpz_init(t);

    for(int i=0;i<ITERATIONS;i++)
    {
        HelperFunctions::PrimeGenerator(t, 150);
        GTEST_ASSERT_GT(mpz_probab_prime_p(t, 5), 0);
    }
    
    mpz_clear(t);
}

TEST(BilinearMapping, Equality)
{
    Pairing* pg = new Pairing("a", RBITS, QBITS);

    for(int i=0;i<ITERATIONS;++i)
    {
        element_t t,c,mapping1, mapping2, mapping3, pow1, pow2, pow3;

        pg->random_G1(t);
        pg->random_G1(c);
        pg->random_Zr(pow1);
        pg->random_Zr(pow2);

        element_init_same_as(pow3, pow1);
        element_mul(pow3, pow1, pow2);

        BilinearMapping::BilinearMapping(mapping1, c, pow1, t, pow2, pg);
        BilinearMapping::BilinearMapping(mapping2, c, 1, t, pow3, pg);
        BilinearMapping::BilinearMapping(mapping3, c, pow3, t, 1, pg);

        GTEST_ASSERT_EQ(0, element_cmp(mapping1,mapping2));
        GTEST_ASSERT_EQ(0, element_cmp(mapping2,mapping3));

        element_clear(t);
        element_clear(c);
        element_clear(mapping1);
        element_clear(mapping2);
        element_clear(mapping3);
        element_clear(pow1);
        element_clear(pow2);
        element_clear(pow3);
    }

}

TEST(Commitment, Equality)
{
    Pairing* pg = new Pairing("a", RBITS, QBITS);

    for(int i=0;i<ITERATIONS;++i)
    {
        element_t commitment, g, msg, h, ran;
        pg->generator_G1(g);
        pg->generator_G2(h);

        pg->random_Zr(msg);
        pg->random_Zr(ran);
        Commitment::commit(commitment, g, msg, h, ran);

        element_clear(g);
        element_clear(h);

        pg->generator_G1(g);
        pg->generator_G2(h);

        GTEST_ASSERT_TRUE(open(commitment, g, msg, h, ran));

        element_clear(commitment);
        element_clear(g);
        element_clear(msg);        
        element_clear(h);
        element_clear(ran);
    }

}

TEST(GroupOperation, Inverse)
{
    Pairing* pg = new Pairing("a", RBITS, QBITS);
    
    for(int i=0;i<ITERATIONS;++i)
    {
        element_t t, t_inv, mul;

        pg->random_G1(t);
        pg->inverse(t_inv, t);
        pg->mul(mul, t, t_inv);
        GTEST_ASSERT_TRUE(element_is1(mul));

        pg->random_G2(t);
        pg->inverse(t_inv, t);
        pg->mul(mul, t, t_inv);
        GTEST_ASSERT_TRUE(element_is1(mul));

        pg->random_GT(t);
        pg->inverse(t_inv, t);
        pg->mul(mul, t, t_inv);
        GTEST_ASSERT_TRUE(element_is1(mul));

        pg->random_Zr(t);
        pg->inverse(t_inv, t);
        pg->mul(mul, t, t_inv);
        GTEST_ASSERT_TRUE(element_is1(mul));

        element_clear(t);
        element_clear(t_inv);
        element_clear(mul);
    }
}

TEST(GroupOperation, Negate)
{
    Pairing* pg = new Pairing("a", RBITS, QBITS);
    
    for(int i=0;i<ITERATIONS;++i)
    {
        element_t t, t_neg, t_fin;

        pg->random_G1(t);
        pg->negate(t_neg, t);
        pg->add(t_fin, t, t_neg);
        GTEST_ASSERT_TRUE(element_is0(t_fin));

        pg->random_G1(t);
        pg->negate(t_neg, t);
        pg->add(t_fin, t, t_neg);
        GTEST_ASSERT_TRUE(element_is0(t_fin));

        pg->random_G1(t);
        pg->negate(t_neg, t);
        pg->add(t_fin, t, t_neg);
        GTEST_ASSERT_TRUE(element_is0(t_fin));

        pg->random_G1(t);
        pg->negate(t_neg, t);
        pg->add(t_fin, t, t_neg);
        GTEST_ASSERT_TRUE(element_is0(t_fin));

        element_clear(t);
        element_clear(t_neg);
        element_clear(t_fin);

    }    
}

TEST(GroupOperation, add_sub)
{
    Pairing* pg = new Pairing("a", RBITS, QBITS);
    element_t a, b, c;

    for(int i=0;i<ITERATIONS;i++)
    {
        pg->random_G1(a);
        pg->random_G1(b);
        pg->random_G1(c);

        element_set(c, a);

        pg->add(a, a, b);
        pg->sub(a, a, b);

        GTEST_ASSERT_EQ(0, element_cmp(a, c));
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}