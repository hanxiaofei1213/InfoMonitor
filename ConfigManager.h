#pragma once

#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "MonitorPage.h"

class ConfigManager {
public:
    ConfigManager();

    // 配置文件管理
    bool loadConfiguration();
    bool saveConfiguration();
    QString getConfigFilePath() const;

    // 全局设置
    int getCheckInterval() const {
        return m_checkInterval;
    }
    void setCheckInterval(int interval) {
        m_checkInterval = interval;
    }

    bool isAutoStart() const {
        return m_autoStart;
    }
    void setAutoStart(bool autoStart) {
        m_autoStart = autoStart;
    }

    // 页面管理
    QList<MonitorPage>& getPages() {
        return m_pages;
    }
    const QList<MonitorPage>& getPages() const {
        return m_pages;
    }

    void addPage(const MonitorPage& page);
    void removePage(const QString& pageId);
    void removePage(int index);
    MonitorPage* findPage(const QString& pageId);
    int findPageIndex(const QString& pageId) const;
    void clearPages();
    int getPageCount() const {
        return m_pages.size();
    }

    // 配置验证和修复
    bool validateConfiguration() const;
    void createDefaultConfiguration();

private:
    QJsonObject createDefaultConfig() const;
    bool parseConfiguration(const QJsonObject& config);

private:
    QString m_configFilePath;
    QString m_version;
    int m_checkInterval;        // 检查间隔（毫秒）
    bool m_autoStart;           // 自动开始监控
    QList<MonitorPage> m_pages; // 监控页面列表
};
