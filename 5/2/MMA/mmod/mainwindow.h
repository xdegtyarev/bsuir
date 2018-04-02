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
#include "limits.h"
#include <cmath>
#include <float.h>

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

private:
    void BRVFrequencyTest(uint barCount, uint64_t max, QVector<uint64_t>* set, QVector<float>* frequencyChartData,float* MSum, float* DSum);
    Ui::MainWindow *ui;
    QChart* BRVMSChart;
    QChart* BRVMCGChart;
    QChart* CRVChart;

    QChartView* BRVMSChartView;
    QChartView* BRVMCGChartView;
    QChartView* CRVChartView;
};
#endif // MAINWINDOW_H
