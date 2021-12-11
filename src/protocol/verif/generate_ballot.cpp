#include "common.hpp"
#include "evm/receipt.hpp"
#include "verif/generate_ballot.hpp"

void generateBallot(int TotalCount)
{
    Ballot *bt;
    
    #ifndef __UNIT_TESTING__
    cout << "\e[1m\nBallot Generation ... \n";
    #endif

    for(int i=0; i < TotalCount+1; ++i)
    {
        bt = new Ballot(pg, Candidate_List);
        ballot_paper[i] = bt;

        #ifndef __UNIT_TESTING__
        if(i == TotalCount - 1)std::cout << "Generated: " << i+1 << "/" << TotalCount<<"\n";
        else if(i<TotalCount) std::cout << "Generated: " << i+1 << "/" << TotalCount  << "    " << '\r' << flush;
        #endif
    }

    #ifndef __UNIT_TESTING__
    cout << "\nVoting Procedure ...\n";
    #endif

    cout << "\e[0m\n";

    delete(bt);
}
