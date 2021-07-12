#include <cstring>
#include "common.hpp"
#include "signature/signature.hpp"

using namespace std;

namespace Signature
{
    void bls_signature(element_t signature, element_t message, element_t private_key)
    {
        element_t hash;
        element_init_same_as(hash, message);
        element_init_same_as(signature, message);
        
        char c_msg[_ARRAY_SIZE_];
        element_snprint(c_msg, _ARRAY_SIZE_, message);

        // SHA-256 algorithm
        SHA256 sha256;
        string s_hash = sha256(c_msg);
        char c_hash[s_hash.length()+1];
        strcpy(c_hash, s_hash.c_str());

        element_from_hash(hash, c_hash, s_hash.size());
        element_pow_zn(signature, hash, private_key);

        element_clear(hash);
    }

    bool verify_signature(element_t g, element_t signature, element_t public_key, element_t message)
    {
        element_t temp1, temp2;
        
        element_init_GT(temp1, pg->pairing);
        element_init_GT(temp2, pg->pairing);

        element_t hash;
        element_init_same_as(hash, message);
        
        char c_msg[_ARRAY_SIZE_];
        element_snprint(c_msg, _ARRAY_SIZE_, message);

        // SHA-256 algorithm
        SHA256 sha256;
        string s_hash = sha256(c_msg);
        char c_hash[s_hash.length()+1];
        strcpy(c_hash, s_hash.c_str());

        element_from_hash(hash, c_hash, s_hash.size());
        
        pairing_apply(temp1, g, signature, pg->pairing);        
        pairing_apply(temp2, public_key, hash, pg->pairing);

        bool flag = false;
        if(element_cmp(temp1, temp2)==0)
        {
            flag = true;
        }
        else
        {
            element_invert(temp1, temp1);
            if(element_cmp(temp1, temp2) == 0)
            {
                flag = true;
            }
        }
        element_clear(temp1);
        element_clear(temp2);
        element_clear(hash);

        return flag;
    }
}