#include "MonitorPage.h"
#include <QUuid>

MonitorPage::MonitorPage()
    : m_id(generateId())
    , m_name(QString::fromStdWString(L"新页面"))
    , m_enabled(true)
{
}

MonitorPage::MonitorPage(const QString& id, const QString& name)
    : m_id(id.isEmpty() ? generateId() : id)
    , m_name(name)
    , m_enabled(true)
{
}

void MonitorPage::addItem(const MonitorItem& item)
{
    m_items.append(item);
}

void MonitorPage::removeItem(const QString& itemId)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].getId() == itemId) {
            m_items.removeAt(i);
            break;
        }
    }
}

void MonitorPage::removeItem(int index)
{
    if (index >= 0 && index < m_items.size()) {
        m_items.removeAt(index);
    }
}

MonitorItem* MonitorPage::findItem(const QString& itemId)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].getId() == itemId) {
            return &m_items[i];
        }
    }
    return nullptr;
}

int MonitorPage::findItemIndex(const QString& itemId) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].getId() == itemId) {
            return i;
        }
    }
    return -1;
}

void MonitorPage::clearItems()
{
    m_items.clear();
}

void MonitorPage::setColumnWidth(int columnIndex, int width)
{
    m_columnWidths[columnIndex] = width;
}

int MonitorPage::getColumnWidth(int columnIndex, int defaultWidth) const
{
    if (m_columnWidths.contains(columnIndex)) {
        return m_columnWidths[columnIndex];
    }
    return defaultWidth;
}

int MonitorPage::getEnabledItemCount() const
{
    int count = 0;
    for (const auto& item : m_items) {
        if (item.isEnabled()) {
            count++;
        }
    }
    return count;
}

void MonitorPage::refreshAll()
{
    for (auto& item : m_items) {
        item.checkStatus();
    }
}

void MonitorPage::refreshEnabledItems()
{
    if (!m_enabled) {
        return;
    }
    
    for (auto& item : m_items) {
        if (item.isEnabled()) {
            item.checkStatus();
        }
    }
}

QJsonObject MonitorPage::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["enabled"] = m_enabled;
    
    QJsonArray itemsArray;
    for (const auto& item : m_items) {
        itemsArray.append(item.toJson());
    }
    json["items"] = itemsArray;
    
    // 保存表格列宽配置
    QJsonObject columnWidths;
    for (auto it = m_columnWidths.constBegin(); it != m_columnWidths.constEnd(); ++it) {
        columnWidths[QString::number(it.key())] = it.value();
    }
    json["columnWidths"] = columnWidths;
    
    return json;
}

void MonitorPage::fromJson(const QJsonObject& json)
{
    m_id = json["id"].toString();
    m_name = json["name"].toString();
    m_enabled = json["enabled"].toBool(true);
    
    m_items.clear();
    QJsonArray itemsArray = json["items"].toArray();
    for (const auto& value : itemsArray) {
        MonitorItem item;
        item.fromJson(value.toObject());
        m_items.append(item);
    }
    
    // 加载表格列宽配置
    m_columnWidths.clear();
    if (json.contains("columnWidths") && json["columnWidths"].isObject()) {
        QJsonObject columnWidths = json["columnWidths"].toObject();
        for (auto it = columnWidths.constBegin(); it != columnWidths.constEnd(); ++it) {
            bool ok;
            int columnIndex = it.key().toInt(&ok);
            if (ok) {
                m_columnWidths[columnIndex] = it.value().toInt();
            }
        }
    }
}

QString MonitorPage::generateId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
