#pragma once
#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QIcon>

class IMTray : public QObject {
    Q_OBJECT

public:
    explicit IMTray(QObject* parent = nullptr);
    ~IMTray();

    void setIcon(const QIcon& icon);
    void setToolTip(const QString& toolTip);
    void showMessage(const QString& title, const QString& message,
                     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information,
                     int msecs = 10000);

    // 显示/隐藏托盘图标
    void show();
    void hide();

    // 检查系统是否支持托盘
    bool isSystemTrayAvailable() const;

signals:
    // 当用户点击显示窗口时发出
    void showWindowRequested();

    // 当用户点击退出应用时发出
    void exitApplicationRequested();

protected slots:
    // 处理托盘图标被激活的事件
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    // 处理显示窗口的槽函数
    void onShowWindow();

    // 处理退出应用的槽函数
    void onExitApplication();

protected:
    // 初始化托盘菜单
    void initMenu();

private:
    QSystemTrayIcon* m_trayIcon = nullptr;
    QMenu m_trayMenu;
    QAction* m_showAction = nullptr;
    QAction* m_exitAction = nullptr;
};

