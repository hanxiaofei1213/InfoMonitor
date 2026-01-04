#include "MonitorPageManager.h"

MonitorPageManager::MonitorPageManager(InfoMonitorCoreWrapper* pCoreWrapper, QObject* pParent) : QObject(pParent), m_pCoreWrapper(pCoreWrapper) {

}

MonitorPageManager::~MonitorPageManager() {

}

// todo(wangwenxi)：如果配置文件是空的，则创建一个测试的页面
QVector<MonitorPageWidget*> MonitorPageManager::CreateMonitorPages() {
    if (!m_pCoreWrapper) {
        assert(false && "core wrapper shouldn't be empty");
        return;
    }



    return {};
}
