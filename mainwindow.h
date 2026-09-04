#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSystemMetrics();
    void on_cleanButton_clicked();
    void on_genReport_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;

    // CPU Calculation Helper Variables
    FILETIME prevSysKernel;
    FILETIME prevSysUser;
    FILETIME prevSysIdle;

    double getCpuUsage();
    void checkBatteryAlerts(int percentage, bool isCharging);
    ULONGLONG subtractTimes(const FILETIME& ftA, const FILETIME& ftB);
};

#endif // MAINWINDOW_H