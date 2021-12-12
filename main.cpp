#include "common.hpp"
#include "evm/receipt.hpp"
#include "verif/generate_ballot.hpp"
#include "encryption/encryption.hpp"
// #include "qr/qrcode.hpp"
#include "/home/abhishek/Downloads/e-voting-project_12thDec_1_python_To_cpp/e-voting-project/pybind11/include/pybind11/pybind11.h"
#include "/home/abhishek/Downloads/e-voting-project_12thDec_1_python_To_cpp/e-voting-project/pybind11/include/pybind11/stl.h"
#include "/home/abhishek/Downloads/e-voting-project_12thDec_1_python_To_cpp/e-voting-project/pybind11/include/pybind11/numpy.h"
// #include "pypbc/pypbc.h"

#include <chrono>
#include <thread>

// #include <boost/python.hpp>

namespace py = pybind11;
using namespace std;

int func(int n) {
    cout << "It's working!" << endl;
    return n*10;
}


// initialization, ballot generation
// Loop over number of voters
// ballot_paper[i]->get_w_m_list(w_m); ballot_paper[i]->get_candidate_list(candidates);
// ev->candidate_selection(vote);
// partial_evm_receipt(i,vote);
// ev->ballot_scanning(ballot_paper[i]);
// evm_vvpr_receipt(i);
// voter_receipt(i)



class protocol
{
    public:
        vector<int> w_m;
        vector<string> candidates;
        int TotalCount;
        
        protocol(int n)
        {
            PairingGeneration::initialize();
            int TotalCount = n;
            generateBallot(TotalCount);
        }
        // protocol()
        // {
        //     PairingGeneration::initialize();
        //     int TotalCount = _VOTERS_;
        //     generateBallot(TotalCount);
        // }

        map<string,string> ballot_paper_python(int VoterIndex)
        {
            return ballot_paper_wrapper(VoterIndex);
        }

        vector<string> candidate_list_python(int VoterIndex)
        {
            int vote;
            ballot_paper[VoterIndex]->get_w_m_list(w_m);
            ballot_paper[VoterIndex]->get_candidate_list(candidates);

            vector<string> list_of_candidates_w_m;
            for(size_t i=0;i<w_m.size();i++)
            {
                string s = candidates[i] + "," + to_string(w_m[i]);
                list_of_candidates_w_m.push_back(s);
            }

            vote = rand() % _CANDIDATES_;

            return list_of_candidates_w_m;
        }

        void candidate_selection_python(int VoterIndex, int vote)
        {
            ev->candidate_selection(vote);
            return;
        }

        string partial_evm_receipt_python(int VoterIndex, int Vote)
        {
            return partial_evm_receipt(VoterIndex, Vote);
        }
        
        void ballot_scanning_python(int VoterIndex)
        {
            return ballot_scanning_wrapper(VoterIndex);
        }

        map<string,string> evm_vvpr_receipt_python(int VoterIndex)
        {
            return evm_vvpr_receipt(VoterIndex);
        }

        map<string,string> voter_receipt_python(int VoterIndex)
        {
            return voter_receipt(VoterIndex);
        }

};

protocol class_generation(int n)
{
    return protocol(n);
}

// PYBIND11_MODULE(voting, handle) {
//     handle.doc() = "Pairing Initializing"; // optional module docstring
//     handle.def("func", &func, "A function which multiplies the number by 10");
// }


PYBIND11_MODULE(voting, handle) {
    handle.doc() = "Pairing Initializing"; // optional module docstring
    handle.def("func", &func, "A function which multiplies the number by 10");
    handle.def("class_generation", &class_generation, "Protocol class generation");
    py::class_<protocol>(
        handle, "protocol").def(py::init<int>())
        .def("candidate_list", &protocol::candidate_list_python)
        .def("candidate_selection", &protocol::candidate_selection_python)
        .def("partial_evm_receipt", &protocol::partial_evm_receipt_python)
        .def("ballot_scanning", &protocol::ballot_scanning_python)
        .def("evm_vvpr_receipt", &protocol::evm_vvpr_receipt_python)
        .def("voter_receipt", &protocol::voter_receipt_python);
        ;
        
    // handle.def("protocol", &protocol, "A function which multiplies the number by 10");
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
