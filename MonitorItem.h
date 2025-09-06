#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QFileInfo>
#include <QMap>
#include <windows.h>

class MonitorItem
{
public:
    enum Type {
        Registry,
        File
    };

    MonitorItem();
    MonitorItem(const QString& id, Type type, const QString& name, const QString& path);
    
    // 属性访问
    QString getId() const { return m_id; }
    void setId(const QString& id) { m_id = id; }
    
    Type getType() const { return m_type; }
    void setType(Type type) { m_type = type; }
    
    QString getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    QString getPath() const { return m_path; }
    void setPath(const QString& path) { m_path = path; }
    
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    
    QDateTime getLastCheckTime() const { return m_lastCheckTime; }
    void setLastCheckTime(const QDateTime& time) { m_lastCheckTime = time; }
    
    QString getLastStatus() const { return m_lastStatus; }
    void setLastStatus(const QString& status) { m_lastStatus = status; }
    
    QString getErrorMessage() const { return m_errorMessage; }
    void setErrorMessage(const QString& error) { m_errorMessage = error; }
    
    // 功能方法
    QString checkStatus(); // 执行实际检查
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    
    // 静态方法
    static QString generateId();
    static QString typeToString(Type type);
    static Type stringToType(const QString& typeStr);

private:
    QString checkRegistryValue();
    QString checkFileExists();
    
private:
    QString m_id;
    Type m_type;
    QString m_name;
    QString m_path;
    bool m_enabled;
    QDateTime m_lastCheckTime;
    QString m_lastStatus;
    QString m_errorMessage;
};
