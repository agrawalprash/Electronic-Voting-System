#pragma once

#include "ballot/ballot.hpp"
#include "vvpr_receipt.hpp"
#include "evm_receipt.hpp"
#include "voter_receipt.hpp"

class EVM
{
    public:
        
        EVM(int vote, Ballot* ballot);
        EVM(int vote);
        ~EVM();
        void get_vvpr_receipt(VVPR_Receipt* t);
        void get_evm_receipt(EVM_Receipt* t);
        void get_voter_receipt(Voter_Receipt* t);
        void get_c_vote(element_t t);
        void ballot_scanning(Ballot *ballot);


    private:

        Ballot*   ballot;
        element_t c_vote;
        element_t r_vote;
        element_t r_u;
        element_t r_w;
        element_t rid;
        element_t w;

        // Selected Candidate
        int w_m;
        int vote;

        // VVPR_Receipt
        VVPR_Receipt vvpr;

        // EVM_Receipt
        EVM_Receipt evm_receipt;
};