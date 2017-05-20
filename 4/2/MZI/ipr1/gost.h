//
// Created by Alexander Degtyarev on 5/17/17.
//

#ifndef IPR1_GOST_H
#define IPR1_GOST_H
#include <cstdint>
void gost_encrypt_block(uint64_t* block, uint64_t* keys);
void gost_decrypt_block(uint64_t* block, uint64_t* keys);



#endif //IPR1_GOST_H
