
// // m_pointQueue.clear();
// // m_drawIndex = 0;


// //then when new data arrives you must reset it:

// #include "spectrumwidget.h"
// #include "ui_spectrumwidget.h"

// #include <QPainter>
// #include <QPainterPath>
// #include <QWheelEvent>
// #include <QMouseEvent>
// #include <QtMath>

// SpectrumWidget::SpectrumWidget(QWidget *parent)
//     : QWidget(parent),absoluteMinFreq(0.0f),      // e.g. your device's lowest tunable freq
//     absoluteMaxFreq(6000.0f),  // e.g. your device's highest tunable freq (MHz)
//     ui(new Ui::SpectrumWidget)
// {
//     setFocusPolicy(Qt::StrongFocus);
//     ui->setupUi(this);
//     setAutoFillBackground(false);

//     // ── Data timer: one new point enters every 100 ms ────────
//     m_dataTimer = new QTimer(this);
//     m_dataTimer->setInterval(200);
//     connect(m_dataTimer, &QTimer::timeout,
//             this, &SpectrumWidget::onDataTimer);
//     m_dataTimer->start();

//     // ── Animation timer: smooth slide at 10 ms ───────────────
//     m_animTimer = new QTimer(this);
//     m_animTimer->setInterval(100);
//     connect(m_animTimer, &QTimer::timeout,
//             this, &SpectrumWidget::onAnimTimer);
// }

// SpectrumWidget::~SpectrumWidget()
// {
//     delete ui;
// }

// // ── Public API ────────────────────────────────────────────────────────────────
// // void SpectrumWidget::setSpectrumData(
// //    const QVector<double> &freqmhz,const QVector<double>&powermhz)
// // {


// //     qDebug()<<"hithere"<<freqmhz;
// //     if (freqmhz.size() < 2 ||
// //         freqmhz.size() != powermhz.size())
// //         return;

// //     for (int i = 0; i < freqmhz.size(); ++i)
// //     {
// //         SpectrumPoint point;

// //         point.freq = freqmhz[i];
// //         point.power = powermhz[i];

// //         m_pointQueue.enqueue(point);
// //     }

// //     m_incoming.clear();

// //     for (int i = 0; i < freqMHz.size(); ++i)
// //     {
// //         m_incoming.enqueue({ freqMHz[i], powerDbm[i] });
// //     }

// //     m_window.clear();
// //     m_animShift = 0.0f;
// //     m_animating = false;
// //     m_animTimer->stop();

// //     update();
// // }// ── Data timer: pull one point, animate it in ────────────────────────────────


// /*void SpectrumWidget::setSpectrumData(
//     const QVector<double> &freqMHz,
//     const QVector<double> &powerDbm)
// {
//     qDebug() << "Received spectrum with" << freqMHz.size() << "points";

//     if (freqMHz.size() < 2 || freqMHz.size() != powerDbm.size())
//         return;
//     // Remove old spectrum
//     // Remove old spectrum completely
//     m_pointQueue.clear();
//     m_window.clear();

//     m_animShift = 0.0f;
//     m_animating = false;
//     m_animTimer->stop();





//     // Add all received points to the queue.
//     // Do NOT clear the queue, otherwise you'll lose undrawn points.
//     for (int i = 0; i < freqMHz.size(); ++i)
//     {
//         SpectrumPoint point;

//         point.freq = freqMHz[i];
//         point.power = powerDbm[i];

//         m_pointQueue.enqueue(point);
//     }

//     qDebug() << "Points waiting in queue:" << m_pointQueue.size();


//     emit spectrumDataReady(powerDbm,startFreq,stopFreq);
// }*/

// void SpectrumWidget::onDataTimer()
// {
//     if (m_pointQueue.isEmpty())
//         return;

//     m_window.append(m_pointQueue.dequeue());

//     if (m_window.size() > MAX_POINTS)
//     {
//         m_window.removeFirst();
//     }

//     update();
// }

// void SpectrumWidget::onAnimTimer()
// {
//     const int graphW = width() - 50;
//     float slotW = float(graphW) / MAX_POINTS;
//     float stepSize = slotW / ANIM_STEPS;

//     // Move to the RIGHT
//     m_animShift += stepSize;

//     if (m_animShift >= slotW)
//     {
//         m_animShift = 0.0f;
//         m_animating = false;
//         m_animTimer->stop();

//         // Remove the oldest point from the LEFT
//         if (m_window.size() > MAX_POINTS)
//             m_window.removeFirst();
//     }

//     update();
// }


// void SpectrumWidget::setSpectrumData(
//     const QVector<double> &freqMHz,
//     const QVector<double> &powerDbm)
// {
//     if (freqMHz.size() < 2 || freqMHz.size() != powerDbm.size())
//         return;

//     m_window.clear();
//     m_window.reserve(freqMHz.size());
//     for (int i = 0; i < freqMHz.size(); ++i)
//     {
//         SpectrumPoint point;
//         point.freq  = freqMHz[i];
//         point.power = powerDbm[i];
//         m_window.append(point);
//     }

