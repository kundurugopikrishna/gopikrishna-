/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QComboBox *BaudRate;
    QTextEdit *Received_data;
    QPushButton *Connect;
    QPushButton *Disconnect;
    QPushButton *Clear;
    QComboBox *COM;
    QLineEdit *Send_data;
    QPushButton *Send;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(473, 350);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    border: 4px solid red;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        BaudRate = new QComboBox(centralwidget);
        BaudRate->setObjectName("BaudRate");
        BaudRate->setGeometry(QRect(310, 0, 151, 28));
        Received_data = new QTextEdit(centralwidget);
        Received_data->setObjectName("Received_data");
        Received_data->setGeometry(QRect(10, 0, 301, 241));
        Connect = new QPushButton(centralwidget);
        Connect->setObjectName("Connect");
        Connect->setGeometry(QRect(310, 60, 151, 29));
        Disconnect = new QPushButton(centralwidget);
        Disconnect->setObjectName("Disconnect");
        Disconnect->setGeometry(QRect(310, 90, 151, 29));
        Clear = new QPushButton(centralwidget);
        Clear->setObjectName("Clear");
        Clear->setGeometry(QRect(310, 120, 151, 29));
        COM = new QComboBox(centralwidget);
        COM->setObjectName("COM");
        COM->setGeometry(QRect(310, 30, 151, 28));
        Send_data = new QLineEdit(centralwidget);
        Send_data->setObjectName("Send_data");
        Send_data->setGeometry(QRect(10, 250, 301, 21));
        Send = new QPushButton(centralwidget);
        Send->setObjectName("Send");
        Send->setGeometry(QRect(320, 250, 71, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 473, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Connect->setText(QCoreApplication::translate("MainWindow", "connect", nullptr));
        Disconnect->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        Clear->setText(QCoreApplication::translate("MainWindow", "clear", nullptr));
        Send->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
