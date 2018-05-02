#include "smonode.h"
smonode::smonode(){}

smonode::smonode(QVector<smonode*>* _input, double _mean, double _stdev, double _min, double _max, uint _queueLength, uint _chanelCount, uint _distType)
{
    input = _input;

    inputQueue = 0;
    outputQueue = 0;

    maxInputQueueLength = _queueLength;
    averageRDP = 0.0;

    totalBusyTime = 0.0;
    rejectedRequestCount = 0;
    processedRequestsCount = 0;
    totalWaitTime = 0.0;
    takenRequestCount = 0;
    distributionType = _distType;
    //mean
    poissonDistribution = new std::poisson_distribution<uint>(1.0/_mean);
    normalDistribution = new std::normal_distribution<double>(1.0/_mean,_stdev);
    gammaDistribution = new std::gamma_distribution<double>(1.0/_mean,_stdev);
    exponentialDistribution = new std::exponential_distribution<double>(_mean);
    linearDistribution = 1.0/_mean;
    //ab
    uniformDistribution = new std::uniform_real_distribution<double>(1.0/_max,1.0/_min);
    chanelCount = _chanelCount;
    requestTimeLeft = new QVector<double>(chanelCount);
    for(uint chanelId=0;chanelId<chanelCount;chanelId++) rollTimeDice(chanelId);
}

void smonode::rollTimeDice(uint chanelId){
    std::random_device rd;
    std::mt19937 gen(rd());
    switch(distributionType){
    case 0:
      requestTimeLeft->data()[chanelId] = (*poissonDistribution)(gen);
      break;
    case 1:
      requestTimeLeft->data()[chanelId] = (*normalDistribution)(gen);
      break;
    case 2:
      requestTimeLeft->data()[chanelId] = linearDistribution;
      break;
    case 3:
      requestTimeLeft->data()[chanelId] = (*exponentialDistribution)(gen);
      break;
    case 4:
      requestTimeLeft->data()[chanelId] = (*uniformDistribution)(gen);
      break;
    default:
      requestTimeLeft->data()[chanelId] = (*poissonDistribution)(gen);
      break;
    }
    averageRDP += requestTimeLeft->data()[chanelId];
}

void smonode::update(double deltaTime)
{
    if(chanelCount>0){
        if(input!=NULL && input->count()>0){
            for(int i = 0; i<input->count(); i++){
                if(input->data()[i]->giveRequest()){
                    takeRequest();
                    break;
                }
            }
        }
        bool isBusyThisRound = false;
        for(int chanelId = chanelCount-1; chanelId>=0; chanelId--){
            if(inputQueue>chanelId){
                requestTimeLeft->data()[chanelId]-=deltaTime;
                totalBusyTime+=deltaTime;
                isBusyThisRound = true;
                if(requestTimeLeft->data()[chanelId]<=0.0){//request done
                    inputQueue--;
                    processedRequestsCount++;
                    outputQueue++;
                    rollTimeDice(chanelId);
                }
            }
        }
        if(!isBusyThisRound){
            totalWaitTime+=deltaTime;
        }
    }else{
        while(inputQueue>0){
            inputQueue--;
            outputQueue++;
        }
    }
}

bool smonode::hasInput(){return inputQueue>0;}
bool smonode::hasOut(){return outputQueue>0;}

bool smonode::takeRequest(){
    if(inputQueue<=(maxInputQueueLength+chanelCount)){
        inputQueue++;
        takenRequestCount++;
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