//     update();   // repaint immediately with the full trace

//     emit spectrumDataReady(powerDbm, startFreq, stopFreq);
// }
// // ── Zoom ──────────────────────────────────────────────────────────────────────
// // ── Paint ─────────────────────────────────────────────────────────────────────
// void SpectrumWidget::paintEvent(QPaintEvent *)
// {
//     QPainter painter(this);
//     painter.fillRect(rect(), Qt::black);
//     painter.setRenderHint(QPainter::Antialiasing);

//     const int leftMargin   = 50;
//     const int bottomMargin = 30;
//     const int graphWidth   = width()  - leftMargin;
//     const int graphHeight  = height() - bottomMargin;
//    // bool  m_draggingMarker = true;
//    // minPower = -150.0f;
//   //  maxPower =   0.0f;

//     // ── Axes ─────────────────────────────────────────────────
//     painter.setPen(QPen(Qt::yellow, 2));
//     painter.drawLine(leftMargin, 0, leftMargin, graphHeight);
//     painter.drawLine(leftMargin, graphHeight, width(), graphHeight);

//     // ── Horizontal grid + Y labels ───────────────────────────
//     for (int p = -150; p <= 0; p += 10)
//     {
//         double y = graphHeight -
//                    ((double(p) - minPower) /
//                     (maxPower - minPower)) * graphHeight;

//         painter.setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
//         painter.drawLine(leftMargin, y, width(), y);

//         painter.setPen(Qt::white);
//         painter.drawText(5, int(y) + 5, QString::number(p));
//     }


//     // ── X-axis frequency labels (fixed, based on startFreq/stopFreq) ─────
//     painter.setFont(QFont("Monospace", 8));
//     const int divX = 5;
//     for (int i = 0; i <= divX; ++i)
//     {
//        float freq = startFreq + (stopFreq - startFreq) * float(i) / divX;
//         int x = leftMargin + i * graphWidth / divX;
//         painter.setPen(Qt::yellow);
//         painter.drawLine(x, graphHeight, x, graphHeight + 4);
//         painter.setPen(Qt::white);
//         painter.drawText(x - 18, graphHeight + 20,
//                          QString::number(freq, 'f', 2));
//     }

//         // Draw the vertical marker only if it is enabled.

//         if (m_showVerticalMarker)
//         {


// double x = leftMargin +
//     (m_verticalFrequency - startFreq) /
//         (stopFreq - startFreq) *
//         graphWidth;

//             // Create a red pen with thickness of 2 pixels.
//             // Everything drawn after this will use this pen.
//             painter.setPen(QPen(Qt::red, 2));

//             // Draw the vertical line.
//             //
//             // Start Point:
//             //     (x, plotRect.top())
//             //
//             // End Point:
//             //     (x, plotRect.bottom())
//             //
//             // Since both points have the same X coordinate,
//             // Qt draws a perfectly vertical line.
//             painter.drawLine(
//                 QPointF(x, 0),
//                 QPointF(x, graphHeight));
//         }


//   //  ── Threshold Line ────────────────────────────────────────────

//         // Convert m_threshold (dBm) to a Y pixel position using the same
//         // formula as the grid lines to keep it on the same scale.
//         double thresholdY = graphHeight -
//                             ((m_threshold - minPower) /
//                              (maxPower - minPower)) * graphHeight;

//         // Red dashed line spanning the full graph width at the threshold level.
//         // Dashed style distinguishes it visually from the solid grid lines.
//         painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
//         painter.drawLine(leftMargin, thresholdY, width(), thresholdY);

//         // Red label showing the exact threshold value in dBm.
//         // Drawn 5px above the line so it does not overlap the line itself.
//         painter.setPen(Qt::red);
//         painter.drawText(leftMargin + 5, thresholdY - 5,
//                          QString("Threshold: %1 dBm").arg(m_threshold));

//     // ── Spectrum sliding window ───────────────────────────────
//     //==============================================================
//     // Spectrum Drawing
//     //==============================================================

//     if (m_window.isEmpty())
//         return;

//     // Draw only inside graph area
//     painter.setClipRect(leftMargin, 0, graphWidth, graphHeight);

//     int n = m_window.size();

//     float slotW = float(graphWidth) / MAX_POINTS;

//     //--------------------------------------------------
//     // Smooth the FFT values
//     //--------------------------------------------------
//     QVector<float> smooth(n);

//     const int halfWindow = 4;

//     for (int i = 0; i < n; ++i)
//     {
//         float sum = 0.0f;
//         int count = 0;

//         for (int j = qMax(0, i - halfWindow);
//              j <= qMin(n - 1, i + halfWindow);
//              ++j)
//         {
//             sum += m_window[j].power;
//             count++;
//         }

//         smooth[i] = sum / count;
//     }

//     //--------------------------------------------------
//     // X coordinate
//     //--------------------------------------------------
//     auto pointX = [&](int idx)
//     {
//         return leftMargin +
//                idx * slotW +
//                slotW * 0.5 -
//                m_animShift;
//     };


