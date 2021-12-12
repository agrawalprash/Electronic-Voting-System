#include "ballot/ballot.hpp"

using namespace Commitment;

Ballot::~Ballot()
{
    element_clear(rid);
    element_clear(c_rid);
    element_clear(c_obf_token);
    element_clear(r_rid);    
    element_clear(obfuscation_token);
    element_clear(r_obf_token);    
}

Ballot::Ballot(Pairing* p, vector<string> c)
{
    candidates = c;
    m = c.size();

    p->random_Zr(rid);
    p->random_Zr(obfuscation_token);
    p->random_Zr(r_rid);
    p->random_Zr(r_obf_token);

    mpz_t u_;
    mpz_init(u_);

    element_to_mpz(u_, obfuscation_token);
    mpz_mod_ui(u_, u_, m);

    int w = mpz_get_ui(u_);
    u = w;    
    
    for(int i=0;i<m;i++)w_m.push_back((w+i)%m);
    mpz_clear(u_);

    Commitment::commit(c_rid, rid, r_rid, p);
    Commitment::commit(c_obf_token, obfuscation_token, r_obf_token, p);
}

void Ballot::get_r_obf_token(element_t t)
{
    element_init_same_as(t, r_obf_token);
    element_set(t, r_obf_token);
}

void Ballot::get_rid(element_t t)
{
    element_init_same_as(t, rid);
    element_set(t, rid);
}

void Ballot::get_r_rid(element_t t)
{
    element_init_same_as(t, r_rid);
    element_set(t, r_rid);
}

void Ballot::get_c_rid(element_t t)
{
    element_init_same_as(t, c_rid);
    element_set(t, c_rid);
}

void Ballot::get_c_u(element_t t)
{
    element_init_same_as(t, c_obf_token);
    element_set(t, c_obf_token);
}

void Ballot::get_obf_token(element_t t)
{
    element_init_same_as(t, obfuscation_token);
    element_set(t, obfuscation_token);
}

void Ballot::get_total_candidates(int &t)
{
    t = m;
}

void Ballot::get_obf_token_rem(int &t)
{
    t = u;
}

void Ballot::get_w_m_list(vector<int> &v)
{
    v.clear();
    for(size_t i=0;i<candidates.size();i++)
    {
        v.push_back(w_m[i]);
    }
}

void Ballot::get_candidate_list(vector<string> &v)
{
    v.clear();
    for(size_t i=0;i<candidates.size();i++)
    {
        v.push_back(candidates[i]);
    }
}