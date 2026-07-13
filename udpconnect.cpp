#include "udpconnect.h"
#include"spectrumwidget.h"
#include"waterfalldata.h"
UDPconnect::UDPconnect(QObject *parent)
    : QObject{parent}
{

    socket = new QUdpSocket(this);

    if (!socket->bind(QHostAddress::Any, 12345))
    {
        qDebug() << "Bind failed:" << socket->errorString();
    }
    else
    {
        qDebug() << "UDP socket bound to port 12345";
    }
   connect(socket, &QUdpSocket::readyRead,this, &UDPconnect::receiveData);

    // connect(socket,
    //         &QUdpSocket::readyRead,
    //         this,
    //         []()
    //         {
    //             qDebug() << "readyRead emitted";
    //         });
}

void UDPconnect::receiveData()
{


    qDebug() << "receiveData() entered";

    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());

        socket->readDatagram(datagram.data(), datagram.size());

        qDebug() << "Datagram Size =" << datagram.size();

        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setByteOrder(QDataStream::BigEndian);

        quint16 header;
        quint16 footer;

        QVector<double> freqData;
        QVector<double> powerData;

        in >> header;

        qDebug() << "Header =" << Qt::hex << header;

        in >> freqData;
        qDebug() << "Freq Size =" << freqData.size();

        in >> powerData;
        qDebug() << "Power Size =" << powerData.size();

        in >> footer;

        qDebug() << "Footer =" << Qt::hex << footer;

        if(header==0x8520&&footer==0x0258)
        {
            emit  spectrumDataReady(freqData,powerData);

            emit waterfallData(powerData);
        }
    }
}
