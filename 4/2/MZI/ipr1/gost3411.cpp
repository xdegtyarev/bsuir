//
// Created by Alexander Degtyarev on 5/22/17.
//

#include <cstdint>
#include "gost3411.h"

uint32_t h0[8] = {0,0,0,0,0,0,0,0};
uint32_t k1[8] = {0,0,0,0,0,0,0,0};
uint32_t k2[8] = {0,0,0,0,0,0,0,0};
uint32_t k3[8] = {0,0,0,0,0,0,0,0};
uint32_t k4[8] = {0,0,0,0,0,0,0,0};

uint32_t c2[8] = {0,0,0,0,0,0,0,0};
uint32_t c3[8] = {0xFF00FFFF,0x000000FF,0xFF0000FF,0x00FFFF00,0x00FF00FF,0x00FF00FF,0xFF00FF00,0xFF00FF00};
uint32_t c4[8] = {0,0,0,0,0,0,0,0};
//φ(i + 1 + 4(k - 1)) = 8i + k ; i = 0 ÷ 3, k = 1 ÷ 8
//
//gost3411_permutation(){
////    32 uint8_t
//    y = f(x) x - > num of
//
//}
//
//gost3411_shif(){
//    a(x) = x1 ^ x2 | x4 | x3 | x2
//}

//uint32_t* gost3411_gen_keys(uint32_t* h,uint32_t* block){
//
//}
//
//uint32_t* gost3411(uint32_t* prev_hash,uint32_t* block){
//    block is 256 bit size/
    //gen key
    //encrypt 64bit values of H with 28147
    //permutation


//    uint32_t hash;

//    return hash;
//}