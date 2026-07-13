#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

// #include <QWidget>
// #include <QVector>

// namespace Ui
// {
// class SpectrumWidget;
// }

// class SpectrumWidget : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit SpectrumWidget(QWidget *parent = nullptr);
//     ~SpectrumWidget();
//     void zoomIn();
//     void zoomOut();

//     void setThreshold(int value);
//     void setVerticalMarker(float  bin);
//     void updatewaterfall(const QVector<float> &power, float startFreq,float stopFreq);
// public slots:
//     //void sendSpectrum();
//         void setSpectrumData(const QVector<float> &fftData);

// protected:
//     void paintEvent(QPaintEvent *event) override;
//     void wheelEvent(QWheelEvent *event) override;

// void mouseMoveEvent(QMouseEvent *event) override;
// void mousePressEvent(QMouseEvent *event) override;
// private:
//     Ui::SpectrumWidget *ui;

//    /* QVector<float> freqMHz =
//         {
//             -16.9900,-16.9775,-16.9650,-16.9525,-16.9400,
//             -16.9275,-16.9150,-16.9025,-16.8900,-16.8775,
//             -16.8650,-16.8525,-16.8400,-16.8275,-16.8150,
//             -16.8025,-16.7900,-16.7775,-16.7650,-16.7525,
//             -16.7400,-16.7275,-16.7150,-16.7025,-16.6900,
//             -16.6775,-16.6650,-16.6525,-16.6400,-16.6275
//         };

//  QVector<float> powerDbm =
//         {
//             -132.348,-133.067,-146.860,-132.981,-130.372,
//             -131.154,-134.026,-136.442,-139.257,-141.833,
//             -145.621,-142.905,-139.438,-135.172,-131.947,
//             -128.653,-125.480,-121.318,-118.724,-116.540,
//             -118.826,-122.483,-126.752,-131.461,-136.075,
//             -140.562,-143.924,-145.870,-147.221,-146.934
//         };

// */

//   QVector<float> freqMHz;
//     QVector<float> powerDbm;
//       QVector<float> originalPowerDbm;   // Add this
//   // float startFreq = -18.0;
//   //  float stopFreq  = -16.60;
//      float startFreq = -17.0f;
//    float stopFreq  = -16.60f;

//     float minPower = -150.0f;
//     float maxPower = -110.0f;
//     int m_visiblePoints = 0;
//     QTimer *m_timer;


//     double m_xZoom = 1.0;
//     double m_yZoom = 1.0;
//     int m_threshold = -80;
//     bool m_showVerticalMarker;
//     float m_verticalFrequency;


//     bool m_dragVertical = false;
//     bool m_dragThreshold = false;


//     // spectrumwidget.h additions — add these private members:


// signals:
//     void spectrumDataReady(const QVector<float> &power, float startFreq,float stopFreq);
// };



#pragma once
#include <QWidget>
#include <QVector>
#include <QList>
#include <QQueue>
#include <QTimer>
#include<udpconnect.h>
#include <QToolTip>
// ── Forward declare the UI class ─────────────────────────────────────────────
QT_BEGIN_NAMESPACE
namespace Ui { class SpectrumWidget; }
QT_END_NAMESPACE

// ── Declare Point OUTSIDE the class (MinGW + Qt6 nested struct limitation) ───
struct SpectrumPoint {
    float freq;
    float power;
};

class SpectrumWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpectrumWidget(QWidget *parent = nullptr);
    ~SpectrumWidget();

    void setSpectrumData(const QVector<double> &freqmhz,const QVector<double>&powermhz);


  void setVerticalMarker(float bin);
    void setThreshold(int value);

 public slots:
    void zoomIn(const QPointF &anchorPos);
    void zoomOut(const QPointF &anchorPos);

    void setTooltipEnabled(bool enabled);
    bool isTooltipEnabled() const { return m_tooltipEnabled; }


signals:
    void spectrumDataReady(const QVector<double> &power,
                           float startFreq,
                           float stopFreq);
    void thresholdChanged(float value);
protected:
    void paintEvent(QPaintEvent *)      override;
     void wheelEvent(QWheelEvent *)      override;
    void mousePressEvent(QMouseEvent *) override;
   void mouseMoveEvent(QMouseEvent *)  override;
    void mouseReleaseEvent(QMouseEvent *event)override;
   void leaveEvent(QEvent *)override;
private slots:
    void onDataTimer();
    void onAnimTimer();
    void zoomAtPoint(float zoomFactor,const QPointF &pos,bool zoomX,bool zoomY);
private:
    Ui::SpectrumWidget *ui;

    // ── Spectrum source data ──────────────────────────────────
    QVector<double> freqMHz;
    QVector<double> powerDbm;
    QVector<double> originalPowerDbm;
    float startFreq = 20;
    float stopFreq  = 6000;

    // ── Sliding window settings ───────────────────────────────
    static constexpr int MAX_POINTS = 100;
    static constexpr int ANIM_STEPS = 50;

    QList<SpectrumPoint>  m_window;      // visible points (max 20)
    QQueue<SpectrumPoint> m_incoming;    // buffered points waiting to show

    // ── Animation state ───────────────────────────────────────
    float m_animShift = 0.0f;           // current slide-left offset in px
    bool  m_animating = false;

    // ── Timers ────────────────────────────────────────────────
    QTimer *m_dataTimer = nullptr;      // 100 ms — one point per tick
    QTimer *m_animTimer = nullptr;      // 10  ms — smooth slide animation

    // ── Display / interaction state ───────────────────────────
    double m_xZoom              = 1.0;
    int    m_visiblePoints      = 0;
  //  float  minPower             = -150.0f;
   // float  maxPower             =   0.0f;
    float  m_threshold          = -100.0f;
    bool   m_showVerticalMarker = false;
    float  m_verticalFrequency  = 0.0;
    bool   m_dragVertical       = false;
    bool   m_dragThreshold      = false;

bool m_draggingMarker = false;

    float minPower = -150.0f;
    float maxPower =    0.0f;

    // Absolute limits so the user can't zoom out forever
    // or zoom in until the range collapses to nothing.
    const float absoluteMinPower =  -200.0f;
    const float absoluteMaxPower =    20.0f;

    const float absoluteMinFreq;   // set these in constructor to your
    const float absoluteMaxFreq;   // hardware's full sweep range
QQueue<SpectrumPoint> m_pointQueue;
    QPointF m_mousePos;
    bool m_showCrosshair = false;

     bool m_tooltipEnabled = false;
    int m_nearestIndex = -1;   // index into m_window of the point closest to the cursor

};
#endif
