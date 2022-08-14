#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "zk/set_membership_zkp.h"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;

namespace ZKPSetMembership{
    void provers_message_D(element_t D, element_t g, element_t s, element_t h, element_t m, Pairing* pairing){
        //Returns D = g^s.h^m

        element_t comm_D;

        Commitment::commit(comm_D, g, s, h, m);
        element_init_same_as(D, comm_D);
        element_set(D, comm_D);
        // element_t gs, hm, gshm;

        // pairing->exp(gs, g, s);
        // pairing->exp(hm, h, m);
        // pairing->mul(gshm, gs, hm);

        // element_init_same_as(D, gshm);
        // element_set(D, gshm);


        // element_init_same_as(temp1, gshm);
        // element_set(temp1, gshm);
        // element_set(D, temp1);
        // element_clear(temp1); 

        return;
    }

    void provers_message_a(element_t a, element_t sig, element_t g, element_t s, element_t t, Pairing* pairing){
        //Returns a = e(V,g)^-s. e(g,g)^t
        element_t term1, term2, neg_s, a_temp, temp2;
        
        unsigned int intone= 1;
        unsigned long one = (unsigned long)intone;
        pairing->negate(neg_s, s);
        BilinearMapping::BilinearMapping(term1, sig, one, g, neg_s, pairing);
        BilinearMapping::BilinearMapping(term2, g, one, g, t, pairing);
        pairing->mul(a_temp, term1, term2);

        element_init_same_as(a, a_temp);
        element_set(a, a_temp);
        element_init_same_as(temp2, a_temp);
        element_set(temp2, a_temp);
        element_set(a, temp2);
        element_clear(temp2);

        return;
    }

    void helper_message_D(element_t D, element_t g, element_t s, element_t h, element_t m, Pairing* pairing){
        //Returns D = g^s.h^m

        element_t comm_D;

        Commitment::commit(comm_D, g, s, h, m);
        element_init_same_as(D, comm_D);
        element_set(D, comm_D);

        // element_t gs, hm, gshm;

        // pairing->exp(gs, g, s);
        // pairing->exp(hm, h, m);
        // pairing->mul(gshm, gs, hm);

        // element_init_same_as(D, gshm);
        // element_set(D, gshm);


        // element_init_same_as(temp1, gshm);
        // element_set(temp1, gshm);
        // element_set(D, temp1);
        // element_clear(temp1); 

        return;
    }

    int helper_index(element_t *phi, element_t el, int n){
        int index = -1;
        for (int i = 0; i < n; i++){
            
            if (element_cmp(phi[i], el) == 0){
                index = i;
                return index;
            }
        }
        return index;
    }

    void verifiers_generate_signature(element_t* signatures, element_t x, element_t g, element_t *phi, Pairing* pairing, int n){
        element_t xp_sums[n];
        element_t sig_pows[n];
        element_t sig_temps[n];

        //Returns signature Ap = g^(1/x+p)

        for (int i = 0; i < n; i++){
            
            element_t xp_sum, sig_pow, sig_temp, temp, sig;
            pairing->add(xp_sum, x, phi[i]);
            // cerr << "xp sum : " <<xp_sum << endl;
            element_init_same_as(xp_sums[i], xp_sum);
            element_set(xp_sums[i], xp_sum);
            
            pairing->inverse(sig_pow, xp_sum);
            element_init_same_as(sig_pows[i], sig_pow);
            element_set(sig_pows[i], sig_pow);
            // cerr << sig_pow << endl;
            
            pairing->exp(sig_temp, g, sig_pow);
            element_init_same_as(sig_temps[i], sig_temp);
            element_set(sig_temps[i], sig_temp);
            // cerr << sig_temp << endl;
            
            // element_init_same_as(temp, sig_temp);
            element_init_same_as(sig, sig_temp);
            element_set(sig, sig_temp);
            element_init_same_as(signatures[i], sig);
            // element_set(sig, temp);
            // cerr << "temp" << temp << endl;
            // cerr << sig << endl;
            element_set(signatures[i], sig);
            // element_clear(temp);
            element_clear(xp_sum);
            element_clear(sig_temp);
            element_clear(sig_pow);
            element_clear(sig);
        }
        return;
    }

    void provers_obtain_signature(element_t Ap, element_t* signatures, element_t p, element_t *phi, Pairing* pairing, int n){        
        // element_t temp;
        
        int p_buf = helper_index(phi, p, n);
        element_init_same_as(Ap, signatures[p_buf]);
        // element_init_same_as(temp, signatures[p_buf]);
        element_set(Ap, signatures[p_buf]);
        // element_clear(temp);

        return;
    }

    void verifiers_blind_signature(element_t blindsig, element_t sig, element_t v, Pairing* pairing){
        //Blinds the signature V = Ap^v
        element_t blindsigbuf, temp;

        pairing->exp(blindsigbuf, sig, v);
        element_init_same_as(blindsig, blindsigbuf);
        element_set(blindsig, blindsigbuf);
        element_init_same_as(temp, blindsigbuf);
        element_set(temp, blindsigbuf);
        element_set(blindsig, temp);
        element_clear(temp);

        return;
    }

