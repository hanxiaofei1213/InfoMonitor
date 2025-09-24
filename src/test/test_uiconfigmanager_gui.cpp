#include "UIConfigManager.h"
#include <QApplication>
#include <QMainWindow>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建UI配置管理器
    UIConfigManager uiConfig(&app);
    
    // 创建主窗口
    QMainWindow mainWindow;
    
    // 如果有保存的窗口尺寸，使用它
    int savedWidth = uiConfig.GetWindowWidth();
    int savedHeight = uiConfig.GetWindowHeight();
    
    if (savedWidth > 0 && savedHeight > 0) {
        mainWindow.resize(savedWidth, savedHeight);
        qDebug() << QString::fromStdWString(L"使用保存的窗口尺寸:") 
                 << savedWidth << "x" << savedHeight;
    } else {
        mainWindow.resize(800, 600); // 默认尺寸
        qDebug() << QString::fromStdWString(L"使用默认窗口尺寸: 800x600");
    }
    
    // 连接窗口大小改变信号到配置保存
    QObject::connect(&mainWindow, &QMainWindow::resizeEvent, 
        [&uiConfig, &mainWindow](QResizeEvent* event) {
            Q_UNUSED(event)
            uiConfig.SetWindowWidth(mainWindow.width());
            uiConfig.SetWindowHeight(mainWindow.height());
            qDebug() << QString::fromStdWString(L"窗口尺寸已保存:") 
                     << mainWindow.width() << "x" << mainWindow.height();
        });
    
    mainWindow.show();
    
    return app.exec();
}