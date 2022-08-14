#include "zk/reverseZKPfolder/verifier_r.hpp"

verifier_r::verifier_r(element_t g_p, element_t h_p, element_t p_p, element_t *commitments_p, int n, int m, prover_r *prover_p){
    element_init_same_as(g, g_p);
    element_set(g, g_p);
    element_init_same_as(h, h_p);
    element_set(h, h_p);
    element_init_same_as(p, p_p);
    element_set(p, p_p);
    numOfCandidates = n;
    numOfCommitments = m;
    commitments = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    for (int i=0; i<numOfCommitments ; i++) {
        element_init_same_as(commitments[i], commitments_p[i]);
        element_set(commitments[i], commitments_p[i]);
    }
    prover = prover_p;
}


bool verifier_r::pok1_pie_com_verifier(){

    bool pie_com = true;

    for(int i=0 ; i<numOfCommitments ; i++) {
        element_t challenge,z1,z2, C;
        prover->pok_piecom_prover(challenge, z1,z2, i);

        cout<<"pie_com_prover_done"<<endl;
        element_init_same_as(C, commitments[i]);
        element_set(C, commitments[i]);

        element_t t1,t2,t3,t4,t5, verif, hash_val;
        pg->exp(t1, C, challenge);
        pg->exp(t2, g, z1);
        pg->exp(t3, h, z2);
        pg->mul(t4, t1,t2);
        pg->mul(t5, t4, t3);
        element_init_same_as(verif, t5);
        element_set(verif, t5);

        element_t tuple[4];
        element_init_same_as(tuple[0], g);
        element_set(tuple[0], g);
        element_init_same_as(tuple[1], h);
        element_set(tuple[1], h);
        element_init_same_as(tuple[2], C);
        element_set(tuple[2], C);
        element_init_same_as(tuple[3], verif);
        element_set(tuple[3] , verif);

        char c_msg[_ARRAY_SIZE_];
        element_snprintf(c_msg, _ARRAY_SIZE_, "%B", tuple);

        SHA256 sha256;
        string s_hash = sha256(c_msg);
        char c_hash[s_hash.length()+1];
        strcpy(c_hash, s_hash.c_str());

        element_init_same_as(hash_val, challenge);

        element_from_hash(hash_val, c_hash, s_hash.size());

        if(element_cmp(hash_val, challenge) == 1)
        {
            pie_com = false;
        }
        cout<<"rest also done  afterpie_com_prover_done"<<endl;
    }
    
    return pie_com;

    
}
  

bool verifier_r::implementReverseZKP(){

    //cout<<"Entered implementRZKP"<<endl;

    bool pie_com = pok1_pie_com_verifier();
    //cout<<"The pie_com verifier val is "<<pie_com<<endl;
    if (pie_com == false){
        return false;
    }

    element_t x,y;
    pg->random_Zr(x);
    while (x == 0) {
        pg->random_Zr(x);
    }
    pg->exp(y, g, x);

    element_t* A_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    element_t* c_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    element_t* ri_dash_array = (element_t*)malloc(numOfCommitments*sizeof(element_t));

    for(int i=0 ; i<numOfCommitments ; i++){
        element_t ci_plus_x, inverse_cix, h_power_ri, Ai_helper1, Ai_helper2;
        element_init_Zr(c_array[i], pg->pairing);
        pg->random_Zr(c_array[i]);
        // element_init_same_as(ri_dash, r);
        pg->random_Zr(ri_dash_array[i]);
        //cout<<"Before adding cix"<<endl;
        pg->add(ci_plus_x, c_array[i], x);
        pg->inverse(inverse_cix, ci_plus_x);

        pg->exp(h_power_ri, h, ri_dash_array[i]);
        pg->mul(Ai_helper1, g,h_power_ri);
        pg->mul(Ai_helper2, Ai_helper1, commitments[i]);
        element_init_same_as(A_array[i], Ai_helper2);
        pg->exp(A_array[i], Ai_helper2, inverse_cix);
    }
    
    element_t B1,B2,challenge, z1,z2,z3,z4,z5,z6, B1_c, LHS_piesig_val;
    prover->prover_gives_r_double_dash_array(B1,B2,challenge, z1,z2,z3,z4,z5,z6, B1_c, LHS_piesig_val,A_array, c_array, ri_dash_array, y);

    element_t tuple[8];
    element_init_same_as(tuple[0], g);
    element_set(tuple[0], g);
    element_init_same_as(tuple[1], h);
    element_set(tuple[1], h);
    element_init_same_as(tuple[2], B1);
    element_set(tuple[2], B1);
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

}

void verifier_r::compute_RHS1(element_t t, element_t delta1, element_t delta2){
    element_t temp1, temp2, temp3;
    pg->exp(temp1, g,delta1);
    pg->exp(temp2, h, delta2);
    pg->mul(temp3, temp1,temp2);

    element_init_same_as(t, temp3);
    element_set(t, temp3);
}

void verifier_r::compute_RHS_pieSig(element_t t, element_t B2, element_t c, element_t y, element_t s2, element_t delta2, element_t p, element_t r)  {
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
