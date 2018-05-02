#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linearcongruentialrng.h"
#include "middlesquarerng.h"
#include "paretodistribution.h"
#include <QRandomGenerator>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QValidator *intValidator0MAX = new QIntValidator(0,INT_MAX);
    QValidator *intValidator1MAX = new QIntValidator(1,INT_MAX);
    QValidator *doubleValidatorMINMAX = new QDoubleValidator(DBL_MIN,DBL_MAX,100);
    QValidator *doubleValidator0MAX = new QDoubleValidator(0.0,DBL_MAX,100);
    maxTime = new QVector<double>();
    //setup BRV window
    ui->BRVMCGCountEdit->setValidator(intValidator1MAX);
    ui->BRVMCGIncrementEdit->setValidator(intValidator0MAX);
    ui->BRVMCGModulusEdit->setValidator(intValidator1MAX);
    ui->BRVMCGMultiplierEdit->setValidator(intValidator1MAX);
    ui->BRVMCGSeedEdit->setValidator(intValidator0MAX);

    BRVMSChart = new QChart();
        BRVMSChart->legend()->hide();
        BRVMSChart->setTitle("Метод середины квадрата");
        BRVMSChart->legend()->setVisible(true);
        BRVMSChart->legend()->setAlignment(Qt::AlignBottom);


    BRVMCGChart = new QChart();
        BRVMCGChart->legend()->hide();
        BRVMCGChart->setTitle("Мультипликативный конгруэнтный метод");
        BRVMCGChart->legend()->setVisible(true);
        BRVMCGChart->legend()->setAlignment(Qt::AlignBottom);

     BRVMSChartView = new QChartView(BRVMSChart,ui->BRVMSGroupBox);
     BRVMSChartView->resize(400,400);
     BRVMSChartView->move(32,32);
     BRVMSChartView->setRenderHint(QPainter::Antialiasing);
     BRVMSChartView->show();

     BRVMCGChartView = new QChartView(BRVMCGChart,ui->BRVMCGGroupBox);
     BRVMCGChartView->resize(400,400);
     BRVMCGChartView->move(32,32);
     BRVMCGChartView->setRenderHint(QPainter::Antialiasing);
     BRVMCGChartView->show();

    //setup CRV window
    ui->CRVAEdit->setValidator(doubleValidator0MAX);
    ui->CRVBEdit->setValidator(doubleValidator0MAX);
    ui->CRVCountEdit->setValidator(intValidator1MAX);
    ui->CRVBoundsLeft->setValidator(doubleValidator0MAX);
    ui->CRVBoundsRight->setValidator(doubleValidator0MAX);
    ui->CRVBarsEdit->setValidator(intValidator1MAX);

    CRVChartPDF = new QChart();
        CRVChartPDF->legend()->hide();
        CRVChartPDF->setTitle("Парето: плотность распределения вероятностей");
        CRVChartPDF->legend()->setVisible(true);
        CRVChartPDF->legend()->setAlignment(Qt::AlignBottom);

    CRVChartCDF = new QChart();
        CRVChartCDF->legend()->hide();
        CRVChartCDF->setTitle("Парето: функция распределения вероятностей");
        CRVChartCDF->legend()->setVisible(true);
        CRVChartCDF->legend()->setAlignment(Qt::AlignBottom);


    //setup SMO window
    ModelChart = new QChart();
    ModelChart->legend()->hide();
    ModelChart->legend()->setVisible(true);
    ModelChart->legend()->setAlignment(Qt::AlignBottom);

    ModelChartTime = new QChart();
    ModelChartTime->legend()->hide();
    ModelChartTime->legend()->setVisible(true);
    ModelChartTime->legend()->setAlignment(Qt::AlignBottom);

    ModelQ1rej = new QLineSeries();
    ModelQ1rej->setName("RQ1");
    ModelQ1midTime = new QLineSeries();
    ModelQ1midTime->setName("TQ1");
    ModelQ2rej = new QLineSeries();
    ModelQ2rej->setName("RQ2");
    ModelQ2midTime = new QLineSeries();
    ModelQ2midTime->setName("TQ2");
    ModelQ3rej = new QLineSeries();
    ModelQ3rej->setName("RQ3");
    ModelQ3midTime = new QLineSeries();
    ModelQ3midTime->setName("TQ3");
    Modelrej = new QLineSeries();
    Modelrej->setName("R");
    ModelmidTime = new QLineSeries();
    ModelmidTime->setName("T");

    ModelChart->addSeries(Modelrej);
    ModelChartTime->addSeries(ModelmidTime);

    ModelChartView = new QChartView(ModelChart,ui->ModelRejView);
    ModelChartView ->resize(400,400);
    ModelChartView ->setRenderHint(QPainter::Antialiasing);
    ModelChartView ->show();

    ModelChartTimeView = new QChartView(ModelChartTime,ui->ModelTimeView);
    ModelChartTimeView ->resize(400,400);
    ModelChartTimeView ->setRenderHint(QPainter::Antialiasing);
    ModelChartTimeView ->show();

    errorMessageWindow = new QErrorMessage(this);
    on_ModelDistDropdown_activated(0);
    on_ModelQ1DistDropdown_activated(0);
    on_ModelQ2DistDropdown_activated(0);
    on_ModelQ3DistDropdown_activated(0);
}

