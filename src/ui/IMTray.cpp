#include "IMTray.h"
#include <QCoreApplication>
#include <QDebug>

IMTray::IMTray(QObject* parent) : QObject(parent) {
    // 创建托盘图标
    m_trayIcon = new QSystemTrayIcon(this);
    
    // 初始化菜单
    initMenu();
    
    // 连接信号槽
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &IMTray::onTrayIconActivated);
}

IMTray::~IMTray() {
    // 清理资源
}

void IMTray::initMenu() {
    
    // 创建显示窗口动作并使用对象树管理
    m_showAction = new QAction(QString::fromStdWString(L"显示窗口"), &m_trayMenu);
    connect(m_showAction, &QAction::triggered, this, &IMTray::onShowWindow);
    
    // 创建退出动作并使用对象树管理
    m_exitAction = new QAction(QString::fromStdWString(L"退出"), &m_trayMenu);
    connect(m_exitAction, &QAction::triggered, this, &IMTray::onExitApplication);

    // 添加动作到菜单
    m_trayMenu.addAction(m_showAction);
    m_trayMenu.addSeparator();
    m_trayMenu.addAction(m_exitAction);

    // 设置托盘菜单
    m_trayIcon->setContextMenu(&m_trayMenu);
}

void IMTray::setIcon(const QIcon& icon) {
    if (m_trayIcon) {
        m_trayIcon->setIcon(icon);
    }
}

void IMTray::setToolTip(const QString& toolTip) {
    if (m_trayIcon) {
        m_trayIcon->setToolTip(toolTip);
    }
}

void IMTray::showMessage(const QString& title, const QString& message,
                         QSystemTrayIcon::MessageIcon icon, int msecs) {
    if (m_trayIcon && isSystemTrayAvailable()) {
        m_trayIcon->showMessage(title, message, icon, msecs);
    }
}

void IMTray::show() {
    if (m_trayIcon && isSystemTrayAvailable()) {
        m_trayIcon->show();
    }
}

void IMTray::hide() {
    if (m_trayIcon) {
        m_trayIcon->hide();
    }
}

bool IMTray::isSystemTrayAvailable() const {
    return QSystemTrayIcon::isSystemTrayAvailable();
}

void IMTray::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    // 双击托盘图标显示窗口
    if (reason == QSystemTrayIcon::DoubleClick) {
        emit showWindowRequested();
    }
}

void IMTray::onShowWindow() {
    emit showWindowRequested();
}

void IMTray::onExitApplication() {
    emit exitApplicationRequested();
}