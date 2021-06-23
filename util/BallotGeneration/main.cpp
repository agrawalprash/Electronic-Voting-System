// #include "candidates.hpp"
#include <vector>
#include "ballot.hpp"
#include "../evm/evm.hpp"
#include "../common/common.hpp"
// std::vector<std::string> Candidate_List = {"Alice", "Bob", "Carrol", "David", "Eve", "Jade", "Harry", "Kristen", "Lacey", "Pablo", "Pacey"};

using namespace PairingGeneration;

void testing(int Voters)
{
    vector<VVPR_Receipt*> v;
    vector<int> VoteCount(_CANDIDATES_);

    clock_t start,end;

    start = clock();

    for(int i=0;i<Voters;i++)
    {
        Ballot* bt = new Ballot(pg, Candidate_List);
        int vote = rand() % _CANDIDATES_;
        EVM* ev = new EVM(vote, bt);
        VVPR_Receipt* vvpr = new VVPR_Receipt();
        ev->get_vvpr_receipt(vvpr);
        v.push_back(vvpr);

        if(i == Voters- 1)std::cout << "Processed: " << i+1 << "/" << Voters<<"\n";
        else std::cout << "Processed: " << i+1 << "/" << Voters  << "    " << '\r' << flush;
    }

    end = clock();

    long long int maxVote = 0;

    for(size_t i=0;i<v.size();i++)
    {
        VoteCount[v[i]->vote]++;
        maxVote = max(maxVote, (long long) VoteCount[v[i]->vote]);
    }

    vector<string> CandidateWon;

    for(size_t i=0;i<v.size();i++)
    {
        if(VoteCount[i] == maxVote)CandidateWon.push_back(Candidate_List[i]);
    }

    cout << "\e[1m\nResults\n";
    cout << "All Candidates (" << _CANDIDATES_ << "): ";
    for(size_t i=0;i<_CANDIDATES_;i++)
    {
        cout << Candidate_List[i] << ", ";
    }
    cout << "\n";
    if(CandidateWon.size() == 1)
    {
        cout << "Candidate Won: " << CandidateWon[0] << ", Votes: " << maxVote << "\n";
    }
    else
    {
        cout << "Tie between: "; 
        for(size_t i=0;i<CandidateWon.size();i++)
        {
            cout << CandidateWon[i] << " ";
        }    
        cout << "\nVotes: " << maxVote << "\n";
    }

    cout << "Time taken to complete whole procedure for one voter: " << (double)(end-start)*1000/(CLOCKS_PER_SEC*Voters) << " ms\n";
    // cout << "Total Time taken: " << (double)(end-start)/(CLOCKS_PER_SEC) << " s\n";


}

int main(int argc, char *argv[])
{
    PairingGeneration::initialize();

    int Voters = (argc > 1)? stoi(argv[1]) : _VOTERS_;

    testing(Voters);

    // Ballot* bt = new Ballot(pg, Candidate_List);
    // EVM* ev = new EVM(1, bt);
    // VVPR_Receipt* vvpr = new VVPR_Receipt();

    // ev->get_vvpr_receipt(vvpr);
    // cout << vvpr->rid << "\n";
    // cout << vvpr->name << " " << vvpr->vote << "\n";
}

