#include "caesar.h"
void caesar_process_block(uint8_t* block, uint8_t shift,bool encrypt) {
    uint16_t tempblock = *block;
    if(encrypt) {
        *block = static_cast<uint8_t>((tempblock + shift) % UINT8_MAX);
    } else{
        *block = static_cast<uint8_t>((tempblock - shift) % UINT8_MAX);
    }
}