MainWindow::~MainWindow()
{
    delete errorMessageWindow;
    delete ModelChartView;
    delete ui;
}

void MainWindow::BRVFrequencyTest(uint corrShift, uint barCount, uint64_t max, QVector<uint64_t>* set, QVector<double>* frequencyChartData,double* MSum, double* DSum, double* RX){
    uint64_t fraction = max/barCount;
    double _MSum = 0.0;
    double _DSum = 0.0;
    double normalizedVal = 0.0;
    double normalizedValS = 0.0;
    int s = corrShift;
    int subsetCount = set->count()-s;
    double _MXY = 0.0;

    double _RX = 0.0;
    double _RMSum = 0.0;
    double _RDsum = 0.0;

    frequencyChartData->clear();
    frequencyChartData->resize(barCount);
    int val = 0 ;
    for(int i = 0; i<set->count();i++){
        normalizedVal = (double)set->at(i)/(double)max;
        if(i>=s){
            normalizedValS = (double)set->at(i-s)/(double)max;
            _RMSum += normalizedValS;
            _RDsum += normalizedValS*normalizedValS;
            _MXY += normalizedVal*normalizedValS;
        }
        val = set->at(i)/fraction;
        _MSum += normalizedVal;
        _DSum += normalizedVal*normalizedVal;
        frequencyChartData->data()[val]+=1.0;
    }
    for(uint i=0;i<barCount;i++){
        frequencyChartData->data()[i]/=(double)set->count();
    }
    _MSum /= set->count();
    _RMSum /= subsetCount;
    _MXY /= subsetCount;
    _DSum = _DSum/set->count() - (_MSum*_MSum);
    _RDsum = _RDsum/subsetCount - (_RMSum*_RMSum);
    *MSum = _MSum;
    *DSum = _DSum;
    *RX = (_MXY-_MSum*_RMSum)/(std::sqrtf(_DSum*_RDsum));
}


void MainWindow::on_BRVMSGenerateButton_clicked()
{
    bool ok = false;
    bool fullOk = true;
    ulong seed = ui->BRVMSSeedEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong count = ui->BRVMSCountEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong bars = ui->BRVMSBarsEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong corrShift = ui->BRVMSCorrShift->text().toUInt(&ok);
    fullOk &= ok;
    if(fullOk){
        MiddleSquareRNG* msrng = new MiddleSquareRNG(seed);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        QVector<double>* histogramDataSet = new QVector<double>();
        double MSum, DSum, RX = 0.0f;
        double barWidthDelta = 1.0/(double)bars;

        msrng->RandomSet(count,set);
        BRVFrequencyTest(corrShift,bars,UINT64_MAX,set,histogramDataSet,&MSum,&DSum,&RX);

        ui->BRVMSMXValue->setText(QString::number(MSum));
        ui->BRVMSDXValue->setText(QString::number(DSum));
        ui->BRVMSRXValue->setText(QString::number(RX));

//      display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("N:"+QString::number(set->count()));
        for(ulong i = 0; i<bars;i++){
            lineseries->append(QPointF(i*barWidthDelta,histogramDataSet->data()[i]*(double)bars));
            lineseries->append(QPointF((i+1)*barWidthDelta,histogramDataSet->data()[i]*(double)bars));
        }
        BRVMSChart->addSeries(lineseries);
        BRVMSChart->createDefaultAxes();

        BRVMSChart->axisX()->setRange(0.0,1.0);
        BRVMSChart->axisY()->setRange(0.0,2.0);

        delete(histogramDataSet);
        delete(set);
        delete(msrng);
    }else{
        errorMessageWindow->showMessage("Wrong Input");
    }
}

