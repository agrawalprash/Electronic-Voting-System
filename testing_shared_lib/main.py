import voting
# print(dir(voting.protocol))

# x = voting.func(10)
# print(x)
TotalCount = 5
m = voting.class_generation(TotalCount)

for VoterIndex in range(1):
    vote = m.candidate_list(VoterIndex)
    print("Vote:",vote)
    # vote =1

    candidate_selection = m.candidate_selection(VoterIndex, vote)
    # print("Candidate selection:",candidate_selection)

    partial_evm_receipt = m.partial_evm_receipt(VoterIndex, vote)
    print("Partial_EVM_Receipt:",partial_evm_receipt)

    ballot_scanning = m.ballot_scanning(VoterIndex)
    print("Ballot_Scanning:",ballot_scanning,'\n')

    evm_vvpr_receipt = m.evm_vvpr_receipt(VoterIndex)
    print("\nEvm vvpr receipt:",evm_vvpr_receipt,'\n')

    voter_receipt = m.voter_receipt(VoterIndex)
    # print("Voter Receipt:",voter_receipt)
