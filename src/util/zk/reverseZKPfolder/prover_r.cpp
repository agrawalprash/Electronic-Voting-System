#include "zk/reverseZKPfolder/prover_r.hpp"

prover_r::prover_r(element_t g_p, element_t h_p, element_t p_p, element_t c_p, element_t rval_p, element_t *r_p, element_t *commitments_p, element_t *p_vals_p, int n, int m){
    element_init_same_as(g, g_p);
    element_set(g, g_p);
    element_init_same_as(h, h_p);
    element_set(h, h_p);
    element_init_same_as(p, p_p);
    element_set(p, p_p);
    element_init_same_as(C, c_p);
    element_set(C, c_p);
    element_init_same_as(rval, rval_p);
    element_set(rval, rval_p);
    numOfCandidates = n;
    numOfCommitments = m;
    commitments = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    r = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    pvals = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    for (int i=0; i<numOfCommitments ; i++) {
        element_init_same_as(commitments[i], commitments_p[i]);
        element_set(commitments[i], commitments_p[i]);
        element_init_same_as(r[i], r_p[i]);
        element_set(r[i], r_p[i]);
        element_init_same_as(pvals[i], p_vals_p[i]);
        element_set(pvals[i], p_vals_p[i]);
    }
}

void prover_r::pok_piecom_prover(element_t challenge, element_t z1, element_t z2, int i){

    //cout<<"Entered pok_piecom_prover_for_iindex "<<i<<endl;

    element_t commitment, ri, pi;
    element_init_same_as(commitment, commitments[i]);
    element_set(commitment, commitments[i]);
    element_init_same_as(ri, r[i]);
    element_set(ri, r[i]);
    element_init_same_as(pi, pvals[i]);
    element_set(pi, pvals[i]);

    cout<<pi<<endl;
    cout<<ri<<endl;

    cout<<"init is fine"<<endl;

    element_t r1,r2,s;
    pg->random_Zr(r1);
    pg->random_Zr(r2);
    element_t temp1,temp2, temp3;
    pg->exp(temp1, g,r1);
    pg->exp(temp2, h, r2);

    pg->mul(temp3, temp1, temp2);
    element_init_same_as(s, temp3);
    element_set(s, temp3);

    element_t tuple[4];
    element_init_same_as(tuple[0], g);
    element_set(tuple[0], g);
    element_init_same_as(tuple[1], h);
    element_set(tuple[1], h);
    element_init_same_as(tuple[2], C);
    element_set(tuple[2], C);
    element_init_same_as(tuple[3], s);
    element_set(tuple[3] , s);

    cout<<"Tuples made"<<endl;

    char c_msg[_ARRAY_SIZE_];
    element_snprintf(c_msg, _ARRAY_SIZE_, "%B", tuple);

    SHA256 sha256;
    string s_hash = sha256(c_msg);
    char c_hash[s_hash.length()+1];
    strcpy(c_hash, s_hash.c_str());

    cout<<"c_hash done"<<endl;

    element_init_same_as(challenge, r1);

    element_from_hash(challenge, c_hash, s_hash.size());

    cout<<"Hash done"<<endl;

    element_t temp4, temp5, temp6, temp7, temp8, temp9;
    pg->mul(temp4, pi, challenge);
    pg->mul(temp5, ri, challenge);
    pg->negate(temp6, temp4);
    pg->negate(temp7, temp5);
    pg->add(temp8, r1, temp6);
    pg->add(temp9, r2, temp7);

    element_init_same_as(z1, temp8);
    element_set(z1, temp8);
    element_init_same_as(z2, temp9);
    element_set(z2, temp9);

    element_init_same_as(commitment, C);
    element_set(commitment, C);

    cout<<commitment<<endl;
    cout<<z1<<endl;
    cout<<z2<<endl;


    element_clear(temp1);
    element_clear(temp2);
    element_clear(temp3);
    element_clear(temp4);
    element_clear(temp5);
    element_clear(temp6);
    element_clear(temp7);
    element_clear(temp8);
    element_clear(temp9);

    cout<<"func com[;ete"<<endl;

}

int prover_r::find_index() {
    int index = 0;
    for(int i=0 ; i<numOfCommitments; i++) {
        element_t comm;
        Commitment::commit(comm, g, p, h, r[i]);
        if(element_cmp(comm, C) == 0) {
            cout<<"The index is: "<<i<<endl;
            return i;
        }
    }
    return index;
}

