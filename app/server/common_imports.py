from . import voting
import re
import qrcode

TOTAL_COUNT = 5

m = voting.class_generation(TOTAL_COUNT)

C_VOTE_VVPR_RECEIPT = "c_vote_vvpr_receipt"
C_VOTE_VOTER_RECEIPT = "c_vote_voter_receipt"
C_VOTE_PARTIAL_RECEIPT = "c_vote_partial_receipt"
W_RW_EVM_VVPR_RECEIPT = "w_rw_evm_vvpr_receipt"
C_RID_C_U_VOTER_RECEIPT = "c_rid_cu_voter_receipt"

# Char Lengths for DataBase
