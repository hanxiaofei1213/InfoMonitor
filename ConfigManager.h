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
    QString m_version = "1.0";
    int m_checkInterval = 60000;
    bool m_autoStart = true;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    QList<MonitorPage> m_pages;
};
