/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *leftPanel;
    QPushButton *dashButton;
    QPushButton *batterySaverButton;
    QPushButton *optimizerButton;
    QPushButton *settingsButton;
    QLabel *mainTitle;
    QGroupBox *cardCPU;
    QProgressBar *progCPU;
    QProgressBar *progRAM;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *cardBattery;
    QProgressBar *progBattery;
    QLabel *chargingStatus;
    QGroupBox *CardActions;
    QPushButton *cleanButton;
    QPushButton *genReport;
    QPushButton *btnMiniMode;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1302, 678);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow,\n"
"QWidget#centralwidget {\n"
"    background-color: #080f1f;\n"
"}\n"
"\n"
"/* =========================================================\n"
"LEFT SIDEBAR\n"
"========================================================= */\n"
"\n"
"QFrame#leftPanel {\n"
"    background-color: #111c2f;\n"
"    border-right: 2px solid #1e3a5f;\n"
"}\n"
"\n"
"/* =========================================================\n"
"SIDEBAR BUTTONS\n"
"========================================================= */\n"
"\n"
"QPushButton#dashButton,\n"
"QPushButton#batterySaverButton,\n"
"QPushButton#optimizerButton,\n"
"QPushButton#settingsButton {\n"
"    background-color: #0b1426;\n"
"    color: #94a3b8;\n"
"    border: 1px solid #263b59;\n"
"    border-left: 4px solid #0284c7;\n"
"    padding: 12px 16px;\n"
"    text-align: left;\n"
"    font-size: 13px;\n"
"    font-weight: bold;\n"
"    border-radius: 7px;\n"
"    margin-bottom: 6px;\n"
"}\n"
"\n"
"QPushButton#dashButton:hover,\n"
"QPushButton#batterySaverButton:hover,\n"
"QPus"
                        "hButton#optimizerButton:hover,\n"
"QPushButton#settingsButton:hover {\n"
"    background-color: #17263d;\n"
"    color: #38bdf8;\n"
"    border: 1px solid #38bdf8;\n"
"    border-left: 5px solid #00e5ff;\n"
"}\n"
"\n"
"/* =========================================================\n"
"MAIN TITLE\n"
"========================================================= */\n"
"\n"
"QLabel#mainTitle {\n"
"    color: #f8fafc;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* =========================================================\n"
"GROUP BOX\n"
"========================================================= */\n"
"\n"
"QGroupBox {\n"
"    background-color: #152238;\n"
"    color: #38bdf8;\n"
"    border: 1px solid #29425f;\n"
"    border-radius: 14px;\n"
"    font-weight: bold;\n"
"    font-size: 11px;\n"
"    margin-top: 20px;\n"
"    padding-top: 15px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    padding: 0 8px;\n"
"    left: 12px;\n"
"}"
                        "\n"
"\n"
"/* =========================================================\n"
"LABELS\n"
"========================================================= */\n"
"\n"
"QLabel#chargingStatus,\n"
"QLabel#label_2,\n"
"QLabel#label_3 {\n"
"    color: #f8fafc;\n"
"    font-size: 13px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QGroupBox QLabel {\n"
"    color: #cbd5e1;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* =========================================================\n"
"PROGRESS BAR\n"
"========================================================= */\n"
"\n"
"QProgressBar {\n"
"    background-color: #0a1324;\n"
"    border: 1px solid #304762;\n"
"    border-radius: 7px;\n"
"    text-align: center;\n"
"    color: #ffffff;\n"
"    font-size: 11px;\n"
"    font-weight: bold;\n"
"    min-height: 18px;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #06b6d4;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"/* =========================================================\n"
"QUICK ACTION BUTTONS\n"
"==============================="
                        "========================== */\n"
"\n"
"QPushButton#cleanButton,\n"
"QPushButton#genReport {\n"
"    background-color: #0284c7;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 7px;\n"
"    padding: 10px;\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton#cleanButton:hover,\n"
"QPushButton#genReport:hover {\n"
"    background-color: #0369a1;\n"
"}\n"
"\n"
"/* =========================================================\n"
"MINI MODE BUTTON\n"
"========================================================= */\n"
"\n"
"QPushButton#btnMiniMode {\n"
"    background-color: #0b1b30;\n"
"    color: #38bdf8;\n"
"    border: 1px solid #0284c7;\n"
"    border-radius: 8px;\n"
"    padding: 9px 18px;\n"
"    font-size: 12px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton#btnMiniMode:hover {\n"
"    background-color: #0284c7;\n"
"    color: #ffffff;\n"
"    border: 1px solid #38bdf8;\n"
"}\n"
"\n"
"QPushButton#btnMiniMode:pressed {\n"
"    background-color: #0369a1;\n"
"    col"
                        "or: #ffffff;\n"
