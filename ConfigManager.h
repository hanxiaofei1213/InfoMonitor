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
    int getCheckInterval() const;
    void setCheckInterval(int interval);

    bool isAutoStart() const;
    void setAutoStart(bool autoStart);

    // 窗口大小设置
    int getWindowWidth() const;
    void setWindowWidth(int width);

    int getWindowHeight() const;
    void setWindowHeight(int height);

    // 页面管理
    QList<MonitorPage>& getPages();
    const QList<MonitorPage>& getPages() const;

    void addPage(const MonitorPage& page);
    void removePage(const QString& pageId);
    void removePage(int index);
    MonitorPage* findPage(const QString& pageId);
    int findPageIndex(const QString& pageId) const;
    void clearPages();
    int getPageCount() const;

    // 配置验证和修复
    bool validateConfiguration() const;
    void createDefaultConfiguration();

protected:
    QJsonObject createDefaultConfig() const;
    bool parseConfiguration(const QJsonObject& config);

private:
    QString m_configFilePath;
    QString m_version;
    int m_checkInterval;        // 检查间隔（毫秒）
    bool m_autoStart;           // 自动开始监控
    int m_windowWidth;          // 窗口宽度
    int m_windowHeight;         // 窗口高度
    QList<MonitorPage> m_pages; // 监控页面列表
};
