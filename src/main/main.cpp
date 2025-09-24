#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QString>
#include "src\ui\InfoMonitor.h"

// todo(wangwenxi): 监控文件夹支持快速打开文件夹，监控文件支持快速运行

// todo(wangwenxi): 注册表最好能支持简单修改

// todo(wangwenxi)：加一个日志模块

bool IsAlreadyRunning();


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("InfoMonitor");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("InfoMonitor");

    if (IsAlreadyRunning()) {
        return -1;
    }

    InfoMonitor window;
    window.show();

    return app.exec();

    return 0;
}

bool IsAlreadyRunning() {
    QSharedMemory sharedMemory("InfoMonitor_SingleInstance");
    if (!sharedMemory.create(1)) {
        // 如果创建失败，说明已经有实例在运行
        QMessageBox::information(nullptr, QString::fromStdWString(L"InfoMonitor"),QString::fromStdWString(L"程序已经在运行中！"));
        return true;
    }

    return false;
}
