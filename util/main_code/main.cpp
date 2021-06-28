#include "../evm/evm.hpp"
#include "../common/common.hpp"
#include "../ballot-generation/ballot.hpp"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

Ballot* Ballot_Paper[max(1004, _VOTERS_+4)];
EVM *ev;
int CheckVote;

void generateBallot(int TotalCount)
{
    Ballot *bt;
    cout << "\e[1m\nBallot Generation ... \n";
    for(int i=0; i < TotalCount+2; ++i)
    {
        bt = new Ballot(pg, Candidate_List);
        Ballot_Paper[i] = bt;

        if(i == TotalCount - 1)std::cout << "Generated: " << i+1 << "/" << TotalCount<<"\n";
        else if(i<TotalCount) std::cout << "Generated: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
    }
    cout << "\nVoting Procedure ...\n";
    delete(bt);
}

void partial_evm_receipt(int VoterIndex, int vote)
{
    element_t t;
    ev->get_C_vote(t);
    
    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "C_vote: " << t;
        cout << "\n\nScan the ballot paper ... \n\n";        
    }
    element_clear(t);
}

void evm_vvpr_receipt(int VoterIndex)
{    
    EVM_Receipt*  e_r = new EVM_Receipt();
    VVPR_Receipt* v_r = new VVPR_Receipt();

    ev->get_evm_receipt(e_r);
    ev->get_vvpr_receipt(v_r);

    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "EVM Receipt:\n";
        cout << "(C_vote, w_m, w, r_w): (" << e_r->C_vote << ", " << e_r->w_m << ", " << e_r->w << ", " << e_r->r_w << ")\n";
    }
    
    if(PRINT_PROCEDURE && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "\nVVPR Receipt:\n";
        cout << "(name, rid): (" << Candidate_List[v_r->vote] << ", " << v_r->rid << ")\n";
    }
    delete(e_r);
    delete(v_r);
}

void voter_receipt(int VoterIndex)
{    
    Voter_Receipt* vt_receipt = new Voter_Receipt();
    ev->get_voter_receipt(vt_receipt);
    Ballot_Paper[VoterIndex]->get_C_rid(vt_receipt->C_rid);
    Ballot_Paper[VoterIndex]->get_C_u(vt_receipt->C_u);

    if(PRINT_PROCEDURE && !VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {    
        cout << "\nVoter Receipt:\n";
        cout << "(C_rid, C_u, C_vote, w_m, w, r_w): (" << vt_receipt->C_rid << ", " 
             << vt_receipt->C_u << ", " << vt_receipt->C_vote << ", " 
             << vt_receipt->w_m << ", " << vt_receipt->w << ", " << vt_receipt->r_w << ")\n";
    }
    
    if(PRINT_PROCEDURE && VVPR_ONLY && VoterIndex%REMAINDER_FOR_PRINT==0)
    {
        cout << "\nVoter Receipt (w_m): " << vt_receipt->w_m << "\n";
    }

    if(CheckVote != vt_receipt->w_m)
    {
        cerr << "ERROR: Vote does not match\n\n";
        exit(0);
    }

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
        Ballot_Paper[i]->get_w_m_list(w_m);
        CheckVote = w_m[vote];
                
        if(PRINT_PROCEDURE && i%REMAINDER_FOR_PRINT==0)
        {            
            cout << "\e[1m\n\n========================================= Voter: " << i+1 << " =========================================" << endl;
            for(size_t i=0;i<w_m.size();i++)
            {
                cout << i << ".) " << Candidate_List[i] << ": " << w_m[i] << "\n";
            }
            cout << "\nVote: " << vote << "\n\n";            
        }

        partial_evm_receipt(i,vote);
        
        ev->ballot_scanning(Ballot_Paper[i]); // EVM receipt and VVPR receipt generated

        evm_vvpr_receipt(i);

        voter_receipt(i);

        if(i == TotalCount - 1)std::cout << "Processed: " << i+1 << "/" << TotalCount<<"\n";
        else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
        // else std::cout << "Processed: " << i+1 << "/" << TotalCount  << "\n";
    }
}


int main(int argc, char *argv[])
{
    PairingGeneration::initialize();

    int Voters = (argc > 1)? stoi(argv[1]) : _VOTERS_;

    double d1,d2;
    clock_t start,end;

    start = clock();
    generateBallot(Voters);
    end = clock();

    d1 = (double)(end-start)*1000/(double)CLOCKS_PER_SEC;

    start = clock();
    procedure(Voters);
    end = clock();

    d2 = (double)(end-start)*1000/(double)CLOCKS_PER_SEC;
    
    cout << "\nSUCCESS\n";

    cout << "Avg. Time for ballot generation: " << d1/max(1004, _VOTERS_+4) << "ms\n";
    cout << "Avg. Time for voting procedure : " << d2/Voters << "ms\n";
    return EXIT_SUCCESS;
}