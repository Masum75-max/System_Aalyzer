#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QLayout>
#include <QLayoutItem>
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
    void toggleMiniMode();

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;

    // CPU Calculation Helper Variables
    FILETIME prevSysKernel;
    FILETIME prevSysUser;
    FILETIME prevSysIdle;

    // Mini Mode
    bool isMiniMode = false;

    // Window Dragging
    QPoint dragPosition;

    // Normal Window State
    QRect normalWindowGeometry;
    Qt::WindowFlags normalWindowFlags;

    // Original widget geometry
    QRect originalCardGeometry;
    QRect originalMiniButtonGeometry;

    // Original layouts
    QLayout *cardOriginalLayout = nullptr;
    QLayout *buttonOriginalLayout = nullptr;

    // Original layout items
    QLayoutItem *cardOriginalItem = nullptr;
    QLayoutItem *buttonOriginalItem = nullptr;

    double getCpuUsage();

    void checkBatteryAlerts(
        int percentage,
        bool isCharging
        );

    ULONGLONG subtractTimes(
        const FILETIME& ftA,
        const FILETIME& ftB
        );

    // Mini Mode helpers
    void enterMiniMode();
    void exitMiniMode();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H