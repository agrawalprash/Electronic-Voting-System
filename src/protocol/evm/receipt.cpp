#include <map>
#include "common.hpp"
#include "evm/receipt.hpp"
#include "evm/evm.hpp"
#include "signature/signature.hpp"

Ballot* ballot_paper[_VOTERS_+3];
Voter_Receipt* vt_receipt_list[_VOTERS_+3];
EVM *ev = new EVM();
int check_vote;

using namespace std;
using namespace ZKPSetMembership;
using namespace reverseZKPSetMembership;
using namespace Commitment;
using namespace Signature;
using namespace BilinearMapping;

string elem_to_string(element_t t)
{
    char c_msg[_ARRAY_SIZE_];
    element_snprintf(c_msg, _ARRAY_SIZE_, "%B", t);
    return c_msg;
}

string partial_evm_receipt(int VoterIndex, int vote)
{
    element_t t;
    ev->get_c_vote(t);

    string s = elem_to_string(t);
    element_clear(t);
    return s;
}

map<string,string> evm_vvpr_receipt(int VoterIndex)
{    
    EVM_Receipt*  e_r = new EVM_Receipt();
    VVPR_Receipt* v_r = new VVPR_Receipt();

    ev->get_evm_receipt(e_r);
    ev->get_vvpr_receipt(v_r);

    map<string,string> m;

    // EVM Receipt
    m["w"]      = elem_to_string(e_r->w);
    m["w_m"]    = to_string(e_r->w_m);
    m["r_w"]    = elem_to_string(e_r->r_w);
    m["c_vote"] = elem_to_string(e_r->c_vote);
    
    // VVPR Receipt
    m["rid"]    = elem_to_string(v_r->rid);
    m["name"]   = Candidate_List[v_r->vote];

    delete(e_r);
    delete(v_r);

    return m;
}

map<string,string> voter_receipt(int VoterIndex)
{
    Voter_Receipt* vt_receipt = new Voter_Receipt();
    ev->get_voter_receipt(vt_receipt);
    ballot_paper[VoterIndex]->get_c_rid(vt_receipt->c_rid);
    ballot_paper[VoterIndex]->get_c_u(vt_receipt->c_u);

    map<string,string> m;

    m["w"]      = elem_to_string(vt_receipt->w);
    m["c_u"]    = elem_to_string(vt_receipt->c_u);
    m["w_m"]    = to_string(vt_receipt->w_m);
    m["r_w"]    = elem_to_string(vt_receipt->r_w);
    m["c_vote"] = elem_to_string(vt_receipt->c_vote);
    m["c_rid"]  = elem_to_string(vt_receipt->c_rid);
    
    // populating these values for ZKP verifications
    ev->populate_bb1();
    ev->populate_bb2();
    ev->populate_rvote();
    
    return m;

    element_t private_key;
    element_t public_key;
    element_t gen;
    element_t signature_c_rid;
    element_t signature_c_u;
    element_t signature_c_vote;
    
    pg->generator_G1(gen);
    pg->random_Zr(private_key);
    pg->exp(public_key, gen, private_key);

    Signature::bls_signature(signature_c_rid, vt_receipt->c_rid, private_key);
    Signature::bls_signature(signature_c_u, vt_receipt->c_u, private_key);
    Signature::bls_signature(signature_c_vote, vt_receipt->c_vote, private_key);

    // #ifndef __UNIT_TESTING__
    // if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    // {    
    //     cout << "\nVoter Receipt:\n";
    //     cout << "(c_rid, c_u, c_vote, w_m, w, r_w): (" << vt_receipt->c_rid << ", " 
    //          << vt_receipt->c_u << ", " << vt_receipt->c_vote << ", " 
    //          << vt_receipt->w_m << ", " << vt_receipt->w << ", " << vt_receipt->r_w << ")\n";
            
    //     cout << "\nDigital Signature:\n\n";
    //     cout << "Private key: " << private_key << "\n";
    //     cout << "c_rid, c_u, c_vote: " << signature_c_rid << ", " << 
    //             signature_c_u << ", " << signature_c_vote << "\n\n";
    // }
    
    // if(PRINT_PROCEDURE && VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    // {
    //     cout << "\nVoter Receipt (w_m): " << vt_receipt->w_m << "\n";
    // }
    // #endif

    // #ifdef __INTERACTIVE__
    //     cout << "\nVoter Receipt:\n";
    //     cout << "(c_rid, c_u, c_vote, w_m, w, r_w): (" << vt_receipt->c_rid << ", " 
    //          << vt_receipt->c_u << ", " << vt_receipt->c_vote << ", " 
    //          << vt_receipt->w_m << ", " << vt_receipt->w << ", " << vt_receipt->r_w << ")\n";
    // #endif

    // element_t c_w;

    // pg->mul(c_w, vt_receipt->c_u, vt_receipt->c_vote);
    // bool proof = Commitment::open(c_w, vt_receipt->w, vt_receipt->r_w, pg);

    // #ifndef __UNIT_TESTING__
    // if(check_vote != vt_receipt->w_m || !proof)
    // {
    //     cerr << "ERROR: Vote does not match\n\n";
    //     exit(0);
    // }
    // #endif
    // element_clear(c_w);
    
    // return vt_receipt;
}

