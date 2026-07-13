








// //==============================================================
// // File        : waterfalldata.cpp
// // Description : Waterfall display widget
// //
// // This widget displays FFT spectrum data as a scrolling
// // waterfall. Each incoming FFT frame is stored as one row.
// // New rows appear at the top and smoothly slide downward.
// // The widget also draws fixed X and Y axes with frequency
// // labels and row numbers.
// //
// // Features:
// //  - Real-time waterfall display
// //  - Smooth row animation
// //  - FFT power-to-color mapping
// //  - Fixed frequency axis
// //  - Automatic resizing
// //==============================================================

// #include "waterfalldata.h"

// #include <QPainter>      // Used for drawing rectangles, lines and text
// #include <QResizeEvent>  // Handles widget resize events
// #include <QTimer>        // Creates periodic timers for data and animation
// #include <QQueue>        // Stores incoming FFT frames (FIFO queue)
// #include <QVector>       // Stores one FFT spectrum frame
// #include <QColor>        // Represents RGB colors
// #include <QFont>         // Used to set axis label font
// #include <QtMath>        // Provides qAbs(), qBound(), qMax(), etc.




// //==============================================================
// // Constructor
// //==============================================================



// WaterfallData::WaterfallData(QWidget *parent)
//     : QWidget(parent)
// {
//     // Disable Qt automatic background painting.
//     // We paint the entire widget ourselves.
//     // If removed: Qt paints the background first, which may cause flickering.
//     setAutoFillBackground(false);

//     //---------------- DATA TIMER ----------------
//     // Receives one new FFT frame every 100 ms.
//     m_dataTimer = new QTimer(this);
//     m_dataTimer->setInterval(100);

//     // Every 100 ms:
//     // timeout() --> onDataTimer()
//     connect(m_dataTimer, &QTimer::timeout,
//             this, &WaterfallData::onDataTimer);

//     m_dataTimer->start();

//     //---------------- ANIMATION TIMER ----------------
//     // Moves rows smoothly.
//     m_animTimer = new QTimer(this);
//     m_animTimer->setInterval(10);

//     // Every 10 ms:
//     // timeout() --> onAnimTimer()
//     connect(m_animTimer, &QTimer::timeout,
//             this, &WaterfallData::onAnimTimer);

//     // Animation starts only when new data arrives.
// }


// //==============================================================
// // Widget resized
// //==============================================================
// void WaterfallData::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);

//     // Left margin for Y-axis labels.
//     m_plotX = LEFT_MARGIN;

//     // Small top margin.
//     m_plotY = 4;

//     // Width of waterfall drawing area.
//     m_plotW = width() - LEFT_MARGIN;

//     // Height of waterfall drawing area.
//     m_plotH = height() - BOT_MARGIN - m_plotY;

//     // Height of one waterfall row.
//     // qMax() prevents row height becoming smaller than 4 pixels.
//     m_rowH = qMax(4,
//                   (m_plotH - GAP * (MAX_ROWS - 1)) / MAX_ROWS);

//     // Redraw widget.
//     update();
// }


// //==============================================================
// // Receive new FFT spectrum
// //==============================================================
// void WaterfallData::setSpectrum(const QVector<double> &power1,
//                                 float startFreq,
//                                 float stopFreq)
// {
//     // Save frequency range for X-axis labels.
//     m_startFreq = startFreq;
//     m_stopFreq  = stopFreq;


//     QVector<float> powerFloat;

//     for (double value : power1)
//     {
//         powerFloat.append(static_cast<float>(value));
//     }    // Store FFT frame inside queue.
//     // Queue = First In First Out (FIFO)
//     //
//     // Frame1
//     // Frame2
//     // Frame3
//     //
//     // dequeue() always removes Frame1 first.
//     m_queue.enqueue(powerFloat);
// }


