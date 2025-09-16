#include "InfoMonitor.h"
#include "MonitorItem.h"
#include "MonitorPage.h"
#include "MonitorManager.h"
#include "ConfigManager.h"
#include <QApplication>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QComboBox>
#include <QCheckBox>
#include <QMenu>
#include <QAbstractItemView>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QIcon>

InfoMonitor& InfoMonitor::getInstance() {
    static InfoMonitor instance;
    return instance;
}

InfoMonitor::InfoMonitor()
    : QMainWindow(nullptr) {
    // 先创建监控管理器
    m_monitorManager = new MonitorManager(this);
    m_configManager = m_monitorManager->getConfigManager();

    setupUI();

    // 连接信号
    connect(m_monitorManager, &MonitorManager::monitoringStarted, this, &InfoMonitor::onMonitoringUpdate);
    connect(m_monitorManager, &MonitorManager::monitoringStopped, this, &InfoMonitor::onMonitoringUpdate);
    connect(m_monitorManager, &MonitorManager::itemsUpdated, this, &InfoMonitor::onMonitoringUpdate);
    connect(m_monitorManager, &MonitorManager::pageUpdated, this, [this](int pageIndex) {
        if (pageIndex == getCurrentPageIndex()) {
            updateCurrentPageUI();
        }
        updateStatusBar();
        });

    // 加载配置并更新UI
    loadConfiguration();

    // 如果设置了自动启动，则开始监控
    if (m_configManager->isAutoStart()) {
        m_monitorManager->startMonitoring();
        m_isMonitoring = true;
        m_startStopButton->setText(QString::fromStdWString(L"停止监控"));
    }

    updateStatusBar();
}

InfoMonitor::~InfoMonitor() {
    if (m_monitorManager) {
        m_monitorManager->stopMonitoring();
    }
    saveConfiguration();
}

void InfoMonitor::setupUI() {
    setWindowTitle(QString::fromStdWString(L"InfoMonitor - 系统监控工具"));
    setWindowIcon(QIcon(":/InfoMonitor/res/main.png"));
    setMinimumSize(1000, 600);

    // todo(wangwenxi): 这里的大小从配置文件读取
    resize(1800, 1200);

    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupTrayIcon();

    // 创建中央部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建标签页控件
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &InfoMonitor::onTabChanged);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        if (m_tabWidget->count() > 1) { // 至少保留一个标签页
            m_configManager->removePage(index);
            m_tabWidget->removeTab(index);
            saveConfiguration();
        }
        });

    mainLayout->addWidget(m_tabWidget);

    // 创建默认页面（如果没有页面的话）
    if (m_tabWidget->count() == 0) {
        createNewPage(QString::fromStdWString(L"默认页面"));
    }
}

void InfoMonitor::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();

    // 文件菜单
    QMenu* fileMenu = menuBar->addMenu(QString::fromStdWString(L"文件(&F)"));
    fileMenu->addAction(QString::fromStdWString(L"新建页面(&N)"), this, &InfoMonitor::onAddPage, QKeySequence::New);
    fileMenu->addSeparator();
    fileMenu->addAction(QString::fromStdWString(L"退出(&X)"), this, &QWidget::close, QKeySequence::Quit);

    // 编辑菜单
    QMenu* editMenu = menuBar->addMenu(QString::fromStdWString(L"编辑(&E)"));
    editMenu->addAction(QString::fromStdWString(L"添加监控项(&A)"), this, &InfoMonitor::onAddItem, QKeySequence("Ctrl+A"));
    editMenu->addAction(QString::fromStdWString(L"删除监控项(&D)"), this, &InfoMonitor::onDeleteItem, QKeySequence::Delete);
    editMenu->addSeparator();
    editMenu->addAction(QString::fromStdWString(L"刷新当前页(&R)"), this, &InfoMonitor::onRefreshPage, QKeySequence::Refresh);

    // 视图菜单
    QMenu* viewMenu = menuBar->addMenu(QString::fromStdWString(L"视图(&V)"));
    viewMenu->addAction(QString::fromStdWString(L"工具栏"), m_toolBar, &QToolBar::setVisible)->setCheckable(true);
    viewMenu->addAction(QString::fromStdWString(L"状态栏"), m_statusBar, &QStatusBar::setVisible)->setCheckable(true);

    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu(QString::fromStdWString(L"帮助(&H)"));
    helpMenu->addAction(QString::fromStdWString(L"关于(&A)"), this, [this]() {
        QMessageBox::about(this, QString::fromStdWString(L"关于 InfoMonitor"),
            QString::fromStdWString(L"InfoMonitor v1.0\n\n"
                L"系统监控工具，用于监控注册表键值和文件状态。\n\n"
                L"功能特性：\n"
                L"• 注册表键值监控\n"
                L"• 文件存在性监控\n"
                L"• 分页管理\n"
                L"• 定时自动检查\n"
                L"• 配置持久化保存"));
        });
}

