#include "evm.hpp"

EVM::EVM(int vote_)
{
    pg->random_Zr(r_vote);
    vote = vote_;

    element_t temp;
    element_init_same_as(temp, r_vote);
    element_set_si(temp, vote_);
    Commitment::commit(C_vote, temp, r_vote, pg);

    element_clear(temp);
}

EVM::EVM(int vote_, Ballot* b)
{
    ballot = b;
    element_t obf_token;
    ballot->get_obf_token(obf_token);

    pg->random_Zr(r_vote);

    element_t vote;
    element_init_same_as(vote, r_vote);
    element_set_si(vote, vote_);
    Commitment::commit(C_vote, vote, r_vote, pg);

    element_init_same_as(w, obf_token);
    element_add_ui(w, obf_token, vote_);
    
    int total_candidates;
    ballot->get_total_candidates(total_candidates);
    
    int obf_token_rem;
    ballot->get_obf_token_rem(obf_token_rem);
    
    w_m = (obf_token_rem + vote_)%total_candidates;

    ballot->get_r_obf_token(r_u);
    pg->add(r_w, r_vote, r_u);  

    // vvpr
    ballot->get_rid(rid);
    vvpr.vote = vote_;
    element_init_same_as(vvpr.rid, rid);
    element_set(vvpr.rid, rid); 

    element_clear(obf_token);
    element_clear(vote);
}

void EVM::get_vvpr_receipt(VVPR_Receipt* t)
{    
    t->vote = vvpr.vote;
    element_init_same_as(t->rid, vvpr.rid);
    element_set(t->rid, vvpr.rid);
}

void EVM::get_evm_receipt(EVM_Receipt* t)
{
    t->w_m = evm_receipt.w_m;
    
    element_init_same_as(t->w, evm_receipt.w);
    element_set(t->w, evm_receipt.w);
    
    element_init_same_as(t->r_w, evm_receipt.r_w);    
    element_set(t->r_w, evm_receipt.r_w);

    element_init_same_as(t->C_vote, evm_receipt.C_vote);
    element_set(t->C_vote, evm_receipt.C_vote);
}

void EVM::get_voter_receipt(Voter_Receipt* t)
{
    t->w_m = evm_receipt.w_m;
    
    element_init_same_as(t->w, evm_receipt.w);
    element_set(t->w, evm_receipt.w);
    
    element_init_same_as(t->r_w, evm_receipt.r_w);    
    element_set(t->r_w, evm_receipt.r_w);

    element_init_same_as(t->C_vote, evm_receipt.C_vote);
    element_set(t->C_vote, evm_receipt.C_vote);
}

void EVM::get_C_vote(element_t t)
{
    element_init_same_as(t, C_vote);
    element_set(t, C_vote);
}

void EVM::ballot_scanning(Ballot *b)
{
    ballot = b;

    element_t obf_token;
    ballot->get_obf_token(obf_token);

    element_init_same_as(w, obf_token);
    element_add_ui(w, obf_token, vote); // w = u + vote

    int total_candidates;
    ballot->get_total_candidates(total_candidates); // m
    
    int obf_token_rem;
    ballot->get_obf_token_rem(obf_token_rem); // u mod m
    
    w_m = (obf_token_rem + vote)%total_candidates; // w_m = (u + vote) mod m

    ballot->get_r_obf_token(r_u);
    pg->add(r_w, r_vote, r_u);  

    // vvpr
    ballot->get_rid(rid);
    vvpr.vote = vote;
    element_init_same_as(vvpr.rid, rid);
    element_set(vvpr.rid, rid); 

    //evm_receipt
    evm_receipt.w_m = w_m;
    element_init_same_as(evm_receipt.C_vote, C_vote);
    element_set(evm_receipt.C_vote, C_vote);
    element_init_same_as(evm_receipt.w, w);
    element_set(evm_receipt.w, w);
    element_init_same_as(evm_receipt.r_w, r_w);
    element_set(evm_receipt.r_w, r_w);

    element_clear(obf_token);
}
