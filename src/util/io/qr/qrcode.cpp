#include "QR_Code_generator/QrCode.hpp"
#include "qr/qrcode.hpp"

using namespace qrcodegen;

namespace Qrcode
{
    void generate_qr_code(Voter_Receipt* vt_receipt)
    {
        element_t private_key;
        element_t public_key;
        element_t gen;
        element_t signature_c_rid;
        element_t signature_c_u;
        element_t signature_c_vote;
        
        pg->generator_G1(gen);
        pg->random_Zr(private_key);
        pg->exp(public_key, gen, private_key);

        Signature::bls_signature(signature_c_rid, vt_receipt->c_rid, private_key);
        Signature::bls_signature(signature_c_u,   vt_receipt->c_u, private_key);
        Signature::bls_signature(signature_c_vote, vt_receipt->c_vote, private_key);

        char sig[2000];
        element_snprintf(sig, 2000, "%B, %B, %B, %B, %B",signature_c_rid, signature_c_u, signature_c_vote,
                                                 vt_receipt->w, vt_receipt->r_w);
        
        QrCode qr = QrCode::encodeText(sig, QrCode::Ecc::MEDIUM);
        std::string svg = qr.toSvgString(4);
        
        string filename = "img";
        ofstream outputFile(filename);

        int border = 4;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                int color = qr.getModule(x,y);
                // cout << "(" << y << "," << x << "), ";
                color == 1 ? std::cout << "#" : std::cout << " ";
                // color == 1? outputFile << 255 : outputFile << 0;
                // if(x != qr.getSize()+border-1)outputFile << ", ";
                // std::cout << (qr.getModule(x, y) ? "1" : " ");
            }
            // outputFile << std::endl;
            std::cout << std::endl;
        }
        // outputFile << std::endl;
        std::cout << std::endl << std::endl << std::endl;
    }
}

// XML
// schema (SQL) have a separate column which stores hash
// Uml diagrams
// System design
// Component design
// Write on a white paper which explains the algorithm in some sort of diagrams

// White paper repository
// Github repository
// Documentation