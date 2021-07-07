#include "pbc.h"


#include "pairing/pairing.hpp"

namespace Signature
{
    void bls_signature(element_t, element_t message, element_t private_key);

    bool verify_signature(element_t g, element_t signature, element_t private_key, element_t message);
}