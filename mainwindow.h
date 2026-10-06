#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QLayout>
#include <QLayoutItem>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QDialog>
#include <QCheckBox>
#include <windows.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSystemMetrics();
    void on_cleanButton_clicked();
    void on_genReport_clicked();
    void toggleMiniMode();
    void on_batterySaverButton_clicked();
    void on_dashButton_clicked();
    void updateUptimeLive();
    void showBreakPopup();

    // Network Speed & Usage Slots
    void on_optimizerButton_clicked();
    void updateNetworkMetrics();

    // Settings Slots
    void on_settingsButton_clicked();
    void updateBrightness(int value);
    void toggleAutoStartup(bool enabled);

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer = nullptr;

    // CPU Calculation Helper Variables
    FILETIME prevSysKernel;
    FILETIME prevSysUser;
    FILETIME prevSysIdle;

    // Mini Mode & Dragging
    bool isMiniMode = false;
    QPoint dragPosition;

    // Normal Window State & Layout Preservation
    QRect normalWindowGeometry;
    Qt::WindowFlags normalWindowFlags;
    QRect originalCardGeometry;
    QRect originalMiniButtonGeometry;

    QLayout *cardOriginalLayout = nullptr;
    QLayout *buttonOriginalLayout = nullptr;
    QLayoutItem *cardOriginalItem = nullptr;
    QLayoutItem *buttonOriginalItem = nullptr;

    // Uptime View Widgets (Dynamic Creation)
    QWidget *uptimePage = nullptr;
    QLabel *lblUptimeDisplay = nullptr;
    QTimer *uptimeTimer = nullptr;
    QPushButton *btnBack = nullptr;

    // Eye Care Break Reminder
    QTimer *breakTimer = nullptr;
    QTimer *breakCountdownTimer = nullptr;
    int remainingBreakSeconds = 20;
    QDialog *breakDialog = nullptr;
    QLabel *lblBreakCountdown = nullptr;

    // Network Speed & Data Usage Widgets
    QWidget *networkPage = nullptr;
    QLabel *lblDownloadSpeed = nullptr;
    QLabel *lblUploadSpeed = nullptr;
    QLabel *lblTotalData = nullptr;
    QTimer *networkTimer = nullptr;
    QPushButton *btnNetworkBack = nullptr;

    // Network Helper Variables
    ULONG64 prevInBytes = 0;
    ULONG64 prevOutBytes = 0;
    ULONG64 initialTotalBytes = 0;
    bool isFirstNetworkCheck = true;

    // Settings Page Widgets
    QWidget *settingsPage = nullptr;
    QPushButton *btnThemeToggle = nullptr;
    QSlider *brightnessSlider = nullptr;
    QPushButton *btnSettingsBack = nullptr;
    QWidget *brightnessOverlay = nullptr;
    QCheckBox *chkAutoStartup = nullptr;

    bool isDarkMode = true;

    // Internal Helper Methods
    double getCpuUsage();
    void checkBatteryAlerts(int percentage, bool isCharging);
    ULONGLONG subtractTimes(const FILETIME& ftA, const FILETIME& ftB);
    void hideAllDynamicPages();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H