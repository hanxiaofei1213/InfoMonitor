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

    QVector<MonitorPageWidget*> CreateMonitorPages(QWidget* pPageParent);

protected:
    MonitorPageWidget* CreateDefaultMonitorPage(QWidget* pPageParent);    


private:
    InfoMonitorCoreWrapper* m_pCoreWrapper = nullptr;

};

