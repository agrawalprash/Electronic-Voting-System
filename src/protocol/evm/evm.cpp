#include "evm/evm.hpp"

EVM::EVM()
{
    BB1.clear();
    BB2.clear();
    R_vote.clear();
}

void EVM::candidate_selection(int vote_)
{
    pg->random_Zr(r_vote);
    vote = vote_;
    cout << "The voted candidate is "<< vote <<endl;
    element_t temp;
    element_init_same_as(temp, r_vote);
    element_set_si(temp, vote_);
    Commitment::commit(c_vote, temp, r_vote, pg);
    cout<< "r_val is: "<< r_vote <<endl;
    element_clear(temp);
}

void EVM::ballot_scanning(Ballot *ballot)
{
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
    
    ballot->get_rid(rid);
    ballot->get_c_rid(c_rid);

    // vvpr    
    generate_vvpr();

    //evm_receipt
    generate_evm();

    //Database Population
    // populate_bb1();
    // populate_bb2();

    element_clear(obf_token);
}

void EVM::generate_evm()
{
    evm_receipt.w_m = w_m;
    element_init_same_as(evm_receipt.c_vote, c_vote);
    element_set(evm_receipt.c_vote, c_vote);

    element_init_same_as(evm_receipt.w, w);
    element_set(evm_receipt.w, w);

    element_init_same_as(evm_receipt.r_w, r_w);
    element_set(evm_receipt.r_w, r_w);
}

void EVM::generate_vvpr()
{
    vvpr.vote = vote;
    element_init_same_as(vvpr.rid, rid);
    element_set(vvpr.rid, rid); 
}

void EVM::populate_bb1()
{

    cout<<"This function ran for population"<<endl;

    BB1_element *bb1 = new BB1_element();
    element_init_same_as(bb1->c_rid, c_rid);
    element_set(bb1->c_rid, c_rid);

    element_init_same_as(bb1->c_vote, c_vote);
    element_set(bb1->c_vote, c_vote);
    BB1.push_back(*bb1);
    delete(bb1);
}

void EVM::populate_bb2()
{
    BB2_element* bb2 = new BB2_element();
    element_init_same_as(bb2->rid, rid);
    element_set(bb2->rid, rid);

    bb2->candidate = Candidate_List[vote];
    BB2.push_back(*bb2);
    delete(bb2);
}

void EVM::populate_rvote()
{
    Rvote_element* rv = new Rvote_element();
    element_init_same_as(rv->r_vote, r_vote);
    element_set(rv->r_vote, r_vote);

    R_vote.push_back(*rv);
    delete(rv);
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

    element_init_same_as(t->c_vote, evm_receipt.c_vote);
    element_set(t->c_vote, evm_receipt.c_vote);
}

void EVM::get_voter_receipt(Voter_Receipt* t)
{
    t->w_m = evm_receipt.w_m;
    
    element_init_same_as(t->w, evm_receipt.w);
    element_set(t->w, evm_receipt.w);
    
    element_init_same_as(t->r_w, evm_receipt.r_w);    
    element_set(t->r_w, evm_receipt.r_w);

    element_init_same_as(t->c_vote, evm_receipt.c_vote);
    element_set(t->c_vote, evm_receipt.c_vote);
}

void EVM::get_c_vote(element_t t)
{
    element_init_same_as(t, c_vote);
    element_set(t, c_vote);
}
