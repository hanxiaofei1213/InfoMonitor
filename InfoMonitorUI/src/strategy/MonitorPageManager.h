#pragma once
#include <QObject>
#include <QVector>

class MonitorPageWidget;
class InfoMonitorCoreWrapper;

class MonitorPageManager : public QObject {
    Q_OBJECT
public:
    MonitorPageManager(InfoMonitorCoreWrapper* pCoreWrapper, QObject* pParent);
    ~MonitorPageManager();

    QVector<MonitorPageWidget*> CreateMonitorPages();


private:
    InfoMonitorCoreWrapper* m_pCoreWrapper = nullptr;

};

