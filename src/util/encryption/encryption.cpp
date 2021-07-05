#include "encryption/encryption.hpp"
#include "common.hpp"

using namespace std;

namespace Encryption
{
    bool encryption()
    {
        element_t message; //  Elliptic Curve element

        element_t priv_alice, priv_bob, pub_alice, pub_bob;
        element_t gen, g_k, k, s, m_s, g_ak, dec_message;
        pg->generator_Zr(gen);

        pg->random_Zr(priv_alice);
        pg->random_Zr(priv_bob);
        pg->random_Zr(k);

        pg->mul(pub_alice, gen, priv_alice);
        pg->mul(pub_bob, gen, priv_bob);

        pg->random_G1(message);
        
        #ifndef __UNIT_TESTING__
        cout << "\n================================================================\n";
        cout << "\nElgamal Encryption Scheme \n";
        cout << "\e[1m\n============ENCRYPTION============\n";
        cout << "Alice's key (a, g^a): " << priv_alice << ", " << pub_alice << "\n";
        cout << "Bob's key (b, g^b): " << priv_bob << ", " << pub_bob << "\n\n";
        cout << "Message: " << message << "\n";
        cout << "\nAlice wants to send this message to Bob\n";
        #endif

        pg->mul(g_k, gen, k);
        pg->mul(s, pub_bob, k);
        pg->mul(m_s, message, s);

        #ifndef __UNIT_TESTING__
        cout << "Encrypted Message (g^k, Message*s) where s = g^ak: " << g_k << ", " << m_s << "\n";        
        cout << "\n============DECRYPTION============\n";
        #endif

        pg->mul(g_ak, g_k, priv_bob);

        pg->div(dec_message, m_s, g_ak);
        
        #ifndef __UNIT_TESTING__
        cout << "Decrypted Message: " << dec_message << "\n";
        #endif

        if(element_cmp(dec_message, message) == 0)
        {
            #ifndef __UNIT_TESTING__
            cout << "\nOriginal and decrypted message match\n\n";
            #endif
            return true;
        }
        return false;
    }    
} 

