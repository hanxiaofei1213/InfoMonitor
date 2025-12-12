#include "InfoMonitorStrategy.h"
#include "../ui/UIConfigManager.h"
#include "../data/MonitorPageInfo.h"

#include <QDebug>
#include <QApplication>
#include <QVariant>
#include <QDateTime>

InfoMonitorStrategy::InfoMonitorStrategy(QObject* parent)
    : QObject(parent)
    , m_monitoringTimer(nullptr)
    , m_isInitialized(false)
    , m_isMonitoring(false)
    , m_isPaused(false)
    , m_checkInterval(60000)  // 默认60秒
    , m_autoStart(true) {
    qDebug() << "InfoMonitorStrategy created";
}

InfoMonitorStrategy::~InfoMonitorStrategy() {
    cleanup();
    qDebug() << "InfoMonitorStrategy destroyed";
}

bool InfoMonitorStrategy::initialize() {
    if (m_isInitialized) {
        qWarning() << "InfoMonitorStrategy already initialized";
        return true;
    }

    try {
        // 创建配置管理器
        m_uiConfigManager = new UIConfigManager(this);
        //m_coreConfigManager = new IMCoreConfigManager();

        // 设置定时器
        setupTimer();

        // 连接信号
        connectSignals();

        // 加载配置
        if (!loadConfiguration()) {
            qWarning() << "Failed to load configuration, using defaults";
            resetToDefault();
        }

        m_isInitialized = true;
        emit initializationCompleted(true);

        qDebug() << "InfoMonitorStrategy initialized successfully";
        return true;
    }
    catch (const std::exception& e) {
        m_lastError = QString("Initialization failed: %1").arg(e.what());
        emit errorOccurred(m_lastError);
        emit initializationCompleted(false);
        return false;
    }
}

void InfoMonitorStrategy::cleanup() {
    if (m_isMonitoring) {
        stopMonitoring();
    }

    if (m_monitoringTimer) {
        m_monitoringTimer->stop();
        delete m_monitoringTimer;
        m_monitoringTimer = nullptr;
    }

    //if (m_coreConfigManager) {
    //    delete m_coreConfigManager;
    //    m_coreConfigManager = nullptr;
    //}

    // UIConfigManager 由 QObject 父子关系自动管理
    m_uiConfigManager = nullptr;

    m_isInitialized = false;
}

bool InfoMonitorStrategy::loadConfiguration() {
    //if (!m_coreConfigManager) {
    //    return false;
    //}

    // TODO: 实现配置加载逻辑
    // bool success = m_coreConfigManager->loadConfiguration();

    // 暂时使用默认值
    bool success = true;

    if (success) {
        // 发送配置加载完成信号
        QVariantMap config;
        config["checkInterval"] = m_checkInterval;
        config["autoStart"] = m_autoStart;
        // 将页面列表转换为配置格式
        QVariantList pagesList;
        for (const auto& page : getMonitorPages()) {
            QVariantMap pageMap;
            pageMap["id"] = page.id;
            pageMap["name"] = page.name;
            pageMap["url"] = page.url;
            pageMap["enabled"] = page.enabled;
            pagesList.append(pageMap);
        }
        config["pages"] = pagesList;

        emit configurationLoaded(config);
        emit statusMessageChanged("Configuration loaded successfully");
    }

    return success;
}

bool InfoMonitorStrategy::saveConfiguration() {
    //if (!m_coreConfigManager) {
    //    return false;
    //}

    // TODO: 实现配置保存逻辑
    // bool success = m_coreConfigManager->saveConfiguration();

    // 暂时返回成功
    bool success = true;

    emit configurationSaved(success);

    if (success) {
        emit statusMessageChanged("Configuration saved successfully");
    } else {
        emit errorOccurred("Failed to save configuration");
    }

    return success;
}

void InfoMonitorStrategy::resetToDefault() {
    m_checkInterval = 60000;
    m_autoStart = true;
    m_isPaused = false;

    // TODO: 创建默认配置
    // m_coreConfigManager->createDefaultConfiguration();

    emit defaultConfigurationCreated();
    emit statusMessageChanged("Configuration reset to default");
}

void InfoMonitorStrategy::startMonitoring() {
    if (!m_isInitialized) {
        emit errorOccurred("Strategy not initialized");
        return;
    }

    if (m_isMonitoring && !m_isPaused) {
        qDebug() << "Monitoring already active";
        return;
    }

    if (m_isPaused) {
        resumeMonitoring();
        return;
    }

    m_isMonitoring = true;
    m_isPaused = false;

    if (m_monitoringTimer) {
        m_monitoringTimer->start(m_checkInterval);
    }

    emit monitoringStarted();
    emit monitoringStatusChanged(true);
    emit statusMessageChanged("Monitoring started");

    qDebug() << "Monitoring started with interval:" << m_checkInterval << "ms";
}

