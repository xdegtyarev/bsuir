//
// Created by Alexander Degtyarev on 5/21/17.
//

#ifndef IPR1_RSA_H
#define IPR1_RSA_H
uint64_t powm(uint32_t base,int32_t exp, uint32_t modulus);
void rsa_generate_keys(uint32_t bits, uint32_t e);
template <typename T> uint32_t rsa_process_block(uint32_t n,uint32_t m,T block){
    return powm(block, m, n);
}
#endif //IPR1_RSA_H
