// #ifndef WATERFALLDATA_H
// #define WATERFALLDATA_H

// #pragma once
// #include <QWidget>
// #include <QTimer>
// #include <QQueue>
// #include <QVector>
// #include <QList>
// #include <QColor>

// class WaterfallData : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit WaterfallData(QWidget *parent = nullptr);

//     void setSpectrum(const QVector<double> &power1,
//                      float startFreq,
//                      float stopFreq);

// protected:
//     void resizeEvent(QResizeEvent *event) override;
//     void paintEvent(QPaintEvent *)        override;

// private slots:
//     void onDataTimer();    // pulls one row from queue every 100 ms
//     void onAnimTimer();    // animates the slide-down (runs faster)

// private:
//     QColor powerToColor(float power);

//     static constexpr int MAX_ROWS    = 6;
//     static constexpr int LEFT_MARGIN = 45;
//     static constexpr int BOT_MARGIN  = 30;
//     static constexpr int GAP         = 5;
//     static constexpr int ANIM_STEPS  = 10;   // frames per slide

//     // m_rows[0] = newest (top), m_rows[N-1] = oldest (bottom)
//     QList<QVector<float>> m_rows;
//     QQueue<QVector<float>> m_queue;

//     float m_startFreq = 0.0f;
//     float m_stopFreq  = 0.0f;

//     // animation state: how many pixels the whole stack has slid DOWN so far
//     int   m_animOffset  = 0;
//     bool  m_animating   = false;

//     QTimer *m_dataTimer = nullptr;   // 100 ms  — controls data rate
//     QTimer *m_animTimer = nullptr;   // 10 ms   — controls animation

//     // Geometry (set in resizeEvent)
//     int m_plotX = 0, m_plotY = 0, m_plotW = 0, m_plotH = 0, m_rowH = 0;
// };
// #endif // WATERFALLDATA_H

// #ifndef WATERFALLDATA_H
// #define WATERFALLDATA_H

// #include <QWidget>
// #include <QImage>
// #include <QVector>
// #include <QColor>

// class WaterfallData : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit WaterfallData(QWidget *parent = nullptr);

//     void addSpectrum(const QVector<double> &power
//                     );
// protected:
//     void paintEvent(QPaintEvent *event) override;

// private:
//     QColor powerToColor(double power);

// private:
//     QImage m_image;
//     // WaterfallData.h
//     int m_frameHeight = 20;   // pixels per incoming FFT frame; increase to make frames thicker
//    float  m_startFreq = 0.0;
//     float m_stopFreq  = 0.0;
// private slots:

//     void setFrameHeight(int px);

// };

// #endif // WATERFALLDATA_H
//-----------------------------------------------------------------------------------------------------------------------------------------------














#pragma once

#include <QWidget>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <QTimer>
#include <QMouseEvent>
#include <QDateTime>
class WaterfallData : public QWidget
{
    Q_OBJECT
public:
    explicit WaterfallData(QWidget *parent = nullptr);
    void addSpectrum(const QVector<double> &power);
    void setFrameHeight(int px);
    void setUpdateInterval(int ms);
    static constexpr double kMinPowerDbm = -150.0;
    static constexpr double kMaxPowerDbm = 0.0;
public slots:
    void setStartStopFreq(double startFreq, double stopFreq);
      // QColor powerToColor(double power);

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void processQueue();
  void clearWaterfall();

    void drawColorScale(QPainter &painter, const QRect &plotRect);
  //  QStringList m_timeHistory;
    // void drawColorScale(QPainter &painter, const QRect &plotRect);
private:
    QColor powerToColor(double power);
    void renderSpectrum(const QVector<double> &power);
    QRect plotRectGeometry() const;

    QImage m_image;
    double m_startFreq = 20.0, m_stopFreq = 6000.0;
    int m_frameHeight = 40;

    QQueue<QVector<double>> m_spectrumQueue;
    QTimer *m_updateTimer = nullptr;
    int m_maxQueueSize=50;

    bool m_mouseInPlot = false;
    QPoint m_mousePos;
};