void MainWindow::on_BRVMCGGenerateButton_clicked()
{
    bool ok = false;
    bool fullOk = true;
    ulong seed = ui->BRVMCGSeedEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong count = ui->BRVMCGCountEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong increment = ui->BRVMCGIncrementEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong modulus = ui->BRVMCGModulusEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong multiplier = ui->BRVMCGMultiplierEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong bars = ui->BRVMCGBarsEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong corrShift = ui->BRVMCGCorrShift->text().toUInt(&ok);
    fullOk &= ok;
    if(fullOk){
        LinearCongruentialRNG* lcrng = new LinearCongruentialRNG(seed,increment,modulus,multiplier);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        QVector<double>* histogramDataSet = new QVector<double>();
        double MSum, DSum, RX = 0.0f;
        double barWidthDelta = 1.0/(double)bars;


        lcrng->RandomSet(count,set);
        BRVFrequencyTest(corrShift,bars,modulus,set,histogramDataSet,&MSum,&DSum,&RX);

        ui->BRVMCGMXValue->setText(QString::number(MSum));
        ui->BRVMCGDXValue->setText(QString::number(DSum));
        ui->BRVMCGRXValue->setText(QString::number(RX));

        //display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("N"+QString::number(set->count()));

        for(ulong i = 0; i<bars;i++){
            lineseries->append(QPointF((i)*barWidthDelta,histogramDataSet->data()[i]*(double)bars));
            lineseries->append(QPointF((i+1)*barWidthDelta,histogramDataSet->data()[i]*(double)bars));
        }

        BRVMCGChart->addSeries(lineseries);
        BRVMCGChart->createDefaultAxes();

        BRVMCGChart->axisX()->setRange(0.0,1.0);
        BRVMCGChart->axisY()->setRange(0.0,2.0);

        delete(histogramDataSet);
        delete(set);
        delete(lcrng);

    }else{
        errorMessageWindow->showMessage("Wrong Input");
    }
}

void MainWindow::on_BRVMCGClearButton_clicked()
{
    BRVMCGChart->removeAllSeries();
}

void MainWindow::on_BRVMSClearButton_clicked()
{
    BRVMSChart->removeAllSeries();
}

void MainWindow::on_CRVClearButton_clicked()
{
    CRVChartPDF->removeAllSeries();
    CRVChartCDF->removeAllSeries();
}