void prover_r::compute_B1(element_t b1, element_t s1, element_t s2) {
    element_t helper1, helper2;
    pg->exp(helper1, g, s1);
    pg->exp(helper2, h, s2);
    pg->mul(b1, helper1, helper2);
    return ;
}

void prover_r::compute_B2(element_t b2, element_t Ai ,element_t s2) {
    element_t helper1;
    pg->exp(helper1, g, s2);
    pg->mul(b2, Ai, helper1);
    return ;
}

void prover_r::compute_LHS1(element_t t, element_t b1, element_t c) {
    element_t temp;
    pg->exp(temp, b1,c);

    element_init_same_as(t, temp);
    element_set(t, temp);

    element_clear(temp);
}

void prover_r::compute_RHS1(element_t t, element_t delta1, element_t delta2){
    element_t temp1, temp2, temp3;
    pg->exp(temp1, g,delta1);
    pg->exp(temp2, h, delta2);
    pg->mul(temp3, temp1,temp2);

    element_init_same_as(t, temp3);
    element_set(t, temp3);
}

bool prover_r::helper_comparison(element_t a, element_t b){
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

void prover_r::compute_LHS_pieSig(element_t t, element_t B2, element_t y) {
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

void prover_r::compute_RHS_pieSig(element_t t, element_t B2, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r)  {
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

void prover_r::prover_gives_r_double_dash_array(element_t B1,element_t B2, element_t challenge, element_t z1,element_t z2,element_t z3,element_t z4,element_t z5,element_t z6,element_t  B1_c,element_t  LHS_piesig_val,element_t *A_array, element_t *c_array, element_t *r_dash_array, element_t y) {
    element_t *r_double_dash = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    for (int i=0 ; i<numOfCommitments; i++) {
        element_t temp;
        pg->add(temp, r[i], r_dash_array[i]);
        element_init_same_as(r_double_dash[i], temp);
        element_set(r_double_dash[i], temp);
    }
    int ind = find_index();
    element_t s1, s2, delta1, delta2;
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
    compute_B1(B1, s1, s2);
    compute_B2(B2, A_array[ind], s2);
    pg->mul(delta1, s1, c_array[ind]);
    pg->mul(delta2, s2, c_array[ind]);

    compute_LHS1(B1_c, B1, c_array[ind]);
    compute_LHS_pieSig(LHS_piesig_val, B2, y);
    pok2(challenge, z1,z2,z3,z4,z5,z6, s1,s2,c_array[ind], r_double_dash[ind], delta1, delta2, B1,B2,y, B1_c, LHS_piesig_val);
    
    /*
    element_t tuple[8];
    element_init_same_as(tuple[0], g);
    element_set(tuple[0], g);
    element_init_same_as(tuple[1], h);
    element_set(tuple[1], h);
    element_init_same_as(tuple[2], B1);
    element_set(tuple[2], C);
    element_init_same_as(tuple[3], B1_c);
    element_set(tuple[3] , B1_c);
    element_init_same_as(tuple[4], LHS_piesig_val);
    element_set(tuple[4] , LHS_piesig_val);

    element_t temp1, temp2, verif1, verif2, verif3;

    pg->exp(temp1, B1, challenge);
    compute_RHS1(temp2, z1,z2);
    pg->mul(verif1, temp1, temp2);
    element_init_same_as(tuple[5], verif1);
    element_set(tuple[5], verif1);

    pg->exp(temp1, B1_c, challenge);
    compute_RHS1(temp2, z5,z6);
    pg->mul(verif2, temp1, temp2);
    element_init_same_as(tuple[6], verif2);
    element_set(tuple[6], verif2);

    pg->exp(temp1, LHS_piesig_val, challenge);
    compute_RHS_pieSig(temp2, B2, z3, y, z2, z6, p, z4);
    pg->mul(verif3, temp1, temp2);
    element_init_same_as(tuple[7], verif3);
    element_set(tuple[7], verif3);


    cout<<"Tuples made2"<<endl;

    char c_msg[_ARRAY_SIZE_];
    element_snprintf(c_msg, _ARRAY_SIZE_, "%B", tuple);

    element_t hashed_val;
    SHA256 sha256;
    string s_hash = sha256(c_msg);
    char c_hash[s_hash.length()+1];
    strcpy(c_hash, s_hash.c_str());

    cout<<"c_hash done2"<<endl;
    element_init_same_as(hashed_val, z1);
    element_from_hash(hashed_val, c_hash, s_hash.size());
    cout<<"Hash done2"<<endl;

    cout<<"Challenge is "<<challenge<<endl;
    cout<<"hashed valeu is "<<hashed_val<<endl;

    if(element_cmp(challenge, hashed_val) == 0){
        return true;
    }
    return false;
    
*/
    

/*
    element_t LHS1, RHS1;
    compute_LHS1(LHS1, b1, c_array[ind]);
    compute_RHS1(RHS1, delta1, delta2);
    if(!helper_comparison(LHS1 , RHS1)){
        return false;
    }
    element_t LHS_val, RHS_val;
    compute_LHS_pieSig(LHS_val, b2, y);
    cout<<"LHS_value: "<<LHS_val<<endl;
    compute_RHS_pieSig(RHS_val, b2, c_array[ind], y ,s2,delta2, p, r_double_dash[ind]);
    cout<<"RHS_value: "<<RHS_val<<endl;
    if(!helper_comparison(LHS_val , RHS_val)){
        return false;
    }
    return true;
    */
    

}

void prover_r::pok2(element_t challenge, element_t z1, element_t z2,element_t z3, element_t z4, element_t z5, element_t z6,  element_t s1, element_t s2, element_t c_val, element_t r_val, element_t delta1, element_t delta2, element_t b1, element_t b2, element_t y, element_t B1_c, element_t LHS_piesig_val) {
    cout<<"pok2 entered"<<endl;
    element_t ran1, ran2, ran3, ran4, ran5, ran6;
    pg->random_Zr(ran1); //s1
    pg->random_Zr(ran2); //s2
    pg->random_Zr(ran3); //c_val
    pg->random_Zr(ran4); //r_val
    pg->random_Zr(ran5); //delta1
    pg->random_Zr(ran6); //delta2

    element_t s1s2_commit, delta_commit, pair_muls;
    
    compute_RHS1(s1s2_commit, ran1, ran2);
    compute_RHS1(delta_commit, ran5, ran6);
    compute_RHS_pieSig(pair_muls, b2, ran3, y ,ran2, ran6, p, ran4);

    element_t tuple[8];
    element_init_same_as(tuple[0], g);
    element_set(tuple[0], g);
    element_init_same_as(tuple[1], h);
    element_set(tuple[1], h);
    element_init_same_as(tuple[2], b1);
    element_set(tuple[2], b1);
    element_init_same_as(tuple[3], B1_c);
    element_set(tuple[3] , B1_c);
    element_init_same_as(tuple[4], LHS_piesig_val);
    element_set(tuple[4] , LHS_piesig_val);
    element_init_same_as(tuple[5], s1s2_commit);
    element_set(tuple[5] , s1s2_commit);
    element_init_same_as(tuple[6], delta_commit);
    element_set(tuple[6] , delta_commit);
    element_init_same_as(tuple[7], pair_muls);
    element_set(tuple[7] , pair_muls);

    cout<<"Tuples made"<<endl;

    char c_msg[_ARRAY_SIZE_];
    element_snprintf(c_msg, _ARRAY_SIZE_, "%B", tuple);

    SHA256 sha256;
    string s_hash = sha256(c_msg);
    char c_hash[s_hash.length()+1];
    strcpy(c_hash, s_hash.c_str());

    cout<<"c_hash done"<<endl;
    element_init_same_as(challenge, r_val);
    element_from_hash(challenge, c_hash, s_hash.size());
    cout<<"Hash done"<<endl;

    element_t temp4, temp5, temp6;

    pg->mul(temp4, s1, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran1, temp5);
    element_init_same_as(z1, temp6);
    element_set(z1, temp6);

    pg->mul(temp4, s2, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran2, temp5);
    element_init_same_as(z2, temp6);
    element_set(z2, temp6);

    pg->mul(temp4, c_val, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran3, temp5);
    element_init_same_as(z3, temp6);
    element_set(z3, temp6);

    pg->mul(temp4, r_val, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran4, temp5);
    element_init_same_as(z4, temp6);
    element_set(z4, temp6);

    pg->mul(temp4, delta1, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran5, temp5);
    element_init_same_as(z5, temp6);
    element_set(z5, temp6);

    pg->mul(temp4, delta2, challenge);
    pg->negate(temp5, temp4);
    pg->add(temp6, ran6, temp5);
    element_init_same_as(z6, temp6);
    element_set(z6, temp6);

}

