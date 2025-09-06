#include "InfoMonitor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("InfoMonitor");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("InfoMonitor");
    
    InfoMonitor window;
    window.show();
    
    return app.exec();
}
