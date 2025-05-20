#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLineSeries *series =new QLineSeries();
    series->append(0,3);
    series->append(1,5);
    series->append(2,7);
    series->append(3,8);
    series->append(4,1);
    series->append(5,12);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Line Chart Example");
    chart->createDefaultAxes();

    // Create the chart view and set it as central widget
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);


}

MainWindow::~MainWindow()
{
    delete ui;
}
