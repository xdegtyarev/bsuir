#include "smonode.h"

smonode::smonode(){

}

smonode::smonode(QVector<smonode*>* _input, uint _min, uint _max, double _mean, uint i, uint o, uint inputMax, uint outMax)
{
    input = _input;

    mean = _mean;

    inputQueue = i;
    outputQueue = o;

    inputQueue = 0;
    outputQueue = 0;

    min = _min;
    max = _max;

    maxInputQueueLength = inputMax;
    maxOutputQueueLength = outMax;

    distribution = new std::poisson_distribution<uint>(mean);
    requestTimeLeft = (*distribution)(generator);//roll time dice

    totalRequestTime = 0;
    rejectedRequestCount = 0;
    lostRequestCount = 0;
    processedRequestsCount = 0;
    totalWaitTime = 0;
    takenReuqestCount = 0;

}

void smonode::update(uint deltaTime)
{
    if(input!=NULL && input->count()>0){
        for(int i = 0; i<input->count(); i++){
            if(input->data()[i]->giveRequest()){
                takeRequest();
                break;
            }
        }
    }

    if(inputQueue>0){
        requestTimeLeft-=deltaTime;
        totalRequestTime+=deltaTime;
        if(requestTimeLeft<=0){
            requestTimeLeft = (*distribution)(generator);//roll time dice
            //request done
            inputQueue--;
            processedRequestsCount++;//stats
            if(maxOutputQueueLength==0 || outputQueue<maxOutputQueueLength){
                outputQueue++;
            }else{
                lostRequestCount++;
            }
        }
    }else{
        totalWaitTime+=deltaTime;
    }
}

bool smonode::hasInput(){
    return inputQueue>0;
}

bool smonode::hasOut(){
    return outputQueue>0;
}

bool smonode::takeRequest(){
    if(maxInputQueueLength==0 || inputQueue<=maxInputQueueLength){
        inputQueue++;
        takenReuqestCount++;
        return true;
    }else{
        rejectedRequestCount++;
        return false;
    }
}

bool smonode::giveRequest(){
    if(outputQueue>0){
        outputQueue--;
        return true;
    }else{
        return false;
    }
}
