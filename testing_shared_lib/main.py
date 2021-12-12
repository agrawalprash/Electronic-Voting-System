import voting
import random
import numpy as np


TotalCount = 5
m = voting.class_generation(TotalCount)

def cand_e_m(candidates_w_m):
    candidates = []
    w_m = []

    for i in candidates_w_m:
        val = i.split(",")
        candidates.append(val[0])
        w_m.append(val[1])
    return candidates, w_m


for VoterIndex in range(2):

    print("========================================= Voter:{} =========================================".format(VoterIndex+1))

    candidates_w_m = m.candidate_list(VoterIndex)
    candidates, w_m = cand_e_m(candidates_w_m)

    vote = random.randint(0,len(candidates)-1)

    print(candidates)
    print(w_m)
    print("Vote:",vote)


    candidate_selection = m.candidate_selection(VoterIndex, vote)

    partial_evm_receipt = m.partial_evm_receipt(VoterIndex, vote)
    print("Partial_EVM_Receipt:",partial_evm_receipt)

    ballot_scanning = m.ballot_scanning(VoterIndex)
    print("\nBallot_Scanning successful ...\n")

    evm_vvpr_receipt = m.evm_vvpr_receipt(VoterIndex)
    print("\nEvm vvpr receipt:",evm_vvpr_receipt,'\n')

    voter_receipt = m.voter_receipt(VoterIndex)
    print("Voter Receipt:",voter_receipt,'\n\n')
