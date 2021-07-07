#pragma once

#include "pbc.h"
#include <string>

struct BB2_element
{
    element_t rid;
    std::string candidate;
    
    ~BB2_element()
    {
        // element_clear(rid);
    }
};