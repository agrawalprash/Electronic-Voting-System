#pragma once
#include <vector>
#include <string>
#include "../pairing/helper_functions.hpp"
#include "../pairing/pair.hpp"
#include "../bilinear-mapping/bilinear_mapping.hpp"
#include "../commitment/commitment.hpp"
#include "../common/common.hpp"

class Ballot
{
    public:
        Ballot(Pairing* p, std::vector<std::string> Candidate_List);
        ~Ballot();

        void get_rid(element_t t);
        void get_C_rid(element_t t);
        void get_C_u(element_t t);
        void get_r_obf_token(element_t t);
        void get_obf_token(element_t t);
        void get_total_candidates(int &t);
        void get_obf_token_rem(int &t);
        void get_w_m_list(vector<int> &v);

    private:
        
        // Count Candidates standing for election in a constituency
        int m;   

        // Obfuscation token (mod m)
        int u;
        
        // Ballot id
        element_t rid;
        // Proves by birthday paradox 
        // Two ballot id's should not have same rid
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