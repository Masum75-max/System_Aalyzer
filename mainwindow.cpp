#include "mainwindow.h"
#include "ui_mainwindow.h"

// Windows System Headers (Winsock2 must be included before Windows.h)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#include <netioapi.h>

// Qt Standard Headers
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSlider>
#include <QMouseEvent>
#include <QCheckBox>
#include <QSettings>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnMiniMode, &QPushButton::clicked, this, &MainWindow::toggleMiniMode);

    // Initialize CPU times
    FILETIME ftime, fsys, fuser;
    GetSystemTimes(&ftime, &fsys, &fuser);
    prevSysIdle = ftime;
    prevSysKernel = fsys;
    prevSysUser = fuser;

    // Break timer setup
    breakTimer = new QTimer(this);
    connect(breakTimer, &QTimer::timeout, this, &MainWindow::showBreakPopup);

    // 20 mins (1200000 ms) interval
    breakTimer->start(20 * 60 * 1000);

    // QTimer Setup for 1-Second Loop
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateSystemMetrics);
    updateTimer->start(1000);

    updateSystemMetrics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

ULONGLONG MainWindow::subtractTimes(const FILETIME& ftA, const FILETIME& ftB) {
    ULARGE_INTEGER a, b;
    a.LowPart = ftA.dwLowDateTime;
    a.HighPart = ftA.dwHighDateTime;
    b.LowPart = ftB.dwLowDateTime;
    b.HighPart = ftB.dwHighDateTime;
    return a.QuadPart - b.QuadPart;
}

double MainWindow::getCpuUsage() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) return 0.0;

    ULONGLONG usr = subtractTimes(userTime, prevSysUser);
    ULONGLONG ker = subtractTimes(kernelTime, prevSysKernel);
    ULONGLONG idl = subtractTimes(idleTime, prevSysIdle);

    ULONGLONG sys = ker + usr;

    prevSysUser = userTime;
    prevSysKernel = kernelTime;
    prevSysIdle = idleTime;

    if (sys == 0) return 0.0;
    return (double)(sys - idl) * 100.0 / sys;
}

void MainWindow::checkBatteryAlerts(int percentage, bool isCharging) {
    if (isCharging && percentage >= 100) {
        Beep(1000, 400);
    } else if (!isCharging && percentage <= 20) {
        Beep(500, 300);
    }
}

void MainWindow::updateSystemMetrics() {
    // 1. RAM Usage
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    int ramPercent = memInfo.dwMemoryLoad;

    if (ui->progRAM) {
        ui->progRAM->setValue(ramPercent);
    }
    ui->label_3->setText(QString("RAM: %1%").arg(ramPercent));

    // 2. CPU Usage
    int cpuPercent = static_cast<int>(getCpuUsage());
    ui->progCPU->setValue(cpuPercent);
    ui->label_2->setText(QString("CPU: %1%").arg(cpuPercent));

    // 3. Battery Status
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        int batPercent = status.BatteryLifePercent;
        bool isCharging = (status.ACLineStatus == 1);

        QProgressBar *batBar = findChild<QProgressBar*>("pro_r_3");
        if (!batBar) batBar = findChild<QProgressBar*>("progBattery");

        if (batBar) {
            batBar->setValue(batPercent);
        }

        ui->chargingStatus->setText(QString("%1% (%2)")
                                        .arg(batPercent)
                                        .arg(isCharging ? "Charging / Plugged In" : "Discharging"));

        checkBatteryAlerts(batPercent, isCharging);
    }
}

void MainWindow::on_cleanButton_clicked() {
    QString tempPath = QDir::tempPath();
    QDir tempDir(tempPath);
    tempDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    int deletedCount = 0;
    for (const QString &fileName : tempDir.entryList()) {
        if (tempDir.remove(fileName)) {
            deletedCount++;
        }
    }
    QMessageBox::information(this, "Optimizer", QString("Successfully cleaned %1 temporary files!").arg(deletedCount));
}

