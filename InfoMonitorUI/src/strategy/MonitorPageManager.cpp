#include "MonitorPageManager.h"
#include "src/data/MonitorPageInfo.h"

MonitorPageManager::MonitorPageManager(InfoMonitorCoreWrapper* pCoreWrapper, QObject* pParent) : QObject(pParent), m_pCoreWrapper(pCoreWrapper) {

}

MonitorPageManager::~MonitorPageManager() {

}

// todo(wangwenxi)：如果配置文件是空的，则创建一个测试的页面
QVector<MonitorPageWidget*> MonitorPageManager::CreateMonitorPages(QWidget* pPageParent) {
    if (!m_pCoreWrapper) {
        assert(false && "core wrapper shouldn't be empty");
        return {};
    }


    return {};
}

MonitorPageWidget* MonitorPageManager::CreateDefaultMonitorPage(QWidget* pPageParent) {

    
    return nullptr;
}