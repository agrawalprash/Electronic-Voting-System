#pragma once

#include "pbc.h"

struct BB1_element
{
    element_t c_rid;
    element_t c_vote;
    // element_t sign_po;

    ~BB1_element()
    {
        // element_clear(c_rid);
        // element_clear(c_vote);
        // element_clear(sign_po);
    }
};