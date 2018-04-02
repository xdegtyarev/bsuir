#include "middlesquarerng.h"

MiddleSquareRNG::MiddleSquareRNG(uint64_t seed)
{
    prev = new QVector<uint64_t>();
    prev->push_back(seed+0xb5ad4eceda1ce2a9 | 1);
}

MiddleSquareRNG::~MiddleSquareRNG()
{
    delete(prev);
}

uint64_t MiddleSquareRNG::Next(){

    uint64_t number = prev->last();
    do{
        counter+=prev->last();
        number = ((number*number+counter)>>32) | ((number*number+counter)<<32);
    }while(prev->contains(number));
    prev->push_back(number);
    return prev->last();
}

void MiddleSquareRNG::RandomSet(uint64_t count,QVector<uint64_t>* set){
    set->clear();
    set->resize(count);
    for(uint64_t i = 0; i<count; i++ ){
        set->data()[i] = Next();
    }
}

bool MiddleSquareRNG::Validate()
{
    return true;
}
