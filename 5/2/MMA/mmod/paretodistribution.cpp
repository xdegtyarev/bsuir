#include "paretodistribution.h"

ParetoDistribution::ParetoDistribution(float _a, float _b)
{
    a = _a;
    b = _b;
}

float ParetoDistribution::InverseParetoDistribution(float y)
{
    return b/pow((1.0-y),(1.0/a));
}

float ParetoDistribution::Distribution(float x){
    if(x>b){
        return 1.0-pow((b/x),a);
    }else{
        return 0.0;
    }
}

float ParetoDistribution::ProbabilityDensity(float x){
    if(x<b){
        return 0.0;
    }else{
        return a*pow(b,a)/pow(x,(a+1));
    }
}
