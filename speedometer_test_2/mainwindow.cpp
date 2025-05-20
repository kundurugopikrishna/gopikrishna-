#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcgaugewidget.h"
#include <QPainterPath>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the gauge widget
    mSpeedGauge = new QcGaugeWidget;

    // Background base
    // mSpeedGauge->addBackground(99);

    // // Outer background ring (white ring inside black)
    // QcBackgroundItem *bkg1 = mSpeedGauge->addBackground(92);
    // bkg1->clearrColors();
    // bkg1->addColor(0.1, Qt::black);
    // bkg1->addColor(1.0, Qt::white);

    // // Inner background ring (gray gradient)
    // QcBackgroundItem *bkg2 = mSpeedGauge->addBackground(88);
    // bkg2->clearrColors();
    // bkg2->addColor(0.1, Qt::gray);
    // bkg2->addColor(1.0, Qt::darkGray);

    // // Outer gauge arc
    // mSpeedGauge->addArc(55);

    // // Degree marks from 0 to 80
    // mSpeedGauge->addDegrees(65)->setValueRange(0, 80);

    // // Optional color band arc
    // mSpeedGauge->addColorBand(50)->setValueRange(0, 80);

    // // Digital value display on gauge
    // mSpeedGauge->addValues(80)->setValueRange(0, 80);

    // // Unit label (e.g., Km/h)
    // mSpeedGauge->addLabel(70)->setText("Km/h");

    // // Digital number label (speed)
    // QcLabelItem *lab = mSpeedGauge->addLabel(40);
    // lab->setText("0");

    // // Needle to show speed
    // mSpeedNeedle = mSpeedGauge->addNeedle(60);
    // mSpeedNeedle->setLabel(lab);              // Link label to needle
    // mSpeedNeedle->setColor(Qt::white);        // Needle color
    // mSpeedNeedle->setValueRange(0, 80);       // Needle range

    // // Optional final backgrounds and effects
    // mSpeedGauge->addBackground(7);            // Inner detail
    // mSpeedGauge->addGlass(88);                // Glass overlay

    // Add to layout in your UI


 //    QcGaugeWidget *mSpeedGauge = new QcGaugeWidget(this);
 //    mSpeedGauge->addBackground(99);
 //    mSpeedGauge->addArc(55);
 //    mSpeedGauge->addDegrees(65);
 //    mSpeedGauge->addColorBand(50);
 //    mSpeedGauge->addValues(70);
 //    QcNeedleItem *needle = mSpeedGauge->addNeedle(80);
 //    needle->setColor(Qt::white);
 //    needle->setCurrentValue(0); // Set your initial value here
 //    mSpeedGauge->addBackground(7);

 //   // QcColorBand *band = mSpeedGauge->addColorBand(50);
 // //   band->setColors(1.0, Qt::red); // 1.0 means full coverage in band, or use 0.0 to 1.0 as gradient positions
 //    //QList<QPair<QColor, float>> colorList;
 //   // colorList.append(QPair<QColor, float>(Qt::red, 1.0));
 // //   band->setColors(colorList);  // ✅ Correct

 // //   band->setValueRange(80, 100); // Highlight 80–100 range in red

 //    ui->verticalLayout_4->addWidget(mSpeedGauge);

 //    QLineEdit *speedInput = new QLineEdit(this);
 //    speedInput->setPlaceholderText("Enter speed (0–100)");

 //    QPushButton *updateButton = new QPushButton("Update Speed", this);

 //    // Add to layout
 //    ui->verticalLayout_4->addWidget(speedInput);
 //    ui->verticalLayout_4->addWidget(updateButton);

 //    // === Connect Button Click to Needle Update ===
 //    connect(updateButton, &QPushButton::clicked, this, [=]() {
 //        bool ok;
 //        double value = speedInput->text().toDouble(&ok);
 //        if (ok && value >= 0 && value <= 100) {
 //            needle->setCurrentValue(value);
 //        } else {
 //            QMessageBox::warning(this, "Invalid Input", "Please enter a value between 0 and 100.");
 //        }
 //    });
    QcGaugeWidget *mSpeedGauge = new QcGaugeWidget(this);
  //  mSpeedGauge->addBackground(100);//add a background layer to the gauge as radius 100
    mSpeedGauge->addArc(120);//Add aaDecorative arc at radius 105
    mSpeedGauge->addDegrees(70);//add degreee like speedometer scale at radius 70
  //  mSpeedGauge->addColorBand(70);//placeholder for a colored band at radius 70
    mSpeedGauge->addValues(150);//add value label around the gauge at raddius
    QcNeedleItem *needle = mSpeedGauge->addNeedle(85);
     needle->setColor(Qt::white);
     needle->setCurrentValue(0);
   // mSpeedGauge->addBackground(7);//anthere background layer
    // QcColorBand *band = mSpeedGauge->addColorBand(50);
    // band->setColors(1.0, Qt::red); // Invalid usage – old, incorrect attempt
    // QList<QPair<QColor, float>> colorList;
    // colorList.append(QPair<QColor, float>(Qt::red, 1.0));
    // band->setColors(colorList);  // ✅ Correct
    // band->setValueRange(80, 100); // Highlight 80–100 range in red
      ui->verticalLayout_4->addWidget(mSpeedGauge);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // Update the needle's value when the slider is moved
    mSpeedNeedle->setCurrentValue(value);
}
