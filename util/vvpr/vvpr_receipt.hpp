#pragma once
#include <string>
#include "pbc.h"

struct VVPR_Receipt
{
    int vote = 0; 
    element_t rid;

    ~VVPR_Receipt()
    {
        element_clear(rid);
    }
};