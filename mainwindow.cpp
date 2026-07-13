// mainwindow.cpp
// Main window implementation — wires together the spectrum display,
// waterfall display, file loading, and UI controls.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spectrumwidget.h"
#include <QSpinBox>
#include <waterfalldata.h>
#include<udpconnect.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Initializes all widgets defined in Qt Designer (.ui file).
    // Without this, all ui->* pointers (buttons, layouts, spinbox) are null
    // and any access to them will crash immediately.
    ui->setupUi(this);
    // Allocate the spectrum widget on the heap with 'this' as parent.
    // Using 'this' as parent means Qt automatically deletes it when
    // MainWindow is destroyed — no manual delete needed.
    // Stored as a member so other methods (on_pushButton_clicked, etc.)
    // can access it without passing it around.
    spectrum = new SpectrumWidget(this);

    // Allocate the waterfall display widget, also parented to MainWindow.
    // WaterfallData receives spectrum data via signal and renders it as
    // a scrolling time-vs-frequency color map.
    waterfalldata = new WaterfallData(this);

    // Add the spectrum widget into the first vertical layout (top panel).
    // Without this line the widget is allocated but invisible — Qt requires
    // widgets to be placed in a layout or explicitly shown/positioned.
    ui->verticalLayout->addWidget(spectrum);

    // Add the waterfall into the second vertical layout (bottom panel).
    // verticalLayout_2 was defined in Qt Designer; this call inserts
    // waterfalldata as a child of that layout region.
    // (The commented-out verticalLayoutWidget_2 line was a wrong layout target.)
    ui->verticalLayout_2->addWidget(waterfalldata);
    udpconnect=new UDPconnect(this);

    // Read FFT power values from a text file on disk and push them
    // into the spectrum widget. This populates freqMHz and powerDbm
    // so the widget has data to draw on first paint.
    // Without this call the spectrum widget starts empty.
  //  loadFFTFile();

    // Set the valid input range for the threshold spinbox to −200…+200 dBm.
    // Without a range, the default QDoubleSpinBox range is 0–99, so negative
    // threshold values (e.g. −80 dBm) would be silently clamped to 0.
    ui->doubleSpinBox->setRange(-200, 200);

    // When the user finishes editing the spinbox and presses Enter (or loses
    // focus), call onSpinBoxEditingFinished() to read the new value.
    // editingFinished fires once on commit rather than on every keystroke,
    // so the threshold only updates when the user is done typing.
    // Without this, typing in the spinbox has no effect on the display.
    connect(ui->doubleSpinBox, &QSpinBox::editingFinished,
            this, &MainWindow::onSpinBoxEditingFinished);

    // Forward the spinchanged(int) signal emitted by MainWindow to
    // SpectrumWidget::setThreshold(int), which updates m_threshold and
    // redraws the red dashed threshold line.
    // Without this, spinchanged is emitted but nobody listens, so the
    // threshold line never moves.
     connect(this, &MainWindow::spinchanged,
             spectrum, &SpectrumWidget::setThreshold);

    // When the "Set Marker" push button is clicked, call the slot that
    // places the vertical frequency marker on the spectrum.
    // Without this, clicking the button does nothing at all.
     connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_clicked);

    // Every time the spectrum widget finishes computing a frame it emits
    // spectrumDataReady(powerDbm, startFreq, stopFreq).
    // This connection feeds that data directly into the waterfall widget
    // so it can append a new row to its scrolling display.
    // Without this, the waterfall receives no data and stays blank even
    // though the spectrum is drawing correctly.
   //  connect(spectrum,  &SpectrumWidget::spectrumDataReady,
           // waterfalldata, &WaterfallData::setSpectrum);
     connect(udpconnect,
             &UDPconnect::spectrumDataReady,
             spectrum,
             &SpectrumWidget::setSpectrumData);
     connect(udpconnect,&UDPconnect::waterfallData,waterfalldata,&WaterfallData::addSpectrum);

    /* connect(ui->ZoomIn, &QPushButton::clicked,
             spectrum, &SpectrumWidget::zoomIn);

     connect(ui->Zoomout, &QPushButton::clicked,
             spectrum, &SpectrumWidget::zoomOut);*/
}




MainWindow::~MainWindow()
{
    // Deletes the Ui object created by setupUi.
    // Child widgets (spectrum, waterfalldata) are deleted automatically
    // by Qt's parent-child ownership tree, so we only need to delete ui here.
    delete ui;
}

// void MainWindow::loadFFTFile()
// {
//     QVector<float> powerData;
//     QVector<float> freqData;
//     QFile powerFile("C:/Users/gopik/OneDrive/Desktop/python_code/pwerdata.txt");
//     QFile freqFile("C:/Users/gopik/OneDrive/Desktop/python_code/freqMHz.txt");

//     // Open power file
//     if (!powerFile.open(QIODevice::ReadOnly | QIODevice::Text))
//     {
//         qDebug() << "Cannot open powerData.txt";
//         return;
//     }

//     // Open frequency file
//     if (!freqFile.open(QIODevice::ReadOnly | QIODevice::Text))
//     {
//         qDebug() << "Cannot open freqMHz.txt";
//         powerFile.close();
//         return;
//     }

//     QTextStream powerStream(&powerFile);
//     QTextStream freqStream(&freqFile);

//     // Read power values
//     while (!powerStream.atEnd())
//     {
//         bool ok;
//         float value = powerStream.readLine().trimmed().toFloat(&ok);

//         if (ok)
//             powerData.append(value);
//     }

//     // Read frequency values
//     while (!freqStream.atEnd())
//     {
//         bool ok;
//         float value = freqStream.readLine().trimmed().toFloat(&ok);

//         if (ok)
//             freqData.append(value);
//     }

//     powerFile.close();
//     freqFile.close();

//     // Make sure both files contain the same number of values
//     if (powerData.size() != freqData.size())
//     {
//         qDebug() << "Error: Number of power values ("
//                  << powerData.size()
//                  << ") does not match number of frequency values ("
//                  << freqData.size() << ")";
//         return;
//     }

//     // Pass both vectors to the spectrum widget
//     spectrum->setSpectrumData(freqData, powerData);
// }

void MainWindow::onSpinBoxEditingFinished()
{
    // Read the current integer value from the spinbox.
    // QSpinBox::value() returns int, so any decimal the user types
    // is already rounded by the widget before we see it.
    int value = ui->doubleSpinBox->value();

    // Log the value so it can be verified in Qt Creator's Application Output
    // pane during development. Remove or guard with #ifdef QT_DEBUG in release.
    qDebug() << "Entered Value:" << value;

    // Emit spinchanged to carry the new threshold value to SpectrumWidget.
    // Using a signal here (rather than calling spectrum->setThreshold directly)
    // keeps MainWindow loosely coupled — other widgets could also listen to
    // spinchanged without changing this function.
    emit spinchanged(value);
}

void MainWindow::on_pushButton_clicked()
{
    // Place the vertical marker line at −16.80 MHz on the spectrum.
    // setVerticalMarker stores the frequency, sets m_showVerticalMarker = true,
    // and calls update() so the marker appears on the next paint event.
    // Without this call, clicking the button does nothing visible.
    // In a real application the frequency would come from user input
    // (e.g. a QLineEdit) rather than being hardcoded here.
    spectrum->setVerticalMarker(-132);


}

void MainWindow::on_axis_clicked()
{
    bool enable = spectrum->isTooltipEnabled();

    qDebug()<<"on_axis_clicked"<<enable;
    spectrum->setTooltipEnabled(enable);

    qDebug() << "Tooltip Enabled:" << !enable;
}