// //==============================================================
// // Called every 100 ms
// //==============================================================
// void WaterfallData::onDataTimer()
// {
//     // No FFT data?
//     if (m_queue.isEmpty())
//         return;

//     // Already moving?
//     if (m_animating)
//         return;

//     // Remove oldest frame from queue
//     // and place it at TOP of waterfall.
//     m_rows.prepend(m_queue.dequeue());

//     // Keep only MAX_ROWS.
//     while (m_rows.size() > MAX_ROWS + 1)
//         m_rows.removeLast();

//     // Start animation.
//     m_animOffset = 0;
//     m_animating = true;

//     m_animTimer->start();
// }


// //==============================================================
// // Smooth animation
// //==============================================================
// void WaterfallData::onAnimTimer()
// {
//     // Distance of one row.
//     int totalSlide = m_rowH + GAP;

//     // Small movement every timer.
//     int step = qMax(1, totalSlide / ANIM_STEPS);

//     // Move rows downward.
//     m_animOffset += step;

//     // Animation finished?
//     if (m_animOffset >= totalSlide)
//     {
//         m_animOffset = 0;

//         m_animating = false;

//         m_animTimer->stop();

//         // Remove row that moved outside screen.
//         while (m_rows.size() > MAX_ROWS)
//             m_rows.removeLast();
//     }

//     // Redraw widget.
//     update();
// }


// //==============================================================
// // Convert power to colour
// //==============================================================
// QColor WaterfallData::powerToColor(float power)
// {
//     // Keep power between -150 and 0 dBm.
//     power = qBound(-150.0f, power, 0.0f);

//     // Convert
//     //
//     // -150 -> 0
//     // 0    -> 1
//     float n = (power + 150.0f) / 150.0f;

//     // Generate RGB colour.
//     int r = int(255 * n);
//     int g = int(255 * (1.0f - qAbs(n - 0.5f) * 2.0f));
//     int b = int(255 * (1.0f - n));

//     return QColor(r, g, b);
// }


// //==============================================================
// // Draw everything
// //==============================================================
// void WaterfallData::paintEvent(QPaintEvent *)
// {
//     // Drawing tool.
//     QPainter p(this);

//     // Rectangles don't need smoothing.
//     p.setRenderHint(QPainter::Antialiasing, false);

//     // Clear old drawing.
//     // Without this, old waterfall remains.
//     p.fillRect(rect(), Qt::black);

//     // Invalid drawing area?
//     if (m_plotW <= 0 || m_plotH <= 0)
//         return;

//     //--------------------------------------------------
//     // Draw only inside waterfall area.
//     // Without clipping,
//     // waterfall would draw over X-axis and Y-axis.
//     //--------------------------------------------------
//     p.setClipRect(m_plotX, m_plotY,
//                   m_plotW, m_plotH);

//     //--------------------------------------------------
//     // Draw every waterfall row.
//     //--------------------------------------------------
//     for (int i = 0; i < m_rows.size(); i++)
//     {
//         const QVector<float> &row = m_rows[i];

//         if (row.isEmpty())
//             continue;

//         // Current Y position.
//         // Animation changes this value every 10 ms.
//         int rowTop =
//             m_plotY
//             - (m_rowH + GAP)
//             + m_animOffset
//             + i * (m_rowH + GAP);

//         // Skip rows outside screen.
//         if (rowTop + m_rowH < m_plotY)
//             continue;

//         if (rowTop > m_plotY + m_plotH)
//             break;

//         //--------------------------------------------------
//         // Draw every FFT bin.
//         //--------------------------------------------------
//         int nBins = row.size();

//         for (int bin = 0; bin < nBins; bin++)
//         {
//             // Start X of FFT bin.
//             int x0 = m_plotX +
//                      bin * m_plotW / nBins;

//             // End X of FFT bin.
//             int x1 = m_plotX +
//                      (bin + 1) * m_plotW / nBins;

//             if (x1 <= x0)
//                 x1 = x0 + 1;

