#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach (const QSerialPortInfo &info ,QSerialPortInfo::availablePorts())
    {
        ui->COM->addItem(info.portName());
    }
    COMPORT = new QSerialPort();
   // connect(ui->comboBoxPort, &QComboBox::currentTextChanged, this, &MainWindow::connectSerialPort);
   // connect(ui->comboBoxBaudRate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::connectSerialPort);
    connect(ui->Connect, &QPushButton::clicked, this, &MainWindow::on_Connect_clicked);
    connect(COMPORT, &QSerialPort::readyRead, this, &MainWindow::on_readyRead);


   // COMPORT->setPortName("COM6");
    // Add common baud rates and associate them with real integer values
    ui->BaudRate->addItem("9600", QSerialPort::Baud9600);
    ui->BaudRate->addItem("19200", QSerialPort::Baud19200);
    ui->BaudRate->addItem("38400", QSerialPort::Baud38400);
    ui->BaudRate->addItem("57600", QSerialPort::Baud57600);
    ui->BaudRate->addItem("115200", QSerialPort::Baud115200);
    // COMPORT->setDataBits(QSerialPort::Data8);
    // COMPORT->setParity(QSerialPort::NoParity);
    // COMPORT->setStopBits(QSerialPort::OneStop);
    // COMPORT->setFlowControl(QSerialPort::NoFlowControl);
    // COMPORT->open(QIODevice::ReadWrite);
    // if(COMPORT->isOpen())
    // {
    //     qDebug() <<"Serial port is connected.";
    // }
    // else
    // {
    //     qDebug() <<" serial port is not -connected.";
    // }
}

MainWindow::~MainWindow()
{
    if (COMPORT->isOpen()) {
        COMPORT->close();
    } delete ui;
}

// void MainWindow::on_Connect_clicked()
// {

// }

void MainWindow::connectSerialPort()
{
    QString portName = ui->COM->currentText();
    QVariant baudRateVar = ui->BaudRate->currentData();

    if (portName.isEmpty() || !baudRateVar.isValid())
        return;

    COMPORT->setPortName(portName);
    COMPORT->setBaudRate(static_cast<QSerialPort::BaudRate>(baudRateVar.toInt()));
    COMPORT->setDataBits(QSerialPort::Data8);
    COMPORT->setParity(QSerialPort::NoParity);
    COMPORT->setStopBits(QSerialPort::OneStop);
    COMPORT->setFlowControl(QSerialPort::NoFlowControl);

    // if (COMPORT->open(QIODevice::ReadWrite)) {
    //     qDebug() << "Connected to" << portName << "at" << baudRateVar.toInt() << "baud.";
    // } else {
    //     qDebug() << "Failed to open port:" << COMPORT->errorString();
    // }
}
void MainWindow::on_Connect_clicked()
 {
    connectSerialPort();
     if (COMPORT->isOpen()) {
         COMPORT->close();  // Close the port if it's already open
         qDebug() << "Disconnected from" << COMPORT->portName();
     }
    if (COMPORT->open(QIODevice::ReadWrite)) {
         qDebug() << "Connected to" << COMPORT->portName() << "at" << COMPORT->baudRate() << "baud.";
    } else {
        qDebug() << "Failed to open port:" << COMPORT->errorString();
    }
 }

 void MainWindow::on_Disconnect_clicked()
 {
     if (COMPORT->isOpen()) {
         COMPORT->close();
         qDebug() << "Disconnected from" << COMPORT->portName();
     } else {
         qDebug() << "No port is currently open.";
     }
 }


 void MainWindow::on_Send_clicked()
 {
     QString data =ui->Send_data->text();
     if(COMPORT->isOpen())

     {
         COMPORT->write(data.toLatin1()+char(10));
         qDebug() <<"Data ssent:"<< data;

     }
else
     {
         qDebug() << "Error: COM port is not open !";
     }

 }
 void MainWindow::on_readyRead()
 {
     // Read the data from the serial port
     QByteArray data = COMPORT->readAll();

     // Display the received data in the QTextEdit widget
     ui->Received_data->append(data);  // Append the new data to the text edit

     // Optionally, if you want to print the received data in the debug console
     qDebug() << "Received data:" << data;
 }

 void MainWindow::on_Clear_clicked()
 {
     ui->Received_data->clear();
 }