void InfoMonitorStrategy::stopMonitoring() {
    if (!m_isMonitoring) {
        qDebug() << "Monitoring already stopped";
        return;
    }

    m_isMonitoring = false;
    m_isPaused = false;

    if (m_monitoringTimer) {
        m_monitoringTimer->stop();
    }

    emit monitoringStopped();
    emit monitoringStatusChanged(false);
    emit statusMessageChanged("Monitoring stopped");

    qDebug() << "Monitoring stopped";
}

void InfoMonitorStrategy::pauseMonitoring() {
    if (!m_isMonitoring || m_isPaused) {
        return;
    }

    m_isPaused = true;

    if (m_monitoringTimer) {
        m_monitoringTimer->stop();
    }

    emit monitoringPaused();
    emit statusMessageChanged("Monitoring paused");

    qDebug() << "Monitoring paused";
}

void InfoMonitorStrategy::resumeMonitoring() {
    if (!m_isMonitoring || !m_isPaused) {
        return;
    }

    m_isPaused = false;

    if (m_monitoringTimer) {
        m_monitoringTimer->start(m_checkInterval);
    }

    emit monitoringResumed();
    emit statusMessageChanged("Monitoring resumed");

    qDebug() << "Monitoring resumed";
}

bool InfoMonitorStrategy::isMonitoring() const {
    return m_isMonitoring && !m_isPaused;
}

void InfoMonitorStrategy::addMonitorPage(const QString& name, const QString& url) {
    QString pageId = QString("page_%1").arg(QDateTime::currentMSecsSinceEpoch());
    
    MonitorPageInfo page{
        .id = pageId,
        .name = name,
        .url = url,
        .enabled = true,
        .lastCheckTime = 0  // 初始化为0，表示尚未检查
    };
    
    m_monitorPages.append(page);

    emit pageAdded(pageId, page);
    emit pagesListChanged(getMonitorPages());
    emit statusMessageChanged(QString("Added monitor page: %1").arg(name));

    qDebug() << "Added monitor page:" << name << "URL:" << url;
}

void InfoMonitorStrategy::removeMonitorPage(const QString& pageId) {
    for (int i = 0; i < m_monitorPages.size(); ++i) {
        if (m_monitorPages[i].id == pageId) {
            QString pageName = m_monitorPages[i].name;
            m_monitorPages.removeAt(i);
            
            emit pageRemoved(pageId);
            emit pagesListChanged(getMonitorPages());
            emit statusMessageChanged(QString("Removed monitor page: %1").arg(pageName));
            
            qDebug() << "Removed monitor page:" << pageName << "ID:" << pageId;
            return;
        }
    }
    
    qWarning() << "Page not found for removal:" << pageId;
}

void InfoMonitorStrategy::updateMonitorPage(const QString& pageId, const MonitorPageInfo& page) {
    //for (int i = 0; i < m_monitorPages.size(); ++i) {
    //    if (m_monitorPages[i].id == pageId) {
    //        m_monitorPages[i] = page;
    //        m_monitorPages[i].modifiedTime = QDateTime::currentDateTime();
    //        
    //        emit pageUpdated(pageId, page);
    //        emit pagesListChanged(getMonitorPages());
    //        emit statusMessageChanged(QString("Updated monitor page: %1").arg(page.name));
    //        
    //        qDebug() << "Updated monitor page:" << page.name << "ID:" << pageId;
    //        return;
    //    }
    //}
    //
    qWarning() << "Page not found for update:" << pageId;
}

QList<MonitorPageInfo> InfoMonitorStrategy::getMonitorPages() const {
    return m_monitorPages;
}

MonitorPageInfo* InfoMonitorStrategy::findMonitorPage(const QString& pageId) {
    for (int i = 0; i < m_monitorPages.size(); ++i) {
        if (m_monitorPages[i].id == pageId) {
            return &m_monitorPages[i];
        }
    }
    return nullptr;
}

const MonitorPageInfo* InfoMonitorStrategy::findMonitorPage(const QString& pageId) const {
    for (int i = 0; i < m_monitorPages.size(); ++i) {
        if (m_monitorPages[i].id == pageId) {
            return &m_monitorPages[i];
        }
    }
    return nullptr;
}

