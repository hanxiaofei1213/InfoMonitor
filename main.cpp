#include "InfoMonitor.h"
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QString>

// todo(wangwenxi): 监控文件夹支持快速打开文件夹，监控文件支持快速运行

// todo(wangwenxi): 注册表最好能支持简单修改


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("InfoMonitor");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("InfoMonitor");

    // 单例检测
    QSharedMemory sharedMemory("InfoMonitor_SingleInstance");
    if (!sharedMemory.create(1)) {
        // 如果创建失败，说明已经有实例在运行
        QMessageBox::information(nullptr, QString::fromStdWString(L"InfoMonitor"),
            QString::fromStdWString(L"程序已经在运行中！"));
        return 0;
    }

    // 获取单例实例并显示
    InfoMonitor& window = InfoMonitor::getInstance();
    window.show();

    return app.exec();
}
