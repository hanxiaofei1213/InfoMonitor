#pragma once
#include <QMainWindow>
#include "ui_InfoMonitor.h"


class UIConfigManager;
class WindowManager;
class IMTray;

class InfoMonitor : public QMainWindow {
    Q_OBJECT

public:
    InfoMonitor();
    ~InfoMonitor();

protected:
    void InitUI();
    void InitMenuBar();
    void InitToolBar();
    void InitStatusBar();
    void InitTray();
    void InitConnect();

    // 重写事件
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

protected slots:
    void onTabChanged(int nIndex);
    void onTabCloseRequest(int nIndex);
    
    // 处理托盘相关的槽函数
    void onShowWindow();
    void onExitApplication();

private:
    Ui::InfoMonitor ui;
    UIConfigManager* m_pConfig = nullptr;
    WindowManager* m_windowManager = nullptr;
    IMTray* m_trayIcon = nullptr;
};


/*
class InfoMonitor : public QMainWindow {
    Q_OBJECT

public:

    InfoMonitor();
    ~InfoMonitor();

protected slots:
    void onAddPage();
    void onAddItem();
    void onRefreshPage();
    void onStartStopMonitoring();
    
    void onPageNameChanged();
    void onPageEnabledChanged(bool enabled);
    void onItemEnabledChanged(int row, int column);
    void onItemDoubleClicked(int row, int column);
    void onDeleteItem();
    void onMonitoringUpdate();

    // 托盘相关槽函数
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowWindow();
    void onExitApplication();

protected:
    void setupTrayIcon();  // 设置托盘图标
    void createNewPage(const QString& name = QString());
    void createPageUI(int nPageIndex, const MonitorPage& page);
    QWidget* createPageWidget(int nPageIndex, const QString& pageName, bool enabled, const QList<MonitorItem>& items = QList<MonitorItem>());
    QTableWidget* setupTable(int pageIndex = -1);
    void populateTableWithItems(QTableWidget* table, const QList<MonitorItem>& items);
    void updateCurrentPageUI();
    void updateStatusBar();
    void loadConfiguration();
    void saveConfiguration();
    QTableWidget* getCurrentTable();



    // 保存列宽
    void saveColumnWidthsOnExit();

    // 表格列配置结构体

    // 获取当前页面索引
    int getCurrentPageIndex() const;

private:
    QTabWidget* m_tabWidget = nullptr;
    QToolBar* m_toolBar = nullptr;
    QStatusBar* m_statusBar = nullptr;
    QLabel* m_statusLabel = nullptr;
    QLabel* m_nextCheckLabel = nullptr;
    QLabel* m_itemCountLabel = nullptr;
    QPushButton* m_startStopButton = nullptr;

    // 业务组件
    MonitorManager* m_monitorManager = nullptr;
    ConfigManager* m_configManager = nullptr;
    

    // 托盘相关组件
    QSystemTrayIcon* m_trayIcon = nullptr;
    QMenu* m_trayMenu = nullptr;
    QAction* m_showAction = nullptr;
    QAction* m_exitAction = nullptr;

    // 状态
    bool m_isMonitoring = false;
};



struct ColumnConfig {
    int index = 0;
    int width = 0;
    bool fixed = false;
};
*/