//     // auto pointX = [&](int idx)
//     // {
//     //     return leftMargin +
//     //            (m_window[idx].freq - startFreq) /
//     //                (stopFreq - startFreq) * graphWidth;
//     // };
//     //--------------------------------------------------
//     // Y coordinate
//     //--------------------------------------------------
//     auto pointY = [&](int idx)
//     {
//         return graphHeight -
//                ((smooth[idx] - minPower) /
//                 (maxPower - minPower)) *
//                    graphHeight;
//     };

//   /*  //--------------------------------------------------
//     // Draw filled area
//     //--------------------------------------------------
//     QPainterPath fill;

//     fill.moveTo(pointX(0), graphHeight);

//     for (int i = 1; i < n; ++i)
//     {
//         QPointF p0(pointX(i - 1), pointY(i - 1));
//         QPointF p1(pointX(i), pointY(i));

//         QPointF c1(p0.x() + (p1.x() - p0.x()) / 3.0,
//                    p0.y());

//         QPointF c2(p1.x() - (p1.x() - p0.x()) / 3.0,
//                    p1.y());

//         fill.cubicTo(c1, c2, p1);
//     }

//     fill.lineTo(pointX(n - 1), graphHeight);
//     fill.closeSubpath();

//     QLinearGradient grad(0, 0, 0, graphHeight);

//     grad.setColorAt(0.0, QColor(0,255,120,120));
//     grad.setColorAt(1.0, QColor(0,255,120,20));

//     painter.fillPath(fill, grad);*/

//     //--------------------------------------------------
//     // Draw spectrum
//     //--------------------------------------------------
//     QPainterPath path;

//     path.moveTo(pointX(0), pointY(0));

//     for (int i = 1; i < n; ++i)
//     {
//         QPointF p0(pointX(i - 1), pointY(i - 1));
//         QPointF p1(pointX(i), pointY(i));

//         QPointF c1(p0.x() + (p1.x() - p0.x()) / 3.0,
//                    p0.y());

//         QPointF c2(p1.x() - (p1.x() - p0.x()) / 3.0,
//                    p1.y());

//         path.cubicTo(c1, c2, p1);
//     }

//     painter.setPen(QPen(QColor(0,255,0),2));
//     painter.drawPath(path);

//     //--------------------------------------------------
//     // Draw FFT points
//     //--------------------------------------------------
//     painter.setBrush(Qt::green);
//     painter.setPen(Qt::NoPen);

//     for (int i = 0; i < n; ++i)
//     {
//         painter.drawEllipse(QPointF(pointX(i),
//                                     pointY(i)),
//                             2,
//                             2);
//     }

//     painter.setClipping(false);
// }
// /*
// ///--------------------------------------------------
// // Draw filled area
// //--------------------------------------------------
// QPainterPath fill;

// fill.moveTo(pointX(0), graphHeight);
// fill.lineTo(pointX(0), pointY(0));

// for (int i = 1; i < n; ++i)
// {
//     fill.lineTo(pointX(i), pointY(i));
// }

// fill.lineTo(pointX(n - 1), graphHeight);
// fill.closeSubpath();

// QLinearGradient grad(0, 0, 0, graphHeight);

// grad.setColorAt(0.0, QColor(0,255,120,120));
// grad.setColorAt(1.0, QColor(0,255,120,20));

// painter.fillPath(fill, grad);

// //--------------------------------------------------
// // Draw spectrum
// //--------------------------------------------------
// QPainterPath path;

// path.moveTo(pointX(0), pointY(0));

// for (int i = 1; i < n; ++i)
// {
//     path.lineTo(pointX(i), pointY(i));
// }

// painter.setPen(QPen(QColor(0,255,0),2));
// painter.drawPath(path);*/
// void SpectrumWidget::setVerticalMarker(float bin)
// {
//     // Debug output confirms the correct frequency value is being received.
//     // Remove or guard with #ifdef QT_DEBUG before shipping a release build.
//     qDebug() << "bin" << bin;

//     // Store the frequency and make the marker visible.
//     // m_showVerticalMarker must be set to true here; without it the
//     // marker is stored but paintEvent skips drawing it entirely.
//     m_verticalFrequency  = bin;
//     m_showVerticalMarker = true;

//     // Immediate repaint so the marker appears without waiting for the
//     // next timer tick.
//     update();
// }

// void SpectrumWidget::setThreshold(int value)
// {
//     // Negate the spinbox value because the spinbox emits a positive
//     // number (e.g. 80) but dBm threshold values are negative (−80 dBm).
//     // Without the negation the threshold line would be drawn at +80 dBm,
//     // which is above the 0 dBm top of the graph and never visible.
//     m_threshold = -(value);
//     update();
// }
// // ── Mouse / wheel ─────────────────────────────────────────────────────────────

// void SpectrumWidget::mouseMoveEvent(QMouseEvent *event)
// {


//     const int leftMargin = 50;
//     const int graphWidth = width() - leftMargin;
//     if (!m_draggingMarker)
//         return;

