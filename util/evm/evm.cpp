#include "evm.hpp"

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
    vvpr.name = Candidate_List[vote_];
    element_init_same_as(vvpr.rid, rid);
    element_set(vvpr.rid, rid); 

    element_clear(obf_token);
    element_clear(vote);
}

void EVM::get_vvpr_receipt(VVPR_Receipt* t)
{    
    t->name = vvpr.name;
    t->vote = vvpr.vote;
    element_init_same_as(t->rid, vvpr.rid);
    element_set(t->rid, vvpr.rid);
}

