#ifndef LINEARCONGRUENTIALRNG_H
#define LINEARCONGRUENTIALRNG_H
#include <qvector.h>

class LinearCongruentialRNG
{
public:
    LinearCongruentialRNG(uint64_t seed, uint64_t increment,uint64_t modulus,uint64_t multiplier);
    ~LinearCongruentialRNG();
    uint64_t Next();
    void RandomSet(uint count,QVector<uint64_t>* set);
    bool Validate();
private:
    uint64_t multiplier = 5;
    uint64_t increment = 3; // if 0 Multiplicative CRNG
    uint64_t modulus = 8;//prime | power of 2
    uint64_t seed;
    QVector<uint64_t>* prev;
};

#endif // LINEARCONGRUENTIALRNG_H
