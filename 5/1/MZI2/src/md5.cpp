#include "md5.h"
void md5_process_block(uint32_t* block, uint32_t* h0,uint32_t* h1,uint32_t* h2,uint32_t* h3 ){
    uint32_t a = *h0;
    uint32_t b = *h1;
    uint32_t c = *h2;
    uint32_t d = *h3;

    uint32_t i;
    for(i = 0; i<64; i++) {
        uint32_t f, g;

        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5*i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3*i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7*i) % 16;
        }
        uint32_t temp = d;
        d = c;
        c = b;
        b = b + LEFTROTATE((a + f + md5_k[i] + block[g]), md5_round_shifts[i]);
        a = temp;
    }

    *h0 += a;
    *h1 += b;
    *h2 += c;
    *h3 += d;
}