void MainWindow::on_genReport_clicked() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save System Health Report",
        "System_Report.txt",
        "Text Files (*.txt);;All Files (*)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss AP");

        out << "========================================\n";
        out << "       SYSTEM HEALTH REPORT             \n";
        out << "========================================\n";
        out << "Generated On   : " << currentTime << "\n";
        out << "----------------------------------------\n";
        out << "CPU Usage      : " << ui->label_2->text() << "\n";
        out << "RAM Usage      : " << ui->label_3->text() << "\n";
        out << "Battery Status : " << ui->chargingStatus->text() << "\n";
        out << "========================================\n";

        file.close();

        QMessageBox::information(this, "Success", "System report saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not save the file at the specified location.");
    }
}

// 1. Mini Mode Toggle Logic
void MainWindow::toggleMiniMode()
{
    if (!isMiniMode) {
        // Hide all other widgets
        if (ui->CardActions) ui->CardActions->hide();
        if (ui->cardBattery) ui->cardBattery->hide();
        if (ui->dashButton) ui->dashButton->hide();
        if (ui->batterySaverButton) ui->batterySaverButton->hide();
        if (ui->optimizerButton) ui->optimizerButton->hide();
        if (ui->settingsButton) ui->settingsButton->hide();
        if (ui->leftPanel) ui->leftPanel->hide();
        if (ui->mainTitle) ui->mainTitle->hide();

        // Frameless + Always on top
        this->setWindowFlags(
            Qt::Window |
            Qt::FramelessWindowHint |
            Qt::WindowStaysOnTopHint
            );

        // Mini window size
        this->resize(350, 250);

        // Show only CPU card and Mini Mode button
        ui->cardCPU->show();
        ui->btnMiniMode->show();

        // CPU card position
        ui->cardCPU->move(
            (this->width() - ui->cardCPU->width()) / 2,
            30
            );

        // Button below CPU card
        ui->btnMiniMode->move(
            (this->width() - ui->btnMiniMode->width()) / 2,
            ui->cardCPU->y() + ui->cardCPU->height() + 20
            );

        this->show();
        isMiniMode = true;

    } else {
        // Hide CPU card and mini button
        ui->cardCPU->hide();
        ui->btnMiniMode->hide();

        // Bring back normal window frame
        this->setWindowFlags(Qt::Window);
        this->resize(900, 550);

        // Show notification
        ui->mainTitle->setText("You need to restart the program again");
        ui->mainTitle->setAlignment(Qt::AlignCenter);
        ui->mainTitle->show();

        // Center title in whole window
        ui->mainTitle->setGeometry(
            0,
            0,
            this->width(),
            this->height()
            );

        this->show();
        isMiniMode = false;
    }
}

// 2. Mouse Press for Dragging Frameless Window
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

// 3. Mouse Move to Drag Window
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::on_batterySaverButton_clicked() {
    hideAllDynamicPages();

    ui->CardActions->hide();
    ui->cardBattery->hide();
    ui->cardCPU->hide();
    ui->btnMiniMode->hide();

    if (!uptimePage) {
        uptimePage = new QWidget(this);
        uptimePage->setGeometry(200, 80, 1050, 500);

        QVBoxLayout *layout = new QVBoxLayout(uptimePage);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("SYSTEM UPTIME TRACKER", uptimePage);
        title->setStyleSheet("color: #00d2ff; font-size: 24px; font-weight: bold; margin-bottom: 20px;");
        title->setAlignment(Qt::AlignCenter);

        lblUptimeDisplay = new QLabel("00 HR : 00 mins : 00 secs", uptimePage);
        lblUptimeDisplay->setStyleSheet(
            "color: #ffffff; "
            "font-size: 38px; "
            "font-weight: bold; "
            "background-color: #121929; "
            "border: 2px solid #00d2ff; "
            "border-radius: 15px; "
            "padding: 25px 40px;"
            );
        lblUptimeDisplay->setAlignment(Qt::AlignCenter);

        btnBack = new QPushButton("Back to Dashboard", uptimePage);
        btnBack->setCursor(Qt::PointingHandCursor);
        btnBack->setStyleSheet(
            "QPushButton { background-color: #00d2ff; color: #000000; font-weight: bold; font-size: 15px; border-radius: 8px; padding: 10px 20px; margin-top: 30px; }"
            "QPushButton:hover { background-color: #0099cc; color: #ffffff; }"
            );

        layout->addWidget(title);
        layout->addWidget(lblUptimeDisplay);
        layout->addWidget(btnBack);

        connect(btnBack, &QPushButton::clicked, this, [this]() {
            uptimePage->hide();
            if (uptimeTimer) uptimeTimer->stop();

            ui->CardActions->show();
            ui->cardBattery->show();
            ui->cardCPU->show();
            ui->btnMiniMode->show();
        });

        uptimeTimer = new QTimer(this);
        connect(uptimeTimer, &QTimer::timeout, this, &MainWindow::updateUptimeLive);
    }

    uptimePage->show();
    updateUptimeLive();
    uptimeTimer->start(1000);
}