void InfoMonitor::setupToolBar() {
    m_toolBar = addToolBar(QString::fromStdWString(L"主工具栏"));
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 添加页面
    m_toolBar->addAction(QString::fromStdWString(L"添加页面"), this, &InfoMonitor::onAddPage);

    m_toolBar->addSeparator();

    // 添加监控项
    m_toolBar->addAction(QString::fromStdWString(L"添加项目"), this, &InfoMonitor::onAddItem);

    // 刷新页面
    m_toolBar->addAction(QString::fromStdWString(L"刷新页面"), this, &InfoMonitor::onRefreshPage);

    m_toolBar->addSeparator();

    // 开始/停止监控
    m_startStopButton = new QPushButton(QString::fromStdWString(L"开始监控"), this);
    connect(m_startStopButton, &QPushButton::clicked, this, &InfoMonitor::onStartStopMonitoring);
    m_toolBar->addWidget(m_startStopButton);
}

void InfoMonitor::setupStatusBar() {
    m_statusBar = statusBar();

    m_statusLabel = new QLabel(QString::fromStdWString(L"就绪"), this);
    m_statusBar->addWidget(m_statusLabel);

    m_statusBar->addPermanentWidget(new QLabel("|"));

    m_nextCheckLabel = new QLabel(QString::fromStdWString(L"下次检查：--"), this);
    m_statusBar->addPermanentWidget(m_nextCheckLabel);

    m_statusBar->addPermanentWidget(new QLabel("|"));

    m_itemCountLabel = new QLabel(QString::fromStdWString(L"项目数：0"), this);
    m_statusBar->addPermanentWidget(m_itemCountLabel);
}

void InfoMonitor::setupTrayIcon() {
    // 检查系统是否支持托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    // 创建托盘图标
    m_trayIcon = new QSystemTrayIcon(this);

    // 设置托盘图标 - 使用程序图标
    QIcon trayIconImage = windowIcon();
    if (trayIconImage.isNull()) {
        // 如果窗口图标为空，直接使用资源文件中的图标
        trayIconImage = QIcon(":/InfoMonitor/res/main.png");
    }

    m_trayIcon->setIcon(trayIconImage);
    m_trayIcon->setToolTip(QString::fromStdWString(L"InfoMonitor - 系统监控工具"));

    // 创建托盘菜单
    m_trayMenu = new QMenu(this);

    m_showAction = new QAction(QString::fromStdWString(L"显示主窗口"), this);
    connect(m_showAction, &QAction::triggered, this, &InfoMonitor::onShowWindow);
    m_trayMenu->addAction(m_showAction);

    m_trayMenu->addSeparator();

    m_exitAction = new QAction(QString::fromStdWString(L"退出"), this);
    connect(m_exitAction, &QAction::triggered, this, &InfoMonitor::onExitApplication);
    m_trayMenu->addAction(m_exitAction);

    m_trayIcon->setContextMenu(m_trayMenu);

    // 连接托盘图标信号
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &InfoMonitor::onTrayIconActivated);

    // 显示托盘图标
    m_trayIcon->show();
}

