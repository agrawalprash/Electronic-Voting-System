#include "common.hpp"
#include "evm/receipt.hpp"
#include "verif/generate_ballot.hpp"
#include "encryption/encryption.hpp"

using namespace std;

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
    
    #ifndef __UNIT_TESTING__
    cout << "----------------------------------------------------\n";
    cout << "\nSUCCESS\n";
    cout << "C_u * c_vote is a commitment for w\n\n";

    cout << "Avg. Time for ballot generation: " << d1/(_VOTERS_+1) << "ms\n";
    cout << "Avg. Time for voting procedure : " << d2/Voters << "ms\n";
    #endif

    Encryption::encryption();

    return EXIT_SUCCESS;
}