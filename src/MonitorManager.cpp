#include "MonitorManager.h"
#include <QDebug>

MonitorManager::MonitorManager(QObject* parent)
    : QObject(parent)
{
    // 创建定时器
    m_timer = new QTimer(this);
    
    connect(m_timer, &QTimer::timeout, this, &MonitorManager::onTimerTimeout);
    
    // 加载配置
    m_configManager.loadConfiguration();
    
    // 设置定时器间隔
    m_timer->setInterval(m_configManager.getCheckInterval());
}

MonitorManager::~MonitorManager()
{
    stopMonitoring();
}

void MonitorManager::startMonitoring()
{
    if (m_isMonitoring) {
        return;
    }
    
    m_isMonitoring = true;
    updateNextCheckTime();
    m_timer->start();
    
    emit monitoringStarted();
    
    // 立即执行一次检查
    onTimerTimeout();
}

void MonitorManager::stopMonitoring()
{
    if (!m_isMonitoring) {
        return;
    }
    
    m_isMonitoring = false;
    m_timer->stop();
    
    emit monitoringStopped();
}

void MonitorManager::refreshAll()
{
    auto& pages = m_configManager.getPages();
    for (int i = 0; i < pages.size(); ++i) {
        pages[i].refreshEnabledItems();
        emit pageUpdated(i);
    }
    
    emit itemsUpdated();
    
    // 保存配置（更新最后检查时间）
    m_configManager.saveConfiguration();
}

void MonitorManager::refreshPage(const QString& pageId)
{
    MonitorPage* page = m_configManager.findPage(pageId);
    if (page) {
        page->refreshEnabledItems();
        int pageIndex = m_configManager.findPageIndex(pageId);
        if (pageIndex >= 0) {
            emit pageUpdated(pageIndex);
        }
        emit itemsUpdated();
        
        // 保存配置
        m_configManager.saveConfiguration();
    }
}

void MonitorManager::refreshPage(int pageIndex)
{
    auto& pages = m_configManager.getPages();
    if (pageIndex >= 0 && pageIndex < pages.size()) {
        pages[pageIndex].refreshEnabledItems();
        emit pageUpdated(pageIndex);
        emit itemsUpdated();
        
        // 保存配置
        m_configManager.saveConfiguration();
    }
}

QDateTime MonitorManager::getNextCheckTime() const
{
    return m_nextCheckTime;
}

int MonitorManager::getTotalItemCount() const
{
    int count = 0;
    const auto& pages = m_configManager.getPages();
    for (const auto& page : pages) {
        count += page.getItemCount();
    }
    return count;
}

int MonitorManager::getEnabledItemCount() const
{
    int count = 0;
    const auto& pages = m_configManager.getPages();
    for (const auto& page : pages) {
        if (page.isEnabled()) {
            count += page.getEnabledItemCount();
        }
    }
    return count;
}

int MonitorManager::getCheckInterval() const
{
    return m_configManager.getCheckInterval();
}

void MonitorManager::setCheckInterval(int interval)
{
    m_configManager.setCheckInterval(interval);
    m_timer->setInterval(interval);
    
    if (m_isMonitoring) {
        updateNextCheckTime();
    }
    
    // 保存配置
    m_configManager.saveConfiguration();
}

void MonitorManager::onTimerTimeout()
{
    if (!m_isMonitoring) {
        return;
    }
    
    refreshAll();
    updateNextCheckTime();
}

void MonitorManager::updateNextCheckTime()
{
    if (m_isMonitoring) {
        m_nextCheckTime = QDateTime::currentDateTime().addMSecs(m_timer->interval());
    } else {
        m_nextCheckTime = QDateTime();
    }
}
