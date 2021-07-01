#pragma once

#include "../evm/evm.hpp"
#include "../common/common.hpp"
#include "../ballot-generation/ballot.hpp"

#include <vector>
#include <string>
#include <iostream>

extern Ballot* ballot_paper[_VOTERS_+1];
extern EVM *ev;
extern int check_vote;

void generateBallot(int TotalCount);

void partial_evm_receipt(int VoterIndex, int vote);

void evm_vvpr_receipt(int VoterIndex);

void voter_receipt(int VoterIndex);

void procedure(int TotalCount);