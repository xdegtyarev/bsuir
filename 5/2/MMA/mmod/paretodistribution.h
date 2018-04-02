#ifndef PARETODISTRIBUTION_H
#define PARETODISTRIBUTION_H
#include <cmath>

class ParetoDistribution
{
public:
    ParetoDistribution(float _a, float _b);
    ~ParetoDistribution();
    float ProbabilityDensity(float x);
    float Distribution(float x);
    float InverseParetoDistribution(float y);

private:
    float a;
    float b;
};

#endif // PARETODISTRIBUTION_H
