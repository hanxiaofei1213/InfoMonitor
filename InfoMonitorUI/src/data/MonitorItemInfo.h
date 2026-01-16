#pragma once
#include <QString>
#include <QDateTime>
#include <QJsonObject>

struct MonitorItemInfo {
    enum Type {
        Registry,
        File
    };

    // 数据成员（公共访问）
    QString id;
    Type type;
    QString name;
    QString path;
    bool enabled = true;
    QDateTime lastCheckTime;
    QString lastStatus;
    QString errorMessage;
};

// todo(wangwenxi): 这里要不加一个结果的结构体吧
