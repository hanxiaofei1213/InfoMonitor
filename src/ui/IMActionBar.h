#pragma once
#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>

// 管理主窗口的菜单栏、工具栏和状态栏
class IMActionBar : public QObject {
    Q_OBJECT

public:
    IMActionBar(QMainWindow* pMainWindow, QObject* parent = nullptr);
    ~IMActionBar();

    void InitUI();

    // 更新状态栏信息
    void UpdateMonitorStatus(bool bIsRunning);
    void UpdateItemCount(int nTotal, int nEnabled);
    void UpdateNextCheckTime(int nSeconds);

signals:
    // 菜单栏/工具栏操作信号
    void addPageRequested();
    void addItemRequested();
    void deleteItemRequested();
    void refreshPageRequested();
    void startStopRequested();

protected:
    void InitMenuBar();
    void InitToolBar();
    void InitStatusBar();

    void OnAbout();

private:
    QMainWindow* m_pMainWindow = nullptr;

    // 工具栏组件
    QToolBar* m_pToolBar = nullptr;
    QPushButton* m_pStartStopButton = nullptr;

    // 状态栏组件
    QStatusBar* m_pStatusBar = nullptr;
    QLabel* m_pStatusLabel = nullptr;
    QLabel* m_pNextCheckLabel = nullptr;
    QLabel* m_pItemCountLabel = nullptr;

    // 状态
    bool m_bIsMonitoring = false;
};
