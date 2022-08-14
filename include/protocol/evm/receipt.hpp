#pragma once

#include "evm.hpp"
#include "common.hpp"
#include "ballot/ballot.hpp"
#include "pairing/pairing.hpp"
#include "zk/set_membership_zkp.h"
#include "zk/reverse_set_membership.hpp"
#include "zk/forwardZKPfolder/verifier_f.hpp"
#include "zk/reverseZKPfolder/verifier_r.hpp"

#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace ZKPSetMembership;
using namespace reverseZKPSetMembership;

extern Ballot* ballot_paper[_VOTERS_+3];
extern Voter_Receipt* vt_receipt_list[_VOTERS_+3];
extern EVM *ev;
extern int check_vote;

map<string,string> ballot_paper_wrapper(int VoterIndex);

void ballot_scanning_wrapper(int VoterIndex);

string partial_evm_receipt(int VoterIndex, int vote);

map<string,string> evm_vvpr_receipt(int VoterIndex);

map<string,string> voter_receipt(int VoterIndex);

void procedure(int TotalCount);

bool forwardZeroKP(); // new functionality adding for ZKP

bool reverseZeroKP(); //new functionality adding for ZKP