#include "common.hpp"

Pairing* pg;
std::vector<std::string> Candidate_List;

namespace PairingGeneration
{
    void initialize()
    {
        pg = new Pairing("a", RBITS, QBITS);
        Candidate_List = {"Alice", "Bob", "Carrol", "David", "Eve"};
    }
}
