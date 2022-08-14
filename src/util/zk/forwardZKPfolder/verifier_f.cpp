#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "zk/forwardZKPfolder/verifier_f.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;


verifier_f::verifier_f (element_t g_p, element_t h_p, element_t comm_p , int n, prover_f *prover_p){
    element_init_same_as(g, g_p);
    element_set(g, g_p);
    element_init_same_as(h, h_p);
    element_set(h, h_p);
    element_init_same_as(comm, comm_p);
    element_set(comm, comm_p);
    numOfCandidates = n;
    phi = (element_t*)malloc(numOfCandidates*sizeof(element_t));
    for (long int i=0 ; i<numOfCandidates ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    prover = prover_p;
}

void verifier_f::verifiers_generate_signature(element_t* signatures, element_t x){
    element_t xp_sums[numOfCandidates];
    element_t sig_pows[numOfCandidates];
    element_t sig_temps[numOfCandidates];

        //Returns signature Ap = g^(1/x+p)

    for (int i = 0; i < numOfCandidates; i++){
        
        element_t xp_sum, sig_pow, sig_temp, temp, sig;
        pg->add(xp_sum, x, phi[i]);
            // cerr << "xp sum : " <<xp_sum << endl;
        element_init_same_as(xp_sums[i], xp_sum);
        element_set(xp_sums[i], xp_sum);
            
        pg->inverse(sig_pow, xp_sum);
        element_init_same_as(sig_pows[i], sig_pow);
        element_set(sig_pows[i], sig_pow);
        // cerr << sig_pow << endl;
        
        pg->exp(sig_temp, g, sig_pow);
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

void verifier_f::verifiers_challenge(element_t t){
        //Generates a random challenge c for the prover
    element_t c, temp;

    pg->random_Zr(c);
    element_init_same_as(temp, c);
    element_set(temp, c);
    element_init_same_as(t, c);
    element_set(t, c);
    element_clear(temp);

    return;
}

void verifier_f::verifiers_BilinearRHS(element_t t, element_t sig, element_t g, element_t x, element_t c, element_t zp, element_t zv, element_t y){
        //Returns e(V,y)^c. e(V,g)^-zp. e(g,g)^zv
    
    element_t neg_zp, term1, term2, term3, result, temp;
    
    unsigned int intone= 1;
    unsigned long one = (unsigned long)intone;
    pg->negate(neg_zp, zp);
    BilinearMapping::BilinearMapping(term1, sig, one, y, c, pg);
    BilinearMapping::BilinearMapping(term2, sig, one, g, neg_zp, pg);
    BilinearMapping::BilinearMapping(term3, g, one, g, zv, pg);
    pg->mul(result, term1, term2);
    pg->mul(result, result, term3);

    element_init_same_as(temp, result);
    element_set(temp, result);
    element_init_same_as(t, result);
    element_set(t, result);
    element_clear(temp);
        
    return;
}

bool verifier_f::helper_comparison(element_t a, element_t b){
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

void verifier_f::helper_message_D(element_t D, element_t zp, element_t zr){
        //Returns D = g^s.h^m

    element_t comm_D;

    Commitment::commit(comm_D, g, zp, h, zr);
    element_init_same_as(D, comm_D);
    element_set(D, comm_D);
 

    return;
} 

bool verifier_f::verifiers_check(element_t LHS1, element_t RHS1, element_t LHS2, element_t RHS2){
        //Boolean function for verifier's check for verifying both the conditions
    if (!helper_comparison(LHS1, RHS1) or !helper_comparison(LHS2, RHS2)){
        return false;
    }
    return true;
}

bool verifier_f::implementForwardZKP() {

    element_t x, blindsig, sig, D, a, c,zp, zv, zr, y, yc, Dyc, Ap, comm_c, temp1, temp2, LHS1buf, LHS1, RHS1, LHS2, RHS2;
        element_t* signatures = (element_t*)malloc(5*sizeof(element_t));

        pg->random_Zr(x);
        //pg->random_Zr(v);
        // pg->random_Zr(r);

        pg->exp(y, g, x);
        verifiers_generate_signature(signatures, x);

        cout << "All the verifier's signatures were generated"<<endl;

        
        prover->provers_obtain_signature(Ap, signatures);
        prover->provers_blind_signature(blindsig, Ap);
        prover->provers_message_D(D);
        prover->provers_message_a(a, blindsig);

        cout<< "a and D were also formed" <<endl;

        verifiers_challenge(c);
        cout << "Challenge was thrown" <<endl;
        prover->provers_response_1(zp, c);
        cout <<"zp val obtained" <<endl;
        prover->provers_response_2(zv, c);
        cout<< "zv val obtained" <<endl;
        //cout << "The r value is " << r <<endl;
        prover->provers_response_3(zr, c);
        cout<< "zr val obtained" <<endl;
        

        cout<< "Challenge was thrown and three values also received." << endl;

        helper_message_D(LHS1buf, zp,zr);        
        pg->exp(comm_c, comm, c);
        pg->mul(LHS1, LHS1buf, comm_c);

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
        
        verifiers_BilinearRHS(RHS2, blindsig, g, x, c, zp, zv, y);
        bool verified = verifiers_check(LHS1, RHS1, LHS2, RHS2);
        cout <<"The LHS1 is "<<LHS1<<endl;
        cout <<"The RHS1 is "<<RHS1<<endl;
        cout <<"The LHS2 is "<<LHS2<<endl;
        cout <<"The RHS2 is "<<RHS2<<endl;

        if(verified == true){
            return true;
        }
        return false;











/*



    element_t v, x, blindsig, sig, D, a, c,zp, zv, zr, y, yc, Dyc, Ap, comm_c, temp1, temp2, LHS1buf, LHS1, RHS1, LHS2, RHS2;
    element_t* signatures = (element_t*)malloc(numOfCandidates*sizeof(element_t));

    pg->random_Zr(x);
    pg->random_Zr(v);

    pg->exp(y, g, x);
    verifiers_generate_signature(signatures, x);
    prover->provers_obtain_signature(Ap, signatures);
    verifiers_blind_signature(blindsig, Ap, v);
    prover->provers_message_D(D);
    prover->provers_message_a(a, blindsig);

    verifiers_challenge(c);
    cout<<"The challenge value is: "<<c<<endl;
    prover->provers_response_1(zp, c);
    prover->provers_response_2(zv, c);
    prover->provers_response_3(zr, c);
    cout<<"Zp: "<<zp<<endl;
    cout<<"Zv: "<<zv<<endl;
    cout<<"Zr: "<<zr<<endl;

    cout<< "Challenge was thrown and three values also received." << endl;

    element_init_same_as(LHS1buf, D);
    element_set(LHS1buf, D);
    helper_message_D(LHS1buf, zp, zr);        
    pg->exp(comm_c, comm, c);
    pg->mul(LHS1, LHS1buf, comm_c);

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
        
    verifiers_BilinearRHS(RHS2, blindsig, g, x, c, zp, zv, y);
    bool verified = verifiers_check(LHS1, RHS1, LHS2, RHS2);
    cout <<"The LHS1 is "<<LHS1<<endl;
    cout <<"The RHS1 is "<<RHS1<<endl;
    cout <<"The LHS2 is "<<LHS2<<endl;
    cout <<"The RHS2 is "<<RHS2<<endl;

    if(verified == true){
        return true;
    }
    return false;

    */
        
}

// void verifier_f::generate_publickey(element_t y, element_t g){
//     element_t x;
//     pg->random_Zr(x);
//     pg->exp(y,g,x);
// }
