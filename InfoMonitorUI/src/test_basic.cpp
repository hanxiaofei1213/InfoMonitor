// 基本功能测试
#include "MonitorItem.h"
#include "data/MonitorPage.h"
#include "ConfigManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "=== InfoMonitor " << QString::fromStdWString(L"基本功能测试").toStdString() << " ===" << std::endl;
    
    // 测试 MonitorItem
    std::cout << "\n1. " << QString::fromStdWString(L"测试 MonitorItem").toStdString() << "..." << std::endl;
    MonitorItem item1("", MonitorItem::File, QString::fromStdWString(L"测试文件"), "C:\\Windows\\System32\\kernel32.dll");
    QString status1 = item1.checkStatus();
    std::cout << QString::fromStdWString(L"文件监控测试").toStdString() << ": " << status1.toStdString() << std::endl;
    
    MonitorItem item2("", MonitorItem::Registry, QString::fromStdWString(L"系统版本"), 
                     "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProductName");
    QString status2 = item2.checkStatus();
    std::cout << QString::fromStdWString(L"注册表监控测试").toStdString() << ": " << status2.toStdString() << std::endl;
    
    // 测试 MonitorPage
    std::cout << "\n2. " << QString::fromStdWString(L"测试 MonitorPage").toStdString() << "..." << std::endl;
    MonitorPage page("", QString::fromStdWString(L"测试页面"));
    page.addItem(item1);
    page.addItem(item2);
    std::cout << QString::fromStdWString(L"页面项目数量").toStdString() << ": " << page.getItemCount() << std::endl;
    
    // 测试配置管理
    std::cout << "\n3. " << QString::fromStdWString(L"测试 ConfigManager").toStdString() << "..." << std::endl;
    ConfigManager config;
    config.addPage(page);
    
    bool saveResult = config.saveConfiguration();
    std::cout << QString::fromStdWString(L"配置保存结果").toStdString() << ": " << (saveResult ? QString::fromStdWString(L"成功").toStdString() : QString::fromStdWString(L"失败").toStdString()) << std::endl;
    
    bool loadResult = config.loadConfiguration();
    std::cout << QString::fromStdWString(L"配置加载结果").toStdString() << ": " << (loadResult ? QString::fromStdWString(L"成功").toStdString() : QString::fromStdWString(L"失败").toStdString()) << std::endl;
    std::cout << QString::fromStdWString(L"加载后页面数量").toStdString() << ": " << config.getPageCount() << std::endl;
    
    // 测试JSON序列化
    std::cout << "\n4. " << QString::fromStdWString(L"测试 JSON 序列化").toStdString() << "..." << std::endl;
    QJsonObject itemJson = item1.toJson();
    std::cout << "MonitorItem JSON " << QString::fromStdWString(L"序列化").toStdString() << ": " << QString::fromStdWString(L"成功").toStdString() << std::endl;
    
    MonitorItem item3;
    item3.fromJson(itemJson);
    std::cout << "MonitorItem JSON " << QString::fromStdWString(L"反序列化").toStdString() << ": " << (item3.getName() == item1.getName() ? QString::fromStdWString(L"成功").toStdString() : QString::fromStdWString(L"失败").toStdString()) << std::endl;
    
    std::cout << "\n=== " << QString::fromStdWString(L"测试完成").toStdString() << " ===" << std::endl;
    
    return 0;
}
