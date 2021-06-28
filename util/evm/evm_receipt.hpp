#pragma once

#include "pbc.h"

struct EVM_Receipt
{
    int w_m;

    element_t C_vote;    
    element_t w;
    element_t r_w;

    ~EVM_Receipt()
    {
        element_clear(C_vote);
        element_clear(w);
        element_clear(r_w);
    }
};