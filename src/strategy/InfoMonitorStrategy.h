﻿#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QList>
#include <QVariantMap>

// 前向声明
class UIConfigManager;
class IMCoreConfigManager;

// 数据结构
#include "../data/MonitorPageInfo.h"

/**
 * @brief 信息监控业务策略类
 *
 * 负责管理配置、协调各个组件，并通过信号槽与界面层通信
 * 实现界面与业务逻辑的完全分离
 */
class InfoMonitorStrategy : public QObject {
    Q_OBJECT

public:
    explicit InfoMonitorStrategy(QObject* parent = nullptr);
    ~InfoMonitorStrategy();

    // 初始化和清理
    bool initialize();
    void cleanup();

    // 配置管理
    bool loadConfiguration();
    bool saveConfiguration();
    void resetToDefault();

    // 监控控制
    void startMonitoring();
    void stopMonitoring();
    void pauseMonitoring();
    void resumeMonitoring();
    bool isMonitoring() const;

    // 页面管理
    void addMonitorPage(const QString& name, const QString& url);
    void removeMonitorPage(const QString& pageId);
    void updateMonitorPage(const QString& pageId, const MonitorPageInfo& page);
    QList<MonitorPageInfo> getMonitorPages() const;
    MonitorPageInfo* findMonitorPage(const QString& pageId);
    const MonitorPageInfo* findMonitorPage(const QString& pageId) const;

    // 设置管理
    void setCheckInterval(int intervalMs);
    int getCheckInterval() const;
    void setAutoStart(bool enabled);
    bool isAutoStart() const;

public slots:
    // 界面请求处理
    void onInitializeRequest();
    void onStartMonitoringRequest();
    void onStopMonitoringRequest();
    void onConfigurationChangeRequest(const QVariantMap& config);
    void onPageAddRequest(const QString& name, const QString& url);
    void onPageRemoveRequest(const QString& pageId);
    void onPageUpdateRequest(const QString& pageId, const MonitorPageInfo& page);
    void onSettingsChangeRequest(const QVariantMap& settings);

signals:
    // 初始化相关信号
    void initializationCompleted(bool success);
    void configurationLoaded(const QVariantMap& config);
    void defaultConfigurationCreated();

    // 监控状态信号
    void monitoringStarted();
    void monitoringStopped();
    void monitoringPaused();
    void monitoringResumed();
    void monitoringStatusChanged(bool isActive);

    // 页面管理信号
    void pageAdded(const QString& pageId, const MonitorPageInfo& page);
    void pageRemoved(const QString& pageId);
    void pageUpdated(const QString& pageId, const MonitorPageInfo& page);
    void pagesListChanged(const QList<MonitorPageInfo>& pages);

    // 数据更新信号
    void monitorDataUpdated(const QString& pageId, const QVariantMap& data);
    void monitorError(const QString& pageId, const QString& error);
    void globalStatusUpdated(const QVariantMap& status);

    // 配置变更信号
    void configurationSaved(bool success);
    void settingsChanged(const QVariantMap& newSettings);


    // 错误和状态信号
    void errorOccurred(const QString& error, const QString& details = QString());
    void warningIssued(const QString& warning);
    void statusMessageChanged(const QString& message);

private slots:
    // 内部定时器和事件处理
    void onMonitoringTimer();
    void onConfigurationChanged();

private:
    // 内部方法
    void setupTimer();
    void connectSignals();
    QVariantMap createStatusInfo() const;
    void validateConfiguration();
    void notifyConfigurationChange();

    // 成员变量
    UIConfigManager* m_uiConfigManager;
    IMCoreConfigManager* m_coreConfigManager;
    QTimer* m_monitoringTimer;

    bool m_isInitialized;
    bool m_isMonitoring;
    bool m_isPaused;
    int m_checkInterval;
    bool m_autoStart;

    QString m_lastError;
    QVariantMap m_currentStatus;
    QList<MonitorPageInfo> m_monitorPages;
};
