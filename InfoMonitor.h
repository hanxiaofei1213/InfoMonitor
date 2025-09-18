#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QInputDialog>
#include <QTimer>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QUuid>
#include <QList>
#include <QString>
#include <QCloseEvent>
#include "ui_InfoMonitor.h"

class MonitorItem;
class MonitorPage;
class MonitorManager;
class ConfigManager;
class WindowManager;

class InfoMonitor : public QMainWindow {
    Q_OBJECT

public:
    static InfoMonitor& getInstance();
    ~InfoMonitor();

    InfoMonitor(const InfoMonitor&) = delete;
    InfoMonitor& operator=(const InfoMonitor&) = delete;

protected slots:
    void onAddPage();
    void onAddItem();
    void onRefreshPage();
    void onStartStopMonitoring();
    void onTabChanged(int index);
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
    InfoMonitor();

    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupTrayIcon();  // 设置托盘图标
    void createNewPage(const QString& name = QString());
    void createPageUI(const MonitorPage& page);
    QWidget* createPageWidget(const QString& pageName, bool enabled, const QList<MonitorItem>& items = QList<MonitorItem>());
    QTableWidget* setupTable();
    void populateTableWithItems(QTableWidget* table, const QList<MonitorItem>& items);
    void updateCurrentPageUI();
    void updateStatusBar();
    void loadConfiguration();
    void saveConfiguration();
    QTableWidget* getCurrentTable();

    // 重写事件
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

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
    WindowManager* m_windowManager = nullptr;

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
