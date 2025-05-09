#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QSerialPortInfo>
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
    void on_Connect_clicked();
    void connectSerialPort();
    void on_Disconnect_clicked();

    void on_Send_clicked();
    void on_readyRead();
    void on_Clear_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *COMPORT;
};
#endif // MAINWINDOW_H