void MainWindow::on_CRVGenerateButton_clicked()
{
    bool ok = false;
    bool fullOk = true;
    double crvA = ui->CRVAEdit->text().toDouble(&ok);
    fullOk &= ok;
    double crvB = ui->CRVBEdit->text().toDouble(&ok);
    fullOk &= ok;
    ulong count = ui->CRVCountEdit->text().toULong(&ok);
    fullOk &= ok;
    ulong bars = ui->CRVBarsEdit->text().toULong(&ok);
    fullOk &= ok;
    double left = ui->CRVBoundsLeft->text().toDouble(&ok);
    ok &= left>=crvB;
    fullOk &= ok;
    double right = ui->CRVBoundsRight->text().toDouble(&ok);
    ok &= right>left;
    fullOk &= ok;

    if(ok){
        ParetoDistribution* crv = new ParetoDistribution(crvA,crvB);
        QRandomGenerator* rng = new QRandomGenerator();
        QMap<double,double>* set = new QMap<double,double>();
        double xi = 0.0, yi = 0.0;
        double MX = 0.0, MY = 0.0;
        double MX2 = 0.0, MY2 = 0.0;
        double DX = 0.0, DY = 0.0;
        double leftyi = crv->Distribution(left);
        double rightyi = crv->Distribution(right);

        for(uint i = 0; i<count;i++){
            yi = rng->generateDouble()*(rightyi-leftyi)+leftyi;
            xi = crv->InverseParetoDistribution(yi);
            if(isnan(xi)||isinf(xi)){
                continue;
            }
            set->insert(xi,yi);
            MX+=xi;
            MX2+=xi*xi;
            MY+=yi;
            MY2+=yi*yi;
        }

        MX/=double(count);
        MY/=double(count);
        DX=(MX2/double(count)-MX*MX);
        DY=(MY2/double(count)-MY*MY);
        ui->CRVMXText->setText(QString::number(MY));
        ui->CRVDXText->setText(QString::number(DY));

        QLineSeries *theoreticSeriesPDF = new QLineSeries();
        QLineSeries *theoreticSeriesCDF = new QLineSeries();
        theoreticSeriesPDF->setName("TheoryPDF");
        theoreticSeriesCDF->setName("TheoryCDF");
        double theoreticDelta = (right-left)*0.005f;
        for(double i = left; i<right; i+=theoreticDelta){
            theoreticSeriesPDF->append(QPointF(i,crv->ProbabilityDensity(i)));
            theoreticSeriesCDF->append(QPointF(i,crv->Distribution(i)));
        }

        QLineSeries *midSeriesPDF = new QLineSeries();
        QLineSeries *midSeriesCDF = new QLineSeries();
        midSeriesPDF->setName("midPDF");
        midSeriesCDF->setName("midCDF");

        double delta = (right-left)/(double)bars;

        QVector<int>* histogramSetCounts = new QVector<int>(bars);
        QVector<double>* histogramSetPDF = new QVector<double>(bars);
        QVector<double>* histogramSetCDF = new QVector<double>(bars);
        double d = crvA/(double)count;
        QMap<double,double>::const_iterator mapIterator = set->constBegin();
        while(mapIterator != set->constEnd()){
            int intervalX = floor((mapIterator.key()-left)/delta);//??zoom level
            histogramSetPDF->data()[intervalX]+=d;
            histogramSetCDF->data()[intervalX]+=mapIterator.value();
            histogramSetCounts->data()[intervalX]++;
            midSeriesCDF->append(QPointF(mapIterator.key(),mapIterator.value()));
            ++mapIterator;
        }

        QLineSeries *histogramSeriesPDF = new QLineSeries();
        histogramSeriesPDF->setName("#"+QString::number(count));
        QLineSeries *histogramSeriesCDF = new QLineSeries();
        histogramSeriesCDF->setName("#"+QString::number(count));


        for(uint i = 0; i<bars; i++){
            double x = left + delta*i;
            histogramSeriesPDF->append(QPointF(x,histogramSetPDF->data()[i]));
            histogramSeriesPDF->append(QPointF(x+delta,histogramSetPDF->data()[i]));
            double histdata = histogramSetCDF->data()[i]/(double)histogramSetCounts->data()[i];
            histogramSeriesCDF->append(QPointF(x,histdata));
            histogramSeriesCDF->append(QPointF(x+delta,histdata));
        }


        CRVChartPDF->addSeries(histogramSeriesPDF);
        CRVChartPDF->addSeries(theoreticSeriesPDF);
        CRVChartPDF->createDefaultAxes();
        CRVChartPDFView = new QChartView(CRVChartPDF,ui->CRVGroupPDF);
        CRVChartPDFView->resize(400,400);
        CRVChartPDFView->setRenderHint(QPainter::Antialiasing);
        CRVChartPDFView->show();

        CRVChartCDF->addSeries(midSeriesCDF);
        CRVChartCDF->addSeries(histogramSeriesCDF);
        CRVChartCDF->addSeries(theoreticSeriesCDF);
        CRVChartCDF->createDefaultAxes();
        CRVChartCDFView = new QChartView(CRVChartCDF,ui->CRVGroupCDF);
        CRVChartCDFView->resize(400,400);
        CRVChartCDFView->setRenderHint(QPainter::Antialiasing);
        CRVChartCDFView->show();

        delete(crv);
        delete(rng);
        delete(set);
        delete(histogramSetPDF);
        delete(histogramSetCDF);
    }else{
        errorMessageWindow->showMessage("Alert! Wrong Input");
    }
}