    void verifiers_challenge(element_t t, Pairing* prg){
        //Generates a random challenge c for the prover
        element_t c, temp;

        prg->random_Zr(c);
        element_init_same_as(temp, c);
        element_set(temp, c);
        element_init_same_as(t, c);
        element_set(t, c);
        element_set(t, temp);
        element_clear(temp);

        return;
    }

    void provers_response(element_t z, element_t s, element_t p,element_t c, Pairing* pairing){
        //Returns the response from the prover z = s - pc
        element_t spc, pc, neg_pc;

        pairing->mul(pc, p, c);
        pairing->negate(neg_pc, pc);
        pairing->add(spc, s, neg_pc);
        // element_init_same_as(temp1, spc);
        // element_set(temp1, spc);
        element_init_same_as(z, spc);
        element_set(z, spc);
        // element_set(z, temp1);
        // element_clear(temp1);

        return;
    }

    void verifiers_BilinearRHS(element_t t, element_t sig, element_t g, element_t x, element_t c, element_t zp, element_t zv, element_t y, Pairing* pairing){
        //Returns e(V,y)^c. e(V,g)^-zp. e(g,g)^zv
        element_t neg_zp, term1, term2, term3, result, temp;
        
        unsigned int intone= 1;
        unsigned long one = (unsigned long)intone;
        pairing->negate(neg_zp, zp);
        BilinearMapping::BilinearMapping(term1, sig, one, y, c, pairing);
        BilinearMapping::BilinearMapping(term2, sig, one, g, neg_zp, pairing);
        BilinearMapping::BilinearMapping(term3, g, one, g, zv, pairing);
        pairing->mul(result, term1, term2);
        pairing->mul(result, result, term3);

        element_init_same_as(temp, result);
        element_set(temp, result);
        element_init_same_as(t, result);
        element_set(t, result);
        element_set(t, temp);
        element_clear(temp);
        
        return;
    }

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

    bool verifiers_check(element_t LHS1, element_t RHS1, element_t LHS2, element_t RHS2){
        //Boolean function for verifier's check for verifying both the conditions
        if (!helper_comparison(LHS1, RHS1) or !helper_comparison(LHS2, RHS2)){
            return false;
        }
        return true;
    }

    bool testing_verification(element_t p, element_t r, Pairing* pairing, element_t comm, element_t g, element_t h, element_t *phi, int n){

        //Main function that calls all other functions for verification 
        element_t s, t, m, v, x, blindsig, sig, D, a, c,zp, zv, zr, y, yc, Dyc, Ap, comm_c, temp1, temp2, LHS1buf, LHS1, RHS1, LHS2, RHS2;
        element_t* signatures = (element_t*)malloc(n*sizeof(element_t));

        pairing->random_Zr(s);
        pairing->random_Zr(t);
        pairing->random_Zr(m);
        pairing->random_Zr(x);
        pairing->random_Zr(v);
        // pg->random_Zr(r);

        pairing->exp(y, g, x);
        verifiers_generate_signature(signatures, x, g, phi, pairing, n);

        cout << "All the verifier's signatures were generated"<<endl;

        
        provers_obtain_signature(Ap, signatures, p, phi, pairing, n);
        verifiers_blind_signature(blindsig, Ap, v, pairing);
        provers_message_D(D, g, s, h, m, pairing);
        provers_message_a(a, blindsig, g, s, t, pairing);

        cout<< "a and D were also formed" <<endl;

        verifiers_challenge(c, pairing);
        cout << "Challenge was thrown" <<endl;
        provers_response(zp, s, p, c, pairing);
        cout <<"zp val obtained" <<endl;
        provers_response(zv, t, v, c, pairing);
        cout<< "zv val obtained" <<endl;
        cout << "The r value is " << r <<endl;
        provers_response(zr, m, r, c, pairing);
        cout<< "zr val obtained" <<endl;
        

        cout<< "Challenge was thrown and three values also received." << endl;

        helper_message_D(LHS1buf, g, zp, h, zr, pairing);        
        pairing->exp(comm_c, comm, c);
        pairing->mul(LHS1, LHS1buf, comm_c);

        element_init_same_as(temp1, D);
        element_set(temp1, D);
        element_init_same_as(RHS1, D);
        element_set(RHS1, temp1);
        element_clear(temp1);

        element_init_same_as(temp2, a);
        element_set(temp2, a);
        element_init_same_as(LHS2, a);
        element_set(LHS2, temp2);
        element_clear(temp2);
        
        verifiers_BilinearRHS(RHS2, blindsig, g, x, c, zp, zv, y, pairing);
        bool verified = verifiers_check(LHS1, RHS1, LHS2, RHS2);
        cout <<"The LHS1 is "<<LHS1<<endl;
        cout <<"The RHS1 is "<<RHS1<<endl;
        cout <<"The LHS2 is "<<LHS2<<endl;
        cout <<"The RHS2 is "<<RHS2<<endl;

        if(verified == true){
            return true;
        }
        return false;
        
    }
}