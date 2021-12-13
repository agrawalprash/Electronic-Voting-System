from django.shortcuts import render
from django.http import HttpResponse
from . import voting
from .forms import Voterform
from .models import Bulletin1,Bulletin2
from .common_imports import *

def get_candidate(voter_id):
    candidates_w_m = m.candidate_list(voter_id)
    candidates = []
    for i in range(len(candidates_w_m)):
        val = candidates_w_m[i].split(",")
        candidates.append({"original":i,"name":val[0],"id":val[1]}) 
    return candidates 

def index(request):
    return HttpResponse("Welcome to Voting Booth")

def voter(request):
    if request.method == "POST":
        filled_form = Voterform(request.POST)
        if filled_form.is_valid():
            voter_id = filled_form.cleaned_data["voter_id"]
            if (voter_id >= TOTAL_COUNT):
                return HttpResponse("Wrong voter_id")
            else:
                ballot_paper = m.ballot_paper_details(voter_id)
                candidate_list = get_candidate(voter_id)
                bulletin2_data = Bulletin2.objects.filter(rid=ballot_paper['rid']).all()
                if (bulletin2_data.count()>0):
                    return HttpResponse(status=402)
                else:
                    FILE_NAME1 = BALLOT_PAPER_LEFT_QR  + str(voter_id) + ".png"
                    FILE_NAME2 = BALLOT_PAPER_RIGHT_QR + str(voter_id) + ".png"
                    combined1  = ballot_paper["obf_token"] + "," + ballot_paper["r_rid"] + "," + ballot_paper["r_obf_token"]
                    combined2  = ballot_paper["c_rid"] + "," + ballot_paper["c_obf_token"]
                    left_qr = qrcode.make(combined1)
                    left_qr.save("./server/static/"+FILE_NAME1)
                    right_qr = qrcode.make(combined2)
                    right_qr.save("./server/static/"+FILE_NAME2)
                    return render(request,"server/form.html",{"candidate_list":candidate_list,"rid":ballot_paper["rid"] , "left_qr":FILE_NAME1, "right_qr":FILE_NAME2, "voter_id":voter_id})
        else:
            return HttpResponse("Not a valid filling")

    else:
        voter_form = Voterform()
        return render(request,"server/index.html",{"voter_form":voter_form})

def candidate(request):
    if request.method=="POST":
        candidate_id = int(request.POST.get("candidate_voted"))
        voter_id = int(request.POST.get("voter_id"))
        rid = request.POST.get("rid")
        m.candidate_selection(voter_id, candidate_id)
        partial_evm_receipt = m.partial_evm_receipt(voter_id, candidate_id)
        
        FILE_NAME = C_VOTE_PARTIAL_RECEIPT+str(voter_id)+".png"
        qr = qrcode.make(partial_evm_receipt)
        qr.save("./server/static/"+FILE_NAME)
        m.ballot_scanning(voter_id)
        return render(request,"server/partial_receipt_ballot_scanning.html",{"partial_evm_receipt":FILE_NAME,"rid": rid,"voter_id":voter_id})
    else:
        return HttpResponse(status=401)

def evm_vvpr_receipt(request):
    if request.method=="POST":
        voter_id = int(request.POST.get("voter_id"))
        evm_vvpr_receipt = m.evm_vvpr_receipt(voter_id)
        rid = request.POST.get("rid")
        evm_vvpr_receipt['c_vote'] = re.sub(r" ","",evm_vvpr_receipt['c_vote'])
        
        combined = evm_vvpr_receipt['w'] + "," + evm_vvpr_receipt['r_w']
        FILE_NAME1 = C_VOTE_PARTIAL_RECEIPT+str(voter_id)+".png"		
        FILE_NAME2 = W_RW_EVM_VVPR_RECEIPT+str(voter_id)+".png"
        qr = qrcode.make(combined)
        qr.save("./server/static/"+FILE_NAME2)

        return render(request,"server/evm_vvpr_receipt.html",{"evm_vvpr_receipt":evm_vvpr_receipt, "rid":rid, "w_rw_evm_vvpr_receipt":FILE_NAME2,"evm_vvpr_receipt_c_vote":FILE_NAME1,"voter_id":voter_id})
    else:
        return HttpResponse(status=400)

def vvpr_receipt(request):
    if request.method=="POST":
        voter_id = int(request.POST.get("voter_id"))
        name = str(request.POST.get("vvpr_receipt_name"))
        rid = request.POST.get("rid")
        bulletin2_item = Bulletin2(rid=rid,name=name) 
        bulletin2_item.save()

        c_vote = request.POST.get("vvpr_receipt_c_vote")
        c_vote = re.sub(r",",", ",c_vote)
        c_vote = qrcode.make(c_vote)
        
        FILE_NAME = C_VOTE_VVPR_RECEIPT+str(voter_id)+".png"
        c_vote.save("./server/static/"+FILE_NAME)
        
        return render(request,"server/vvpr_receipt.html",{"vvpr_receipt_name":name,"vvpr_receipt_c_vote":FILE_NAME,"voter_id":voter_id})
    else:
        return HttpResponse(status=400)


def voter_receipt(request):
    if request.method=="POST":
        voter_id = int(request.POST.get("voter_id"))
        voter_receipt_var = m.voter_receipt(voter_id)
        name = request.POST.get("vvpr_receipt_name")
        FILE_NAME1 = C_VOTE_PARTIAL_RECEIPT+str(voter_id)+".png"
        FILE_NAME2 = W_RW_EVM_VVPR_RECEIPT+str(voter_id)+".png"
        FILE_NAME3 = C_RID_C_U_VOTER_RECEIPT+str(voter_id)+".png"

        bulletin1_item = Bulletin1(c_rid = voter_receipt_var["c_rid"],c_vote=voter_receipt_var["c_vote"]) 
        bulletin1_item.save()
        
        combined = voter_receipt_var["c_rid"] + voter_receipt_var["c_u"]
        qr = qrcode.make(combined)
        qr.save("./server/static/"+FILE_NAME3)

        return render(request,"server/voter_receipt.html",{"c_vote":FILE_NAME1,"w_rm":FILE_NAME2,"c_rid_cu":FILE_NAME3,"w_m":voter_receipt_var["w_m"], "voter_id":voter_id})
    else:
        return HttpResponse(status=400)