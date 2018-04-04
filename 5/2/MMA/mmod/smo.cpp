#include "smo.h"

smo::smo(uint _requestCount,uint _requestDeltaTime, uint _deltaTime,QVector<smonode*>* _nodes){
    requestCount = _requestCount;
    requestDeltaProgress = 0;
    initialRequestCounter = _requestCount;
    requestDeltaTime = _requestDeltaTime;
    deltaTime = _deltaTime;
    nodes = _nodes;

    servingIntencity = 1.0/(double)requestDeltaTime;
    rejectedCounter = 0;
    completeCounter = 0;
    timeSinceBeginning = 0;
}

QString smo::launch(){
    bool hasWork = true;
    while(hasWork){
        update();
        hasWork = requestCount>0;
        if(!hasWork){
            for(int i = 0; i<nodes->count();i++){
                hasWork|=nodes->data()[i]->hasInput();
                if(i<(nodes->count()-1)){ //dont count last node out
                    hasWork|=nodes->data()[i]->hasOut();
                }
            }
        }
    }
    QString resultString = "\n======================";
    for(int i = 0; i<nodes->count();i++){
        smonode* n = nodes->data()[i];
        resultString = resultString.append("\nQ" + QString::number(i)
                    + " rej:" + QString::number(qrej(i),'g',2)
                    + " loss:" + QString::number(qloss(i),'g',2)
                    + " midProc:" + QString::number(qtime(i),'g',2)
                    + " wait:" + QString::number(n->totalWaitTime)
                    );
    }
    resultString = resultString.append("\nTotal time: " + QString::number(time()));
    resultString = resultString.append("\nTotal reject: " + QString::number(rej()));
    resultString = resultString.append("\nTotal midtime: " + QString::number(midtime()));
    resultString = resultString.append("\n======================");
    return resultString;
}

double smo::qrej(int i){return (double)nodes->data()[i]->rejectedRequestCount/(double)nodes->data()[i]->takenReuqestCount;}
double smo::qloss(int i){return (double)nodes->data()[i]->lostRequestCount/(double)nodes->data()[i]->processedRequestsCount;}
double smo::qtime(int i){return (double)nodes->data()[i]->totalRequestTime/(double)nodes->data()[i]->takenReuqestCount;}
double smo::rej(){return 1.0-(double)nodes->data()[2]->processedRequestsCount/(double)initialRequestCounter;}
double smo::time(){return (double)timeSinceBeginning;}
double smo::midtime(){return (double)timeSinceBeginning/(double)initialRequestCounter;}
void smo::update(){
    //server update
    timeSinceBeginning+=deltaTime;

    if(requestCount>0){
        requestDeltaProgress-=deltaTime;
        if(requestDeltaProgress<=0){
            requestCount--;
            nodes->data()[0]->takeRequest();
            requestDeltaProgress = requestDeltaTime;
        }
    }
    //update nodes
    for(int i = nodes->count()-1; i>=0;i--){
        nodes->data()[i]->update(deltaTime);
    }
}
