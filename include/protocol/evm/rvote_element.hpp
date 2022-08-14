#pragma once

#include "pbc.h"

struct Rvote_element
{
    element_t r_vote;

    ~Rvote_element()
    {
        // element_clear(c_rid);
        // element_clear(c_vote);
        // element_clear(sign_po);
    }
};