QTableWidget* InfoMonitor::setupTable() {
    QTableWidget* table = new QTableWidget(0, 7, this);
    QStringList headers = { QString::fromStdWString(L"启用"), QString::fromStdWString(L"类型"), QString::fromStdWString(L"名称"), QString::fromStdWString(L"路径"), QString::fromStdWString(L"状态"), QString::fromStdWString(L"上次检查时间"), QString::fromStdWString(L"操作") };
    table->setHorizontalHeaderLabels(headers);

    // 设置表格属性
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(false);
    table->horizontalHeader()->resizeSection(0, 60);  // 启用列
    table->horizontalHeader()->resizeSection(1, 100); // 类型列
    table->horizontalHeader()->resizeSection(2, 150); // 名称列
    table->horizontalHeader()->resizeSection(3, 300); // 路径列
    table->horizontalHeader()->resizeSection(4, 200); // 状态列
    table->horizontalHeader()->resizeSection(5, 150); // 时间列
    table->horizontalHeader()->resizeSection(6, 80);  // 操作列
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed); // 类型列固定宽度
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed); // 操作列固定宽度

    connect(table, &QTableWidget::cellChanged, this, &InfoMonitor::onItemEnabledChanged);
    connect(table, &QTableWidget::cellDoubleClicked, this, &InfoMonitor::onItemDoubleClicked);

    return table;
}

void InfoMonitor::populateTableWithItems(QTableWidget* table, const QList<MonitorItem>& items) {
    table->setRowCount(items.size());

    for (int row = 0; row < items.size(); ++row) {
        const MonitorItem& item = items[row];

        // 启用复选框
        QCheckBox* enabledCheckBox = new QCheckBox();
        enabledCheckBox->setChecked(item.isEnabled());
        table->setCellWidget(row, 0, enabledCheckBox);

        // 类型下拉框
        QComboBox* typeCombo = new QComboBox();
        typeCombo->addItem(QString::fromStdWString(L"注册表"), static_cast<int>(MonitorItem::Registry));
        typeCombo->addItem(QString::fromStdWString(L"文件"), static_cast<int>(MonitorItem::File));
        typeCombo->setCurrentIndex(static_cast<int>(item.getType()));
        table->setCellWidget(row, 1, typeCombo);

        // 其他列
        table->setItem(row, 2, new QTableWidgetItem(item.getName()));
        table->setItem(row, 3, new QTableWidgetItem(item.getPath()));
        table->setItem(row, 4, new QTableWidgetItem(item.getLastStatus()));

        QString timeStr = item.getLastCheckTime().isValid() ?
            item.getLastCheckTime().toString("yyyy-MM-dd hh:mm:ss") : "--";
        table->setItem(row, 5, new QTableWidgetItem(timeStr));

        // 操作按钮
        QPushButton* deleteBtn = new QPushButton(QString::fromStdWString(L"删除"));
        connect(deleteBtn, &QPushButton::clicked, this, &InfoMonitor::onDeleteItem);
        table->setCellWidget(row, 6, deleteBtn);

        // 连接信号
        connect(enabledCheckBox, &QCheckBox::toggled, this, [this, row](bool checked) {
            int pageIndex = getCurrentPageIndex();
            if (pageIndex >= 0) {
                auto& pages = m_configManager->getPages();
                if (row < pages[pageIndex].getItems().size()) {
                    pages[pageIndex].getItems()[row].setEnabled(checked);
                    saveConfiguration();
                }
            }
            });

        connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, row](int index) {
            int pageIndex = getCurrentPageIndex();
            if (pageIndex >= 0) {
                auto& pages = m_configManager->getPages();
                if (row < pages[pageIndex].getItems().size()) {
                    MonitorItem::Type type = static_cast<MonitorItem::Type>(index);
                    pages[pageIndex].getItems()[row].setType(type);
                    saveConfiguration();
                }
            }
            });
    }
}

