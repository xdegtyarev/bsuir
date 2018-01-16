#ifndef MZI_ECC_H
#define MZI_ECC_H
#include <cstdint>
#include "bitvector.h"
#include "eccfield.h"
int ecdh_generate_keys(uint8_t* publicKey, uint8_t* privateKey);
int ecdh_shared_secret(const uint8_t* privateKey, const uint8_t* otherPublicKey, uint8_t* sharedSecret);
#endif //MZI_ECC_H