void MainWindow::on_ModelLaunchButton_clicked()
{
    double dt = 0.1;
    bool ok = false;
    bool fullOk = true;
    uint modelTime = ui->ModelTimeEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint modelDistType = ui->ModelDistDropdown->currentIndex();
    double modelLambda = ui->ModelRequestIntencityEdit->isVisible() ? ui->ModelRequestIntencityEdit->text().toDouble(&ok) : 0.0;
    fullOk &= (ui->ModelRequestIntencityEdit->isVisible() ? ok : true);
    double modelParam = ui->ModelParamEdit->isVisible() ? ui->ModelParamEdit->text().toDouble(&ok) : 0.0;
    fullOk &= (ui->ModelParamEdit->isVisible() ? ok : true);
    double modelMin = ui->ModelMinEdit->isVisible() ? ui->ModelMinEdit->text().toDouble(&ok) : 0.0;
    fullOk &= (ui->ModelMinEdit->isVisible() ? ok : true);
    double modelMax = ui->ModelMaxEdit->isVisible() ? ui->ModelMaxEdit->text().toDouble(&ok) : 0.0;
    fullOk &= (ui->ModelMaxEdit->isVisible() ? (ok && modelMax >= modelMin) : true);

    //q1parse
    uint q1inmax = ui->ModelQ1InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q1ChanelCount = ui->ModelQ1ChanelCountEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q1DistType = ui->ModelQ1DistDropdown->currentIndex();

    double q1mean = ui->ModelQ1MEdit->isVisible() ? ui->ModelQ1MEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ1MEdit->isVisible() ? ok : true;
    double q1param = ui->ModelQ1ParamEdit->isVisible() ? ui->ModelQ1ParamEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ1ParamEdit->isVisible() ? ok : true;
    double q1min = ui->ModelQ1MinEdit->isVisible() ? ui->ModelQ1MinEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ1MinEdit->isVisible() ? ok : true;
    double q1max = ui->ModelQ1MaxEdit->isVisible() ? ui->ModelQ1MaxEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ1MaxEdit->isVisible() ? (ok && q1max>=q1min)  : true;

    //q2parse

    uint q2inmax = ui->ModelQ2InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q2ChanelCount = ui->ModelQ2ChanelCountEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q2DistType = ui->ModelQ2DistDropdown->currentIndex();

    double q2mean = ui->ModelQ2MEdit->isVisible() ? ui->ModelQ2MEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ2MEdit->isVisible() ? ok : true;
    double q2param = ui->ModelQ2ParamEdit->isVisible() ? ui->ModelQ2ParamEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ2ParamEdit->isVisible() ? ok : true;
    double q2min = ui->ModelQ2MinEdit->isVisible() ? ui->ModelQ2MinEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ2MinEdit->isVisible() ? ok : true;
    double q2max = ui->ModelQ2MaxEdit->isVisible() ? ui->ModelQ2MaxEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ2MaxEdit->isVisible() ? (ok && q2max>=q2min) : true;
    //q3parse

    uint q3inmax = ui->ModelQ3InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q3ChanelCount = ui->ModelQ3ChanelCountEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q3DistType = ui->ModelQ3DistDropdown->currentIndex();

    double q3mean = ui->ModelQ3MEdit->isVisible() ? ui->ModelQ3MEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ3MEdit->isVisible() ? ok : true;
    double q3param = ui->ModelQ3ParamEdit->isVisible() ? ui->ModelQ3ParamEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ3ParamEdit->isVisible() ? ok : true;
    double q3min = ui->ModelQ3MinEdit->isVisible() ? ui->ModelQ3MinEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ3MinEdit->isVisible() ? ok : true;
    double q3max = ui->ModelQ3MaxEdit->isVisible() ? ui->ModelQ3MaxEdit->text().toDouble(&ok) : 0.0;
    fullOk &= ui->ModelQ3MaxEdit->isVisible() ? (ok && q3max>=q3min) : true;

    if(fullOk){
        QVector<smonode*>* linearNodes = new QVector<smonode*>();
        //Q1
        smonode* q1 = new smonode(NULL,q1mean,q1param,q1min,q1max,q1inmax,q1ChanelCount,q1DistType);
        linearNodes->push_back(q1);
        //Q2
        QVector<smonode*>* q2input = new QVector<smonode*>();
        q2input->push_back(q1);
        smonode* q2 = new smonode(q2input,q2mean,q2param,q2min,q2max,q2inmax,q2ChanelCount,q2DistType);
        linearNodes->push_back(q2);
        //Q3
        QVector<smonode*>* q3input = new QVector<smonode*>();
        q3input->push_back(q2);
        smonode* q3 = new smonode(q3input,q3mean,q3param,q3min,q3max,q3inmax,q3ChanelCount,q3DistType);
        linearNodes->push_back(q3);

        smo* model = new smo(modelLambda,modelParam,modelMin,modelMax,modelTime,dt,modelDistType,linearNodes);
        ui->ModelOut->append(model->launch(modelRun+1));

        ModelQ1midTime->append(QPointF((double)modelRun,model->qtime(0)));
        ModelQ1midTime->append(QPointF((double)modelRun+1,model->qtime(0)));
        ModelQ2midTime->append(QPointF((double)modelRun,model->qtime(1)));
        ModelQ2midTime->append(QPointF((double)modelRun+1,model->qtime(1)));
        ModelQ3midTime->append(QPointF((double)modelRun,model->qtime(2)));
        ModelQ3midTime->append(QPointF((double)modelRun+1,model->qtime(2)));
        ModelQ1rej->append(QPointF((double)modelRun,model->qrej(0)));
        ModelQ1rej->append(QPointF((double)modelRun+1,model->qrej(0)));
        ModelQ2rej->append(QPointF((double)modelRun,model->qrej(1)));
        ModelQ2rej->append(QPointF((double)modelRun+1,model->qrej(1)));
        ModelQ3rej->append(QPointF((double)modelRun,model->qrej(2)));
        ModelQ3rej->append(QPointF((double)modelRun+1,model->qrej(2)));
        Modelrej->append(QPointF((double)modelRun,model->rej()));
        Modelrej->append(QPointF((double)modelRun+1,model->rej()));
        ModelmidTime->append(QPointF((double)modelRun,model->midtime()));
        ModelmidTime->append(QPointF((double)modelRun+1,model->midtime()));

        ModelChart->createDefaultAxes();

        ModelChart->axisX()->setRange(modelRun-2,modelRun+2);
        ModelChart->axisY()->setRange(0.0,110.0);

        maxTime->push_back(model->midtime());
        double localMaxTime=0.0;
        for(int i = (maxTime->count()-5) > 0 ? maxTime->count()-5 : 0 ; i<maxTime->count(); i++){
            if(maxTime->data()[i]>=localMaxTime){
                localMaxTime = maxTime->data()[i];
            }
        }


        ModelChartTime->createDefaultAxes();
        ModelChartTime->axisY()->setRange(0.0,localMaxTime+1);
        ModelChartTime->axisX()->setRange(modelRun-2,modelRun+2);
        modelRun++;

        delete(q3);
        delete(q2);
        delete(q1);
        delete(q3input);
        delete(q2input);
        delete(linearNodes);
        delete(model);

    }else{
        ui->ModelOut->append("\nWrongInput");
    }

}

