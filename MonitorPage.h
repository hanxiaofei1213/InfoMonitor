#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include "MonitorItem.h"

class MonitorPage
{
public:
    MonitorPage();
    MonitorPage(const QString& id, const QString& name);
    
    // 属性访问
    QString getId() const { return m_id; }
    void setId(const QString& id) { m_id = id; }
    
    QString getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    
    QList<MonitorItem>& getItems() { return m_items; }
    const QList<MonitorItem>& getItems() const { return m_items; }
    
    // 项目管理
    void addItem(const MonitorItem& item);
    void removeItem(const QString& itemId);
    void removeItem(int index);
    MonitorItem* findItem(const QString& itemId);
    int findItemIndex(const QString& itemId) const;
    void clearItems();
    int getItemCount() const { return m_items.size(); }
    int getEnabledItemCount() const;
    
    // 功能方法
    void refreshAll(); // 刷新所有启用的项目
    void refreshEnabledItems(); // 只刷新启用的项目
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    
    // 静态方法
    static QString generateId();

private:
    QString m_id;
    QString m_name;
    bool m_enabled;
    QList<MonitorItem> m_items;
};
