#include "pair.hpp"
#include "commitment.hpp"
#include "constant.hpp"

using namespace std;


int main(int argc, char* argv[])
{
    Pairing *pg = new Pairing("a", RBITS, QBITS);    
    
    string msg = "169820918169820918169820918169820918169820918";
    string randomness   = "38296988012992130686";
    
    element_t c,t,mapping;
    commit(c, msg, randomness, pg);
    cout << "\e[1m\nMessage: " << msg << endl;
    cout << "Commitment: " << c << "\n\n";

    cout << "================ Pairing ================\n";

    pg->random_G1(t);

    cout << "g1        : " << c << endl;
    cout << "g2        : " << t << "\n\n";
    
    int pow1 = 3;
    int pow2 = 4;

    BilinearMapping(mapping, c, pow1, t, pow2, pg);
    cout << "e(g1^" << pow1 << ",g2^" << pow2 << "):" << mapping << endl;
    BilinearMapping(mapping, c, 1, t, pow2*pow1, pg);
    cout << "e(g1^" << 1 << ",g2^" << pow2*pow1 << "):" << mapping << endl;
    BilinearMapping(mapping, c, pow2*pow1, t, 1, pg);
    cout << "e(g1^" << pow2*pow1 << ",g2^" << 1 << "):" << mapping << endl;

    return EXIT_SUCCESS;
}