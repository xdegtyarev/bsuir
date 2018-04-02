#ifndef MIDDLESQUARERNG_H
#define MIDDLESQUARERNG_H
#include <stdint.h>
#include <qvector.h>
class MiddleSquareRNG
{
public:
    MiddleSquareRNG(uint64_t seed);
    ~MiddleSquareRNG();
    uint64_t Next();
    void RandomSet(uint64_t count,QVector<uint64_t>* set);
    bool Validate();
private:
    uint64_t counter;
    QVector<uint64_t>* prev;
};

#endif // MIDDLESQUARERNG_H
