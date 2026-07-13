#include "waterfallwidget.h"
#include "headers.h"

WaterfallWidget::WaterfallWidget(QWidget *parent)
    : QWidget(parent), maxHistory(100), yOffset(0), rowHeight(0), animStep(1)
{
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &WaterfallWidget::updateAnimation);
    animTimer->start(30);
}

WaterfallWidget::~WaterfallWidget()
{
    animTimer->stop();
}

void WaterfallWidget::onFallTimer()
{
    update();   // trigger repaint
}

void WaterfallWidget::addSpectrum(const QVector<double> &freqMHz,
                                  const QVector<double> &powerdBm)
{
    if (powerdBm.isEmpty())
        return;

    QVector<int> row(powerdBm.size());

    // Convert dBm -> 0-255 color index
    for (int i = 0; i < powerdBm.size(); ++i)
    {
        double dB = powerdBm[i];

        double norm = (dB + 120.0) / 100.0;  // -120 -> 0 , -20 -> 1
        norm = qBound(0.0, norm, 1.0);

        row[i] = int(norm * 255);
    }

    spectrumHistory.prepend(row);

    if (spectrumHistory.size() > maxHistory)
        spectrumHistory.removeLast();

    timeHistory.prepend(
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    if (timeHistory.size() > maxHistory)
        timeHistory.removeLast();

    qDebug() << "Waterfall received spectrum of size:" << powerdBm.size();

    update();
}

void WaterfallWidget::updateAnimation()
{
    yOffset += animStep;

    if (yOffset >= rowHeight && rowHeight > 0)
    {
        yOffset = 0;
    }

    update();
}

void WaterfallWidget::clearHistory()
{
    spectrumHistory.clear();
    timeHistory.clear();     // keep both arrays in sync
    peakFallY.clear();
    update();
}

void WaterfallWidget::setPeakFrequencies(const QVector<double> &peaks)
{
    peakFrequencies = peaks;
    update(); // repaint to show the new lines
}

void WaterfallWidget::slotupdatepeakData(QVector<double> freqData,
                                         QVector<double> powerData,double startFreqMHz1,  double stopFreqMHz1)
{
    peakFreq.clear();
    peakAmp.clear();

    peakFreq.append(freqData);
    peakAmp.append(powerData);

    startFreqMHz = startFreqMHz1;
    stopFreqMHz = stopFreqMHz1;

    update();
}

void WaterfallWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#1765f5"));
    painter.setRenderHint(QPainter::Antialiasing, false);

    int w = width();
    int h = height();

    int colorBarWidth = 10;          // width of rainbow bar
    int labelWidth = 30;             // width for dB labels

    int timestampHeight = 20;        // height of timestamp row
    int rowGap = 30;                 // empty gap between rows
    rowHeight = timestampHeight + rowGap;

    //  Determine which rows to draw
    int rowsToShow = rowHeight > 0 ? h / rowHeight : 0;
    int startRow = qMax(0, spectrumHistory.size() - rowsToShow);

    QList<QVector<int>> visibleHistory = spectrumHistory.mid(startRow);
    QStringList visibleTimes = timeHistory.mid(startRow);

    const double minYdB = -132.0;
    const double maxYdB = -12.0;

    // --------------------------------------------
    // Plot Rectangle
    // --------------------------------------------
    const int leftMargin = 40;
    const int rightMargin = 10;
    const int topMargin = 20;
    const int bottomMargin = 25;

    int plotXStart = leftMargin;
    int plotWidth = w - leftMargin - rightMargin;   // now matches plotRect's width

    QRectF plotRect(leftMargin, topMargin,
                    w - leftMargin - rightMargin,
                    h - topMargin - bottomMargin);

    /* ===============================
     * Frequency Axis
     * =============================== */
    painter.setPen(Qt::white);                       // was Qt::NoPen -> labels were invisible
    painter.setFont(QFont("Arial", 8, QFont::Bold));

    painter.drawText(plotRect.left()+2, plotRect.topLeft().y(),
                     QString::number(startFreqMHz, 'f', 2));
    painter.drawText(plotRect.right() - 40, plotRect.topRight().y(),
                     QString::number(stopFreqMHz, 'f', 2));

    //  Draw waterfall rows
    int row = 0;
    for (const QVector<int> &spectrum : visibleHistory)
    {
        if (spectrum.isEmpty() || spectrum.size() < 2)
            continue;

        int xStep = plotWidth / spectrum.size();

        for (int i = 0; i < spectrum.size(); ++i)
        {
            int x = plotXStart + i * xStep;

            float norm = spectrum[i] / 255.0f;
            norm = qBound(0.0f, norm, 1.0f);

            QColor color;
            color.setHsvF((1.0 - norm) * 0.7, 0.8, 1.0); // rainbow scale

            painter.fillRect(x, row * rowHeight + yOffset, xStep, rowHeight, color); // was commented out
        }

        //  Draw timestamp on left side
        if (row < visibleTimes.size())
        {
            int textX = colorBarWidth + labelWidth + 5;
            int textY = row * rowHeight + yOffset + (timestampHeight / 2) + 4;

            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 8, QFont::Bold));

            painter.drawText(textX, textY, visibleTimes[row]);
        }
        row++;
    }

    //  Rainbow color scale on LEFT
    QRect colorRect(labelWidth, 2, colorBarWidth, h - 5);

    QLinearGradient gradient(colorRect.topLeft(), colorRect.bottomLeft());
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(0.2, Qt::yellow);
    gradient.setColorAt(0.4, Qt::green);
    gradient.setColorAt(0.6, Qt::cyan);
    gradient.setColorAt(0.8, Qt::blue);
    gradient.setColorAt(1.0, Qt::darkBlue);

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(colorRect);

    //  Dynamic dB labels
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numLabels = std::max(13, h / 40);
    int stepValue = 10;
    int maxValue = 0;

    for (int i = 0; i < numLabels; ++i) {
        int value = maxValue - i * stepValue;
        int y = colorRect.top() + i * (colorRect.height() / (numLabels - 1));
        painter.drawText(labelWidth - 28, y + 5, QString::number(value));
    }

    // Border around color bar
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(colorRect);

    // ------------------------------
    // Peak fall-down markers
    // ------------------------------
    const double barWidth = 0.4;
    const double fallSpeed = 1.5;   // pixels per frame (tune this)

    if (peakFallY.size() != peakFreq.size())
    {
        peakFallY.resize(peakFreq.size());
        for (int i = 0; i < peakFallY.size(); ++i)
            peakFallY[i] = plotRect.top();   // start from top
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);

    for (int i = 0; i < peakFreq.size(); ++i)
    {
        // ---- X position ----
        double xCenter = plotRect.left() + (peakFreq[i] - startFreqMHz) /
                                               (stopFreqMHz - startFreqMHz) * plotRect.width();

        // ---- Target Y (final peak) ----
        double yPeak = plotRect.top() + (maxYdB - peakAmp[i]) /
                                            (maxYdB - minYdB) * plotRect.height();

        xCenter = qBound((double)plotRect.left(), xCenter, (double)plotRect.right());
        yPeak   = qBound((double)plotRect.top(), yPeak, (double)plotRect.bottom());

        // ---- Reset when new higher peak arrives ----
        if (yPeak < peakFallY[i])
        {
            peakFallY[i] = plotRect.top();
        }

        // ---- Slow fall animation ----
        if (peakFallY[i] < yPeak)
        {
            peakFallY[i] += fallSpeed;
        }

        peakFallY[i] = qMin(peakFallY[i], yPeak);

        // ---- Draw vertical peak line ----
        QRectF barRect(xCenter - barWidth * 0.5,
                       plotRect.top(), barWidth,
                       peakFallY[i] - plotRect.top());

        painter.drawRect(barRect);
    }
}

void WaterfallWidget::mouseMoveEvent(QMouseEvent *event)
{
    mousePos = event->pos();
    mouseInside = true;

    if (spectrumHistory.isEmpty() || timeHistory.isEmpty() || rowHeight <= 0)
        return;

    const int leftMargin = 40;
    const int rightMargin = 10;
    const int topMargin = 20;
    const int bottomMargin = 25;

    QRectF plotRect(leftMargin, topMargin,
                    width() - leftMargin - rightMargin,
                    height() - topMargin - bottomMargin);

    if (!plotRect.contains(mousePos))
        return;

    //  Determine row index
    int rowIndex = static_cast<int>((mousePos.y() - plotRect.top()) / rowHeight);
    rowIndex = qBound(0, rowIndex, timeHistory.size() - 1);

    emit waterfallTimestamp(timeHistory[rowIndex]);
}

void WaterfallWidget::leaveEvent(QEvent *)
{
    mouseInside = false;
}
