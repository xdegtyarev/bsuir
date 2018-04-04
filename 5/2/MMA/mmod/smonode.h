#ifndef SMONODE_H
#define SMONODE_H


#include <QVector>
#include <random>
class smonode
{
public:
    smonode();
    smonode(QVector<smonode*>* input, uint min, uint max, double _mean, uint i, uint o, uint inputMax, uint outMax);
    void update(uint deltaTime); //update in reverse order//true if node ended working with request
    bool takeRequest();//reset timer and start over //if false = rejected
    bool giveRequest();//

    bool hasInput();
    bool hasOut();

    uint GetRejectedRequestsCount();
    uint GetProcessedRequestsCount();

    uint totalRequestTime;
    uint rejectedRequestCount;
    uint lostRequestCount;
    uint processedRequestsCount;
    uint totalWaitTime;
    uint takenReuqestCount;
private:
    QVector<smonode*>* input;

    double mean;
    uint inputQueue;
    uint outputQueue;

    uint max;
    uint min;

    uint maxInputQueueLength; //0-no limit
    uint maxOutputQueueLength; //0-no limit

    int requestTimeLeft;

    std::poisson_distribution<uint>* distribution;
    std::default_random_engine generator;
};

#endif // SMONODE_H
