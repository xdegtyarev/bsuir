#ifndef SMO_H
#define SMO_H
#include <QVector>
#include <smonode.h>

class smo
{

public:
    smo(uint _requestCount,uint _serverDeltaTime, uint _deltaTime,QVector<smonode*>* _nodes);
    QString launch();
    double qrej(int i);
    double qloss(int i);
    double qtime(int i);
    double rej();
    double time();
    double midtime();
private:
    void update();
    QVector<smonode*>* nodes;
    uint deltaTime;
    uint timeSinceBeginning;

    int requestDeltaProgress;

    uint requestCount;
    uint requestDeltaTime; //in ticks

    //stats
    int initialRequestCounter;
    double servingIntencity;
    int rejectedCounter;
    int completeCounter;

};

#endif // SMO_H
