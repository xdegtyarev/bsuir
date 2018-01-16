//
// Created by Alexander Degtyarev on 5/17/17.
//
#include <cstdint>
#include "des.h"

void des_process_block(uint64_t* block, uint64_t* keys,bool encrypt) {
    uint64_t temp_block = 0;
    //initial permutation
    permutation(&temp_block,*block,des_ip,64);

    //split
    uint32_t block_l = 0;
    uint32_t block_r = 0;
    for(int i = 0; i < 32; i++)
    {
        set_bit(&block_l, temp_block, i, i);
        set_bit(&block_r, temp_block, 32+i, i);
    }
    if(encrypt) {
        des_encrypt_feistel(&block_l, &block_r, 16, keys);
    }else{
        des_decrypt_feistel(&block_l, &block_r, 16, keys);
    }
    //join
    for(int i = 0; i < 32; i++)
    {
        set_bit(&temp_block, block_l, i, i);
        set_bit(&temp_block, block_r, i, 32+i);
    }
    //final permutation
    permutation(block,temp_block,des_fp,64);
}

uint32_t des_f_round(const uint32_t data, uint64_t key){
    //1.
    uint64_t block48 = 0;
    des_expand(&block48,data);
    //2.
    block48 = block48 ^ key;
    //3.
    uint32_t block32 = 0;
    substitution(&block32,block48,6,4,8,16,des_sbox);
    //4.
    uint32_t temp_data = 0;
    permutation(&temp_data,block32,des_pbox,32);
    return temp_data;
}

void des_expand(uint64_t *target,const uint32_t src){
    for(int i = 0; i < 48; i++){
        set_bit(target,src,des_expansion[i]-1,i);
    }
}



void des_encrypt_feistel(uint32_t * left, uint32_t * right, int rounds, uint64_t * key){
    uint32_t temp = 0;
    for(int i = 0; i<rounds; i++){
        temp = *right ^ des_f_round(*left,key[i+1]);
        *right = *left;
        *left = temp;
    }
}

void des_decrypt_feistel(uint32_t * left, uint32_t * right, int rounds, uint64_t * key){
    uint32_t temp = 0;
    for(int i = rounds; i>0; i--){
        temp = *left ^ des_f_round(*right,key[i]);
        *left = *right;
        *right = temp;
    }
}

void des_key_expansion(uint64_t* key, const uint64_t startKey){
    const int keyblockCount = 7;
    int keyblockSize = sizeof(uint8_t)*8;
    for(int i = 0; i<keyblockCount;i++){
        for(int j = 0; j<keyblockSize+1;j++){
            int to = i*(keyblockSize+1)+j;
            int cbit = (i+1)*keyblockSize-1;
            if(to<cbit) {
                set_bit(key,startKey,i*8+j,to);
            }else if(to == cbit){
//                set_bit(key,1-(startKey[i]%2),0,cbit);
                set_bit(key,1,0,cbit);
            }else{
                set_bit(key,startKey,i*8+j-1,to);
            }
        }
    }
}

void des_generate_keys(uint64_t* keys,const uint64_t startKey) {
    keys[0] = startKey;
//    des_key_expansion(&keys[0],startKey);

    uint32_t* key_l = new uint32_t[16];
    uint32_t* key_r = new uint32_t[16];

    //PC1
    uint64_t pc1key = 0;
    permutation(&pc1key,keys[0],des_key_permutation,56);
    keys[0] = pc1key;
    uint32_t pc1key_l = 0;
    uint32_t pc1key_r = 0;
    //split into 2 keys by bits
    for(int i = 0; i < 28; i++)
    {
        set_bit(&pc1key_l, keys[0], i, i);
        set_bit(&pc1key_r, keys[0], 28+i, i);
    }

    for (int round = 0; round < 16; round++) {
        // 2. key_shifts
        cycle_shift_left(&key_l[round], round == 0 ? pc1key_l : key_l[round-1], 28,des_key_shift[round]);
        cycle_shift_left(&key_r[round], round == 0 ? pc1key_r : key_r[round-1], 28,des_key_shift[round]);
        //3.
        //join parts
        uint64_t joinedKey = 0;
        for(int i = 0; i < 28; i++)
        {
            set_bit(&joinedKey, key_l[round], i, i);
            set_bit(&joinedKey, key_r[round], i, i+28);
        }
        //4. permute2
        for(int i = 0; i < 48; i++){
            set_bit(&keys[round], joinedKey, des_key_permutation[56+i] - 1, i); //used 56 offset to start in 2nd part of perm
        }
//      printf("\n48K%d\n",round);
//      print_bits(keys[round],48);
    }
}
