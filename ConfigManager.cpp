#include "ConfigManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

// todo(wangwenxi)：弄一个日志，保存到本地

ConfigManager::ConfigManager() {
    m_configFilePath = QCoreApplication::applicationDirPath() + "/config.json";
}

bool ConfigManager::loadConfiguration() {
    QFile file(m_configFilePath);
    if (!file.exists()) {
        // 配置文件不存在，创建默认配置
        createDefaultConfiguration();
        return saveConfiguration();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << QString::fromStdWString(L"无法打开配置文件:") << m_configFilePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << QString::fromStdWString(L"配置文件JSON解析错误:") << error.errorString();
        return false;
    }

    return parseConfiguration(doc.object());
}

bool ConfigManager::saveConfiguration() {
    QJsonObject config;
    config["version"] = m_version;

    // 全局设置
    QJsonObject globalSettings;
    globalSettings["checkInterval"] = m_checkInterval;
    globalSettings["autoStart"] = m_autoStart;
    globalSettings["windowWidth"] = m_windowWidth;
    globalSettings["windowHeight"] = m_windowHeight;
    config["globalSettings"] = globalSettings;

    // 页面列表
    QJsonArray pagesArray;
    for (const auto& page : m_pages) {
        pagesArray.append(page.toJson());
    }
    config["pages"] = pagesArray;

    QJsonDocument doc(config);

    QFile file(m_configFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << QString::fromStdWString(L"无法写入配置文件:") << m_configFilePath;
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

QString ConfigManager::getConfigFilePath() const {
    return m_configFilePath;
}

int ConfigManager::getCheckInterval() const {
    return m_checkInterval;
}

void ConfigManager::setCheckInterval(int interval) {
    m_checkInterval = interval;
}

bool ConfigManager::isAutoStart() const {
    return m_autoStart;
}

void ConfigManager::setAutoStart(bool autoStart) {
    m_autoStart = autoStart;
}

int ConfigManager::getWindowWidth() const {
    return m_windowWidth;
}

void ConfigManager::setWindowWidth(int width) {
    m_windowWidth = width;
}

int ConfigManager::getWindowHeight() const {
    return m_windowHeight;
}

void ConfigManager::setWindowHeight(int height) {
    m_windowHeight = height;
}

// 页面管理
QList<MonitorPage>& ConfigManager::getPages() {
    return m_pages;
}

const QList<MonitorPage>& ConfigManager::getPages() const {
    return m_pages;
}

int ConfigManager::getPageCount() const {
    return m_pages.size();
}

void ConfigManager::addPage(const MonitorPage& page) {
    m_pages.append(page);
}

void ConfigManager::removePage(const QString& pageId) {
    for (int i = 0; i < m_pages.size(); ++i) {
        if (m_pages[i].getId() == pageId) {
            m_pages.removeAt(i);
            break;
        }
    }
}

void ConfigManager::removePage(int index) {
    if (index >= 0 && index < m_pages.size()) {
        m_pages.removeAt(index);
    }
}

MonitorPage* ConfigManager::findPage(const QString& pageId) {
    for (int i = 0; i < m_pages.size(); ++i) {
        if (m_pages[i].getId() == pageId) {
            return &m_pages[i];
        }
    }
    return nullptr;
}

int ConfigManager::findPageIndex(const QString& pageId) const {
    for (int i = 0; i < m_pages.size(); ++i) {
        if (m_pages[i].getId() == pageId) {
            return i;
        }
    }
    return -1;
}

void ConfigManager::clearPages() {
    m_pages.clear();
}

bool ConfigManager::validateConfiguration() const {
    // 检查基本设置
    if (m_checkInterval < 1000) { // 最小1秒
        return false;
    }

    // 检查页面ID唯一性
    QStringList pageIds;
    for (const auto& page : m_pages) {
        if (pageIds.contains(page.getId())) {
            return false;
        }
        pageIds.append(page.getId());

        // 检查页面内项目ID唯一性
        QStringList itemIds;
        for (const auto& item : page.getItems()) {
            if (itemIds.contains(item.getId())) {
                return false;
            }
            itemIds.append(item.getId());
        }
    }

    return true;
}

void ConfigManager::createDefaultConfiguration() {
    m_pages.clear();

    // 创建默认页面
    MonitorPage defaultPage("", QString::fromStdWString(L"默认页面"));

    // 添加一些示例项目
    MonitorItem registryItem("", MonitorItem::Registry, QString::fromStdWString(L"系统版本"),
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProductName");
    MonitorItem fileItem("", MonitorItem::File, QString::fromStdWString(L"系统文件"),
        "C:\\Windows\\System32\\kernel32.dll");

    defaultPage.addItem(registryItem);
    defaultPage.addItem(fileItem);

    m_pages.append(defaultPage);
}

QJsonObject ConfigManager::createDefaultConfig() const {
    QJsonObject config;
    config["version"] = m_version;

    QJsonObject globalSettings;
    globalSettings["checkInterval"] = m_checkInterval;
    globalSettings["autoStart"] = m_autoStart;
    globalSettings["windowWidth"] = m_windowWidth;
    globalSettings["windowHeight"] = m_windowHeight;
    config["globalSettings"] = globalSettings;

    config["pages"] = QJsonArray();

    return config;
}

bool ConfigManager::parseConfiguration(const QJsonObject& config) {
    // 解析版本
    m_version = config["version"].toString("1.0");

    // 解析全局设置
    QJsonObject globalSettings = config["globalSettings"].toObject();
    m_checkInterval = globalSettings["checkInterval"].toInt(60000);
    m_autoStart = globalSettings["autoStart"].toBool(true);
    m_windowWidth = globalSettings["windowWidth"].toInt(0);
    m_windowHeight = globalSettings["windowHeight"].toInt(0);

    // 解析页面列表
    m_pages.clear();
    QJsonArray pagesArray = config["pages"].toArray();
    for (const auto& value : pagesArray) {
        MonitorPage page;
        page.fromJson(value.toObject());
        m_pages.append(page);
    }

    // 如果没有页面，创建默认页面
    if (m_pages.isEmpty()) {
        createDefaultConfiguration();
    }

    return validateConfiguration();
}
