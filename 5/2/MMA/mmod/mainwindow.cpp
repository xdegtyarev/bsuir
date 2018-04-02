#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "linearcongruentialrng.h"
#include "middlesquarerng.h"
#include "paretodistribution.h"
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

    CRVChart = new QChart();
        CRVChart->legend()->hide();
        CRVChart->setTitle("CRV Pareto");
        CRVChart->legend()->setVisible(true);
        CRVChart->legend()->setAlignment(Qt::AlignBottom);

    //setup SMO window
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
    if(fullOk){
        MiddleSquareRNG* msrng = new MiddleSquareRNG(seed);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        msrng->RandomSet(count,set);

        QVector<float>* frequencyChartData = new QVector<float>();

        int barCount = 40;
        float MSum, DSum = 0.0f;
        BRVFrequencyTest(barCount,UINT64_MAX,set,frequencyChartData,&MSum,&DSum);
        ui->BRVMSStatus->setText(
            "Num generated:" + QString::number(set->count()) +
            " M(X):" + QString::number(MSum) +
            " D(X):" + QString::number(DSum)
            );

//      display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("#"+QString::number(set->count()));
        for(int i = 0; i<barCount;i++){
            lineseries->append(QPointF(i,frequencyChartData->data()[i]));
            lineseries->append(QPointF(i+1,frequencyChartData->data()[i]));
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
    if(fullOk){
        LinearCongruentialRNG* lcrng = new LinearCongruentialRNG(seed,increment,modulus,multiplier);
        QVector<uint64_t>* set = new QVector<uint64_t>(count);
        lcrng->RandomSet(count,set);

        QVector<float>* frequencyChartData = new QVector<float>();
        int barCount = 40;
        float MSum, DSum = 0.0f;
        BRVFrequencyTest(barCount,modulus,set,frequencyChartData,&MSum,&DSum);
        ui->BRVMCGStatusLabel->setText(
                    "Num generated:" + QString::number(set->count()) +
                    " M(X):" + QString::number(MSum) +
                    " D(X):" + QString::number(DSum)
                    );
        //      display chart
        QLineSeries *lineseries = new QLineSeries();
        lineseries->setName("#"+QString::number(set->count()));

        for(int i = 0; i<barCount;i++){
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
    CRVChart->removeAllSeries();
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
    if(ok){
        ui->CRVStatusText->setText("Input is valid");
        ParetoDistribution* crv = new ParetoDistribution(crvA,crvB);

    }else{
        ui->CRVStatusText->setText("Check your input a,b > 0");
    }


}