//             // Draw coloured rectangle.
//             p.fillRect(x0,
//                        rowTop,
//                        x1 - x0,
//                        m_rowH,
//                        powerToColor(row[bin]));
//         }
//     }

//     //--------------------------------------------------
//     // Disable clipping.
//     // Otherwise axes would also be clipped.
//     //--------------------------------------------------
//     p.setClipping(false);

//     //--------------------------------------------------
//     // Draw X-axis.
//     //--------------------------------------------------
//     int axisY = height() - BOT_MARGIN;

//     p.setPen(Qt::white);
//     p.drawLine(m_plotX,
//                axisY,
//                m_plotX + m_plotW,
//                axisY);

//     //--------------------------------------------------
//     // Draw frequency labels.
//     //--------------------------------------------------
//     for (int i = 0; i <= 5; i++)
//     {
//         float freq =
//             m_startFreq +
//             (m_stopFreq - m_startFreq) * i / 5.0;

//         int x = m_plotX + i * m_plotW / 5;

//         p.drawLine(x,
//                    axisY,
//                    x,
//                    axisY + 4);

//         p.drawText(x - 20,
//                    axisY + 16,
//                    QString::number(freq, 'f', 2));
//     }

//     //--------------------------------------------------
//     // Draw Y-axis.
//     //--------------------------------------------------
//     p.drawLine(m_plotX,
//                m_plotY,
//                m_plotX,
//                axisY);

//     //--------------------------------------------------
//     // Draw row numbers.
//     //--------------------------------------------------
//     for (int i = 0; i < MAX_ROWS; i++)
//     {
//         int y =
//             m_plotY +
//             i * (m_rowH + GAP) +
//             m_rowH / 2;

//         p.drawLine(m_plotX - 4,
//                    y,
//                    m_plotX,
//                    y);

//         p.drawText(2,
//                    y + 4,
//                    QString::number(i + 1));
//     }
// }
// WaterfallData.h — signature change needed:
// void addSpectrum(const QVector<double> &power, float startFreq, float stopFreq);













