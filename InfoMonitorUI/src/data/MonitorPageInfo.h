#pragma once

#include <QString>
#include <QVector>
#include <QMap>
#include <Windows.h>
#include "MonitorItemInfo.h"

/**
 * @brief 监控页面数据结构体
 *
 * 纯数据承载结构，不包含业务逻辑
 */
struct MonitorPageInfo {
    QString srtId;                     // 页面唯一标识
    QString strTitle;                   // 页面名称
    bool bEnabled = true;            // 是否启用

    QVector<MonitorItemInfo> vecItems;
    QMap<int, int> mapColumnWidths;    // 表格列宽配置
    DWORD dwLastCheckTime = 0;        // 最后检查时间
};
