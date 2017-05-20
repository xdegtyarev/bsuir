//
// Created by Alexander Degtyarev on 5/17/17.
//

#include "gost.h"

int gost28147_round(int subblock,int key){
    //(subbloc + key) mod 2^32
    //split in 8 4bit blocks
    //to s-blocks
    //join bitshift 11bit
    //return
}

void gost_encrypt_block(uint64_t *block, uint64_t *keys) {
    //initialPermutation();
    //encodingIteration();
    //finalPermutation();
    //round count 32
}

void gost_decrypt_block(uint64_t *block, uint64_t *keys) {

}
