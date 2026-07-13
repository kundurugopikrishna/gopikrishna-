#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spectrumwidget.h"
#include"waterfalldata.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
   void spinchanged(int value);
private:
    Ui::MainWindow *ui;

    SpectrumWidget *spectrum;
    WaterfallData *waterfalldata;

    //  void loadFFTFile();
    UDPconnect  *udpconnect;

private slots:
    void onSpinBoxEditingFinished();
        void on_pushButton_clicked();

    void on_axis_clicked();
};
#endif // MAINWINDOW_H
