#include "linearcongruentialrng.h"

LinearCongruentialRNG::LinearCongruentialRNG(uint64_t _seed, uint64_t _increment,uint64_t _modulus,uint64_t _multiplier)
{
    prev = new QVector<uint64_t>();
    prev->append(_seed);
    increment = _increment;
    multiplier = _multiplier;
    modulus = _modulus;
    seed = _seed;
}

LinearCongruentialRNG::~LinearCongruentialRNG()
{
    delete(prev);
}

uint64_t LinearCongruentialRNG::Next()
{
    uint64_t number = 0;
    bool end = false;
    do{
        number = multiplier * prev->last();
        number += increment;
        number %= modulus;
        //number = (multiplier * prev->last() + increment) % modulus;
        if(!prev->contains(number)){
            end = false;
            prev->push_back(number);
        }else{
            end = true;
        }
    }while(end);

    return prev->last();
}

void LinearCongruentialRNG::RandomSet(uint count, QVector<uint64_t> *set)
{
    set->clear();
    set->resize(count);
    for(uint i = 0; i<count; i++ ){
        set->data()[i] = Next();
    }
}

bool LinearCongruentialRNG::Validate()
{
    return modulus > 0 &&
            multiplier > 0ULL && multiplier < modulus &&
            increment >= 0ULL && increment < modulus &&
            seed >= 0ULL && seed < modulus;
}
