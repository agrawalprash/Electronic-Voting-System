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
        
        char s[2000];
        element_snprint(s, 2000, message);
        element_from_hash(hash, s, sizeof(s)/sizeof(s[0]));
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
        
        char s[2000];
        element_snprint(s, 2000, message);
        element_from_hash(hash, s, sizeof(s)/sizeof(s[0]));
        
        pairing_apply(temp1, g, signature, pg->pairing);        
        pairing_apply(temp2, public_key, hash, pg->pairing);

        bool flag = false;
        if(element_cmp(temp1, temp2)==0)
        {
            flag = true;
        }
        element_clear(temp1);
        element_clear(temp2);
        element_clear(hash);

        return flag;
    }
}