//     double mouseX = event->position().x();

//     // Keep the marker inside the graph
//     mouseX = std::clamp(mouseX,
//                         double(leftMargin),
//                         double(leftMargin + graphWidth));

//     // Convert pixel back to frequency
//     m_verticalFrequency =
//         startFreq+
//         (mouseX - leftMargin) / graphWidth *
//             (stopFreq - startFreq);

//     update();
// }
// void SpectrumWidget::mousePressEvent(QMouseEvent *event)
// {
//     const int leftMargin = 50;
//     const int graphWidth = width() - leftMargin;

//     double markerX =
//         leftMargin +
//         (m_verticalFrequency - startFreq) /
//             (stopFreq - startFreq) *
//             graphWidth;

//     if (qAbs(event->position().x() - markerX) <= 10)
//         m_draggingMarker = true;
// }
// void SpectrumWidget::mouseReleaseEvent(QMouseEvent *)
// {
//     m_draggingMarker = false;
// }
// void SpectrumWidget::zoomIn()
// {
//     // Calculate the center frequency of the currently displayed spectrum.
//     // Example:
//     // startFreq = 20 MHz
//     // stopFreq  = 6000 MHz
//     //
//     // center = (20 + 6000) / 2
//     //        = 3010 MHz
//     float center = (startFreq + stopFreq) / 2.0f;

//     // Calculate half of the current frequency span.
//     //
//     // Current span = 6000 - 20
//     //              = 5980 MHz
//     //
//     // span = 5980 / 2
//     //      = 2990 MHz
//     float span = (stopFreq - startFreq) / 2.0f;

//     // Reduce the displayed frequency range by half while keeping
//     // the center frequency unchanged.
//     //
//     // New start = center - span/2
//     //           = 3010 - 1495
//     //           = 1515 MHz
//     //
//     // New stop  = center + span/2
//     //           = 3010 + 1495
//     //           = 4505 MHz
//     //
//     // The graph now shows a smaller frequency range, making
//     // the spectrum appear zoomed in.
//     startFreq = center - span / 2.0f;
//     stopFreq  = center + span / 2.0f;

//     // Request Qt to repaint the widget using the new frequency range.
//     update();
// }


// void SpectrumWidget::zoomOut()
// {
//     // Calculate the center frequency of the current view.
//     //
//     // Example:
//     // startFreq = 1515 MHz
//     // stopFreq  = 4505 MHz
//     //
//     // center = (1515 + 4505) / 2
//     //        = 3010 MHz
//     float center = (startFreq + stopFreq) / 2.0f;

//     // Calculate the current frequency span.
//     //
//     // span = 4505 - 1515
//     //      = 2990 MHz
//     float span = stopFreq - startFreq;

//     // Increase the displayed frequency range.
//     //
//     // New start = center - span
//     //           = 3010 - 2990
//     //           = 20 MHz
//     //
//     // New stop  = center + span
//     //           = 3010 + 2990
//     //           = 6000 MHz
//     //
//     // The graph now shows a larger frequency range,
//     // making the spectrum appear zoomed out.
//     startFreq = center - span;
//     stopFreq  = center + span;

//     // Request Qt to redraw the graph using the updated range.
//     update();


// }





// void SpectrumWidget::wheelEvent(QWheelEvent *event)
// {
//     float zoomFactor = (event->angleDelta().y() > 0) ? 0.9f : 1.1f;

//     const int leftMargin   = 50;
//     const int bottomMargin = 30;
//     const int graphWidth   = width()  - leftMargin;
//     const int graphHeight  = height() - bottomMargin;

//     QPointF pos = event->position(); // Qt6; use event->pos() in Qt5

//     bool zoomX = !(event->modifiers() & Qt::ShiftModifier);  // Ctrl or no modifier
//     bool zoomY = !(event->modifiers() & Qt::ControlModifier); // Shift or no modifier

//     // If user holds Ctrl -> X only, Shift -> Y only, neither -> both

//     //--------------------------------------------------
//     // Y-AXIS ZOOM (power, dB)
//     //--------------------------------------------------
//     if (zoomY)
//     {
//         float range = maxPower - minPower;
//         float newRange = range * zoomFactor;

//         const float minRange = 10.0f;
//         const float maxRangeY = absoluteMaxPower - absoluteMinPower;
//         newRange = qBound(minRange, newRange, maxRangeY);

//         float ratio = 1.0f - (pos.y() / graphHeight); // 0 = bottom, 1 = top
//         float centerValue = minPower + ratio * range;

//         float newMin = centerValue - ratio * newRange;
//         float newMax = centerValue + (1.0f - ratio) * newRange;

//         if (newMin < absoluteMinPower)
//         {
//             newMax += (absoluteMinPower - newMin);
//             newMin = absoluteMinPower;
//         }
//         if (newMax > absoluteMaxPower)
//         {
//             newMin -= (newMax - absoluteMaxPower);
//             newMax = absoluteMaxPower;
//         }

//         minPower = newMin;
//         maxPower = newMax;
//     }

