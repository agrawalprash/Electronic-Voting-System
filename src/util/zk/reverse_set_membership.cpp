#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "zk/reverse_set_membership.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

namespace reverseZKPSetMembership {

    bool helper_comparison(element_t a, element_t b){
        //Boolean function for comparison of two variables
        element_t buf;

        element_init_same_as(buf, a);
        element_set(buf, a);
        
        if(element_cmp(a, b) == 0)
        {
            element_clear(buf);
            return true;
        }
        element_clear(buf);
        return false;
    }

    void compute_B1(element_t b1, element_t g , element_t h , element_t s1, element_t s2) {
        element_t helper1, helper2;
        pg->exp(helper1, g, s1);
        pg->exp(helper2, h, s2);
        pg->mul(b1, helper1, helper2);
        return ;
    }

    void compute_B2(element_t b2, element_t Ai , element_t g , element_t s2) {
        element_t helper1;
        pg->exp(helper1, g, s2);
        pg->mul(b2, Ai, helper1);
        return ;
    }

    void compute_LHS_pieSig(element_t t, element_t B2, element_t g, element_t y, Pairing* pg) {
        element_t term_1, term_2, result, temp;
        BilinearMapping::BilinearMapping(term_1, B2, y, pg);
        BilinearMapping::BilinearMapping(term_2, g, g, pg);
        pg->div(result, term_1, term_2);

        element_init_same_as(temp, result);
        element_set(temp, result);
        element_init_same_as(t, temp);
        element_set(t, temp);
        element_clear(temp);
        return;
    }

    void compute_RHS_pieSig(element_t t, element_t B2, element_t g, element_t h, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r, Pairing* pg)  {
        element_t neg_c, term1, term2, term3, term4, term5, result, temp;
        
        unsigned int intone= 1;
        unsigned long one = (unsigned long)intone;
        pg->negate(neg_c, c);

        cout<<"before mappiings"<<endl;

        BilinearMapping::BilinearMapping(term1, B2, one, g, neg_c, pg);
        BilinearMapping::BilinearMapping(term2, g, one, y, s2, pg);
        BilinearMapping::BilinearMapping(term3, g, one, g, delta2, pg);
        BilinearMapping::BilinearMapping(term4, g, one, g, p, pg);
        BilinearMapping::BilinearMapping(term5, h, one, g, r, pg);

        cout<<"mappings made"<<endl;

        pg->mul(result, term1, term2);
        pg->mul(result, result, term3);
        pg->mul(result, result, term4);
        pg->mul(result, result, term5);

        cout<<"muls fine"<<endl;

        element_init_same_as(temp, result);
        element_set(temp, result);
        element_init_same_as(t, temp);
        element_set(t, temp);
        element_clear(temp);
        
        return;

    }

    void compute_LHS1(element_t t, element_t b1, element_t c) {
        element_t temp;
        pg->exp(temp, b1,c);

        element_init_same_as(t, temp);
        element_set(t, temp);

        element_clear(temp);
    }

    void compute_RHS1(element_t t, element_t g, element_t h, element_t delta1, element_t delta2){
        element_t temp1, temp2, temp3;

        pg->exp(temp1, g,delta1);
        pg->exp(temp2, h, delta2);
        pg->mul(temp3, temp1,temp2);

        element_init_same_as(t, temp3);
        element_set(t, temp3);
    }


    bool testing_verification(element_t p, element_t r, Pairing* pg, element_t comm, element_t g, element_t h, element_t *Commitments, int numOfCandidates, int numOfCommitments, int ind) {
        
        cout <<"The p value is : "<<p<<endl;

        element_t x,y;
        pg->random_Zr(x);
        while (x == 0) {
            pg->random_Zr(x);
        }
        pg->exp(y, g, x);
        element_t* A_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));
        element_t* c_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));
        element_t* r_double_dash_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));
        for(int i=0 ; i<numOfCommitments ; i++){
            element_t ri_dash, ci_plus_x, inverse_cix, h_power_ri, Ai_helper1, Ai_helper2;
            element_init_Zr(c_array[i], pg->pairing);
            pg->random_Zr(c_array[i]);
            element_init_same_as(ri_dash, r);
            pg->random_Zr(ri_dash);
            cout<<"Before adding cix"<<endl;
            pg->add(ci_plus_x, c_array[i], x);
            pg->inverse(inverse_cix, ci_plus_x);

            pg->exp(h_power_ri, h, ri_dash);
            pg->mul(Ai_helper1, g,h_power_ri);
            pg->mul(Ai_helper2, Ai_helper1, Commitments[i]);
            element_init_same_as(A_array[i], Ai_helper2);
            pg->exp(A_array[i], Ai_helper2, inverse_cix);

            element_init_same_as(r_double_dash_array[i], r);
            cout <<"Before addinig r''"<<endl;
            cout <<"The r value "<<ri_dash<<endl;
            cout <<"The r value "<<r<<endl;
            pg->add(r_double_dash_array[i] , r, ri_dash);
        }

        cout <<"we reach outside the loop"<<endl;

        //find index i of Ci for p. Here it is 0.
        element_t s1, s2, b1, b2, delta1, delta2;
        element_init_Zr(s1, pg->pairing);
        pg->random_Zr(s1);
        while(s1 == 0){
           pg->random_Zr(s1); 
        }
        element_init_Zr(s2, pg->pairing);
        pg->random_Zr(s2);
        while (s2 == 0)
        {
            pg->random_Zr(s2);
        }
        
        cout<<"Line 128 done"<<endl;



        compute_B1(b1, g ,h, s1, s2);
        compute_B2(b2, A_array[ind], g, s2);
        pg->mul(delta1, s1, c_array[ind]);
        pg->mul(delta2, s2, c_array[ind]);

        cout<<"Line 135 done"<<endl;

        element_t LHS1, RHS1;
        compute_LHS1(LHS1, b1, c_array[ind]);
        compute_RHS1(RHS1, g, h, delta1, delta2);
        if(!helper_comparison(LHS1 , RHS1)){
            return false;
        }

        element_t LHS_val, RHS_val;
        compute_LHS_pieSig(LHS_val, b2, g, y, pg);
        cout<<"LHS_value: "<<LHS_val<<endl;
        compute_RHS_pieSig(RHS_val, b2, g,h ,c_array[ind], y,s2,delta2, p, r_double_dash_array[ind], pg);
        cout<<"RHS_value: "<<RHS_val<<endl;
        if(!helper_comparison(LHS_val , RHS_val)){
            return false;
        }
        return true;
    }


}