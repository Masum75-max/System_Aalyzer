#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

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

        // findChild used to dynamically map UI progress bar name
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
    // সেভ ফাইল ডায়ালগ পপ-আপ করানো
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save System Health Report",
        "System_Report.txt", // ডিফল্ট ফাইলের নাম
        "Text Files (*.txt);;All Files (*)"
        );

    // ইউজার যদি কোনো পাথ সিলেক্ট না করে ক্যানসেল চাপ দেয়
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