//     //--------------------------------------------------
//     // X-AXIS ZOOM (frequency)
//     //--------------------------------------------------
//     if (zoomX)
//     {
//         float range = stopFreq - startFreq;
//         float newRange = range * zoomFactor;

//         const float minRangeX = 1.0f; // smallest allowed span, e.g. 1 MHz — tune to your needs
//         const float maxRangeX = absoluteMaxFreq - absoluteMinFreq;
//         newRange = qBound(minRangeX, newRange, maxRangeX);

//         float mouseX = pos.x() - leftMargin;
//         float ratio = qBound(0.0f, mouseX / graphWidth, 1.0f); // 0 = left, 1 = right

//         float centerValue = startFreq + ratio * range;

//         float newStart = centerValue - ratio * newRange;
//         float newStop  = centerValue + (1.0f - ratio) * newRange;

//         if (newStart < absoluteMinFreq)
//         {
//             newStop += (absoluteMinFreq - newStart);
//             newStart = absoluteMinFreq;
//         }
//         if (newStop > absoluteMaxFreq)
//         {
//             newStart -= (newStop - absoluteMaxFreq);
//             newStop = absoluteMaxFreq;
//         }

//         startFreq = newStart;
//         stopFreq  = newStop;
//     }

//     update();
//     event->accept();
// }






















// m_pointQueue.clear();
// m_drawIndex = 0;


//then when new data arrives you must reset it:

#include "spectrumwidget.h"
#include "ui_spectrumwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QtMath>

SpectrumWidget::SpectrumWidget(QWidget *parent)
    : QWidget(parent),absoluteMinFreq(0.0f),      // e.g. your device's lowest tunable freq
    absoluteMaxFreq(6000.0f),  // e.g. your device's highest tunable freq (MHz)
    ui(new Ui::SpectrumWidget)
{
    setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    setAutoFillBackground(false);

    // ── Data timer: one new point enters every 100 ms ────────
    m_dataTimer = new QTimer(this);
    m_dataTimer->setInterval(200);
    connect(m_dataTimer, &QTimer::timeout,
            this, &SpectrumWidget::onDataTimer);
    m_dataTimer->start();

    // ── Animation timer: smooth slide at 10 ms ───────────────
    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(100);
    connect(m_animTimer, &QTimer::timeout,
            this, &SpectrumWidget::onAnimTimer);
}

SpectrumWidget::~SpectrumWidget()
{
    delete ui;
}

// ── Public API ────────────────────────────────────────────────────────────────

void SpectrumWidget::onDataTimer()
{
    if (m_pointQueue.isEmpty())
        return;

    m_window.append(m_pointQueue.dequeue());

    if (m_window.size() > MAX_POINTS)
    {
        m_window.removeFirst();
    }

    update();
}

void SpectrumWidget::onAnimTimer()
{
    const int graphW = width() - 50;
    float slotW = float(graphW) / MAX_POINTS;
    float stepSize = slotW / ANIM_STEPS;

    // Move to the RIGHT
    m_animShift += stepSize;

    if (m_animShift >= slotW)
    {
        m_animShift = 0.0f;
        m_animating = false;
        m_animTimer->stop();

        // Remove the oldest point from the LEFT
        if (m_window.size() > MAX_POINTS)
            m_window.removeFirst();
    }

    update();
}


void SpectrumWidget::setSpectrumData(
    const QVector<double> &freqMHz,
    const QVector<double> &powerDbm)
{
    if (freqMHz.size() < 2 || freqMHz.size() != powerDbm.size())
        return;

    m_window.clear();
    m_window.reserve(freqMHz.size());
    for (int i = 0; i < freqMHz.size(); ++i)
    {
        SpectrumPoint point;
        point.freq  = freqMHz[i];
        point.power = powerDbm[i];
        m_window.append(point);
    }

    update();   // repaint immediately with the full trace

    emit spectrumDataReady(powerDbm, startFreq, stopFreq);
}
// ── Zoom ──────────────────────────────────────────────────────────────────────

