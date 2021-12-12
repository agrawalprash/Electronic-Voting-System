Ballot
    int m;   

    // Obfuscation token (mod m)
    int u;

    element_t rid;
    element_t c_rid;
    element_t r_rid;
    
    element_t obfuscation_token;
    element_t c_obf_token;        
    element_t r_obf_token;

    // bottom left  QR code contains (obfuscation token, r_rid, r_obf_token)
    // bottom right QR code contains (c_rid, c_obf_token)

    std::vector<std::string> candidates;
    std::vector<int> w_m;        


Ballot generation
    m
    u
    rid
    
    #Bottom left
        obfuscation_token
        r_rid
        r_obf_token

    #Bottom right
        c_rid
        c_obf_token

    candidates
    w_m

EVM receipt
    w_m
    w
    r_w

VVPR receipt
    name
    c_vote

Voter Receipt
    c_rid
    c_u
    c_vote
    w_m
    w
    r_w


Database1
    c_rid
    c_vote
    sign_po


Database2
    rid
    name