void ballot_scanning_wrapper(int VoterIndex)
{
    ev->ballot_scanning(ballot_paper[VoterIndex]);
}

map<string,string> ballot_paper_wrapper(int VoterIndex)
{
    map<string,string> m;
    element_t t;

    ballot_paper[VoterIndex]->get_obf_token(t);
    m["obf_token"] = elem_to_string(t);

    ballot_paper[VoterIndex]->get_r_rid(t);
    m["r_rid"] = elem_to_string(t);

    ballot_paper[VoterIndex]->get_rid(t);
    m["rid"] = elem_to_string(t);

    ballot_paper[VoterIndex]->get_r_obf_token(t);
    m["r_obf_token"] = elem_to_string(t);

    ballot_paper[VoterIndex]->get_c_rid(t);
    m["c_rid"] = elem_to_string(t);

    ballot_paper[VoterIndex]->get_c_obf_token(t);
    m["c_obf_token"] = elem_to_string(t);

    element_clear(t);

    return m;
}

//adding new function forwardZKP
bool forwardZeroKP(){

    

    bool ans = true;
    int numOfEntriesInBB1 = (ev->BB1).size();
    int numOfCandidates = Candidate_List.size();
    element_t* phi = (element_t*)malloc(5*sizeof(element_t));
    for (long int i=0 ; i<numOfCandidates ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    for (int i=0 ; i<numOfEntriesInBB1 ; i++) {
        element_t comm;
        element_init_same_as(comm, ev->BB1[i].c_vote);
        element_set(comm, ev->BB1[i].c_vote);
        element_t rval;
        element_init_same_as(rval, ev->R_vote[i].r_vote);
        element_set(rval, ev->R_vote[i].r_vote);
        element_t g;
        element_t h;
        pg->generator_G1(g);
        pg->generator_G2(h);
        element_t p;
        element_init_same_as(p, phi[0]);
        for (int j=0 ; j<numOfCandidates ; j++){
            if (Candidate_List[j] == (ev->BB2[i].candidate)){
                element_set(p, phi[j]);
                break;
            }
        }
        cout << "The candidate name is: " << ev->BB2[i].candidate <<endl;
        cout << "The set p value is: "<< p<< endl;
        prover_f *p_obj = new prover_f(g, h, rval ,comm,p, numOfCandidates);
        verifier_f *v_obj = new verifier_f(g, h, comm , numOfCandidates, p_obj);

        bool b = v_obj->implementForwardZKP();
        
        //bool b = ZKPSetMembership::testing_verification(p, rval, pg, comm, g ,h ,phi, numOfCandidates);
        cout <<"For i == "<<i << " b is: "<< b<<endl;
        if(b == false){
            ans = false;
        }
    }
    return ans;
    
/*
    bool ans = true;
    int numOfEntriesInBB1 = (ev->BB1).size();
    int numOfCandidates = Candidate_List.size();
    element_t* phi = (element_t*)malloc(5*sizeof(element_t));
    for (long int i=0 ; i<numOfCandidates ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    for (int i=0 ; i<numOfEntriesInBB1 ; i++) {
        element_t comm;
        element_init_same_as(comm, ev->BB1[i].c_vote);
        element_set(comm, ev->BB1[i].c_vote);
        element_t rval;
        element_init_same_as(rval, ev->R_vote[i].r_vote);
        element_set(rval, ev->R_vote[i].r_vote);
        element_t g;
        element_t h;
        pg->generator_G1(g);
        pg->generator_G2(h);
        element_t p;
        element_init_same_as(p, phi[0]);
        for (int j=0 ; j<numOfCandidates ; j++){
            if (Candidate_List[j] == (ev->BB2[i].candidate)){
                element_set(p, phi[j]);
                break;
            }
        }
        cout << "The candidate name is: " << ev->BB2[i].candidate <<endl;
        cout << "The set p value is: "<< p<< endl;
        bool b = ZKPSetMembership::testing_verification(p, rval, pg, comm, g ,h ,phi, numOfCandidates);
        cout <<"For i == "<<i << " b is: "<< b<<endl;
        if(b == false){
            ans = false;
        }
    }
    return ans;
    */

    
    /*
    int length = (ev->BB1).size();
    cout << length << endl;
    element_t comm;
    cout<<ev->BB1[0].c_vote<<endl;
    element_init_same_as(comm, ev->BB1[0].c_vote);
    cout<<"initialise the comm"<<endl;
    element_set(comm, ev->BB1[0].c_vote);
    cout<<"set the commitmet"<<endl;
    
    
    element_t* phi = (element_t*)malloc(5*sizeof(element_t));
    for (long int i=0 ; i<5 ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    
    cout << "The array " << phi[0] <<endl;
    cout << "The array " << phi[1] <<endl;
    cout << "mid" << endl;
    element_t rval;
    element_init_same_as(rval, ev->R_vote[0].r_vote);
    element_set(rval, ev->R_vote[0].r_vote);
    element_t g;
    element_t h;
    element_t g2;
    pg->generator_G1(g);
    pg->generator_G2(h);
    cout<<"r_val is: ______"<<rval<<endl; 
    element_t p;
    element_init_same_as(p, phi[0]);
    element_set(p, phi[0]);
    bool b = ZKPSetMembership::testing_verification(p, rval, pg, comm, g ,h ,phi, 5);
    cout<< "The returned b value is "<< b <<endl;
    return b;
    */
    
}
//ading new function reverseZKP
bool reverseZeroKP() {

    cout << "reverse Function entered" <<endl;
    int numOfCandidates = Candidate_List.size();
    int numOfCommitments = (ev->BB2).size();
    element_t g;
    element_t h;
    pg->generator_G1(g);
    pg->generator_G2(h);

    element_t* phi = (element_t*)malloc(numOfCandidates*sizeof(element_t));
    for (long int i=0 ; i<numOfCandidates ; i++){
        element_init_Zr(phi[i], pg->pairing);
        element_set_si(phi[i] , i);
    }
    element_t* Commitments = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    element_t* R_vals = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    element_t* p_vals = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    for (int i=0; i<numOfCommitments ; i++) {
        element_init_same_as(Commitments[i], ev->BB1[i].c_vote);
        element_set(Commitments[i], ev->BB1[i].c_vote);

        element_init_same_as(R_vals[i], ev->R_vote[i].r_vote);
        element_set(R_vals[i], ev->R_vote[i].r_vote);

        element_init_same_as(p_vals[i], phi[0]);
        for (int j=0 ; j<numOfCandidates ; j++){
            if (Candidate_List[j] == (ev->BB2[i].candidate)){
                element_set(p_vals[i], phi[j]);
                break;
            }
        }
    }
    for(int ind = 0 ; ind<numOfCommitments ; ind++){
        element_t C;
        element_init_same_as(C, ev->BB1[ind].c_vote);
        element_set(C, ev->BB1[ind].c_vote);
        element_t p;
        element_init_same_as(p, phi[0]);
        for (int j=0 ; j<numOfCandidates ; j++){
            if (Candidate_List[j] == (ev->BB2[ind].candidate)){
                element_set(p, phi[j]);
                break;
            }
        }
        
        prover_r *p_obj = new prover_r(g, h, p, C, R_vals[ind] , R_vals , Commitments, p_vals , numOfCandidates, numOfCommitments);
        verifier_r *v_obj = new verifier_r(g,h,p,Commitments, numOfCandidates, numOfCommitments, p_obj);
        bool b  = v_obj->implementReverseZKP();
        if(b == false){
            return false;
        }
    }
    return true;


    /*

    cout << "reverse Function entered" <<endl;
    int numOfCandidates = Candidate_List.size();
    int numOfCommitments = (ev->BB2).size();
    element_t g;
    element_t h;
    pg->generator_G1(g);
    pg->generator_G2(h);
    element_t* Commitments = (element_t*)malloc(numOfCommitments*sizeof(element_t));
    for (int i=0; i<numOfCommitments ; i++) {
        element_init_same_as(Commitments[i], ev->BB1[i].c_vote);
        element_set(Commitments[i], ev->BB1[i].c_vote);
    }
    for(int ind = 0 ; ind<numOfCommitments ; ind++){
        element_t C;
        element_init_same_as(C, ev->BB1[ind].c_vote);
        element_set(C, ev->BB1[ind].c_vote);
        element_t* phi = (element_t*)malloc(numOfCandidates*sizeof(element_t));
        for (long int i=0 ; i<numOfCandidates ; i++){
            element_init_Zr(phi[i], pg->pairing);
            element_set_si(phi[i] , i);
        }
        element_t p;
        element_init_same_as(p, phi[0]);
        for (int j=0 ; j<numOfCandidates ; j++){
            if (Candidate_List[j] == (ev->BB2[ind].candidate)){
                element_set(p, phi[j]);
                break;
            }
        }
        element_t rval;
        element_init_same_as(rval, ev->R_vote[ind].r_vote);
        element_set(rval, ev->R_vote[ind].r_vote);
        cout<< "The r_vote value is  "<< rval<<endl;
        cout <<"mid level before calling main function"<<endl;
        cout<<"The candidate name is: "<<ev->BB2[ind].candidate<<endl;
        bool b = reverseZKPSetMembership::testing_verification(p, rval, pg, C, g, h, Commitments, numOfCandidates, numOfCommitments, ind);
        cout << "The value of b is: " << b << endl;
        if (b == false){
            return false;
        }
    }
    return true;

    */

}

void procedure(int TotalCount)
{
    bool error = false;
    
    for(int i=0; i < TotalCount; ++i)
    {
        int vote;
        vector<int> w_m;
        vector<string> candidates;
        
        ballot_paper[i]->get_w_m_list(w_m);
        ballot_paper[i]->get_candidate_list(candidates);

        #ifndef __INTERACTIVE__
            vote = rand() % _CANDIDATES_;
        #else
            for(size_t i=0;i<w_m.size();i++)
            {
                cout << i << ".) " << candidates[i] << ": " << w_m[i] << "\n";
            }
            cin >> vote;
            while(vote > (int)candidates.size())
            {
                cout << "Invalid option: Choose again\n";
                cin >> vote;
            }
        #endif

        check_vote = w_m[vote];
        ev->candidate_selection(vote);
            
        #ifndef __UNIT_TESTING__
        if(PRINT_PROCEDURE && i%REMAINDER_FOR_PRINT==0)
        {
            cout << "\e[1m\n\n========================================= Voter: " << i+1 << " =========================================" << endl;
            for(size_t i=0;i<w_m.size();i++)
            {
                cout << i << ".) " << candidates[i] << ": " << w_m[i] << "\n";
            }
            cout << "\nVote: " << vote << "\n\n";
        }
        #endif

        string s = partial_evm_receipt(i,vote);
        
        ev->ballot_scanning(ballot_paper[i]); // EVM receipt and VVPR receipt generated

        map<string,string> m_evm_vvpr = evm_vvpr_receipt(i);
        map<string,string> m_vr = voter_receipt(i);

        // Voter_Receipt* vt_receipt = voter_receipt(i);

        // vt_receipt_list[i] = vt_receipt;

        // #ifndef __UNIT_TESTING__
        // if(i == TotalCount - 1)std::cout << "Processed: " << i+1 << "/" << TotalCount<<"\n";
        // else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
        // // else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "\n";
        // #endif
    }

    cout << "\e[0m";

    #ifndef __UNIT_TESTING__
    for(size_t i=0;i<ev->BB2.size();++i)
    {
        cout << ev->BB2[i].rid << ", " << ev->BB2[i].candidate << "\n";
    }
    #endif
}