void MainWindow::on_ModelDistDropdown_activated(int index)
{
    switch(index){
        case 0:
        ui->ModelMinEdit->hide();
        ui->ModelMinLabel->hide();
        ui->ModelMaxEdit->hide();
        ui->ModelMaxLabel->hide();
        ui->ModelParamEdit->hide();
        ui->ModelParamLabel->hide();
        ui->ModelRequestIntencityEdit->show();
        ui->ModelRequestIntencityLabel->show();
        ui->ModelRequestIntencityLabel->setText("Интенсивность λ");
        break;
    case 1:
        ui->ModelMinEdit->hide();
        ui->ModelMinLabel->hide();
        ui->ModelMaxEdit->hide();
        ui->ModelMaxLabel->hide();
        ui->ModelParamEdit->show();
        ui->ModelParamLabel->show();
        ui->ModelRequestIntencityEdit->show();
        ui->ModelRequestIntencityLabel->show();
        ui->ModelRequestIntencityLabel->setText("Медиана(Сдвиг) µ");
        ui->ModelParamLabel->setText("Масштаб σ");
        break;
    case 2:
        ui->ModelMinEdit->hide();
        ui->ModelMinLabel->hide();
        ui->ModelMaxEdit->hide();
        ui->ModelMaxLabel->hide();
        ui->ModelParamEdit->hide();
        ui->ModelParamLabel->hide();
        ui->ModelRequestIntencityEdit->show();
        ui->ModelRequestIntencityLabel->show();
        ui->ModelRequestIntencityLabel->setText("Интенсивность");
//        ui->ModelParamLabel->setText("Коэффициент масштаба k");
        break;
    case 3:
        ui->ModelMinEdit->hide();
        ui->ModelMinLabel->hide();
        ui->ModelMaxEdit->hide();
        ui->ModelMaxLabel->hide();
        ui->ModelParamEdit->hide();
        ui->ModelParamLabel->hide();
        ui->ModelRequestIntencityEdit->show();
        ui->ModelRequestIntencityLabel->show();
        ui->ModelRequestIntencityLabel->setText("Интенсивность λ");
        break;
    case 4:
        ui->ModelMinEdit->show();
        ui->ModelMinLabel->show();
        ui->ModelMaxEdit->show();
        ui->ModelMaxLabel->show();
        ui->ModelParamEdit->hide();
        ui->ModelParamLabel->hide();
        ui->ModelRequestIntencityEdit->hide();
        ui->ModelRequestIntencityLabel->hide();
        break;
    }
}

