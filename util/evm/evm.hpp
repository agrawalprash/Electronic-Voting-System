#pragma once

#include "../ballot-generation/ballot.hpp"
#include "../vvpr/vvpr.hpp"

class EVM
{
    public:
        
        EVM(int vote, Ballot* ballot);
        ~EVM();
        void get_vvpr_receipt(VVPR_Receipt* t);
        // 1st we will have the vote
        // then scan the ballot
        // print voter receipt
        // print vvpr receipt

    private:

        Ballot*   ballot;
        element_t C_vote;
        element_t r_vote;
        element_t r_u;
        element_t r_w;
        element_t rid;
        element_t w;

        // Selected Candidate
        int w_m;

        // VVPR_Receipt
        VVPR_Receipt vvpr;
};