/*
#include "WaterfallData.h"
#include <QPainter>
#include <QDebug>

WaterfallData::WaterfallData(QWidget *parent)
    : QWidget(parent),
    m_image(512, 400, QImage::Format_RGB32)
{
    m_image.fill(Qt::black);
}

void WaterfallData::addSpectrum(const QVector<double> &power
                                )
{
    if (power.isEmpty())
        return;

    m_startFreq = 20;
    m_stopFreq  = 6000;

    if (m_image.width() != power.size())
    {
        m_image = QImage(power.size(), 500, QImage::Format_RGB32);
        m_image.fill(Qt::black);
    }

    const int rowHeight = m_frameHeight;   // e.g. 4 pixels per frame

    // Shift everything down by rowHeight, starting from the bottom
    // so we don't overwrite data before it's copied.
    for (int y = m_image.height() - 1; y >= rowHeight; --y)
    {
        memcpy(m_image.scanLine(y),
               m_image.scanLine(y - rowHeight),
               m_image.bytesPerLine());
    }

    // Precompute this frame's colors once...
    QVector<QRgb> colors(power.size());
    for (int x = 0; x < power.size(); ++x)
        colors[x] = powerToColor(power[x]).rgb();

    // ...then paint the SAME row data into the top `rowHeight` rows.
    for (int y = 0; y < rowHeight; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(m_image.scanLine(y));
        memcpy(line, colors.constData(), colors.size() * sizeof(QRgb));
    }

    update();
}
QColor WaterfallData::powerToColor(double power)
{
    // Match the same dynamic range used elsewhere (SpectrumWidget: -150..0 dBm).
    // Narrow ranges like -120..-80 saturate the color scale and hide detail.
    constexpr double minPower = -150.0;
    constexpr double maxPower = 0.0;

    power = qBound(minPower, power, maxPower);
    double t = (power - minPower) / (maxPower - minPower);   // 0.0 to 1.0

    int r = int(255 * t);
    int g = int(255 * (1.0 - std::abs(t - 0.5) * 2.0));
    int b = int(255 * (1.0 - t));

    return QColor(r, g, b);
}

void WaterfallData::setFrameHeight(int px)
{
    m_frameHeight = qMax(1, px);

}

void WaterfallData::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    const int leftMargin   = 60;
    const int topMargin    = 20;
    const int bottomMargin = 30;

    QRect plotRect(leftMargin,
                   topMargin,
                   width() - leftMargin - 10,
                   height() - topMargin - bottomMargin);

    painter.drawImage(plotRect, m_image);

    painter.setPen(Qt::white);
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    painter.drawLine(plotRect.topLeft(), plotRect.bottomLeft());

    painter.drawText(plotRect.left() - 10,
                     plotRect.bottom() + 20,
                     QString::number(m_startFreq, 'f', 3));

    painter.drawText(plotRect.right() - 40,
                     plotRect.bottom() + 20,
                     QString::number(m_stopFreq, 'f', 3));

    double center = (m_startFreq + m_stopFreq) / 2.0;
    painter.drawText(plotRect.center().x() - 20,
                     plotRect.bottom() + 20,
                     QString::number(center, 'f', 3));

    painter.drawText(5, plotRect.top() + 10, "Newest");
    painter.drawText(5, plotRect.bottom(), "Oldest");
}
*/
#include "waterfalldata.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include <cstring>   // for memcpy
WaterfallData::WaterfallData(QWidget *parent)
    : QWidget(parent),
    m_image(512, 400, QImage::Format_RGB32)
{
    m_image.fill(Qt::black);
    setMouseTracking(true);   // IMPORTANT: get mouseMoveEvent without a button held down

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &WaterfallData::processQueue);
    m_updateTimer->start(30);
}

void WaterfallData::setUpdateInterval(int ms)
{
    m_updateTimer->setInterval(qMax(1, ms));
}

// Connect your source's signal (e.g. signalStartStop(double,double)) to this slot.
void WaterfallData::setStartStopFreq(double startFreq, double stopFreq)
{
    // If the span actually changed, pixels already drawn no longer line up
    // with the new axis, so wipe the old waterfall before the next
    // spectrum gets drawn into it.
    bool rangeChanged = (startFreq != m_startFreq) || (stopFreq != m_stopFreq);

    m_startFreq = startFreq;
    m_stopFreq  = stopFreq;

    if (rangeChanged)
        clearWaterfall();

    update();   // axis labels need to reflect the new range immediately
}

void WaterfallData::clearWaterfall()
{
    m_image.fill(Qt::black);
    m_spectrumQueue.clear();
    update();
}

void WaterfallData::addSpectrum(const QVector<double> &power)
{
    if (power.isEmpty())
        return;

    // Queue capacity now tracks the size of the spectrum currently being received.
    m_maxQueueSize = power.size();

    m_spectrumQueue.enqueue(power);
    while (m_spectrumQueue.size() > m_maxQueueSize)
        m_spectrumQueue.dequeue();
}

void WaterfallData::processQueue()
{
    if (m_spectrumQueue.isEmpty())
        return;

    QVector<double> power = m_spectrumQueue.dequeue();
    renderSpectrum(power);
}

