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
#include <QtWidgets/QWidget>
#include <QTimer>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QUuid>
#include <QList>
#include <QString>
#include "ui_InfoMonitor.h"

// 前向声明
class MonitorItem;
class MonitorPage;
class MonitorManager;
class ConfigManager;

class InfoMonitor : public QMainWindow {
    Q_OBJECT

public:
    InfoMonitor(QWidget* parent = nullptr);
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
    QWidget* createPageWidget(const QString& pageName, bool enabled, const QList<MonitorItem>& items = QList<MonitorItem>());
    QTableWidget* setupTable();
    void populateTableWithItems(QTableWidget* table, const QList<MonitorItem>& items);
    void updateCurrentPageUI();
    void updateStatusBar();
    void loadConfiguration();
    void saveConfiguration();
    QTableWidget* getCurrentTable();
    int getCurrentPageIndex();

private:
    Ui::InfoMonitorClass ui;

    // UI组件
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

    // 状态
    bool m_isMonitoring = false;
};

