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

    //setup BRV window
    ui->BRVMCGCountEdit->setValidator(intValidator1MAX);
    ui->BRVMCGIncrementEdit->setValidator(intValidator0MAX);
    ui->BRVMCGModulusEdit->setValidator(intValidator1MAX);
    ui->BRVMCGMultiplierEdit->setValidator(intValidator1MAX);
    ui->BRVMCGSeedEdit->setValidator(intValidator0MAX);

    BRVMSChart = new QChart();
        BRVMSChart->legend()->hide();
        BRVMSChart->setTitle("BRVMC");
        BRVMSChart->legend()->setVisible(true);
        BRVMSChart->legend()->setAlignment(Qt::AlignBottom);

    BRVMCGChart = new QChart();
        BRVMCGChart->legend()->hide();
        BRVMCGChart->setTitle("BRVMCG");
        BRVMCGChart->legend()->setVisible(true);
        BRVMCGChart->legend()->setAlignment(Qt::AlignBottom);
    //setup CRV window
    ui->CRVAEdit->setValidator(doubleValidator0MAX);
    ui->CRVBEdit->setValidator(doubleValidator0MAX);
    ui->CRVCountEdit->setValidator(intValidator1MAX);
    ui->CRVBoundsLeft->setValidator(doubleValidator0MAX);
    ui->CRVBoundsRight->setValidator(doubleValidator0MAX);
    ui->CRVBarsEdit->setValidator(intValidator1MAX);

    CRVChartPDF = new QChart();
        CRVChartPDF->legend()->hide();
        CRVChartPDF->setTitle("CRV Pareto PDF");
        CRVChartPDF->legend()->setVisible(true);
        CRVChartPDF->legend()->setAlignment(Qt::AlignBottom);

    CRVChartCDF = new QChart();
        CRVChartCDF->legend()->hide();
        CRVChartCDF->setTitle("CRV Pareto CDF");
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

