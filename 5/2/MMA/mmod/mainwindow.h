#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QErrorMessage>
#include "limits.h"
#include <cmath>
#include <float.h>
#include <smo.h>
#include <smonode.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BRVMSGenerateButton_clicked();
    void on_BRVMCGGenerateButton_clicked();
    void on_BRVMCGClearButton_clicked();
    void on_BRVMSClearButton_clicked();

    void on_CRVClearButton_clicked();
    void on_CRVGenerateButton_clicked();

    void on_ModelLaunchButton_clicked();
    void on_ModelQ1DistDropdown_activated(int index);
    void on_ModelDistDropdown_activated(int index);

    void on_ModelQ2DistDropdown_activated(int index);

    void on_ModelQ3DistDropdown_activated(int index);

    void on_ModelClearButton_clicked();

private:
    void BRVFrequencyTest(uint corrShift, uint barCount, uint64_t max, QVector<uint64_t>* set, QVector<double>* frequencyChartData,double* MSum, double* DSum, double* RX);
    Ui::MainWindow *ui;
    //BRV
    //MS
    QChart* BRVMSChart;
    //MCG
    QChart* BRVMCGChart;

    //CRV
    QChart* CRVChartPDF;
    QChart* CRVChartCDF;
    QChart* ModelChart;
    QChart* ModelChartTime;
    QLineSeries* ModelQ1rej;
    QLineSeries* ModelQ1midTime;
    QLineSeries* ModelQ2rej;
    QLineSeries* ModelQ2midTime;
    QLineSeries* ModelQ3rej;
    QLineSeries* ModelQ3midTime;
    QLineSeries* Modelrej;
    QLineSeries* ModelmidTime;
    int modelRun = 0;
    QVector<double>* maxTime;
    QChartView* BRVMSChartView;
    QChartView* BRVMCGChartView;
    QChartView* CRVChartPDFView;
    QChartView* CRVChartCDFView;
    QChartView* ModelChartView;
    QChartView* ModelChartTimeView;
    QErrorMessage* errorMessageWindow;
};
#endif // MAINWINDOW_H
