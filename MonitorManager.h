#pragma once

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "ConfigManager.h"

class MonitorManager : public QObject
{
    Q_OBJECT

public:
    explicit MonitorManager(QObject* parent = nullptr);
    ~MonitorManager();
    
    // 监控控制
    void startMonitoring();
    void stopMonitoring();
    bool isMonitoring() const { return m_isMonitoring; }
    
    // 刷新操作
    void refreshAll();
    void refreshPage(const QString& pageId);
    void refreshPage(int pageIndex);
    
    // 配置管理
    ConfigManager* getConfigManager() { return m_configManager; }
    
    // 状态信息
    QDateTime getNextCheckTime() const;
    int getTotalItemCount() const;
    int getEnabledItemCount() const;
    int getCheckInterval() const;
    void setCheckInterval(int interval);

signals:
    void monitoringStarted();
    void monitoringStopped();
    void itemsUpdated();
    void pageUpdated(int pageIndex);

private slots:
    void onTimerTimeout();

private:
    void updateNextCheckTime();

private:
    ConfigManager* m_configManager;
    QTimer* m_timer;
    bool m_isMonitoring;
    QDateTime m_nextCheckTime;
};