//    ModelChart->addSeries(ModelQ1rej);
//    ModelChart->addSeries(ModelQ2rej);
//    ModelChart->addSeries(ModelQ3rej);
//    ModelChart->addSeries(ModelQ1midTime);
//    ModelChart->addSeries(ModelQ2midTime);
//    ModelChart->addSeries(ModelQ3midTime);
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BRVFrequencyTest(uint barCount, uint64_t max, QVector<uint64_t>* set, QVector<float>* frequencyChartData, float* MSum, float* DSum ){
    uint64_t fraction = max/barCount;
    float frequencyChartAdd = 1.0f/(float)set->count();
    float _MSum = 0.0f;
    float _DSum = 0.0f;
    float normalizedVal;
    frequencyChartData->clear();
    frequencyChartData->resize(barCount);
    int val = 0 ;
    for(int i = 0; i<set->count();i++){
        normalizedVal = (float)set->at(i)/(float)max;
        val = set->at(i)/fraction;
        _MSum += normalizedVal;
        _DSum += normalizedVal*normalizedVal;
        frequencyChartData->data()[val]+=frequencyChartAdd;
    }
    _MSum /= set->count();
    _DSum = _DSum/set->count() - (_MSum*_MSum);
    *MSum = _MSum;
    *DSum = _DSum;
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
    if(fullOk){
        MiddleSquareRNG* msrng = new MiddleSquareRNG(seed);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        msrng->RandomSet(count,set);

        QVector<float>* frequencyChartData = new QVector<float>();

        float MSum, DSum = 0.0f;
        BRVFrequencyTest(bars,UINT64_MAX,set,frequencyChartData,&MSum,&DSum);
        ui->BRVMSStatus->setText(
            "Num generated:" + QString::number(set->count()) +
            " M(X):" + QString::number(MSum) +
            " D(X):" + QString::number(DSum)
            );

//      display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("#"+QString::number(set->count()));
        for(int i = 0; i<bars;i++){
            lineseries->append(QPointF(i,frequencyChartData->data()[i]*bars));
            lineseries->append(QPointF(i+1,frequencyChartData->data()[i]*bars));
        }
        BRVMSChart->addSeries(lineseries);
        BRVMSChart->createDefaultAxes();
        BRVMSChartView = new QChartView(BRVMSChart,ui->BRVMSGroupBox);
        BRVMSChartView->resize(369,350);

        BRVMSChartView->setRenderHint(QPainter::Antialiasing);
        BRVMSChartView->show();

        delete(frequencyChartData);
        delete(set);
        delete(msrng);
    }else{
        ui->BRVMSStatus->setText("ALERT!: Wrong Input");
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
    if(fullOk){
        LinearCongruentialRNG* lcrng = new LinearCongruentialRNG(seed,increment,modulus,multiplier);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        lcrng->RandomSet(count,set);

        QVector<float>* frequencyChartData = new QVector<float>();
        float MSum, DSum = 0.0f;
        BRVFrequencyTest(bars,modulus,set,frequencyChartData,&MSum,&DSum);
        ui->BRVMCGStatusLabel->setText(
                    "Num generated:" + QString::number(set->count()) +
                    " M(X):" + QString::number(MSum) +
                    " D(X):" + QString::number(DSum)
                    );
        //      display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("#"+QString::number(set->count()));

        for(int i = 0; i<bars;i++){
            lineseries->append(QPointF(i,frequencyChartData->data()[i]));
            lineseries->append(QPointF(i+1,frequencyChartData->data()[i]));
        }

        BRVMCGChart->addSeries(lineseries);
        BRVMCGChart->createDefaultAxes();
        BRVMCGChartView = new QChartView(BRVMCGChart,ui->BRVMCGGroupBox);
        BRVMCGChartView->resize(369,350);
        BRVMCGChartView->setRenderHint(QPainter::Antialiasing);
        BRVMCGChartView->show();

        delete(frequencyChartData);
        delete(set);
        delete(lcrng);

    }else{
        ui->BRVMCGStatusLabel->setText("Alert!: Wrong Input");
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
        ui->CRVStatusText->setText("Input is valid [" + QString::number(left) + " : " + QString::number(right) + "]");
        ParetoDistribution* crv = new ParetoDistribution(crvA,crvB);
        QRandomGenerator* rng = new QRandomGenerator();
        QMap<double,double>* set = new QMap<double,double>();
        double xi, yi = 0.0;
        double MX, MY = 0.0;
        double MX2, MY2 = 0.0;
        double DX, DY = 0.0;
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
        ui->CRVStatusText->setText("Check your input a,b > 0");
    }


}

void MainWindow::on_ModelLaunchButton_clicked()
{
    bool ok = false;
    bool fullOk = true;
    uint requestCount = ui->ModelRequestCountEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint requestDeltaTime = ui->ModelRequestDeltaTimeEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint deltaTime = ui->ModelDeltaTimeEdit->text().toUInt(&ok);
    fullOk &= ok;
    //q1parse
    double q1mean = ui->ModelQ1MEdit->text().toDouble(&ok);
    fullOk &= ok;
    uint q1min = ui->ModelQ1MinEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q1max = ui->ModelQ1MaxEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q1inmax = ui->ModelQ1InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q1outmax = ui->ModelQ1OutEdit->text().toUInt(&ok);
    fullOk &= ok;
    //q2parse
    double q2mean = ui->ModelQ2MEdit->text().toDouble(&ok);
    fullOk &= ok;
    uint q2min = ui->ModelQ2MinEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q2max = ui->ModelQ2MaxEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q2inmax = ui->ModelQ2InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q2outmax = ui->ModelQ2OutEdit->text().toUInt(&ok);
    fullOk &= ok;
    //q3parse
    double q3mean = ui->ModelQ3MEdit->text().toDouble(&ok);
    fullOk &= ok;
    uint q3min = ui->ModelQ3MinEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q3max = ui->ModelQ3MaxEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q3inmax = ui->ModelQ3InEdit->text().toUInt(&ok);
    fullOk &= ok;
    uint q3outmax = ui->ModelQ3OutEdit->text().toUInt(&ok);
    fullOk &= ok;

    if(fullOk){
        QVector<smonode*>* linearNodes = new QVector<smonode*>();
        //Q1
        smonode* q1 = new smonode(NULL,q1min,q1max,q1mean,0,0,q1inmax,q1outmax);
        linearNodes->push_back(q1);
        //Q2
        QVector<smonode*>* q2input = new QVector<smonode*>();
        q2input->push_back(q1);
        smonode* q2 = new smonode(q2input,q2min,q2max,q2mean,0,0,q2inmax,q2outmax);
        linearNodes->push_back(q2);
        //Q3
        QVector<smonode*>* q3input = new QVector<smonode*>();
        q3input->push_back(q2);
        smonode* q3 = new smonode(q3input,q3min,q3max,q3mean,0,0,q3inmax,q3outmax);
        linearNodes->push_back(q3);

        smo* model = new smo(requestCount,requestDeltaTime,deltaTime,linearNodes);
        ui->ModelOut->append(model->launch());

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

        ModelChart->axisX()->setRange(0.0,modelRun+2);
        ModelChart->axisY()->setRange(0.0,2.0);

        if(model->midtime()>=maxTime){
            maxTime = model->midtime();
        }

        ModelChartTime->createDefaultAxes();
        ModelChartTime->axisY()->setRange(0.0,maxTime);
        ModelChartTime->axisX()->setRange(0.0,modelRun+2);
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
