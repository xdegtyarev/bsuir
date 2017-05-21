//
// Created by Alexander Degtyarev on 5/17/17.
//
#include <cstdlib>
#include <ctime>
#include "gost.h"

uint32_t* gost28147_generateKeys(char* keytext,int count) {
    uint32_t* keys = new uint32_t[count];
    for(int i = 0; i<count; i++){
        keys[i] = 0;
        for(int j = 0; j<(sizeof(uint32_t)/sizeof(char)); j++){
            for(int k = 0; k<sizeof(char)*8;k++){
                set_bit(&keys[i],keytext[i*sizeof(uint32_t)+j],k,j*sizeof(char)*8+k);
            }
        }
    }
    return keys;
}

uint32_t* generateRandomKeys(int count){
    auto keys = new uint32_t[count];
    srand((unsigned int) time(0));
    for(int i = 0; i<count; i++){
        keys[i] = (uint32_t) rand();
    }
    return keys;
}

void gost28147_process_block(uint64_t *block, uint32_t *keys,bool encrypt) {
    //split
    uint32_t block_l = 0;
    uint32_t block_r = 0;
    for(int i = 0; i < 32; i++)
    {
        set_bit(&block_l, *block, i, i);
        set_bit(&block_r, *block, 32+i, i);
    }
    //f
    if(encrypt) {
        gost28147_encrypt_feistel(&block_l, &block_r, 32, keys);
    }else{
        gost28147_decrypt_feistel(&block_l, &block_r, 32, keys);
    }
    //join
    for(int i = 0; i < 32; i++)
    {
        set_bit(block, block_l, i, i);
        set_bit(block, block_r, i, 32+i);
    }
}

uint32_t gost28147_round(const uint32_t data, uint32_t key){
//    uint64_t data64 = data;
//    data64 = data64 + key;
//    data64 = data64 & 0xFFFFFFFF;
//    uint32_t tempBlock = (uint32_t) data64;
    uint32_t tempBlock = data;
    uint32_t substituted = 0;
    substitution(&substituted,tempBlock,4,4,8,16,gost28147_sbox,true);
    cycle_shift_left(&tempBlock,substituted,32,11);
    return tempBlock;
}

void gost28147_encrypt_feistel(uint32_t * left, uint32_t * right, int rounds, uint32_t * keys){
    uint32_t temp = 0;
    for(int i = 0; i<rounds; i++){
        temp = *right ^ gost28147_round(*left,keys[gost28147_key_indexes[i]-1]);
        *right = *left;
        *left = temp;
    }
}

void gost28147_decrypt_feistel(uint32_t * left, uint32_t * right, int rounds, uint32_t * keys){
    uint32_t temp = 0;
    for(int i = rounds; i>0; i--){
        temp = *left ^ gost28147_round(*right,keys[gost28147_key_indexes[i]-1]);
        *left = *right;
        *right = temp;
    }
}
