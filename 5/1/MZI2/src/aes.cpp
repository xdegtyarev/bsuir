//
// Created by xdegtyarev on 1/11/18.
//

#include "aes.h"
void aes_generate_key(uint8_t *roundKey, const uint8_t *key) {
    unsigned i, j, k;
    uint8_t tempa[4];
    // The first round key is the key itself.
    for (i = 0; i < aes_nkeys; ++i)
    {
        roundKey[(i * 4) + 0] = key[(i * 4) + 0];
        roundKey[(i * 4) + 1] = key[(i * 4) + 1];
        roundKey[(i * 4) + 2] = key[(i * 4) + 2];
        roundKey[(i * 4) + 3] = key[(i * 4) + 3];
    }

    for (i = aes_nkeys; i < aes_ncol * (aes_nrounds + 1); ++i)
    {
        {
            k = (i - 1) * 4;
            tempa[0]=roundKey[k + 0];
            tempa[1]=roundKey[k + 1];
            tempa[2]=roundKey[k + 2];
            tempa[3]=roundKey[k + 3];
        }
        if (i % aes_nkeys == 0)
        {
            // 4byte cyclic shift - Function RotWord()
            {
                k = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = k;
            }
            // Function Subword() SBOX per each 4bytes
            {
                tempa[0] = aes_sbox[tempa[0]];
                tempa[1] = aes_sbox[tempa[1]];
                tempa[2] = aes_sbox[tempa[2]];
                tempa[3] = aes_sbox[tempa[3]];
            }
            tempa[0] = tempa[0] ^ aes_rcon[i/aes_nkeys];
        }
        j = i * 4; k=(i - aes_nkeys) * 4;
        roundKey[j + 0] = roundKey[k + 0] ^ tempa[0];
        roundKey[j + 1] = roundKey[k + 1] ^ tempa[1];
        roundKey[j + 2] = roundKey[k + 2] ^ tempa[2];
        roundKey[j + 3] = roundKey[k + 3] ^ tempa[3];
    }
}

void aes_add_round_key(uint8_t round, state_t* state, uint8_t* roundKey){
    uint8_t i,j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            *state[i][j] ^= roundKey[(round * aes_ncol * 4) + (i * aes_ncol) + j];
        }
    }
}

void aes_subbytes(state_t* state, bool inv){
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            if(inv) {
                (*state)[j][i] = aes_inv_sbox[(*state)[j][i]];
            }else{
                (*state)[j][i] = aes_sbox[(*state)[j][i]];
            }
        }
    }
}

void aes_shift_rows(state_t* state, bool inv) {
    uint8_t temp;
    if(inv){
        //r1 right 1
        temp = (*state)[3][1];
        (*state)[3][1] = (*state)[2][1];
        (*state)[2][1] = (*state)[1][1];
        (*state)[1][1] = (*state)[0][1];
        (*state)[0][1] = temp;
        //r2 right 2
        temp = (*state)[0][2];
        (*state)[0][2] = (*state)[2][2];
        (*state)[2][2] = temp;
        temp = (*state)[1][2];
        (*state)[1][2] = (*state)[3][2];
        (*state)[3][2] = temp;
        //r3 right 3
        temp = (*state)[0][3];
        (*state)[0][3] = (*state)[1][3];
        (*state)[1][3] = (*state)[2][3];
        (*state)[2][3] = (*state)[3][3];
        (*state)[3][3] = temp;
    }else {
        //row1 left 1
        temp = (*state)[0][1];
        (*state)[0][1] = (*state)[1][1];
        (*state)[1][1] = (*state)[2][1];
        (*state)[2][1] = (*state)[3][1];
        (*state)[3][1] = temp;
        //row2 left 2
        temp = (*state)[0][2];
        (*state)[0][2] = (*state)[2][2];
        (*state)[2][2] = temp;
        temp = (*state)[1][2];
        (*state)[1][2] = (*state)[3][2];
        (*state)[3][2] = temp;
        //row3 left 3
        temp = (*state)[0][3];
        (*state)[0][3] = (*state)[3][3];
        (*state)[3][3] = (*state)[2][3];
        (*state)[2][3] = (*state)[1][3];
        (*state)[1][3] = temp;
    }
}

uint8_t xtime(uint8_t x)
{
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

uint8_t aes_multiply(uint8_t x, uint8_t y)
{
    return (((y & 1) * x) ^
            ((y>>1 & 1) * xtime(x)) ^
            ((y>>2 & 1) * xtime(xtime(x))) ^
            ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

void aes_mix_columns(state_t * state,bool inv)
{
    uint8_t i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i)
    {
        if(inv) {
            a = (*state)[i][0];
            b = (*state)[i][1];
            c = (*state)[i][2];
            d = (*state)[i][3];

            (*state)[i][0] = aes_multiply(a, 0x0e) ^ aes_multiply(b, 0x0b) ^ aes_multiply(c, 0x0d) ^ aes_multiply(d, 0x09);
            (*state)[i][1] = aes_multiply(a, 0x09) ^ aes_multiply(b, 0x0e) ^ aes_multiply(c, 0x0b) ^ aes_multiply(d, 0x0d);
            (*state)[i][2] = aes_multiply(a, 0x0d) ^ aes_multiply(b, 0x09) ^ aes_multiply(c, 0x0e) ^ aes_multiply(d, 0x0b);
            (*state)[i][3] = aes_multiply(a, 0x0b) ^ aes_multiply(b, 0x0d) ^ aes_multiply(c, 0x09) ^ aes_multiply(d, 0x0e);
        }else {
            a = (*state)[i][0];
            b = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
            c = (*state)[i][0] ^ (*state)[i][1];
            c = xtime(c);
            (*state)[i][0] ^= c ^ b;
            c = (*state)[i][1] ^ (*state)[i][2];
            c = xtime(c);
            (*state)[i][1] ^= c ^ b;
            c = (*state)[i][2] ^ (*state)[i][3];
            c = xtime(c);
            (*state)[i][2] ^= c ^ b;
            c = (*state)[i][3] ^ a;
            c = xtime(c);
            (*state)[i][3] ^= c ^ b;
        }
    }
}

void aes_encrypt(state_t* state, uint8_t* roundKey)
{
    uint8_t round = 0;
    aes_add_round_key(0, state, roundKey);

    for (round = 1; round < aes_nrounds; ++round)
    {
        aes_subbytes(state,false);
        aes_shift_rows(state, false);
        aes_mix_columns(state,false);
        aes_add_round_key(round, state, roundKey);
    }
    aes_subbytes(state,false);
    aes_shift_rows(state,false);
    aes_add_round_key(aes_nrounds, state, roundKey);
}

void aes_decrypt(state_t* state,uint8_t* roundKey)
{
    uint8_t round = 0;
    aes_add_round_key(aes_nrounds, state, roundKey);

    for (round = (aes_nrounds - 1); round > 0; --round)
    {
        aes_shift_rows(state,true);
        aes_subbytes(state,true);
        aes_add_round_key(round, state, roundKey);
        aes_mix_columns(state,true);
    }
    aes_shift_rows(state, true);
    aes_subbytes(state,true);
    aes_add_round_key(0, state, roundKey);
}

void aes_process_block(uint8_t* block, uint8_t* roundKey,bool encrypt){
    state_t* state = (state_t*) block;
    if(encrypt){
        aes_encrypt(state,roundKey);
    }else{
        aes_decrypt(state,roundKey);
    };
}