void MainWindow::updateUptimeLive() {
    if (!lblUptimeDisplay) return;

    ULONGLONG ms = GetTickCount64();
    ULONGLONG totalSeconds = ms / 1000;
    ULONGLONG hours = totalSeconds / 3600;
    ULONGLONG minutes = (totalSeconds % 3600) / 60;
    ULONGLONG seconds = totalSeconds % 60;

    QString formattedTime = QString("%1 HR : %2 mins : %3 secs")
                                .arg(hours, 2, 10, QChar('0'))
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0'));

    lblUptimeDisplay->setText(formattedTime);
}

void MainWindow::showBreakPopup() {
    Beep(750, 300);

    if (!breakDialog) {
        breakDialog = new QDialog(this);
        breakDialog->setWindowTitle("Eye Care Break Reminder");
        breakDialog->setFixedSize(400, 220);
        breakDialog->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        breakDialog->setStyleSheet("background-color: #0d1117; color: #ffffff;");

        QVBoxLayout *layout = new QVBoxLayout(breakDialog);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("TIME FOR AN EYE BREAK!", breakDialog);
        title->setStyleSheet("color: #00d2ff; font-size: 18px; font-weight: bold;");
        title->setAlignment(Qt::AlignCenter);

        QLabel *desc = new QLabel("Look at something 20 feet away to relax your eyes.", breakDialog);
        desc->setStyleSheet("color: #aaaaaa; font-size: 13px;");
        desc->setWordWrap(true);
        desc->setAlignment(Qt::AlignCenter);

        lblBreakCountdown = new QLabel("20 Seconds Remaining", breakDialog);
        lblBreakCountdown->setStyleSheet("color: #ffcc00; font-size: 22px; font-weight: bold; margin: 15px 0px;");
        lblBreakCountdown->setAlignment(Qt::AlignCenter);

        QPushButton *btnSkip = new QPushButton("Skip Break", breakDialog);
        btnSkip->setCursor(Qt::PointingHandCursor);
        btnSkip->setStyleSheet(
            "QPushButton { background-color: #21262d; color: #ffffff; border: 1px solid #30363d; border-radius: 6px; padding: 6px 15px; }"
            "QPushButton:hover { background-color: #30363d; }"
            );

        connect(btnSkip, &QPushButton::clicked, this, [this]() {
            if (breakCountdownTimer) breakCountdownTimer->stop();
            breakDialog->accept();
        });

        layout->addWidget(title);
        layout->addWidget(desc);
        layout->addWidget(lblBreakCountdown);
        layout->addWidget(btnSkip);

        breakCountdownTimer = new QTimer(this);
        connect(breakCountdownTimer, &QTimer::timeout, this, [this]() {
            remainingBreakSeconds--;
            if (remainingBreakSeconds > 0) {
                lblBreakCountdown->setText(QString("%1 Seconds Remaining").arg(remainingBreakSeconds));
            } else {
                breakCountdownTimer->stop();
                breakDialog->accept();
            }
        });
    }

    remainingBreakSeconds = 20;
    lblBreakCountdown->setText("20 Seconds Remaining");
    breakCountdownTimer->start(1000);

    breakDialog->exec();
}