// Shared zoom math used by wheelEvent() and by zoomIn()/zoomOut().
// `pos` is a widget-local pixel position; the value/frequency currently
// under that pixel stays fixed on screen after the zoom is applied.
void SpectrumWidget::zoomAtPoint(float zoomFactor, const QPointF &pos,
                                 bool zoomX, bool zoomY)
{
    const int leftMargin   = 50;
    const int bottomMargin = 30;
    const int graphWidth   = width()  - leftMargin;
    const int graphHeight  = height() - bottomMargin;

    //--------------------------------------------------
    // Y-AXIS ZOOM (power, dB)
    //--------------------------------------------------
    if (zoomY && graphHeight > 0)
    {
        float range = maxPower - minPower;
        float newRange = range * zoomFactor;

        const float minRange  = 10.0f;
        const float maxRangeY = absoluteMaxPower - absoluteMinPower;
        newRange = qBound(minRange, newRange, maxRangeY);

        float ratio = 1.0f - float(pos.y() / graphHeight); // 0 = bottom, 1 = top
        ratio = qBound(0.0f, ratio, 1.0f);
        float centerValue = minPower + ratio * range;

        float newMin = centerValue - ratio * newRange;
        float newMax = centerValue + (1.0f - ratio) * newRange;

        if (newMin < absoluteMinPower)
        {
            newMax += (absoluteMinPower - newMin);
            newMin = absoluteMinPower;
        }
        if (newMax > absoluteMaxPower)
        {
            newMin -= (newMax - absoluteMaxPower);
            newMax = absoluteMaxPower;
        }

        minPower = newMin;
        maxPower = newMax;
    }

    //--------------------------------------------------
    // X-AXIS ZOOM (frequency)
    //--------------------------------------------------
    if (zoomX && graphWidth > 0)
    {
        float range = stopFreq - startFreq;
        float newRange = range * zoomFactor;

        const float minRangeX = 1.0f; // smallest allowed span, e.g. 1 MHz — tune to your needs
        const float maxRangeX = absoluteMaxFreq - absoluteMinFreq;
        newRange = qBound(minRangeX, newRange, maxRangeX);

        float mouseX = float(pos.x()) - leftMargin;
        float ratio = qBound(0.0f, mouseX / graphWidth, 1.0f); // 0 = left, 1 = right

        float centerValue = startFreq + ratio * range;

        float newStart = centerValue - ratio * newRange;
        float newStop  = centerValue + (1.0f - ratio) * newRange;

        if (newStart < absoluteMinFreq)
        {
            newStop += (absoluteMinFreq - newStart);
            newStart = absoluteMinFreq;
        }
        if (newStop > absoluteMaxFreq)
        {
            newStart -= (newStop - absoluteMaxFreq);
            newStop = absoluteMaxFreq;
        }

        startFreq = newStart;
        stopFreq  = newStop;
    }
}

// Zoom in around anchorPos (defaults to the graph center when not given,
// e.g. when called from a toolbar button instead of the mouse).
void SpectrumWidget::zoomIn(const QPointF &anchorPos)
{
    const int leftMargin   = 50;
    const int bottomMargin = 30;

    QPointF pos = anchorPos;
    if (pos.isNull())
        pos = QPointF(leftMargin + (width() - leftMargin) / 2.0,
                      (height() - bottomMargin) / 2.0);

    zoomAtPoint(0.9f, pos, /*zoomX=*/true, /*zoomY=*/true);
    update();
}

// Zoom out around anchorPos (defaults to the graph center when not given).
void SpectrumWidget::zoomOut(const QPointF &anchorPos)
{
    const int leftMargin   = 50;
    const int bottomMargin = 30;

    QPointF pos = anchorPos;
    if (pos.isNull())
        pos = QPointF(leftMargin + (width() - leftMargin) / 2.0,
                      (height() - bottomMargin) / 2.0);

    zoomAtPoint(1.1f, pos, /*zoomX=*/true, /*zoomY=*/true);
    update();
}

