#ifndef SMONODE_H
#define SMONODE_H


#include <QVector>
#include <random>
class smonode
{
public:
    smonode();
    smonode(QVector<smonode*>* _input, double _mean, double _stdev, double _min, double _max, uint _queueLength, uint _chanelCount, uint _distType);
    void update(double deltaTime); //update in reverse order
    bool takeRequest();//reset timer and start over //if false = rejected
    bool giveRequest();//

    bool hasInput();
    bool hasOut();

    uint GetRejectedRequestsCount();
    uint GetProcessedRequestsCount();

    double averageRDP;
    double totalBusyTime;
    uint rejectedRequestCount;
    uint processedRequestsCount;
    double totalWaitTime;
    uint takenRequestCount;

private:
    void rollTimeDice(uint chanelId);
    QVector<smonode*>* input;
    QVector<double>* requestTimeLeft;

    uint inputQueue;
    uint outputQueue;
    uint chanelCount;
    uint distributionType;
    uint maxInputQueueLength;

    double linearDistribution;
    std::poisson_distribution<uint>* poissonDistribution;
    std::normal_distribution<double>* normalDistribution;
    std::gamma_distribution<double>* gammaDistribution;
    std::exponential_distribution<double>* exponentialDistribution;
    std::uniform_real_distribution<double>* uniformDistribution;
    std::default_random_engine* generator;
};

#endif // SMONODE_H