void MainWindow::on_optimizerButton_clicked() {
    hideAllDynamicPages();

    ui->CardActions->hide();
    ui->cardBattery->hide();
    ui->cardCPU->hide();
    ui->btnMiniMode->hide();

    if (!networkPage) {
        networkPage = new QWidget(this);
        networkPage->setGeometry(200, 80, 1050, 500);

        QVBoxLayout *layout = new QVBoxLayout(networkPage);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("NETWORK SPEED & DATA USAGE", networkPage);
        title->setStyleSheet("color: #00d2ff; font-size: 24px; font-weight: bold; margin-bottom: 25px;");
        title->setAlignment(Qt::AlignCenter);

        QHBoxLayout *speedLayout = new QHBoxLayout();

        // Download Card
        QVBoxLayout *dlCard = new QVBoxLayout();
        QLabel *lblDlTitle = new QLabel("DOWNLOAD SPEED", networkPage);
        lblDlTitle->setStyleSheet("color: #888888; font-size: 13px; font-weight: bold;");
        lblDlTitle->setAlignment(Qt::AlignCenter);

        lblDownloadSpeed = new QLabel("0.0 KB/s", networkPage);
        lblDownloadSpeed->setStyleSheet(
            "color: #00ff88; font-size: 28px; font-weight: bold; "
            "background-color: #121929; border: 2px solid #00ff88; "
            "border-radius: 12px; padding: 20px 30px;"
            );
        lblDownloadSpeed->setAlignment(Qt::AlignCenter);
        dlCard->addWidget(lblDlTitle);
        dlCard->addWidget(lblDownloadSpeed);

        // Upload Card
        QVBoxLayout *ulCard = new QVBoxLayout();
        QLabel *lblUlTitle = new QLabel("UPLOAD SPEED", networkPage);
        lblUlTitle->setStyleSheet("color: #888888; font-size: 13px; font-weight: bold;");
        lblUlTitle->setAlignment(Qt::AlignCenter);

        lblUploadSpeed = new QLabel("0.0 KB/s", networkPage);
        lblUploadSpeed->setStyleSheet(
            "color: #00d2ff; font-size: 28px; font-weight: bold; "
            "background-color: #121929; border: 2px solid #00d2ff; "
            "border-radius: 12px; padding: 20px 30px;"
            );
        lblUploadSpeed->setAlignment(Qt::AlignCenter);
        ulCard->addWidget(lblUlTitle);
        ulCard->addWidget(lblUploadSpeed);

        speedLayout->addLayout(dlCard);
        speedLayout->addSpacing(20);
        speedLayout->addLayout(ulCard);

        lblTotalData = new QLabel("Session Data Used: 0.00 MB", networkPage);
        lblTotalData->setStyleSheet(
            "color: #ffcc00; font-size: 20px; font-weight: bold; "
            "background-color: #1a2234; border: 1px solid #ffcc00; "
            "border-radius: 10px; padding: 15px; margin-top: 20px;"
            );
        lblTotalData->setAlignment(Qt::AlignCenter);

        btnNetworkBack = new QPushButton("Back to Dashboard", networkPage);
        btnNetworkBack->setCursor(Qt::PointingHandCursor);
        btnNetworkBack->setStyleSheet(
            "QPushButton { background-color: #00d2ff; color: #000000; font-weight: bold; font-size: 15px; border-radius: 8px; padding: 10px 25px; margin-top: 25px; }"
            "QPushButton:hover { background-color: #0099cc; color: #ffffff; }"
            );

        layout->addWidget(title);
        layout->addLayout(speedLayout);
        layout->addWidget(lblTotalData);
        layout->addWidget(btnNetworkBack);

        connect(btnNetworkBack, &QPushButton::clicked, this, [this]() {
            networkPage->hide();
            if (networkTimer) networkTimer->stop();

            ui->CardActions->show();
            ui->cardBattery->show();
            ui->cardCPU->show();
            ui->btnMiniMode->show();
        });

        networkTimer = new QTimer(this);
        connect(networkTimer, &QTimer::timeout, this, &MainWindow::updateNetworkMetrics);
    }

    networkPage->show();
    isFirstNetworkCheck = true;
    updateNetworkMetrics();
    networkTimer->start(1000);
}

