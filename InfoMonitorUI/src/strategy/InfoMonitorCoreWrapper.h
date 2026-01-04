#pragma once
#include <QObject>
#include <QVector>
#include <memory>
#include "src\data\MonitorPageInfo.h"

class InfoMonitorCore;

class InfoMonitorCoreWrapper : public QObject {
    Q_OBJECT
public:
    InfoMonitorCoreWrapper(QObject* pParent);
    ~InfoMonitorCoreWrapper();

    void Init();
    void UnInit();

    // ques(wangwenxi): 外面肯定不存数据了，那是这里存呢？还是内核里面存？要不这里维护一份在内存里面？
    // 读写配置
    QVector<MonitorPageInfo> GetMonitorPages() const;

    int SaveMonitorPages();
    

    // 操作配置
    // ques(wangwenxi): 这里怎么设计接口呢？



protected:

    



private:
    std::shared_ptr<InfoMonitorCore> m_pInfoMonitorCore;

};

/*
class InfoMonitorCoreWrapper : public QObject {
    Q_OBJECT

public:
    explicit InfoMonitorCoreWrapper(QObject* parent = nullptr);
    ~InfoMonitorCoreWrapper();

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
    //void onPageUpdateRequest(const QString& pageId, const MonitorPageInfo& page);
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
    UIConfigManager* m_uiConfigManager = nullptr;
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
*/