void MainWindow::on_ModelQ1DistDropdown_activated(int index)
{
    switch(index){
        case 0:
        ui->ModelQ1MinEdit->hide();
        ui->ModelQ1MinLabel->hide();
        ui->ModelQ1MaxEdit->hide();
        ui->ModelQ1MaxLabel->hide();
        ui->ModelQ1ParamEdit->hide();
        ui->ModelQ1ParamLabel->hide();
        ui->ModelQ1MEdit->show();
        ui->ModelQ1MLabel->show();
        ui->ModelQ1MLabel->setText("Интенсивность λ");
        break;
    case 1:
        ui->ModelQ1MinEdit->hide();
        ui->ModelQ1MinLabel->hide();
        ui->ModelQ1MaxEdit->hide();
        ui->ModelQ1MaxLabel->hide();
        ui->ModelQ1ParamEdit->show();
        ui->ModelQ1ParamLabel->show();
        ui->ModelQ1MEdit->show();
        ui->ModelQ1MLabel->show();
        ui->ModelQ1MLabel->setText("Медиана(Сдвиг) µ");
        ui->ModelQ1ParamLabel->setText("Масштаб σ");
        break;
    case 2:
        ui->ModelQ1MinEdit->hide();
        ui->ModelQ1MinLabel->hide();
        ui->ModelQ1MaxEdit->hide();
        ui->ModelQ1MaxLabel->hide();
        ui->ModelQ1ParamEdit->hide();
        ui->ModelQ1ParamLabel->hide();
        ui->ModelQ1MEdit->show();
        ui->ModelQ1MLabel->show();
        ui->ModelQ1MLabel->setText("Интенсивность");
//        ui->ModelQ1ParamLabel->setText("Коэффициент масштаба k");
        break;
    case 3:
        ui->ModelQ1MinEdit->hide();
        ui->ModelQ1MinLabel->hide();
        ui->ModelQ1MaxEdit->hide();
        ui->ModelQ1MaxLabel->hide();
        ui->ModelQ1ParamEdit->hide();
        ui->ModelQ1ParamLabel->hide();
        ui->ModelQ1MEdit->show();
        ui->ModelQ1MLabel->show();
        ui->ModelQ1MLabel->setText("Интенсивность λ");
        break;
    case 4:
        ui->ModelQ1MinEdit->show();
        ui->ModelQ1MinLabel->show();
        ui->ModelQ1MaxEdit->show();
        ui->ModelQ1MaxLabel->show();
        ui->ModelQ1ParamEdit->hide();
        ui->ModelQ1ParamLabel->hide();
        ui->ModelQ1MEdit->hide();
        ui->ModelQ1MLabel->hide();
        break;
    }
}