QWidget* InfoMonitor::createPageWidget(const QString& pageName, bool enabled, const QList<MonitorItem>& items) {
    QWidget* pageWidget = new QWidget();
    QVBoxLayout* pageLayout = new QVBoxLayout(pageWidget);

    // 页面控制栏
    QHBoxLayout* controlLayout = new QHBoxLayout();

    // 页面名称编辑
    QLineEdit* nameEdit = new QLineEdit(pageName, this);
    nameEdit->setMaximumWidth(200);
    connect(nameEdit, &QLineEdit::textChanged, this, &InfoMonitor::onPageNameChanged);
    controlLayout->addWidget(new QLabel(QString::fromStdWString(L"页面名称:")));
    controlLayout->addWidget(nameEdit);

    controlLayout->addSpacing(20);

    // 页面总开关
    QCheckBox* enabledCheck = new QCheckBox(QString::fromStdWString(L"启用此页面"), this);
    enabledCheck->setChecked(enabled);
    connect(enabledCheck, &QCheckBox::toggled, this, &InfoMonitor::onPageEnabledChanged);
    controlLayout->addWidget(enabledCheck);

    controlLayout->addSpacing(20);

    // 强制刷新按钮
    QPushButton* refreshBtn = new QPushButton(QString::fromStdWString(L"强制刷新"), this);
    connect(refreshBtn, &QPushButton::clicked, this, &InfoMonitor::onRefreshPage);
    controlLayout->addWidget(refreshBtn);

    // 添加项目按钮
    QPushButton* addItemBtn = new QPushButton(QString::fromStdWString(L"添加项目"), this);
    connect(addItemBtn, &QPushButton::clicked, this, &InfoMonitor::onAddItem);
    controlLayout->addWidget(addItemBtn);

    controlLayout->addStretch();

    pageLayout->addLayout(controlLayout);

    // 创建表格
    QTableWidget* table = setupTable();
    populateTableWithItems(table, items);

    pageLayout->addWidget(table);

    return pageWidget;
}

void InfoMonitor::createNewPage(const QString& name) {
    // 如果没有提供名称，使用默认名称
    QString pageName = name.isEmpty() ? QString::fromStdWString(L"新页面") : name;

    // 创建页面数据
    MonitorPage page("", pageName);
    m_configManager->addPage(page);

    // 创建页面UI
    QWidget* pageWidget = createPageWidget(pageName, true, QList<MonitorItem>());

    // 添加到标签页
    int tabIndex = m_tabWidget->addTab(pageWidget, pageName);
    m_tabWidget->setCurrentIndex(tabIndex);

    saveConfiguration();
}

// 槽函数实现
void InfoMonitor::onAddPage() {
    bool ok;
    QString name = QInputDialog::getText(this, QString::fromStdWString(L"添加页面"), QString::fromStdWString(L"页面名称:"), QLineEdit::Normal, QString::fromStdWString(L"新页面"), &ok);
    if (ok && !name.isEmpty()) {
        createNewPage(name);
    }
}

void InfoMonitor::onAddItem() {
    QTableWidget* table = getCurrentTable();
    if (!table) return;

    int currentPageIndex = getCurrentPageIndex();
    if (currentPageIndex < 0) return;

    // 创建新的监控项
    MonitorItem item("", MonitorItem::File, QString::fromStdWString(L"新项目"), "");

    // 添加到数据模型
    auto& pages = m_configManager->getPages();
    pages[currentPageIndex].addItem(item);

    // 添加到表格
    int row = table->rowCount();
    table->insertRow(row);

    // 启用复选框
    QCheckBox* enabledCheck = new QCheckBox();
    enabledCheck->setChecked(true);
    table->setCellWidget(row, 0, enabledCheck);

    // 类型下拉框
    QComboBox* typeCombo = new QComboBox();
    typeCombo->addItem(QString::fromStdWString(L"注册表"), static_cast<int>(MonitorItem::Registry));
    typeCombo->addItem(QString::fromStdWString(L"文件"), static_cast<int>(MonitorItem::File));
    table->setCellWidget(row, 1, typeCombo);

    // 其他列
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(L"新项目")));
    table->setItem(row, 3, new QTableWidgetItem(""));
    table->setItem(row, 4, new QTableWidgetItem(QString::fromStdWString(L"未检查")));
    table->setItem(row, 5, new QTableWidgetItem("--"));

    // 操作按钮
    QPushButton* deleteBtn = new QPushButton(QString::fromStdWString(L"删除"));
    connect(deleteBtn, &QPushButton::clicked, this, &InfoMonitor::onDeleteItem);
    table->setCellWidget(row, 6, deleteBtn);

    // 连接信号
    connect(enabledCheck, &QCheckBox::toggled, this, [this, row](bool checked) {
        int pageIndex = getCurrentPageIndex();
        if (pageIndex >= 0) {
            auto& pages = m_configManager->getPages();
            if (row < pages[pageIndex].getItems().size()) {
                pages[pageIndex].getItems()[row].setEnabled(checked);
                saveConfiguration();
            }
        }
        });

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, row](int index) {
        int pageIndex = getCurrentPageIndex();
        if (pageIndex >= 0) {
            auto& pages = m_configManager->getPages();
            if (row < pages[pageIndex].getItems().size()) {
                MonitorItem::Type type = static_cast<MonitorItem::Type>(index);
                pages[pageIndex].getItems()[row].setType(type);
                saveConfiguration();
            }
        }
        });

    saveConfiguration();
}

