#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// #include <qcendarwidget.h>
#include <QPainterPath>
#include "qcgaugewidget.h"
// #include <QPainterPath>
#include <QWidget>


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
private slots:
    void on_horizontalSlider_valueChanged(int value);
private:
    Ui::MainWindow *ui;
    QcGaugeWidget *mSpeedGauge;
    QcNeedleItem *mSpeedNeedle;
 };
#endif // MAINWINDOW_H
