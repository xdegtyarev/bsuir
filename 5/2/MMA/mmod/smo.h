#ifndef SMO_H
#define SMO_H
#include <QVector>
#include <smonode.h>

class smo
{

public:
    smo(double _requestIntensity, double _optparam, double _min, double _max, double _modelTime, double _deltaTime,uint _distType,QVector<smonode*>* _nodes);
    QString launch(int num);
    double qcount(int i);
    double qrej(int i);
    double qrejcount(int i);
    double qtime(int i);
    double rej();
    double rejcount();
    double time();
    double midtime();
private:
    void update();
    void rollTimeDice();
    QVector<smonode*>* nodes;
    double deltaTime;
    double timeSinceBeginning;
    double averageRDP = 0.0;
    uint distributionType;

    double requestDeltaProgress;

    double requestIntensity;
    double modelTime;

    uint requestCount;

    double linearDistribution;
    std::poisson_distribution<uint>* poissonDistribution;
    std::normal_distribution<double>* normalDistribution;
    std::gamma_distribution<double>* gammaDistribution;
    std::exponential_distribution<double>* exponentialDistribution;
    std::uniform_real_distribution<double>* uniformDistribution;
};

#endif // SMO_H