void InfoMonitor::onRefreshPage() {
    int pageIndex = getCurrentPageIndex();
    if (pageIndex >= 0) {
        m_monitorManager->refreshPage(pageIndex);
    }
}

void InfoMonitor::onStartStopMonitoring() {
    if (m_isMonitoring) {
        m_monitorManager->stopMonitoring();
        m_isMonitoring = false;
        m_startStopButton->setText(QString::fromStdWString(L"开始监控"));
    } else {
        m_monitorManager->startMonitoring();
        m_isMonitoring = true;
        m_startStopButton->setText(QString::fromStdWString(L"停止监控"));
    }
}

void InfoMonitor::onTabChanged(int index) {
    updateCurrentPageUI();
    updateStatusBar();
}

void InfoMonitor::onPageNameChanged() {
    QLineEdit* nameEdit = qobject_cast<QLineEdit*>(sender());
    if (!nameEdit) return;

    int pageIndex = getCurrentPageIndex();
    if (pageIndex >= 0) {
        QString newName = nameEdit->text();
        auto& pages = m_configManager->getPages();
        pages[pageIndex].setName(newName);
        m_tabWidget->setTabText(pageIndex, newName);
        saveConfiguration();
    }
}

void InfoMonitor::onPageEnabledChanged(bool enabled) {
    int pageIndex = getCurrentPageIndex();
    if (pageIndex >= 0) {
        auto& pages = m_configManager->getPages();
        pages[pageIndex].setEnabled(enabled);
        saveConfiguration();
    }
}

void InfoMonitor::onItemEnabledChanged(int row, int column) {
    if (column != 2 && column != 3) return; // 只处理名称和路径列的编辑

    QTableWidget* table = getCurrentTable();
    if (!table) return;

    int pageIndex = getCurrentPageIndex();
    if (pageIndex < 0) return;

    auto& pages = m_configManager->getPages();
    if (row >= pages[pageIndex].getItems().size()) return;

    QTableWidgetItem* item = table->item(row, column);
    if (!item) return;

    if (column == 2) { // 名称列
        pages[pageIndex].getItems()[row].setName(item->text());
    } else if (column == 3) { // 路径列
        pages[pageIndex].getItems()[row].setPath(item->text());
    }

    saveConfiguration();
}

void InfoMonitor::onItemDoubleClicked(int row, int column) {
    // 双击编辑功能已通过表格的默认行为实现
}

void InfoMonitor::onDeleteItem() {
    QPushButton* deleteBtn = qobject_cast<QPushButton*>(sender());
    if (!deleteBtn) return;

    QTableWidget* table = getCurrentTable();
    if (!table) return;

    // 找到按钮所在的行
    int row = -1;
    for (int i = 0; i < table->rowCount(); ++i) {
        if (table->cellWidget(i, 6) == deleteBtn) {
            row = i;
            break;
        }
    }

    if (row < 0) return;

    int pageIndex = getCurrentPageIndex();
    if (pageIndex >= 0) {
        auto& pages = m_configManager->getPages();
        pages[pageIndex].removeItem(row);
        table->removeRow(row);
        saveConfiguration();
    }
}

void InfoMonitor::onMonitoringUpdate() {
    updateCurrentPageUI();
    updateStatusBar();
}

// 托盘相关槽函数实现
void InfoMonitor::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        onShowWindow();
        break;
    default:
        break;
    }
}

void InfoMonitor::onShowWindow() {
    show();
    raise();
    activateWindow();
}

void InfoMonitor::onExitApplication() {
    QApplication::quit();
}