void MainWindow::updateNetworkMetrics() {
    DWORD dwSize = 0;
    if (GetIfTable(NULL, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        MIB_IFTABLE* pIfTable = (MIB_IFTABLE*)malloc(dwSize);
        if (pIfTable != NULL) {
            if (GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR) {
                ULONG64 totalInBytes = 0;
                ULONG64 totalOutBytes = 0;

                for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
                    MIB_IFROW row = pIfTable->table[i];
                    if (row.dwType != 24 && row.dwOperStatus == MIB_IF_OPER_STATUS_OPERATIONAL) {
                        totalInBytes += row.dwInOctets;
                        totalOutBytes += row.dwOutOctets;
                    }
                }

                if (isFirstNetworkCheck) {
                    prevInBytes = totalInBytes;
                    prevOutBytes = totalOutBytes;
                    if (initialTotalBytes == 0) {
                        initialTotalBytes = totalInBytes + totalOutBytes;
                    }
                    isFirstNetworkCheck = false;
                    free(pIfTable);
                    return;
                }

                ULONG64 bytesInDiff = (totalInBytes >= prevInBytes) ? (totalInBytes - prevInBytes) : 0;
                ULONG64 bytesOutDiff = (totalOutBytes >= prevOutBytes) ? (totalOutBytes - prevOutBytes) : 0;

                prevInBytes = totalInBytes;
                prevOutBytes = totalOutBytes;

                double dlSpeedKB = bytesInDiff / 1024.0;
                double ulSpeedKB = bytesOutDiff / 1024.0;

                QString dlStr = (dlSpeedKB >= 1024.0) ?
                                    QString("%1 MB/s").arg(dlSpeedKB / 1024.0, 0, 'f', 2) :
                                    QString("%1 KB/s").arg(dlSpeedKB, 0, 'f', 1);

                QString ulStr = (ulSpeedKB >= 1024.0) ?
                                    QString("%1 MB/s").arg(ulSpeedKB / 1024.0, 0, 'f', 2) :
                                    QString("%1 KB/s").arg(ulSpeedKB, 0, 'f', 1);

                if (lblDownloadSpeed) lblDownloadSpeed->setText(dlStr);
                if (lblUploadSpeed) lblUploadSpeed->setText(ulStr);

                ULONG64 currentTotal = totalInBytes + totalOutBytes;
                ULONG64 sessionUsageBytes = (currentTotal >= initialTotalBytes) ? (currentTotal - initialTotalBytes) : 0;
                double usageMB = sessionUsageBytes / (1024.0 * 1024.0);

                if (lblTotalData) {
                    if (usageMB >= 1024.0) {
                        lblTotalData->setText(QString("Session Data Used: %1 GB").arg(usageMB / 1024.0, 0, 'f', 2));
                    } else {
                        lblTotalData->setText(QString("Session Data Used: %1 MB").arg(usageMB, 0, 'f', 2));
                    }
                }
            }
            free(pIfTable);
        }
    }
}

void MainWindow::on_dashButton_clicked() {
    hideAllDynamicPages();

    ui->CardActions->show();
    ui->cardBattery->show();
    ui->cardCPU->show();
    ui->btnMiniMode->show();
}

