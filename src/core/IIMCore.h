#pragma once
#include <vector>
#include "IMCoreDefine.h"


// todo(wangwenxi)��������ñ���Qt����Ϊ����Ҫ������չ����ĵط�

class IIMCore {
public:
    virtual ~IIMCore() = default;

    // ��ɾ�Ĳ�ҳ��
    virtual std::vector<MonitorPageInfo> QueryMonitorPages() = 0;

    

    // ��ɾ�Ĳ�item����Ҫ��ҳ��id
    virtual std::vector<MonitorItemInfo> QueryMonitorItems(const QString& strPageId) = 0;


    // ֪ͨ���������Ѿ����ˣ������ûص�������ʵ��

};

