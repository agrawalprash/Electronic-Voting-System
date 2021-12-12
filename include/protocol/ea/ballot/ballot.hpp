#pragma once
#include <vector>
#include <string>
#include "helper_functions.hpp"
#include "pairing/pairing.hpp"
#include "bilinear-mapping/bilinear_mapping.hpp"
#include "commitment/commitment.hpp"
#include "common.hpp"

class Ballot
{
    public:
        Ballot(Pairing* p, std::vector<std::string> Candidate_List);
        ~Ballot();

        void get_rid(element_t t);
        void get_r_rid(element_t t);
        void get_c_rid(element_t t);
        void get_c_u(element_t t);
        void get_r_obf_token(element_t t);
        void get_obf_token(element_t t);
        void get_total_candidates(int &t);
        void get_obf_token_rem(int &t);
        void get_w_m_list(vector<int> &v);
        void get_candidate_list(vector<string> &v);

    private:
        
        // Count Candidates standing for election in a constituency
        int m;   

        // Obfuscation token (mod m)
        int u;
        
        // Ballot id
        element_t rid;

        element_t c_rid;
        element_t r_rid;
        
        element_t obfuscation_token;
        element_t c_obf_token;        
        element_t r_obf_token;

        // bottom left  QR code contains (obfuscation token, r_rid, r_obf_token)
        // bottom right QR code contains (c_rid, c_obf_token)

        std::vector<std::string> candidates;
        std::vector<int> w_m;        
};