#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include<spectrumwidget.h>
class UDPconnect : public QObject
{
    Q_OBJECT
public:
    explicit UDPconnect(QObject *parent = nullptr);


public slots:
    void receiveData();

private:
    QUdpSocket *socket;
   // slots:


signals:
    void spectrumDataReady(const QVector<double> &freq,
                           const QVector<double> &power);

    void waterfallData(const QVector<double>&powerData);
};

#endif // UDPCONNECT_H
