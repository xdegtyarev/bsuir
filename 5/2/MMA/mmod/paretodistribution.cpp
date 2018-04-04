#include "paretodistribution.h"

ParetoDistribution::ParetoDistribution(double _a, double _b)
{
    a = _a;
    b = _b;
}

ParetoDistribution::~ParetoDistribution(){

}

double ParetoDistribution::InverseParetoDistribution(double y)
{
    double a1 = 1.0-y;
    double a2 = 1.0/a;
    double powr = pow(a1,a2);
    double res = b/powr;
    return res;
}

double ParetoDistribution::Distribution(double x){
    if(x>b){
        return 1.0-pow((b/x),a);
    }else{
        return 0.0;
    }
}

double ParetoDistribution::ProbabilityDensity(double x){
    if(x<b){
        return 0.0;
    }else{
        return a*pow(b,a)/pow(x,(a+1));
    }
}