// renderSpectrum no longer hardcodes start/stop freq - it's set externally now.
void WaterfallData::renderSpectrum(const QVector<double> &power)
{
    // ***** THIS is where the size of the waterfall image changes *****
    // Width is re-sized to match however many frequency bins the incoming
    // spectrum has; height is fixed at 500px regardless of widget size.
    // If the widget is resized, this block does NOT run again on its own
    // (there's no resizeEvent override), so the image stays 500px tall
    // until the next spectrum with a different bin count arrives.
    if (m_image.width() != power.size())
    {
        m_image = QImage(power.size(), 500, QImage::Format_RGB32);
        m_image.fill(Qt::black);
    }

    const int rowHeight = m_frameHeight;

    for (int y = m_image.height() - 1; y >= rowHeight; --y)
    {
        memcpy(m_image.scanLine(y),
               m_image.scanLine(y - rowHeight),
               m_image.bytesPerLine());
    }

    QVector<QRgb> colors(power.size());
    for (int x = 0; x < power.size(); ++x)
        colors[x] = powerToColor(power[x]).rgb();

    for (int y = 0; y < rowHeight; ++y)
    {
        QRgb *line = reinterpret_cast<QRgb *>(m_image.scanLine(y));
        memcpy(line, colors.constData(), colors.size() * sizeof(QRgb));
    }

    update();
}
QColor WaterfallData::powerToColor(double power)
{
    // Match the same dynamic range used elsewhere (SpectrumWidget: -150..0 dBm).
    // Narrow ranges like -120..-80 saturate the color scale and hide detail.
    power = qBound(kMinPowerDbm, power, kMaxPowerDbm);
    double t = (power - kMinPowerDbm) / (kMaxPowerDbm - kMinPowerDbm);   // 0.0 to 1.0

    int r = int(255 * t);
    int g = int(255 * (1.0 - std::abs(t - 0.5) * 2.0));
    int b = int(255 * (1.0 - t));

    return QColor(r, g, b);
}

// Vertical color-scale legend (like a standalone dBm colorbar), drawn on the
// left edge of the widget, aligned top-to-bottom with the plot area so its
// top always corresponds to kMaxPowerDbm and its bottom to kMinPowerDbm.
void WaterfallData::drawColorScale(QPainter &painter, const QRect &plotRect)
{
    const int barX          = 5;
    const int colorBarWidth = 15;

    QRect colorRect(barX, plotRect.top(), colorBarWidth, plotRect.height());

    // Sample powerToColor() itself so the legend always matches the
    // waterfall's actual color mapping, even if that mapping changes later.
    QLinearGradient gradient(colorRect.topLeft(), colorRect.bottomLeft());
    const int stops = 8;
    for (int i = 0; i <= stops; ++i)
    {
        double t = double(i) / stops;                 // 0 = top, 1 = bottom
        double power = kMaxPowerDbm - t * (kMaxPowerDbm - kMinPowerDbm);
        gradient.setColorAt(t, powerToColor(power));
    }

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(colorRect);

    painter.setPen(Qt::gray);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(colorRect);

    // dBm tick labels beside the bar, every 10 dB from max down to min.
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 8, QFont::Bold));

    const int stepDb = 10;
    for (int value = int(kMaxPowerDbm); value >= int(kMinPowerDbm); value -= stepDb)
    {
        double t = (kMaxPowerDbm - value) / (kMaxPowerDbm - kMinPowerDbm);
        int y = colorRect.top() + int(t * colorRect.height());
        painter.drawText(barX + colorBarWidth + 4, y + 4, QString::number(value));
    }
}

// Shared so mouseMoveEvent and paintEvent always agree on where the plot lives.
QRect WaterfallData::plotRectGeometry() const
{
    const int leftMargin   = 60;
    const int topMargin    = 20;
    const int bottomMargin = 30;
    return QRect(leftMargin,
                 topMargin,
                 width() - leftMargin - 10,
                 height() - topMargin - bottomMargin);
}

void WaterfallData::mouseMoveEvent(QMouseEvent *event)
{
    QRect plotRect = plotRectGeometry();
    m_mousePos = event->pos();
    m_mouseInPlot = plotRect.contains(m_mousePos);
    update();   // repaint so the readout follows the cursor
    QWidget::mouseMoveEvent(event);
}

void WaterfallData::leaveEvent(QEvent *event)
{
    m_mouseInPlot = false;
    update();
    QWidget::leaveEvent(event);
}

