#pragma once

#include "pbc.h"

struct Voter_Receipt
{
    int w_m;
    element_t C_rid;
    element_t C_u;
    element_t C_vote;
    element_t w;
    element_t r_w;

    ~Voter_Receipt()
    {
        element_clear(C_rid);
        element_clear(C_u);
        element_clear(C_vote);
        element_clear(w);
        element_clear(r_w);
    }
};