void InfoMonitorStrategy::setCheckInterval(int intervalMs) {
    if (intervalMs < 1000) { // 最小1秒
        intervalMs = 1000;
    }

    m_checkInterval = intervalMs;

    if (m_monitoringTimer && m_isMonitoring && !m_isPaused) {
        m_monitoringTimer->setInterval(m_checkInterval);
    }

    QVariantMap settings;
    settings["checkInterval"] = m_checkInterval;
    emit settingsChanged(settings);

    qDebug() << "Check interval set to:" << intervalMs << "ms";
}

int InfoMonitorStrategy::getCheckInterval() const {
    return m_checkInterval;
}

void InfoMonitorStrategy::setAutoStart(bool enabled) {
    m_autoStart = enabled;

    QVariantMap settings;
    settings["autoStart"] = m_autoStart;
    emit settingsChanged(settings);

    qDebug() << "Auto start set to:" << enabled;
}

bool InfoMonitorStrategy::isAutoStart() const {
    return m_autoStart;
}

// 公共槽函数实现
void InfoMonitorStrategy::onInitializeRequest() {
    initialize();
}

void InfoMonitorStrategy::onStartMonitoringRequest() {
    startMonitoring();
}

void InfoMonitorStrategy::onStopMonitoringRequest() {
    stopMonitoring();
}

void InfoMonitorStrategy::onConfigurationChangeRequest(const QVariantMap& config) {
    if (config.contains("checkInterval")) {
        setCheckInterval(config["checkInterval"].toInt());
    }

    if (config.contains("autoStart")) {
        setAutoStart(config["autoStart"].toBool());
    }

    saveConfiguration();
}

void InfoMonitorStrategy::onPageAddRequest(const QString& name, const QString& url) {
    addMonitorPage(name, url);
}

void InfoMonitorStrategy::onPageRemoveRequest(const QString& pageId) {
    removeMonitorPage(pageId);
}

//void InfoMonitorStrategy::onPageUpdateRequest(const QString& pageId, const QVariantMap& config) {
//    updateMonitorPage(pageId, config);
//}

void InfoMonitorStrategy::onSettingsChangeRequest(const QVariantMap& settings) {
    onConfigurationChangeRequest(settings);
}

// 私有槽函数实现
void InfoMonitorStrategy::onMonitoringTimer() {
    if (!m_isMonitoring || m_isPaused) {
        return;
    }

    // TODO: 执行实际的监控逻辑
    qDebug() << "Monitoring timer triggered";

    // 更新全局状态
    QVariantMap status = createStatusInfo();
    emit globalStatusUpdated(status);
}

void InfoMonitorStrategy::onConfigurationChanged() {
    notifyConfigurationChange();
}

// 私有方法实现
void InfoMonitorStrategy::setupTimer() {
    m_monitoringTimer = new QTimer(this);
    m_monitoringTimer->setSingleShot(false);
    connect(m_monitoringTimer, &QTimer::timeout, this, &InfoMonitorStrategy::onMonitoringTimer);
}

void InfoMonitorStrategy::connectSignals() {
    // 这里可以连接其他组件的信号
    // 例如配置管理器的信号等
}

//QVariantMap InfoMonitorStrategy::createPageInfo(const MonitorPageInfo& page) const {
//    // TODO: 实现页面信息创建
//    QVariantMap info;
//    return info;
//}

QVariantMap InfoMonitorStrategy::createStatusInfo() const {
    QVariantMap status;
    status["isMonitoring"] = m_isMonitoring;
    status["isPaused"] = m_isPaused;
    status["checkInterval"] = m_checkInterval;
    status["autoStart"] = m_autoStart;
    status["pageCount"] = getMonitorPages().size();
    status["lastUpdate"] = QDateTime::currentDateTime();

    return status;
}

void InfoMonitorStrategy::validateConfiguration() {
    // TODO: 实现配置验证逻辑
}

void InfoMonitorStrategy::notifyConfigurationChange() {
    QVariantMap config;
    config["checkInterval"] = m_checkInterval;
    config["autoStart"] = m_autoStart;
    
    // 将页面列表转换为配置格式
    QVariantList pagesList;
    for (const auto& page : getMonitorPages()) {
        QVariantMap pageMap;
        pageMap["id"] = page.id;
        pageMap["name"] = page.name;
        pageMap["url"] = page.url;
        pageMap["enabled"] = page.enabled;
        pagesList.append(pageMap);
    }
    config["pages"] = pagesList;

    emit configurationLoaded(config);
}
