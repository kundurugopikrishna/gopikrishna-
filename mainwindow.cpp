#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QDebug>
#include <QMouseEvent>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCustomPlot *customPlot = ui->customplot;

    // --- Basic plot setup ---
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue, 1.5));
    customPlot->xAxis->setLabel("Frequency (Hz)");
    customPlot->yAxis->setLabel("Power (dB)");

    // --- Movable vertical marker line ---
    markerLine = new QCPItemStraightLine(customPlot);
    markerLine->setPen(QPen(Qt::red, 2, Qt::DashLine));
    markerLine->point1->setTypeX(QCPItemPosition::ptPlotCoords);
    markerLine->point1->setTypeY(QCPItemPosition::ptAxisRectRatio);
    markerLine->point2->setTypeX(QCPItemPosition::ptPlotCoords);
    markerLine->point2->setTypeY(QCPItemPosition::ptAxisRectRatio);
    markerLine->point1->setCoords(0, 0); // bottom
    markerLine->point2->setCoords(0, 1); // top

    // --- Label showing value at marker line ---
    markerLabel = new QCPItemText(customPlot);
    markerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);
    markerLabel->position->setTypeX(QCPItemPosition::ptPlotCoords);
    markerLabel->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
    markerLabel->position->setCoords(0, 0.02);
    markerLabel->setText("");
    markerLabel->setColor(Qt::red);
    markerLabel->setFont(QFont(font().family(), 9));
    markerLabel->setPadding(QMargins(3, 3, 3, 3));




    verticalLine = new QCPItemStraightLine(ui->customplot);
    horizontalLine = new QCPItemStraightLine(ui->customplot);

    verticalLine->setPen(QPen(Qt::yellow,1));
    horizontalLine->setPen(QPen(Qt::yellow,1));

    connect(ui->customplot, &QCustomPlot::mouseMove,
            this, &MainWindow::onMouseMove);







    // --- Mouse events for dragging the marker line ---
    connect(customPlot, &QCustomPlot::mousePress, this, &MainWindow::onMousePress);
    connect(customPlot, &QCustomPlot::mouseMove, this, &MainWindow::onMouseMove);
    connect(customPlot, &QCustomPlot::mouseRelease, this, &MainWindow::onMouseRelease);

    // --- Animation timer for sweep-style plotting ---
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::animateNextPoint);

    // --- UDP receiver setup ---
    udpSocket = new QUdpSocket(this);
    if (!udpSocket->bind(QHostAddress::Any, 12345))
        qDebug() << "Bind failed:" << udpSocket->errorString();
    else
        qDebug() << "Receiver listening on port 12345";

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::readPendingDatagrams);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================
// UDP receiving
// ============================================================

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Received packet size =" << datagram.size();
        processPacket(datagram);
    }
}

void MainWindow::processPacket(const QByteArray &packet)
{
    QDataStream stream(packet);
    stream.setByteOrder(QDataStream::BigEndian);

    quint16 msgId = 0;
    quint16 endId = 0;
    QVector<float> freqData;
    QVector<float> powerData;

    stream >> msgId;
    if (msgId != 0x8520)
    {
        qDebug() << "Unexpected msgId:" << Qt::hex << msgId;
        return;
    }

    stream >> freqData;
    stream >> powerData;
    stream >> endId;

    if (stream.status() != QDataStream::Ok)
    {
        qDebug() << "Stream parse error, status =" << stream.status();
        return;
    }
    if (endId != 0x0258)
    {
        qDebug() << "Unexpected endId:" << Qt::hex << endId;
        return;
    }
    if (freqData.size() != powerData.size())
    {
        qDebug() << "Mismatched sizes: freq =" << freqData.size()
        << " power =" << powerData.size();
        return;
    }

    qDebug() << "Parsed freq count:" << freqData.size();
    qDebug() << "Parsed power count:" << powerData.size();

    QVector<double> xData(freqData.size());
    QVector<double> yData(powerData.size());
    for (int i = 0; i < freqData.size(); ++i)
    {
        xData[i] = static_cast<double>(freqData[i]);
        yData[i] = static_cast<double>(powerData[i]);
    }

    startAnimatedPlot(xData, yData);
}