// ── Paint ─────────────────────────────────────────────────────────────────────
void SpectrumWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);

    const int leftMargin   = 50;
    const int bottomMargin = 30;
    const int graphWidth   = width()  - leftMargin;
    const int graphHeight  = height() - bottomMargin;

    // ── Axes ─────────────────────────────────────────────────
    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawLine(leftMargin, 0, leftMargin, graphHeight);
    painter.drawLine(leftMargin, graphHeight, width(), graphHeight);

    // ── Horizontal grid + Y labels ───────────────────────────
    for (int p = -150; p <= 0; p += 10)
    {
        double y = graphHeight -
                   ((double(p) - minPower) /
                    (maxPower - minPower)) * graphHeight;

        painter.setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
        painter.drawLine(leftMargin, y, width(), y);

        painter.setPen(Qt::white);
        painter.drawText(5, int(y) + 5, QString::number(p));
    }


    // ── X-axis frequency labels (fixed, based on startFreq/stopFreq) ─────
    painter.setFont(QFont("Monospace", 8));
    const int divX = 5;
    for (int i = 0; i <= divX; ++i)
    {
        float freq = startFreq + (stopFreq - startFreq) * float(i) / divX;
        int x = leftMargin + i * graphWidth / divX;
        painter.setPen(Qt::yellow);
        painter.drawLine(x, graphHeight, x, graphHeight + 4);
        painter.setPen(Qt::white);
        painter.drawText(x - 18, graphHeight + 20,
                         QString::number(freq, 'f', 2));
    }

    // Draw the vertical marker only if it is enabled.

    if (m_showVerticalMarker)
    {


        double x = leftMargin +
                   (m_verticalFrequency - startFreq) /
                       (stopFreq - startFreq) *
                       graphWidth;

        // Create a red pen with thickness of 2 pixels.
        // Everything drawn after this will use this pen.
        painter.setPen(QPen(Qt::red, 2));

        // Draw the vertical line.
        painter.drawLine(
            QPointF(x, 0),
            QPointF(x, graphHeight));
    }


    //  ── Threshold Line ────────────────────────────────────────────

    // Convert m_threshold (dBm) to a Y pixel position using the same
    // formula as the grid lines to keep it on the same scale.
    double thresholdY = graphHeight -
                        ((m_threshold - minPower) /
                         (maxPower - minPower)) * graphHeight;

    // Draw it a little thicker while it's being dragged so the user
    // gets clear visual feedback that they've grabbed it.
    painter.setPen(QPen(Qt::red, m_dragThreshold ? 3 : 2, Qt::DashLine));
    painter.drawLine(leftMargin, thresholdY, width(), thresholdY);

    // Red label showing the exact threshold value in dBm.
    // Drawn 5px above the line so it does not overlap the line itself.
    painter.setPen(Qt::red);
    painter.drawText(leftMargin + 5, thresholdY - 5,
                     QString("Threshold: %1 dBm").arg(m_threshold, 0, 'f', 1));





    if (m_showCrosshair)
    {
        painter.setPen(QPen(Qt::yellow,1,Qt::DashLine));

        painter.drawLine(QPointF(m_mousePos.x(),0),
                         QPointF(m_mousePos.x(),graphHeight));

        painter.drawLine(QPointF(leftMargin,m_mousePos.y()),
                         QPointF(width(),m_mousePos.y()));
    }

    // ── Freq/Power readout label, follows the cursor ──────────
    if (m_tooltipEnabled && m_nearestIndex >= 0 && m_nearestIndex < m_window.size())
    {
        QString label = QString("Freq : %1 MHz\nPower : %2 dBm")
        .arg(m_window[m_nearestIndex].freq, 0, 'f', 2)
            .arg(m_window[m_nearestIndex].power, 0, 'f', 1);

        painter.setFont(QFont("Arial", 9));
        QFontMetrics fm(painter.font());
        QStringList lines = label.split('\n');

        int textWidth = 0;
        for (const QString &line : lines)
            textWidth = qMax(textWidth, fm.horizontalAdvance(line));
        textWidth += 12;
        int textHeight = fm.height() * lines.size() + 8;

        int boxX = qMin(int(m_mousePos.x()) + 12, width() - textWidth - 2);
        int boxY = qMax(int(m_mousePos.y()) - textHeight - 4, 0);

        QRect labelRect(boxX, boxY, textWidth, textHeight);
        painter.fillRect(labelRect, QColor(0, 0, 0, 190));
        painter.setPen(QPen(Qt::yellow));
        painter.drawRect(labelRect);
        painter.drawText(labelRect, Qt::AlignCenter, label);
    }

    // ── Spectrum sliding window ───────────────────────────────
    //==============================================================
    // Spectrum Drawing
    //==============================================================

    if (m_window.isEmpty())
        return;

    // Draw only inside graph area
    painter.setClipRect(leftMargin, 0, graphWidth, graphHeight);

    int n = m_window.size();

    float slotW = float(graphWidth) / MAX_POINTS;

    //--------------------------------------------------
    // Smooth the FFT values
    //--------------------------------------------------
    QVector<float> smooth(n);

    const int halfWindow = 4;

    for (int i = 0; i < n; ++i)
    {
        float sum = 0.0f;
        int count = 0;

        for (int j = qMax(0, i - halfWindow);
             j <= qMin(n - 1, i + halfWindow);
             ++j)
        {
            sum += m_window[j].power;
            count++;
        }

        smooth[i] = sum / count;
    }

    //--------------------------------------------------
    // X coordinate
    //--------------------------------------------------
    auto pointX = [&](int idx)
    {
        return leftMargin +
               idx * slotW +
               slotW * 0.5 -
               m_animShift;
    };

    //--------------------------------------------------
    // Y coordinate
    //--------------------------------------------------
    auto pointY = [&](int idx)
    {
        return graphHeight -
               ((smooth[idx] - minPower) /
                (maxPower - minPower)) *
                   graphHeight;
    };

    //--------------------------------------------------
    // Draw spectrum
    //--------------------------------------------------
    QPainterPath path;

    path.moveTo(pointX(0), pointY(0));

    for (int i = 1; i < n; ++i)
    {
        QPointF p0(pointX(i - 1), pointY(i - 1));
        QPointF p1(pointX(i), pointY(i));

        QPointF c1(p0.x() + (p1.x() - p0.x()) / 3.0,
                   p0.y());

        QPointF c2(p1.x() - (p1.x() - p0.x()) / 3.0,
                   p1.y());

        path.cubicTo(c1, c2, p1);
    }

    painter.setPen(QPen(QColor(0,255,0),2));
    painter.drawPath(path);

    //--------------------------------------------------
    // Draw FFT points
    //--------------------------------------------------
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < n; ++i)
    {
        painter.drawEllipse(QPointF(pointX(i),
                                    pointY(i)),
                            2,
                            2);
    }

    painter.setClipping(false);
}

