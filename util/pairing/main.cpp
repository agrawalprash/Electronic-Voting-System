#include "pair.hpp"
#include "commitment.hpp"
#include "constant.hpp"

using namespace std;


void testing(Pairing* pg, float PairingTime)
{
    float TotalTime=0.0f;
    clock_t start, end;

    for(int i=0;i<ITERATIONS;++i)
    {
        element_t t,c,mapping1, mapping2, mapping3, pow1, pow2, pow3;
        pg->random_G1(t);
        pg->random_G1(c);
        // cout << "g1        : " << c << "\n";
        // cout << "g2        : " << t << "\n\n";
        pg->random_Zr(pow1);
        pg->random_Zr(pow2);
        
        element_init_same_as(pow3, pow1);
        element_mul(pow3, pow1, pow2);

        start = clock();

        BilinearMapping(mapping1, c, pow1, t, pow2, pg);
        BilinearMapping(mapping2, c, 1, t, pow3, pg);
        BilinearMapping(mapping3, c, pow3, t, 1, pg);

        end = clock();

        TotalTime+=(float)(end-start)*1000/CLOCKS_PER_SEC;

        if(element_cmp(mapping1,mapping2)!=0 || element_cmp(mapping3,mapping2)!=0)
        {
            cerr << "ERROR\n";
            exit(0);
        }

        
        if(i == ITERATIONS - 1)std::cout << "Processed: " << i+1 << "/" << ITERATIONS<<"\n";
        else std::cout << "Processed: " << i+1 << "/" << ITERATIONS  << "    " << '\r' << flush;

        element_clear(t);
        element_clear(c);
        element_clear(mapping1);
        element_clear(mapping2);
        element_clear(mapping3);
        element_clear(pow1);
        element_clear(pow2);
        element_clear(pow3);
    }
    cout << "Time taken to generate the Pairing: " << PairingTime << " ms\n";
    cout << "Time taken to generate 3 Bilinear Mappings for Verification: " << TotalTime/ITERATIONS << " ms\n";    
    std::cout << "SUCCESS\n";
}

int main(int argc, char* argv[])
{

    float PairingTime = 0.0f;

    clock_t start,end;
    
    start = clock();
    Pairing *pg = new Pairing("a", RBITS, QBITS);    
    end = clock();
    
    PairingTime = (float)(end-start)*1000/CLOCKS_PER_SEC;
    testing(pg, PairingTime);
    
    // string msg = "169820918169820918169820918169820918169820918";
    // string randomness   = "38296988012992130686";
    
    // element_t c,t,mapping1, mapping2, mapping3;
    // commit(c, msg, randomness, pg);
    // cout << "\e[1m\nMessage: " << msg << endl;
    // cout << "Commitment: " << c << "\n\n";

    return EXIT_SUCCESS;
}