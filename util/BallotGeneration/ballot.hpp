#pragma once
#include <vector>
#include <string>
#include "../pairing/helper_functions.hpp"
#include "../pairing/pair.hpp"
#include "../pairing/BilinearMapping.hpp"
#include "../pairing/commitment.hpp"
#include "../common/common.hpp"
#include "../vvpr/vvpr.hpp"

class Ballot
{
    public:
        Ballot(Pairing* p, std::vector<std::string> Candidate_List);
        ~Ballot();

        void get_rid(element_t t);
        void get_r_obf_token(element_t t);
        void get_obf_token(element_t t);
        void get_total_candidates(int &t);
        void get_obf_token_rem(int &t);

    private:
        
        // Count Candidates standing for election in a constituency
        int m;   

        // Obfuscation token (mod m)
        int u;
        
        // Ballot id
        element_t rid; 
        element_t C_rid;
        element_t r_rid;
        
        element_t obfuscation_token;
        element_t C_obf_token;        
        element_t r_obf_token;

        // bottom left  QR code contains (obfuscation token, r_rid, r_obf_token)
        // bottom right QR code contains (C_rid, C_obf_token)

        std::vector<std::string> candidates;
        std::vector<int> w_m;

        
};