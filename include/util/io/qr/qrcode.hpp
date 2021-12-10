#pragma once

#include <string>
#include <vector>
#include "common.hpp"
#include "evm/receipt.hpp"
#include "signature/signature.hpp"
#include "QR_Code_generator/QrCode.hpp"


namespace Qrcode
{
    void generate_qr_code(Voter_Receipt* vt_receipt);
}