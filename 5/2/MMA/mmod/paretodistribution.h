#ifndef PARETODISTRIBUTION_H
#define PARETODISTRIBUTION_H
#include <cmath>

class ParetoDistribution
{
public:
    ParetoDistribution(double _a, double _b);
    ~ParetoDistribution();
    double ProbabilityDensity(double x);
    double Distribution(double x);
    double InverseParetoDistribution(double y);

private:
    double a;
    double b;
};

#endif // PARETODISTRIBUTION_H
