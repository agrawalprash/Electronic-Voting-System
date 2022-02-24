#pragma once

#include "ballot/ballot.hpp"
#include "evm/vvpr_receipt.hpp"
#include "evm/evm_receipt.hpp"
#include "evm/voter_receipt.hpp"
#include "evm/bb1_element.hpp"
#include "evm/bb2_element.hpp"

class EVM
{
    public:

        EVM();
        ~EVM();
        
        void get_vvpr_receipt(VVPR_Receipt* t);
        void get_evm_receipt(EVM_Receipt* t);
        void get_voter_receipt(Voter_Receipt* t);
        void get_c_vote(element_t t);
        
        void candidate_selection(int vote);
        void ballot_scanning(Ballot *ballot);

        // Bulletin Board
        vector<BB1_element> BB1;
        vector<BB2_element> BB2;

    private:

        element_t c_vote;
        element_t r_vote;
        element_t c_rid;
        element_t rid;
        element_t r_u;
        element_t r_w;
        element_t w;

        // Selected Candidate
        int w_m;
        int vote;

        // VVPR_Receipt
        VVPR_Receipt vvpr;

        // EVM_Receipt
        EVM_Receipt evm_receipt;

        // Private functions
        void generate_vvpr();
        void generate_evm();
        void populate_bb1();
        void populate_bb2();
};