"}\n"
"\n"
"/* =========================================================\n"
"CPU CARD\n"
"========================================================= */\n"
"\n"
"QGroupBox#cardCPU {\n"
"    background-color: #152238;\n"
"    border: 1px solid #155e75;\n"
"    border-radius: 14px;\n"
"}\n"
"\n"
"QGroupBox#cardCPU::title {\n"
"    color: #38bdf8;\n"
"}\n"
"\n"
"/* =========================================================\n"
"BATTERY CARD\n"
"========================================================= */\n"
"\n"
"QGroupBox#cardBattery {\n"
"    background-color: #152238;\n"
"    border: 1px solid #29425f;\n"
"    border-radius: 14px;\n"
"}\n"
"\n"
"/* =========================================================\n"
"ACTION CARD\n"
"========================================================= */\n"
"\n"
"QGroupBox#CardActions {\n"
"    background-color: #152238;\n"
"    border: 1px solid #29425f;\n"
"    border-radius: 14px;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        leftPanel = new QFrame(centralwidget);
        leftPanel->setObjectName("leftPanel");
        leftPanel->setGeometry(QRect(0, 90, 151, 391));
        leftPanel->setFrameShape(QFrame::Shape::StyledPanel);
        leftPanel->setFrameShadow(QFrame::Shadow::Raised);
        dashButton = new QPushButton(leftPanel);
        dashButton->setObjectName("dashButton");
        dashButton->setGeometry(QRect(10, 30, 111, 51));
        batterySaverButton = new QPushButton(leftPanel);
        batterySaverButton->setObjectName("batterySaverButton");
        batterySaverButton->setGeometry(QRect(10, 100, 121, 51));
        optimizerButton = new QPushButton(leftPanel);
        optimizerButton->setObjectName("optimizerButton");
        optimizerButton->setGeometry(QRect(10, 180, 101, 51));
        settingsButton = new QPushButton(leftPanel);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setGeometry(QRect(10, 260, 101, 51));
        mainTitle = new QLabel(centralwidget);
        mainTitle->setObjectName("mainTitle");
        mainTitle->setGeometry(QRect(470, 60, 411, 51));
        cardCPU = new QGroupBox(centralwidget);
        cardCPU->setObjectName("cardCPU");
        cardCPU->setGeometry(QRect(210, 160, 251, 191));
        progCPU = new QProgressBar(cardCPU);
        progCPU->setObjectName("progCPU");
        progCPU->setGeometry(QRect(20, 40, 181, 31));
        progCPU->setValue(24);
        progRAM = new QProgressBar(cardCPU);
        progRAM->setObjectName("progRAM");
        progRAM->setGeometry(QRect(20, 110, 181, 31));
        progRAM->setValue(24);
        label_2 = new QLabel(cardCPU);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 80, 63, 20));
        label_3 = new QLabel(cardCPU);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(60, 150, 81, 21));
        cardBattery = new QGroupBox(centralwidget);
        cardBattery->setObjectName("cardBattery");
        cardBattery->setGeometry(QRect(520, 160, 261, 191));
        progBattery = new QProgressBar(cardBattery);
        progBattery->setObjectName("progBattery");
        progBattery->setGeometry(QRect(30, 40, 211, 51));
        progBattery->setValue(24);
        chargingStatus = new QLabel(cardBattery);
        chargingStatus->setObjectName("chargingStatus");
        chargingStatus->setGeometry(QRect(20, 110, 201, 41));
        CardActions = new QGroupBox(centralwidget);
        CardActions->setObjectName("CardActions");
        CardActions->setGeometry(QRect(830, 159, 271, 191));
        cleanButton = new QPushButton(CardActions);
        cleanButton->setObjectName("cleanButton");
        cleanButton->setGeometry(QRect(52, 40, 161, 51));
        genReport = new QPushButton(CardActions);
        genReport->setObjectName("genReport");
        genReport->setGeometry(QRect(60, 110, 151, 51));
        btnMiniMode = new QPushButton(centralwidget);
        btnMiniMode->setObjectName("btnMiniMode");
        btnMiniMode->setGeometry(QRect(280, 370, 111, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1302, 26));
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
        dashButton->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        batterySaverButton->setText(QCoreApplication::translate("MainWindow", "Uptime Checker", nullptr));
        optimizerButton->setText(QCoreApplication::translate("MainWindow", "Speed And Usage", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        mainTitle->setText(QCoreApplication::translate("MainWindow", "                       System Health Dashboard", nullptr));
        cardCPU->setTitle(QCoreApplication::translate("MainWindow", "CPU AND RAM STATUS", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "CPU:  0%", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "  RAM : 0%", nullptr));
        cardBattery->setTitle(QCoreApplication::translate("MainWindow", "BATTERY STATUS", nullptr));
        chargingStatus->setText(QCoreApplication::translate("MainWindow", "Charging status: (plugged in)", nullptr));
        CardActions->setTitle(QCoreApplication::translate("MainWindow", "QUICK ACTIONS", nullptr));
        cleanButton->setText(QCoreApplication::translate("MainWindow", "Clean Temp Files", nullptr));
        genReport->setText(QCoreApplication::translate("MainWindow", "Generate Report", nullptr));
        btnMiniMode->setText(QCoreApplication::translate("MainWindow", "Mini-Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
