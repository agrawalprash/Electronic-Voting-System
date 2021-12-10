#pragma once

#include "evm.hpp"
#include "common.hpp"
#include "ballot/ballot.hpp"

#include <vector>
#include <string>
#include <iostream>

extern Ballot* ballot_paper[_VOTERS_+1];
extern Voter_Receipt* vt_receipt_list[_VOTERS_+1];
extern EVM *ev;
extern int check_vote;

void partial_evm_receipt(int VoterIndex, int vote);

void evm_vvpr_receipt(int VoterIndex);

Voter_Receipt* voter_receipt(int VoterIndex);

void procedure(int TotalCount);