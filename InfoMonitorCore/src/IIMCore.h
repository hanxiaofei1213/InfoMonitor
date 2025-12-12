#pragma once
#include <vector>
#include "IMCoreDefine.h"
#include "../data/MonitorItemInfo.h"


// todo(wangwenxi)：这里最好别用Qt，因为我需要把它扩展到别的地方

// todo(wangwenxi): 读写配置也放在这里

class IIMCore {
public:
    virtual ~IIMCore() = default;

    // 增删改查页面
    virtual std::vector<MonitorPageInfo> QueryMonitorPages() = 0;

    

    // 增删改查item，需要传页面id
    virtual std::vector<MonitorItemInfo> QueryMonitorItems(const QString& strPageId) = 0;


    // 通知外面数据已经变了，可以用回调函数来实现

};

