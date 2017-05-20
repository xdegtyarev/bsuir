//
// Created by Alexander Degtyarev on 5/17/17.
//

#ifndef IPR1_DES_H
#define IPR1_DES_H
#include <cstdint>
#include <cstdio>

template <class T,class U>
void set_bit(T* target,const U src,const int from,const int to){
    *target ^= (-((src & (1ULL << from)) != 0) ^ *target) & (1ULL << to);
}

template <class T>
void cycle_shift_left(T* target, const T src,const int size, int count){
    for(int i = 0;i<count;i++) {
        set_bit(target, src, size - count + i, i);
    }
    for(int i = 0;i<size-count;i++){
        set_bit(target,src,i,i+count);
    }
}

template <class T,class U>
void permutation(T *target, const U src, const int* map, int length){
    for(int i = 0; i<length; i++){
        set_bit(target,src,map[i]-1,i);
    }
}

template <class T,class U>
void substitution(T *target, const U src, const int blockSize, const int newBlockSize, const int blockCount, const int columns, const int *map){
    for(int i = 0; i < blockCount; i++)
    {
        int subblock = 0;
        int substituted = 0;
        //split block
        for(int j = 0; j<blockSize; j++) {
            set_bit(&subblock, src, i * blockSize + j, j);
        }
        //get y
        uint32_t y = 0;
        set_bit(&y,subblock,0,0);
        set_bit(&y,subblock,blockSize-1,1);
        //get x
        uint32_t x = 0;
        for(int j = 1, k = 0;  j<(blockSize-2);j++, k++){
            set_bit(&x,subblock,j,k);
        }
        substituted = map[i*64+y*columns+x];
        //join block
        for(int j = 0; j<newBlockSize; j++){
            set_bit(target, substituted, j,i*newBlockSize + j);
        }
    }
}

template <class T>
void print_bits(T x,int size = 0){
    int numbits = size == 0 ? (8*sizeof(T)) : size;
    while(--numbits >= 0) printf(numbits%8 == 0 ? "%c ":"%c", (x & ((T)1<<numbits)) ? '1' : '0');
    printf("\n");
}

void des_generate_keys(uint64_t* keys,const uint64_t startKey);

void des_expand(uint64_t *target,const uint32_t src);


uint32_t des_f_round(const uint32_t data, uint64_t key);

void des_encrypt_block(uint64_t* block, uint64_t* keys);
void des_decrypt_block(uint64_t* block, uint64_t* keys);

//generic feistel stuff
void decrypt_feistel(uint32_t *left, uint32_t *right, int rounds, uint64_t *key);
void encrypt_feistel(uint32_t* left, uint32_t* right, int rounds, uint64_t* key);
#endif //IPR1_DES_H
