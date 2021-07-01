#include "../common/defines.hpp"
#include "protocol.hpp"

Ballot* ballot_paper[_VOTERS_+1];
EVM *ev;
int check_vote;

using namespace std;

void generateBallot(int TotalCount)
{
    Ballot *bt;
    
    #ifndef __TESTING__
    cout << "\e[1m\nBallot Generation ... \n";
    #endif

    for(int i=0; i < TotalCount+1; ++i)
    {
        bt = new Ballot(pg, Candidate_List);
        ballot_paper[i] = bt;

        #ifndef __TESTING__
        if(i == TotalCount - 1)std::cout << "Generated: " << i+1 << "/" << TotalCount<<"\n";
        else if(i<TotalCount) std::cout << "Generated: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
        #endif
    }

    #ifndef __TESTING__
    cout << "\nVoting Procedure ...\n";
    #endif

    delete(bt);
}

void partial_evm_receipt(int VoterIndex, int vote)
{
    element_t t;
    ev->get_c_vote(t);
    
    #ifndef __TESTING__
    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "c_vote: " << t;
        cout << "\n\nScan the ballot paper ... \n\n";        
    }
    #endif

    element_clear(t);
}

void evm_vvpr_receipt(int VoterIndex)
{    
    EVM_Receipt*  e_r = new EVM_Receipt();
    VVPR_Receipt* v_r = new VVPR_Receipt();

    ev->get_evm_receipt(e_r);
    ev->get_vvpr_receipt(v_r);

    #ifndef __TESTING__
    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "EVM Receipt:\n";
        cout << "(c_vote, w_m, w, r_w): (" << e_r->c_vote << ", " << e_r->w_m << ", " << e_r->w << ", " << e_r->r_w << ")\n";
    }
    
    if(PRINT_PROCEDURE && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "\nVVPR Receipt:\n";
        cout << "(name, rid): (" << Candidate_List[v_r->vote] << ", " << v_r->rid << ")\n";
    }
    #endif

    delete(e_r);
    delete(v_r);
}

void voter_receipt(int VoterIndex)
{    
    Voter_Receipt* vt_receipt = new Voter_Receipt();
    ev->get_voter_receipt(vt_receipt);
    ballot_paper[VoterIndex]->get_c_rid(vt_receipt->c_rid);
    ballot_paper[VoterIndex]->get_c_u(vt_receipt->C_u);

    #ifndef __TESTING__
    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {    
        cout << "\nVoter Receipt:\n";
        cout << "(c_rid, C_u, c_vote, w_m, w, r_w): (" << vt_receipt->c_rid << ", " 
             << vt_receipt->C_u << ", " << vt_receipt->c_vote << ", " 
             << vt_receipt->w_m << ", " << vt_receipt->w << ", " << vt_receipt->r_w << ")\n";
    }
    
    if(PRINT_PROCEDURE && VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "\nVoter Receipt (w_m): " << vt_receipt->w_m << "\n";
    }
    #endif

    element_t C_w;

    pg->mul(C_w, vt_receipt->C_u, vt_receipt->c_vote);
    bool proof = Commitment::open(C_w, vt_receipt->w, vt_receipt->r_w, pg);

    #ifndef __TESTING__
    if(check_vote != vt_receipt->w_m || !proof)
    {
        cerr << "ERROR: Vote does not match\n\n";
        exit(0);
    }
    #endif

    element_clear(C_w);
    delete(vt_receipt);    
}

// DEBUG__ CODE
// precondtion and postcondition (For every function as  an assert expression)

// Pass a flag while compilation and use it as #ifdef DEBUG then print the values

// Make a note of replications
// Each function should come with Doxygen (search for) annotated summary
// Annotation languages
// Document code
// Each calss should have pretty printing functions (DEBUGGING)

// EXceptions
// Protocol Exception
// Input    Exceptions
// Use try and catch block
// Exception object
// Error code
// Guard

// Collecting all the votes (Table)

// API for the EVM

void procedure(int TotalCount)
{
    bool error = false;

    for(int i=0; i < TotalCount; ++i)
    {
        int vote = rand() % _CANDIDATES_;
        
        ev = new EVM(vote);

        vector<int> w_m;
        ballot_paper[i]->get_w_m_list(w_m);
        check_vote = w_m[vote];
        
        #ifndef __TESTING__
        if(PRINT_PROCEDURE && i%REMAINDER_FOR_PRINT==0)
        {            
            cout << "\e[1m\n\n========================================= Voter: " << i+1 << " =========================================" << endl;
            for(size_t i=0;i<w_m.size();i++)
            {
                cout << i << ".) " << Candidate_List[i] << ": " << w_m[i] << "\n";
            }
            cout << "\nVote: " << vote << "\n\n";            
        }
        #endif

        partial_evm_receipt(i,vote);
        
        ev->ballot_scanning(ballot_paper[i]); // EVM receipt and VVPR receipt generated

        evm_vvpr_receipt(i);

        voter_receipt(i);

        #ifndef __TESTING__
        if(i == TotalCount - 1)std::cout << "Processed: " << i+1 << "/" << TotalCount<<"\n";
        else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
        // else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "\n";
        #endif
        
    }
}


