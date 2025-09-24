#include "MonitorItem.h"
#include <QUuid>
#include <QDebug>

MonitorItem::MonitorItem()
    : m_id(generateId())
    , m_type(File)
    , m_enabled(true)
    , m_lastCheckTime()
    , m_lastStatus()
    , m_errorMessage()
{
}

MonitorItem::MonitorItem(const QString& id, Type type, const QString& name, const QString& path)
    : m_id(id.isEmpty() ? generateId() : id)
    , m_type(type)
    , m_name(name)
    , m_path(path)
    , m_enabled(true)
{
}

QString MonitorItem::checkStatus()
{
    m_errorMessage.clear();
    m_lastCheckTime = QDateTime::currentDateTime();
    
    if (m_type == Registry) {
        m_lastStatus = checkRegistryValue();
    } else {
        m_lastStatus = checkFileExists();
    }
    
    return m_lastStatus;
}

QString MonitorItem::checkRegistryValue()
{
    if (m_path.isEmpty()) {
        return QString::fromStdWString(L"路径为空");
    }
    
    // 解析注册表路径
    QString path = m_path;
    HKEY rootKey = nullptr;
    QString subKeyPath;
    QString valueName;
    
    // 注册表根键映射表
    static const QMap<QString, HKEY> rootKeyMap = {
        {"HKEY_LOCAL_MACHINE\\", HKEY_LOCAL_MACHINE},
        {"HKEY_CURRENT_USER\\", HKEY_CURRENT_USER},
        {"HKEY_CLASSES_ROOT\\", HKEY_CLASSES_ROOT},
        {"HKEY_USERS\\", HKEY_USERS},
        {"HKEY_CURRENT_CONFIG\\", HKEY_CURRENT_CONFIG}
    };
    
    // 查找匹配的根键
    QString matchedPrefix;
    for (auto it = rootKeyMap.begin(); it != rootKeyMap.end(); ++it) {
        if (path.startsWith(it.key())) {
            rootKey = it.value();
            matchedPrefix = it.key();
            break;
        }
    }
    
    if (!rootKey) {
        return QString::fromStdWString(L"无效的注册表根键，请使用完整格式如：HKEY_LOCAL_MACHINE\\...");
    }
    
    // 移除根键前缀
    path = path.mid(matchedPrefix.length());
    
    // 分离子键路径和值名
    int lastBackslash = path.lastIndexOf('\\');
    if (lastBackslash == -1) {
        subKeyPath = "";
        valueName = path;
    } else {
        subKeyPath = path.left(lastBackslash);
        valueName = path.mid(lastBackslash + 1);
    }
    
    // 打开注册表键
    HKEY hKey;
    LONG result = RegOpenKeyExA(rootKey, subKeyPath.toLocal8Bit().constData(), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        if (result == ERROR_FILE_NOT_FOUND) {
            return QString::fromStdWString(L"不存在");
        } else if (result == ERROR_ACCESS_DENIED) {
            return QString::fromStdWString(L"访问被拒绝");
        } else {
            return QString::fromStdWString(L"打开注册表键失败 (错误代码: %1)").arg(result);
        }
    }
    
    // 查询值
    DWORD dataType;
    DWORD dataSize = 0;
    result = RegQueryValueExA(hKey, valueName.toLocal8Bit().constData(), nullptr, &dataType, nullptr, &dataSize);
    
    if (result == ERROR_FILE_NOT_FOUND) {
        RegCloseKey(hKey);
        return QString::fromStdWString(L"不存在");
    } else if (result != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return QString::fromStdWString(L"查询注册表值失败 (错误代码: %1)").arg(result);
    }
    
    // 读取值数据
    QByteArray data(dataSize, 0);
    result = RegQueryValueExA(hKey, valueName.toLocal8Bit().constData(), nullptr, &dataType, 
                             reinterpret_cast<LPBYTE>(data.data()), &dataSize);
    RegCloseKey(hKey);
    
    if (result != ERROR_SUCCESS) {
        return QString::fromStdWString(L"读取注册表值失败 (错误代码: %1)").arg(result);
    }
    
    // 根据数据类型格式化输出
    QString value;
    switch (dataType) {
        case REG_SZ:
        case REG_EXPAND_SZ:
            value = QString::fromLocal8Bit(data.constData());
            break;
        case REG_DWORD:
        case REG_QWORD:
            if (dataType == REG_DWORD && dataSize >= sizeof(DWORD)) {
                DWORD dwordValue = *reinterpret_cast<const DWORD*>(data.constData());
                value = QString::number(dwordValue);
            } else if (dataType == REG_QWORD && dataSize >= sizeof(ULONGLONG)) {
                ULONGLONG qwordValue = *reinterpret_cast<const ULONGLONG*>(data.constData());
                value = QString::number(qwordValue);
            } else {
                value = QString::fromStdWString(L"整数数据 (%1 字节)").arg(dataSize);
            }
            break;
        case REG_BINARY:
            value = QString::fromStdWString(L"二进制数据 (%1 字节)").arg(dataSize);
            break;
        case REG_MULTI_SZ:
            value = QString::fromStdWString(L"多字符串数据 (%1 字节)").arg(dataSize);
            break;
        default:
            value = QString::fromStdWString(L"未知类型 (%1, %2 字节)").arg(dataType).arg(dataSize);
            break;
    }
    
    return value;
}

QString MonitorItem::checkFileExists()
{
    if (m_path.isEmpty()) {
        return QString::fromStdWString(L"路径为空");
    }
    
    QFileInfo fileInfo(m_path);
    return fileInfo.exists() ? QString::fromStdWString(L"存在") : QString::fromStdWString(L"不存在");
}

QJsonObject MonitorItem::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["type"] = typeToString(m_type);
    json["name"] = m_name;
    json["path"] = m_path;
    json["enabled"] = m_enabled;
    json["lastCheckTime"] = m_lastCheckTime.toString(Qt::ISODate);
    json["lastStatus"] = m_lastStatus;
    json["errorMessage"] = m_errorMessage;
    return json;
}

void MonitorItem::fromJson(const QJsonObject& json)
{
    m_id = json["id"].toString();
    m_type = stringToType(json["type"].toString());
    m_name = json["name"].toString();
    m_path = json["path"].toString();
    m_enabled = json["enabled"].toBool(true);
    m_lastCheckTime = QDateTime::fromString(json["lastCheckTime"].toString(), Qt::ISODate);
    m_lastStatus = json["lastStatus"].toString();
    m_errorMessage = json["errorMessage"].toString();
}

QString MonitorItem::generateId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString MonitorItem::typeToString(Type type)
{
    switch (type) {
        case Registry: return "registry";
        case File: return "file";
        default: return "unknown";
    }
}

MonitorItem::Type MonitorItem::stringToType(const QString& typeStr)
{
    if (typeStr == "registry") return Registry;
    if (typeStr == "file") return File;
    return File; // 默认为文件类型
}
