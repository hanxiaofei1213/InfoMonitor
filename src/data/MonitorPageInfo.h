#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <Windows.h>
#include "../MonitorItem.h"

/**
 * @brief 监控页面数据结构体
 *
 * 纯数据承载结构，不包含业务逻辑
 */
struct MonitorPageInfo {
    // 基本属性
    QString id;                     // 页面唯一标识
    QString name;                   // 页面名称
    QString url;                    // 页面URL
    bool enabled = true;            // 是否启用

    // 监控项目列表
    QList<MonitorItemInfo> items;

    // 界面配置
    QMap<int, int> columnWidths;    // 表格列宽配置

    // 状态信息
    DWORD lastCheckTime;        // 最后检查时间



};
