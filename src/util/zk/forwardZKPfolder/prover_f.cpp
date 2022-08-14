#include <iostream>
#include <pbc/pbc.h>
#include "common.hpp"
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "zk/forwardZKPfolder/prover_f.hpp"
using namespace std;
using namespace BilinearMapping;
using namespace Commitment;


prover_f::prover_f (element_t g_p, element_t h_p, element_t r_p, element_t comm_p, element_t p_p, int n ){
    element_init_same_as(g, g_p);
    element_set(g, g_p);
    element_init_same_as(h, h_p);
    element_set(h, h_p);
    element_init_same_as(r, r_p);
    element_set(r, r_p);
    element_init_same_as(comm, comm_p);
    element_set(comm, comm_p);
    numOfCandidates = n;
    phi = (element_t*)malloc(numOfCandidates*sizeof(element_t));
    for (long int i=0 ; i<numOfCandidates ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    element_init_same_as(p, p_p);
    element_set(p, p_p);

    pg->random_Zr(v);
    pg->random_Zr(s);
    pg->random_Zr(t);
    pg->random_Zr(m);

}

void prover_f::provers_message_D(element_t D){
        //Returns D = g^s.h^m

    element_t comm_D;

    Commitment::commit(comm_D, g, s, h, m);
    element_init_same_as(D, comm_D);
    element_set(D, comm_D); 

    return;
}

void prover_f::provers_message_a(element_t a, element_t sig){
        //Returns a = e(V,g)^-s. e(g,g)^t
    element_t term1, term2, neg_s, a_temp, temp2;
        
    unsigned int intone= 1;
    unsigned long one = (unsigned long)intone;
    pg->negate(neg_s, s);
    BilinearMapping::BilinearMapping(term1, sig, one, g, neg_s, pg);
    BilinearMapping::BilinearMapping(term2, g, one, g, t, pg);
    pg->mul(a_temp, term1, term2);

    element_init_same_as(a, a_temp);
    element_set(a, a_temp);
    element_init_same_as(temp2, a_temp);
    element_set(temp2, a_temp);
    element_set(a, temp2);
    element_clear(temp2);

    return;
}

int prover_f::helper_index(element_t *phi, element_t el, int n){
    int index = -1;
    for (int i = 0; i < n; i++){
            
        if (element_cmp(phi[i], el) == 0){
            index = i;
            return index;
        }
    }
    return index;
}

void prover_f::provers_obtain_signature(element_t Ap, element_t* signatures){        
        // element_t temp;
        
    int p_buf = helper_index(phi, p, numOfCandidates);
    cout<<"The buffer index is: "<<p_buf<<endl;
    element_init_same_as(Ap, signatures[p_buf]);
        // element_init_same_as(temp, signatures[p_buf]);
    element_set(Ap, signatures[p_buf]);
        // element_clear(temp);

    return;
}

void prover_f::provers_blind_signature(element_t blindsig, element_t sig){
        //Blinds the signature V = Ap^v
    element_t blindsigbuf, temp;

    pg->exp(blindsigbuf, sig, v);
    element_init_same_as(blindsig, blindsigbuf);
    element_set(blindsig, blindsigbuf);
    element_init_same_as(temp, blindsigbuf);
    element_set(temp, blindsigbuf);
    element_set(blindsig, temp);
    element_clear(temp);
    return;
}


void prover_f::provers_response_1(element_t z, element_t c){
        //Returns the response from the prover z = s - pc
    element_t spc, pc, neg_pc;

    pg->mul(pc, p, c);
    pg->negate(neg_pc, pc);
    pg->add(spc, s, neg_pc);
        
    element_init_same_as(z, spc);
    element_set(z, spc);

    return;
}

void prover_f::provers_response_2(element_t z, element_t c){
        //Returns the response from the prover z = t - vc
    element_t spc, pc, neg_pc;

    pg->mul(pc, v, c);
    pg->negate(neg_pc, pc);
    pg->add(spc, t, neg_pc);
        
    element_init_same_as(z, spc);
    element_set(z, spc);

    return;
}

void prover_f::provers_response_3(element_t z, element_t c){
        //Returns the response from the prover z = m - rc
    element_t spc, pc, neg_pc;

    pg->mul(pc, r, c);
    pg->negate(neg_pc, pc);
    pg->add(spc, m, neg_pc);
        
    element_init_same_as(z, spc);
    element_set(z, spc);

    return;
}