void MainWindow::on_settingsButton_clicked() {
    hideAllDynamicPages();

    ui->CardActions->hide();
    ui->cardBattery->hide();
    ui->cardCPU->hide();
    ui->btnMiniMode->hide();

    if (!settingsPage) {
        settingsPage = new QWidget(this);
        settingsPage->setGeometry(200, 80, 1050, 500);

        QVBoxLayout *layout = new QVBoxLayout(settingsPage);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("DASHBOARD SETTINGS", settingsPage);
        title->setStyleSheet("color: #00d2ff; font-size: 24px; font-weight: bold; margin-bottom: 20px;");
        title->setAlignment(Qt::AlignCenter);

        // Blank layout (Theme controls removed)
        QHBoxLayout *themeLayout = new QHBoxLayout();

        // Brightness Slider Control
        QVBoxLayout *brightLayout = new QVBoxLayout();
        QLabel *lblBright = new QLabel("Screen Dimmer / Brightness Overlay:", settingsPage);
        lblBright->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: bold; margin-top: 15px;");

        brightnessSlider = new QSlider(Qt::Horizontal, settingsPage);
        brightnessSlider->setRange(0, 70);
        brightnessSlider->setValue(0);
        brightnessSlider->setFixedWidth(300);

        connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::updateBrightness);

        brightLayout->addWidget(lblBright);
        brightLayout->addWidget(brightnessSlider);

        // Sound Test Control
        QHBoxLayout *soundLayout = new QHBoxLayout();
        QLabel *lblSound = new QLabel("Alert Sound Test:", settingsPage);
        lblSound->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: bold; margin-top: 15px;");

        QPushButton *btnTestSound = new QPushButton("Play Test Beep", settingsPage);
        btnTestSound->setCursor(Qt::PointingHandCursor);
        btnTestSound->setStyleSheet(
            "QPushButton { background-color: #21262d; color: #ffcc00; border: 1px solid #ffcc00; border-radius: 6px; padding: 6px 14px; font-weight: bold; }"
            "QPushButton:hover { background-color: #ffcc00; color: #000000; }"
            );

        connect(btnTestSound, &QPushButton::clicked, this, []() {
            Beep(750, 300);
        });

        soundLayout->addWidget(lblSound);
        soundLayout->addSpacing(20);
        soundLayout->addWidget(btnTestSound);

        // Back Button
        btnSettingsBack = new QPushButton("Back to Dashboard", settingsPage);
        btnSettingsBack->setCursor(Qt::PointingHandCursor);
        btnSettingsBack->setStyleSheet(
            "QPushButton { background-color: #00d2ff; color: #000000; font-weight: bold; font-size: 15px; border-radius: 8px; padding: 10px 25px; margin-top: 30px; }"
            "QPushButton:hover { background-color: #0099cc; color: #ffffff; }"
            );

        connect(btnSettingsBack, &QPushButton::clicked, this, [this]() {
            settingsPage->hide();
            ui->CardActions->show();
            ui->cardBattery->show();
            ui->cardCPU->show();
            ui->btnMiniMode->show();
        });

        layout->addWidget(title);
        layout->addLayout(themeLayout);
        layout->addLayout(brightLayout);
        layout->addLayout(soundLayout);
        layout->addWidget(btnSettingsBack);
    }

    settingsPage->show();
}

// Brightness Dimmer Logic
void MainWindow::updateBrightness(int value) {
    if (!brightnessOverlay) {
        brightnessOverlay = new QWidget(this);
        brightnessOverlay->setGeometry(this->rect());
        brightnessOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    brightnessOverlay->setGeometry(this->rect());
    int alpha = (value * 255) / 100;
    brightnessOverlay->setStyleSheet(QString("background-color: rgba(0, 0, 0, %1);").arg(alpha));
    brightnessOverlay->show();
    brightnessOverlay->raise();
}

void MainWindow::hideAllDynamicPages() {
    if (uptimePage) uptimePage->hide();
    if (networkPage) networkPage->hide();
    if (settingsPage) settingsPage->hide();

    if (uptimeTimer) uptimeTimer->stop();
    if (networkTimer) networkTimer->stop();
}