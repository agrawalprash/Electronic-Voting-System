#include "common.hpp"
#include "evm/receipt.hpp"
#include "verif/generate_ballot.hpp"
#include "encryption/encryption.hpp"
// #include "qr/qrcode.hpp"
#include <pybind11/pybind11.h>

// #include <boost/python.hpp>

using namespace std;

int func(int n) {
    cout << "It's working!" << endl;
    return n*10;
}

PYBIND11_MODULE(protocol, handle) {
    handle.doc() = "pybind11 example plugin"; // optional module docstring
    handle.def("func", &func, "A function which multiplies the number by 10");
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

    // #ifndef __UNIT_TESTING__
    // cout << "\n";
    // Qrcode::generate_qr_code(vt_receipt_list[0]);
    // cout << endl;
    // #endif
    
    #ifndef __UNIT_TESTING__
    cout << "----------------------------------------------------\n";
    cout << "\nSUCCESS\n";
    cout << "c_u * c_vote is a commitment for w\n\n";

    cout << "Avg. Time for ballot generation: " << d1/(_VOTERS_+1) << "ms\n";
    cout << "Avg. Time for voting procedure : " << d2/Voters << "ms\n";
    #endif

    // Encryption::encryption();

    return EXIT_SUCCESS;
}

// HTTP Request
// Apache
// Django <- Python
// handlers
// Make functions interactive

// API to commumicate between the EVM and Voter.
// http interface
// GUI Programming (onClick, listeners)