// 辅助函数实现
void InfoMonitor::updateCurrentPageUI() {
    QTableWidget* table = getCurrentTable();
    if (!table) return;

    int pageIndex = getCurrentPageIndex();
    if (pageIndex < 0) return;

    const auto& pages = m_configManager->getPages();
    if (pageIndex >= pages.size()) return;

    const MonitorPage& page = pages[pageIndex];
    const auto& items = page.getItems();

    // 更新表格内容
    for (int row = 0; row < items.size() && row < table->rowCount(); ++row) {
        const MonitorItem& item = items[row];

        // 更新启用状态
        QCheckBox* enabledCheck = qobject_cast<QCheckBox*>(table->cellWidget(row, 0));
        if (enabledCheck) {
            enabledCheck->setChecked(item.isEnabled());
        }

        // 更新类型
        QComboBox* typeCombo = qobject_cast<QComboBox*>(table->cellWidget(row, 1));
        if (typeCombo) {
            typeCombo->setCurrentIndex(static_cast<int>(item.getType()));
        }

        // 更新其他列
        if (table->item(row, 2)) table->item(row, 2)->setText(item.getName());
        if (table->item(row, 3)) table->item(row, 3)->setText(item.getPath());
        if (table->item(row, 4)) table->item(row, 4)->setText(item.getLastStatus());
        if (table->item(row, 5)) {
            QString timeStr = item.getLastCheckTime().isValid() ?
                item.getLastCheckTime().toString("yyyy-MM-dd hh:mm:ss") : "--";
            table->item(row, 5)->setText(timeStr);
        }
    }
}

void InfoMonitor::updateStatusBar() {
    if (m_isMonitoring) {
        m_statusLabel->setText(QString::fromStdWString(L"监控状态：运行中"));

        QDateTime nextCheck = m_monitorManager->getNextCheckTime();
        if (nextCheck.isValid()) {
            int secondsToNext = QDateTime::currentDateTime().secsTo(nextCheck);
            m_nextCheckLabel->setText(QString::fromStdWString(L"下次检查：%1秒后").arg(qMax(0, secondsToNext)));
        } else {
            m_nextCheckLabel->setText(QString::fromStdWString(L"下次检查：--"));
        }
    } else {
        m_statusLabel->setText(QString::fromStdWString(L"监控状态：已停止"));
        m_nextCheckLabel->setText(QString::fromStdWString(L"下次检查：--"));
    }

    int totalItems = m_monitorManager->getTotalItemCount();
    int enabledItems = m_monitorManager->getEnabledItemCount();
    m_itemCountLabel->setText(QString::fromStdWString(L"项目数：%1 (启用：%2)").arg(totalItems).arg(enabledItems));
}

void InfoMonitor::loadConfiguration() {
    const auto& pages = m_configManager->getPages();

    // 清空现有标签页
    while (m_tabWidget->count() > 0) {
        m_tabWidget->removeTab(0);
    }

    // 重新创建标签页
    for (const auto& page : pages) {
        createPageUI(page);
    }

    // 如果没有页面，创建默认页面
    if (m_tabWidget->count() == 0) {
        createNewPage(QString::fromStdWString(L"默认页面"));
    }
}

void InfoMonitor::saveConfiguration() {
    if (m_configManager) {
        m_configManager->saveConfiguration();
    }
}

QTableWidget* InfoMonitor::getCurrentTable() {
    int currentIndex = m_tabWidget->currentIndex();
    if (currentIndex < 0) return nullptr;

    QWidget* currentWidget = m_tabWidget->widget(currentIndex);
    if (!currentWidget) return nullptr;

    return currentWidget->findChild<QTableWidget*>();
}

int InfoMonitor::getCurrentPageIndex() {
    return m_tabWidget->currentIndex();
}

void InfoMonitor::createPageUI(const MonitorPage& page) {
    // 使用公共函数创建页面UI
    QWidget* pageWidget = createPageWidget(page.getName(), page.isEnabled(), page.getItems());

    // 添加到标签页
    m_tabWidget->addTab(pageWidget, page.getName());
}

void InfoMonitor::closeEvent(QCloseEvent* event) {
#ifdef DEBUG
    // 如果托盘图标可用，最小化到托盘而不是退出
    if (m_trayIcon && m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    } else {
        // 如果托盘不可用，正常退出
        event->accept();
    }

#endif
}

