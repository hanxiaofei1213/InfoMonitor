#include "UIConfigManager.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

UIConfigManager::UIConfigManager(QObject* parent)
    : QObject(parent)
    , m_settings(GetConfigFilePath(), QSettings::IniFormat) {
}

UIConfigManager::~UIConfigManager() {
    m_settings.sync();
}

void UIConfigManager::SetWindowWidth(int nWidth) {
    m_settings.setValue("Window/Width", nWidth);
}

int UIConfigManager::GetWindowWidth(int defaultValue) const {
    return m_settings.value("Window/Width", defaultValue).toInt();
}

void UIConfigManager::SetWindowHeight(int nHeight) {
    m_settings.setValue("Window/Height", nHeight);
}

int UIConfigManager::GetWindowHeight(int defaultValue) const {
    return m_settings.value("Window/Height", defaultValue).toInt();
}

QString UIConfigManager::GetConfigFilePath() const {
    QString appDir = QCoreApplication::applicationDirPath();
    return QDir(appDir).filePath("uiconfig.ini");
}