void MainWindow::on_ModelQ2DistDropdown_activated(int index)
{
    switch(index){
        case 0:
        ui->ModelQ2MinEdit->hide();
        ui->ModelQ2MinLabel->hide();
        ui->ModelQ2MaxEdit->hide();
        ui->ModelQ2MaxLabel->hide();
        ui->ModelQ2ParamEdit->hide();
        ui->ModelQ2ParamLabel->hide();
        ui->ModelQ2MEdit->show();
        ui->ModelQ2MLabel->show();
        ui->ModelQ2MLabel->setText("Интенсивность λ");
        break;
    case 1:
        ui->ModelQ2MinEdit->hide();
        ui->ModelQ2MinLabel->hide();
        ui->ModelQ2MaxEdit->hide();
        ui->ModelQ2MaxLabel->hide();
        ui->ModelQ2ParamEdit->show();
        ui->ModelQ2ParamLabel->show();
        ui->ModelQ2MEdit->show();
        ui->ModelQ2MLabel->show();
        ui->ModelQ2MLabel->setText("Медиана(Сдвиг) µ");
        ui->ModelQ2ParamLabel->setText("Масштаб σ");
        break;
    case 2:
        ui->ModelQ2MinEdit->hide();
        ui->ModelQ2MinLabel->hide();
        ui->ModelQ2MaxEdit->hide();
        ui->ModelQ2MaxLabel->hide();
        ui->ModelQ2ParamEdit->hide();
        ui->ModelQ2ParamLabel->hide();
        ui->ModelQ2MEdit->show();
        ui->ModelQ2MLabel->show();
        ui->ModelQ2MLabel->setText("Интенсивность");
//        ui->ModelQ2ParamLabel->setText("Коэффициент масштаба k");
        break;
    case 3:
        ui->ModelQ2MinEdit->hide();
        ui->ModelQ2MinLabel->hide();
        ui->ModelQ2MaxEdit->hide();
        ui->ModelQ2MaxLabel->hide();
        ui->ModelQ2ParamEdit->hide();
        ui->ModelQ2ParamLabel->hide();
        ui->ModelQ2MEdit->show();
        ui->ModelQ2MLabel->show();
        ui->ModelQ2MLabel->setText("Интенсивность λ");
        break;
    case 4:
        ui->ModelQ2MinEdit->show();
        ui->ModelQ2MinLabel->show();
        ui->ModelQ2MaxEdit->show();
        ui->ModelQ2MaxLabel->show();
        ui->ModelQ2ParamEdit->hide();
        ui->ModelQ2ParamLabel->hide();
        ui->ModelQ2MEdit->hide();
        ui->ModelQ2MLabel->hide();
        break;
    }
}

void MainWindow::on_ModelQ3DistDropdown_activated(int index)
{
    switch(index){
        case 0:
        ui->ModelQ3MinEdit->hide();
        ui->ModelQ3MinLabel->hide();
        ui->ModelQ3MaxEdit->hide();
        ui->ModelQ3MaxLabel->hide();
        ui->ModelQ3ParamEdit->hide();
        ui->ModelQ3ParamLabel->hide();
        ui->ModelQ3MEdit->show();
        ui->ModelQ3MLabel->show();
        ui->ModelQ3MLabel->setText("Интенсивность λ");
        break;
    case 1:
        ui->ModelQ3MinEdit->hide();
        ui->ModelQ3MinLabel->hide();
        ui->ModelQ3MaxEdit->hide();
        ui->ModelQ3MaxLabel->hide();
        ui->ModelQ3ParamEdit->show();
        ui->ModelQ3ParamLabel->show();
        ui->ModelQ3MEdit->show();
        ui->ModelQ3MLabel->show();
        ui->ModelQ3MLabel->setText("Медиана(Сдвиг) µ");
        ui->ModelQ3ParamLabel->setText("Масштаб σ");
        break;
    case 2:
        ui->ModelQ3MinEdit->hide();
        ui->ModelQ3MinLabel->hide();
        ui->ModelQ3MaxEdit->hide();
        ui->ModelQ3MaxLabel->hide();
        ui->ModelQ3ParamEdit->hide();
        ui->ModelQ3ParamLabel->hide();
        ui->ModelQ3MEdit->show();
        ui->ModelQ3MLabel->show();
        ui->ModelQ3MLabel->setText("Интенсивность");
//        ui->ModelQ3ParamLabel->setText("Коэффициент масштаба k");
        break;
    case 3:
        ui->ModelQ3MinEdit->hide();
        ui->ModelQ3MinLabel->hide();
        ui->ModelQ3MaxEdit->hide();
        ui->ModelQ3MaxLabel->hide();
        ui->ModelQ3ParamEdit->hide();
        ui->ModelQ3ParamLabel->hide();
        ui->ModelQ3MEdit->show();
        ui->ModelQ3MLabel->show();
        ui->ModelQ3MLabel->setText("Интенсивность λ");
        break;
    case 4:
        ui->ModelQ3MinEdit->show();
        ui->ModelQ3MinLabel->show();
        ui->ModelQ3MaxEdit->show();
        ui->ModelQ3MaxLabel->show();
        ui->ModelQ3ParamEdit->hide();
        ui->ModelQ3ParamLabel->hide();
        ui->ModelQ3MEdit->hide();
        ui->ModelQ3MLabel->hide();
        break;
    }
}

void MainWindow::on_ModelClearButton_clicked()
{
    ui->ModelOut->clear();
}
