#include "IMActionBar.h"
#include <QMenu>
#include <QMessageBox>
#include <QKeySequence>

IMActionBar::IMActionBar(QMainWindow* pMainWindow, QObject* parent)
    : QObject(parent)
    , m_pMainWindow(pMainWindow) {
    

}

IMActionBar::~IMActionBar() {
}

void IMActionBar::InitUI() {
    InitMenuBar();
    InitToolBar();
    InitStatusBar();
}

void IMActionBar::InitMenuBar() {
    if (!m_pMainWindow) {
        return;
    }

    QMenuBar* pMenuBar = m_pMainWindow->menuBar();
    if (!pMenuBar) {
        return;
    }

    // 文件菜单
    QMenu* pFileMenu = pMenuBar->addMenu(QString::fromStdWString(L"文件(&F)"));
    pFileMenu->addAction(QString::fromStdWString(L"新建页面(&N)"), this, &IMActionBar::addPageRequested, QKeySequence::New);
    pFileMenu->addSeparator();
    pFileMenu->addAction(QString::fromStdWString(L"退出(&X)"), m_pMainWindow, &QWidget::close, QKeySequence::Quit);

    // 编辑菜单
    QMenu* pEditMenu = pMenuBar->addMenu(QString::fromStdWString(L"编辑(&E)"));
    pEditMenu->addAction(QString::fromStdWString(L"添加监控项(&A)"), this, &IMActionBar::addItemRequested, QKeySequence("Ctrl+A"));
    pEditMenu->addAction(QString::fromStdWString(L"删除监控项(&D)"), this, &IMActionBar::deleteItemRequested, QKeySequence::Delete);
    pEditMenu->addSeparator();
    pEditMenu->addAction(QString::fromStdWString(L"刷新当前页(&R)"), this, &IMActionBar::refreshPageRequested, QKeySequence::Refresh);

    // 视图菜单
    QMenu* pViewMenu = pMenuBar->addMenu(QString::fromStdWString(L"视图(&V)"));
    QAction* pToolBarAction = pViewMenu->addAction(QString::fromStdWString(L"工具栏"));
    pToolBarAction->setCheckable(true);
    pToolBarAction->setChecked(true);
    
    QAction* pStatusBarAction = pViewMenu->addAction(QString::fromStdWString(L"状态栏"));
    pStatusBarAction->setCheckable(true);
    pStatusBarAction->setChecked(true);

    // 帮助菜单
    QMenu* pHelpMenu = pMenuBar->addMenu(QString::fromStdWString(L"帮助(&H)"));
    pHelpMenu->addAction(QString::fromStdWString(L"关于(&A)"), this, &IMActionBar::OnAbout);
}

void IMActionBar::InitToolBar() {
    if (!m_pMainWindow) {
        return;
    }

    m_pToolBar = m_pMainWindow->addToolBar(QString::fromStdWString(L"主工具栏"));
    if (!m_pToolBar) {
        return;
    }

    m_pToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 添加页面
    m_pToolBar->addAction(QString::fromStdWString(L"添加页面"), this, &IMActionBar::addPageRequested);

    m_pToolBar->addSeparator();

    // 添加监控项
    m_pToolBar->addAction(QString::fromStdWString(L"添加项目"), this, &IMActionBar::addItemRequested);

    // 刷新页面
    m_pToolBar->addAction(QString::fromStdWString(L"刷新页面"), this, &IMActionBar::refreshPageRequested);

    m_pToolBar->addSeparator();

    // 开始/停止监控按钮
    m_pStartStopButton = new QPushButton(QString::fromStdWString(L"开始监控"), m_pMainWindow);
    connect(m_pStartStopButton, &QPushButton::clicked, this, &IMActionBar::startStopRequested);
    m_pToolBar->addWidget(m_pStartStopButton);
}

void IMActionBar::InitStatusBar() {
    if (!m_pMainWindow) {
        return;
    }

    m_pStatusBar = m_pMainWindow->statusBar();
    if (!m_pStatusBar) {
        return;
    }

    m_pStatusLabel = new QLabel(QString::fromStdWString(L"就绪"), m_pMainWindow);
    m_pStatusBar->addWidget(m_pStatusLabel);

    m_pStatusBar->addPermanentWidget(new QLabel("|"));

    m_pNextCheckLabel = new QLabel(QString::fromStdWString(L"下次检查：--"), m_pMainWindow);
    m_pStatusBar->addPermanentWidget(m_pNextCheckLabel);

    m_pStatusBar->addPermanentWidget(new QLabel("|"));

    m_pItemCountLabel = new QLabel(QString::fromStdWString(L"项目数：0"), m_pMainWindow);
    m_pStatusBar->addPermanentWidget(m_pItemCountLabel);
}

void IMActionBar::UpdateMonitorStatus(bool bIsRunning) {
    m_bIsMonitoring = bIsRunning;

    if (m_bIsMonitoring) {
        m_pStatusLabel->setText(QString::fromStdWString(L"监控状态：运行中"));
        m_pStartStopButton->setText(QString::fromStdWString(L"停止监控"));
    } else {
        m_pStatusLabel->setText(QString::fromStdWString(L"监控状态：已停止"));
        m_pStartStopButton->setText(QString::fromStdWString(L"开始监控"));
    }
}

void IMActionBar::UpdateItemCount(int nTotal, int nEnabled) {
    m_pItemCountLabel->setText(QString::fromStdWString(L"项目数：%1 (启用：%2)")
        .arg(nTotal)
        .arg(nEnabled));
}

void IMActionBar::UpdateNextCheckTime(int nSeconds) {
    if (m_bIsMonitoring && nSeconds >= 0) {
        m_pNextCheckLabel->setText(QString::fromStdWString(L"下次检查：%1秒后").arg(nSeconds));
    } else {
        m_pNextCheckLabel->setText(QString::fromStdWString(L"下次检查：--"));
    }
}

void IMActionBar::OnAbout() {
    QMessageBox::about(m_pMainWindow, QString::fromStdWString(L"关于 InfoMonitor"),
        QString::fromStdWString(L"InfoMonitor v1.0\n\n"
            L"系统监控工具，用于监控注册表键值和文件状态。\n\n"
            L"功能特性：\n"
            L"• 注册表键值监控\n"
            L"• 文件存在性监控\n"
            L"• 分页管理\n"
            L"• 定时自动检查\n"
            L"• 配置持久化保存"));
}