void WaterfallData::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    QRect plotRect = plotRectGeometry();
    painter.drawImage(plotRect, m_image);

    // Color-scale legend on the far left (matches the dBm range used by powerToColor).
    drawColorScale(painter, plotRect);

    painter.setPen(Qt::white);
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    painter.drawLine(plotRect.topLeft(), plotRect.bottomLeft());

    painter.drawText(plotRect.left() - 10,
                     plotRect.bottom() + 20,
                     QString::number(m_startFreq, 'f', 3));
    painter.drawText(plotRect.right() - 40,
                     plotRect.bottom() + 20,
                     QString::number(m_stopFreq, 'f', 3));

    double center = (m_startFreq + m_stopFreq) / 2.0;
    painter.drawText(plotRect.center().x() - 20,
                     plotRect.bottom() + 20,
                     QString::number(center, 'f', 3));

    // Continuous per-row timestamps flowing top (newest) to bottom (oldest).
    // No stored history needed: row 0 is "now", and each row below it is
    // one frame-interval further back in time, computed from the system
    // clock and the timer interval that drives new rows.
    const int rowHeight = qMax(1, m_frameHeight);
    const int intervalMs = qMax(1, m_updateTimer->interval());
    const QDateTime now = QDateTime::currentDateTime();

    painter.setFont(QFont("Arial", 7));
    int rowsVisible = plotRect.height() / rowHeight;
    for (int i = 0; i < rowsVisible; ++i)
    {
        int y = plotRect.top() + i * rowHeight;
        if (y > plotRect.bottom())
            break;

        QDateTime rowTime = now.addMSecs(-qint64(i) * intervalMs);
        QString label = rowTime.toString("hh:mm:ss.zzz");

        QRect rowLabelRect(plotRect.left() + 2, y, 90, rowHeight);
        painter.fillRect(rowLabelRect, QColor(0, 0, 0, 90));
        painter.setPen(Qt::white);
        painter.drawText(rowLabelRect, Qt::AlignVCenter | Qt::AlignLeft, label);
    }

    // ---- Mouse coordinate / frequency readout ----
    if (m_mouseInPlot)
    {
        // Map pixel X -> frequency using the current start/stop range.
        double xRatio = double(m_mousePos.x() - plotRect.left()) / plotRect.width();
        double freq = m_startFreq + xRatio * (m_stopFreq - m_startFreq);

        // Map pixel Y -> "age" of that row (0 = newest/top, image.height() = oldest/bottom).
        double yRatio = double(m_mousePos.y() - plotRect.top()) / plotRect.height();
        int rowIndex = int(yRatio * m_image.height());
        rowIndex = qBound(0, rowIndex, m_image.height() - 1);

        QString label = QString("x:%1  y:%2  Freq:%3")
                            .arg(m_mousePos.x())
                            .arg(m_mousePos.y())
                            .arg(freq, 0, 'f', 3);

        // Small readout box near the cursor, kept inside the widget bounds.
        QFontMetrics fm(painter.font());
        int textWidth  = fm.horizontalAdvance(label) + 10;
        int textHeight = fm.height() + 6;

        int boxX = qMin(m_mousePos.x() + 12, width() - textWidth - 2);
        int boxY = qMax(m_mousePos.y() - textHeight - 4, 0);

        QRect labelRect(boxX, boxY, textWidth, textHeight);
        painter.fillRect(labelRect, QColor(0, 0, 0, 180));
        painter.setPen(Qt::yellow);
        painter.drawText(labelRect, Qt::AlignCenter, label);

        // Crosshair lines at the cursor position, clipped to the plot area.
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(plotRect.left(), m_mousePos.y(), plotRect.right(), m_mousePos.y());
        painter.drawLine(m_mousePos.x(), plotRect.top(), m_mousePos.x(), plotRect.bottom());
    }
}
