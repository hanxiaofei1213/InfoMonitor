#include "../ui/UIConfigManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <chrono>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << QString::fromStdWString(L"测试UIConfigManager开始...");
    qDebug() << QString::fromStdWString(L"优化版本: 直接写入配置文件 + 无检查 + 无缓存");
    
    // 创建UIConfigManager实例
    auto start = std::chrono::high_resolution_clock::now();
    UIConfigManager configManager(nullptr);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug() << QString::fromStdWString(L"创建实例耗时:") << duration.count() << "μs";
    
    // 测试初始值
    qDebug() << QString::fromStdWString(L"初始值:");
    qDebug() << "  Width:" << configManager.GetWindowWidth();
    qDebug() << "  Height:" << configManager.GetWindowHeight();
    
    // 性能测试：100次设置操作
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        configManager.SetWindowWidth(800 + i);
        configManager.SetWindowHeight(600 + i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug() << "100次设置操作耗时:" << duration.count() << "微秒";
    
    // 设置最终值
    qDebug() << QString::fromStdWString(L"设置最终值: Width=800, Height=600");
    configManager.SetWindowWidth(800);
    configManager.SetWindowHeight(600);
    
    // 验证设置
    qDebug() << QString::fromStdWString(L"最终值:");
    qDebug() << "  Width:" << configManager.GetWindowWidth();
    qDebug() << "  Height:" << configManager.GetWindowHeight();
    
    // 检查配置文件是否存在
    QString configPath = configManager.GetConfigFilePath();
    QFile configFile(configPath);
    if (configFile.exists()) {
        qDebug() << QString::fromStdWString(L"配置文件已创建:") << configPath;
        
        // 显示配置文件内容
        if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&configFile);
            QString content = stream.readAll();
            configFile.close();
            
            qDebug() << QString::fromStdWString(L"配置文件内容:");
            qDebug() << content;
        }
    } else {
        qDebug() << QString::fromStdWString(L"配置文件未找到:");
    }
    
    qDebug() << QString::fromStdWString(L"测试UIConfigManager完成！");
    qDebug() << QString::fromStdWString(L"优化效果: 直接写入配置文件 + 无值检查 + 极简设计");
    
    return 0;
}