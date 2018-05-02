#include "smo.h"

smo:: smo(double _requestIntensity, double _optparam, double _min, double _max, double _modelTime, double _deltaTime,uint _distType,QVector<smonode*>* _nodes){
    requestIntensity = _requestIntensity;
    requestCount = 0;
    modelTime = _modelTime;
    deltaTime = _deltaTime;
    nodes = _nodes;
    distributionType = _distType;
    timeSinceBeginning = 0;
    requestDeltaProgress = 0;

    //setup distr
    //mean
    poissonDistribution = new std::poisson_distribution<uint>(1.0/_requestIntensity);
    normalDistribution = new std::normal_distribution<double>(1.0/_requestIntensity,_optparam);
    exponentialDistribution = new std::exponential_distribution<double>(_requestIntensity);
    linearDistribution = 1.0/_requestIntensity;
    //ab
    uniformDistribution = new std::uniform_real_distribution<double>(1.0/_min,1.0/_max);
    rollTimeDice();
}

QString smo::launch(int num){
    bool hasWork = true;
    while(hasWork){
        update();
        hasWork = timeSinceBeginning<modelTime;
        if(!hasWork){
            for(int i = 0; i<nodes->count();i++){
                hasWork|=nodes->data()[i]->hasInput();
                if(i<(nodes->count()-1)){ //dont count last node out
                    hasWork|=nodes->data()[i]->hasOut();
                }
            }
        }
    }

    QString resultString = "\nЭксперимент №" + QString::number(num);
    resultString = resultString.append("\nСреднее время между заявками: " + QString::number(averageRDP/((double)requestCount)));
    resultString = resultString.append("\nСредняя интенсивность заявок: " + QString::number(((double)requestCount)/averageRDP));
//    resultString = resultString.append("\nИнтенсивность входящего потока:" + QString::number((double)requestCount/(double)modelTime,'f',4) + " заявок в ед.времени");
    resultString = resultString.append("\nВсего заявок создано:" + QString::number(requestCount));
    resultString = resultString.append("| обработано:" + QString::number(nodes->data()[2]->processedRequestsCount));
    resultString = resultString.append("| отказов "  + QString::number(rejcount()) + " [ " + QString::number(rej()) + "%]");
    resultString = resultString.append("\nВсего времени затрачено: " + QString::number(time()));
    resultString = resultString.append("| среднее время обработки заявки: " + QString::number(midtime()));
    resultString = resultString.append("\n------------------------------------------------------");
    for(int i = 0; i<nodes->count();i++){
        smonode* n = nodes->data()[i];
        resultString = resultString.append("\nФаза № " + QString::number(i)
                    + " | Обработано: " + QString::number(qcount(i))
                    + " | Отказ: " + QString::number(qrejcount(i)) + " [ " + QString::number(qrej(i),'f',3) + "%]"
                    + " | Среднее время обработки: " + QString::number(qtime(i),'f',6)
                    + " | Средняя интенсивность обработки: " + QString::number(1.0/qtime(i),'f',6)
                    + " | Ожидание: " + QString::number(n->totalWaitTime,'f',4));
    }

    resultString = resultString.append("\n========================================================");
    return resultString;
}

double smo::qcount(int i){return nodes->data()[i]->processedRequestsCount;}
double smo::qrej(int i){return 100.0*(double)nodes->data()[i]->rejectedRequestCount/(double)requestCount;}
double smo::qrejcount(int i){return (double)nodes->data()[i]->rejectedRequestCount;}
//double smo::qtime(int i){return (double)nodes->data()[i]->totalBusyTime/(double)nodes->data()[i]->takenRequestCount;}
double smo::qtime(int i){return (double)nodes->data()[i]->averageRDP/(double)nodes->data()[i]->takenRequestCount;}
double smo::rej(){return 100.0*(1.0-(double)nodes->data()[2]->processedRequestsCount/(double)requestCount);}
double smo::rejcount(){return (double)(requestCount - nodes->data()[2]->processedRequestsCount);}
double smo::time(){return (double)timeSinceBeginning;}
double smo::midtime(){return (double)timeSinceBeginning/(double)requestCount;}

void smo::rollTimeDice(){
    std::random_device rd;
    std::mt19937 gen(rd());
    switch(distributionType){
    case 0:
      requestDeltaProgress = (*poissonDistribution)(gen);
      break;
    case 1:
      requestDeltaProgress = (*normalDistribution)(gen);
      break;
    case 2:
      requestDeltaProgress = linearDistribution;
      break;
    case 3:
      requestDeltaProgress = (*exponentialDistribution)(gen);
      break;
    case 4:
      requestDeltaProgress = (*uniformDistribution)(gen);
      break;
    default:
      requestDeltaProgress = (*poissonDistribution)(gen);
      break;
    }
    averageRDP += requestDeltaProgress;
}

void smo::update(){
    //update nodes
    for(int i = nodes->count()-1; i>=0;i--){
        nodes->data()[i]->update(deltaTime);
    }

    //server update
    if(timeSinceBeginning<modelTime){
        requestDeltaProgress-=deltaTime;
        if(requestDeltaProgress<=0){
            nodes->data()[0]->takeRequest();
            rollTimeDice();
            requestCount++;
        }
    }

    timeSinceBeginning+=deltaTime;
}
