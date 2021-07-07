#pragma once

#include "pbc.h"

struct Voter_Receipt
{
    int w_m;
    element_t c_rid;
    element_t c_u;
    element_t c_vote;
    element_t w;
    element_t r_w;

    ~Voter_Receipt()
    {
        element_clear(c_rid);
        element_clear(c_u);
        element_clear(c_vote);
        element_clear(w);
        element_clear(r_w);
    }
};