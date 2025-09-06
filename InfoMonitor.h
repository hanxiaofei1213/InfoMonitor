#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QUuid>
#include "ui_InfoMonitor.h"

// 前向声明
class MonitorItem;
class MonitorPage;
class MonitorManager;
class ConfigManager;

class InfoMonitor : public QMainWindow
{
    Q_OBJECT

public:
    InfoMonitor(QWidget *parent = nullptr);
    ~InfoMonitor();

private slots:
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

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createNewPage(const QString& name = QString());
    void createPageUI(const MonitorPage& page);
    void updateCurrentPageUI();
    void updateStatusBar();
    void loadConfiguration();
    void saveConfiguration();
    QTableWidget* getCurrentTable();
    int getCurrentPageIndex();

private:
    Ui::InfoMonitorClass ui;
    
    // UI组件
    QTabWidget* m_tabWidget;
    QToolBar* m_toolBar;
    QStatusBar* m_statusBar;
    QLabel* m_statusLabel;
    QLabel* m_nextCheckLabel;
    QLabel* m_itemCountLabel;
    QPushButton* m_startStopButton;
    
    // 业务组件
    MonitorManager* m_monitorManager;
    ConfigManager* m_configManager;
    
    // 状态
    bool m_isMonitoring;
};