// ============================================================
// Animated (sweep) plotting
// ============================================================

void MainWindow::startAnimatedPlot(const QVector<double> &xData, const QVector<double> &yData)
{
    if (xData.isEmpty() || yData.isEmpty())
        return;

    animTimer->stop();

    pendingX = xData;
    pendingY = yData;
    animIndex = 0;

    QCustomPlot *customPlot = ui->customplot;
    customPlot->graph(0)->data()->clear();
    customPlot->xAxis->setRange(pendingX.first(), pendingX.last());

    double yMin = *std::min_element(pendingY.begin(), pendingY.end());
    double yMax = *std::max_element(pendingY.begin(), pendingY.end());
    customPlot->yAxis->setRange(yMin - 5, yMax + 5);

    // Place marker line at the middle of the frequency range initially
    double midX = (pendingX.first() + pendingX.last()) / 2.0;
    markerLine->point1->setCoords(midX, 0);
    markerLine->point2->setCoords(midX, 1);
    updateMarkerLabel(midX);

    animTimer->start(5); // ms per tick
}

void MainWindow::animateNextPoint()
{
    QCustomPlot *customPlot = ui->customplot;

    int pointsPerTick = 5; // increase for faster sweep on large datasets

    for (int i = 0; i < pointsPerTick && animIndex < pendingX.size(); ++i, ++animIndex)
    {
        customPlot->graph(0)->addData(pendingX[animIndex], pendingY[animIndex]);
    }

    customPlot->replot();

    if (animIndex >= pendingX.size())
        animTimer->stop();
}

// ============================================================
// Draggable vertical marker line
// ============================================================

void MainWindow::onMousePress(QMouseEvent *event)
{
    QCustomPlot *customPlot = ui->customplot;
    double clickX = customPlot->xAxis->pixelToCoord(event->pos().x());
    double lineX = markerLine->point1->coords().x();

    double tolerance = customPlot->xAxis->range().size() * 0.01; // 1% of visible range

    if (qAbs(clickX - lineX) < tolerance)
    {
        draggingLine = true;
        customPlot->setInteraction(QCP::iRangeDrag, false); // pause panning while dragging
    }
}

void MainWindow::onMouseMove(QMouseEvent *event)
{
    if (!draggingLine)
        return;

    QCustomPlot *customPlot = ui->customplot;
    double newX = customPlot->xAxis->pixelToCoord(event->pos().x());

    double x = ui->customplot->xAxis->pixelToCoord(event->pos().x());
    double y = ui->customplot->yAxis->pixelToCoord(event->pos().y());

    // Vertical line
    verticalLine->point1->setCoords(x, 0);
    verticalLine->point2->setCoords(x, 1);

    // Horizontal line
    horizontalLine->point1->setCoords(0, y);
    horizontalLine->point2->setCoords(1, y);

    ui->customplot->replot(QCustomPlot::rpQueuedReplot);



    // Clamp to data range
    if (!pendingX.isEmpty())
        newX = qBound(pendingX.first(), newX, pendingX.last());



    markerLine->point1->setCoords(newX, 0);
    markerLine->point2->setCoords(newX, 1);

    updateMarkerLabel(newX);
    customPlot->replot();
}

void MainWindow::onMouseRelease(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (draggingLine)
    {
        draggingLine = false;
        ui->customplot->setInteraction(QCP::iRangeDrag, true); // re-enable panning
    }
}

void MainWindow::updateMarkerLabel(double xPos)
{
    if (pendingX.isEmpty())
        return;

    // Find nearest data point to xPos
    int nearestIndex = 0;
    double minDiff = std::abs(pendingX[0] - xPos);
    for (int i = 1; i < pendingX.size(); ++i)
    {
        double diff = std::abs(pendingX[i] - xPos);
        if (diff < minDiff)
        {
            minDiff = diff;
            nearestIndex = i;
        }
    }

    double freqVal = pendingX[nearestIndex];
    double powerVal = pendingY[nearestIndex];

    markerLabel->position->setCoords(freqVal, 0.02);
    markerLabel->setText(QString("Freq: %1 Hz\nPower: %2 dB")
                             .arg(freqVal, 0, 'f', 2)
                             .arg(powerVal, 0, 'f', 2));
}