void SpectrumWidget::setVerticalMarker(float bin)
{
    // Debug output confirms the correct frequency value is being received.
    // Remove or guard with #ifdef QT_DEBUG before shipping a release build.
    qDebug() << "bin" << bin;

    // Store the frequency and make the marker visible.
    // m_showVerticalMarker must be set to true here; without it the
    // marker is stored but paintEvent skips drawing it entirely.
    m_verticalFrequency  = bin;
    m_showVerticalMarker = true;

    // Immediate repaint so the marker appears without waiting for the
    // next timer tick.
    update();
}

void SpectrumWidget::setThreshold(int value)
{
    // Negate the spinbox value because the spinbox emits a positive
    // number (e.g. 80) but dBm threshold values are negative (−80 dBm).
    // Without the negation the threshold line would be drawn at +80 dBm,
    // which is above the 0 dBm top of the graph and never visible.
    m_threshold = -(value);
    update();
}
// ── Mouse / wheel ─────────────────────────────────────────────────────────────

void SpectrumWidget::mouseMoveEvent(QMouseEvent *event)
{
    const int leftMargin   = 50;
    const int bottomMargin = 30;
    const int graphWidth   = width() - leftMargin;
    const int graphHeight  = height() - bottomMargin;

    // Store mouse position for crosshair
    m_mousePos = event->position();
    m_showCrosshair = false;

    //--------------------------------------------------
    // Convert mouse X position to frequency
    //--------------------------------------------------
    double mouseX = std::clamp(
        double(event->position().x()),
        double(leftMargin),
        double(leftMargin + graphWidth));

    double currentFreq =
        startFreq +
        (mouseX - leftMargin) / graphWidth *
            (stopFreq - startFreq);

    //--------------------------------------------------
    // Find nearest FFT point
    //--------------------------------------------------
    int nearest = -1;
    double minDist = std::numeric_limits<double>::max();

    for (int i = 0; i < m_window.size(); ++i)
    {
        double d = qAbs(m_window[i].freq - currentFreq);

        if (d < minDist)
        {
            minDist = d;
            nearest = i;
        }
    }

    //--------------------------------------------------
    // Track nearest FFT point for the on-canvas readout label
    // (drawn in paintEvent, not via QToolTip).
    //--------------------------------------------------
    m_nearestIndex = (m_tooltipEnabled && nearest >= 0) ? nearest : -1;

    //--------------------------------------------------
    // Drag vertical marker
    //--------------------------------------------------
    if (m_draggingMarker)
    {
        m_verticalFrequency = currentFreq;
    }

    //--------------------------------------------------
    // Drag threshold line
    //--------------------------------------------------
    if (m_dragThreshold)
    {
        double mouseY = std::clamp(
            double(event->position().y()),
            0.0,
            double(graphHeight));

        m_threshold =
            minPower +
            (1.0 - mouseY / graphHeight) *
                (maxPower - minPower);

        m_threshold = qBound(minPower,
                             m_threshold,
                             maxPower);

        emit thresholdChanged(m_threshold);
    }

    update();
}
void SpectrumWidget::mousePressEvent(QMouseEvent *event)
{
    const int leftMargin   = 50;
    const int bottomMargin = 30;
    const int graphWidth   = width()  - leftMargin;
    const int graphHeight  = height() - bottomMargin;

    // ── Check if the click grabbed the vertical frequency marker ────
    double markerX =
        leftMargin +
        (m_verticalFrequency - startFreq) /
            (stopFreq - startFreq) *
            graphWidth;

    if (qAbs(event->position().x() - markerX) <= 10)
    {
        m_draggingMarker = true;
        return;
    }

    // ── Check if the click grabbed the threshold line ────────────────
    double thresholdY = graphHeight -
                        ((m_threshold - minPower) /
                         (maxPower - minPower)) * graphHeight;

    if (qAbs(event->position().y() - thresholdY) <= 6)
    {
        m_dragThreshold = true;
        qDebug() << "Threshold grabbed at:" << m_threshold << "dBm";
        return;
    }
}
void SpectrumWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (m_dragThreshold)
        qDebug() << "Threshold released at:" << m_threshold << "dBm";

    m_draggingMarker    = false;
    m_dragThreshold = false;
}

void SpectrumWidget::wheelEvent(QWheelEvent *event)
{
    float zoomFactor = (event->angleDelta().y() > 0) ? 0.9f : 1.1f;

    QPointF pos = event->position(); // Qt6; use event->pos() in Qt5

    bool zoomX = !(event->modifiers() & Qt::ShiftModifier);  // Ctrl or no modifier
    bool zoomY = !(event->modifiers() & Qt::ControlModifier); // Shift or no modifier

    // If user holds Ctrl -> X only, Shift -> Y only, neither -> both
    zoomAtPoint(zoomFactor, pos, zoomX, zoomY);

    update();
    event->accept();
}
void SpectrumWidget::leaveEvent(QEvent *)
{
    m_showCrosshair = false;
    m_nearestIndex  = -1;
    update();
}
void SpectrumWidget::setTooltipEnabled(bool enabled)
{
    m_tooltipEnabled = enabled;
    if (!enabled)
        m_nearestIndex = -1;   // immediately drop the readout label if it's showing

    update();
}
