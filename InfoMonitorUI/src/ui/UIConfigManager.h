#pragma once
#include <QObject>
#include <QString>
#include <QSettings>

class UIConfigManager : public QObject {
    Q_OBJECT

public:
    UIConfigManager(QObject* pParent);
    ~UIConfigManager();

    void SetWindowWidth(int nWidth);
    int GetWindowWidth(int defaultValue = 0) const;

    void SetWindowHeight(int nHeight);
    int GetWindowHeight(int defaultValue = 0) const;

protected:
    QString GetConfigFilePath() const;

private:
    QSettings m_settings;
};

