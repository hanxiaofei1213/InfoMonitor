#include "InfoMonitor.h"
#include "UIConfigManager.h"
#include "WindowManager.h"
#include "IMTray.h"
#include <QCloseEvent>
#include <QApplication>
#include <QIcon>
#include "IMActionBar.h"
#include "MonitorPageWidget.h"
#include "src/strategy/MonitorPageManager.h"
#include "src/strategy/InfoMonitorCoreWrapper.h"

// todo(wangwenxi): 时间到了能刷新一下UI
/*
InfoMonitor::InfoMonitor()
    : QMainWindow(nullptr) {
    // 先创建监控管理器
    m_monitorManager = new MonitorManager(this);
    m_configManager = m_monitorManager->getConfigManager();

    // 创建窗口管理器
    m_windowManager = new WindowManager(this, m_configManager, this);

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

    // 设置窗口大小
    m_windowManager->setupWindowSize();

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

QTableWidget* InfoMonitor::setupTable(int nPageIndex) {
    if (nPageIndex < 0 || nPageIndex >= m_configManager->getPages().size()) {
        return nullptr;
    }

    QTableWidget* table = new QTableWidget(0, 7, this);
    QStringList headers = { QString::fromStdWString(L"启用"), QString::fromStdWString(L"类型"), QString::fromStdWString(L"名称"), QString::fromStdWString(L"路径"), QString::fromStdWString(L"状态"), QString::fromStdWString(L"上次检查时间"), QString::fromStdWString(L"操作") };
    table->setHorizontalHeaderLabels(headers);

    // 默认列配置
    const ColumnConfig defaultColumns[] = {
        {COLUMN_ENABLED, 60,  false},     // 启用列
        {COLUMN_TYPE, 120, true},         // 类型列（固定宽度）
        {COLUMN_NAME, 150, false},        // 名称列
        {COLUMN_PATH, 400, false},        // 路径列
        {COLUMN_STATUS, 200, false},      // 状态列
        {COLUMN_LAST_CHECK, 150, false},  // 上次检查时间列
        {COLUMN_ACTION, 80,  true}        // 操作列（固定宽度）
    };

    // 设置表格属性
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(false);

    // 配置所有列 - 使用循环消除重复代码
    for (const auto& column : defaultColumns) {
        // 从配置中获取列宽，如果没有配置则使用默认值
        int width = column.width;
        auto& pages = m_configManager->getPages();
        if (nPageIndex < pages.size()) {
            width = pages[nPageIndex].getColumnWidth(column.index, column.width);
        }

        table->horizontalHeader()->resizeSection(column.index, width);
        if (column.fixed) {
            table->horizontalHeader()->setSectionResizeMode(column.index, QHeaderView::Fixed);
        } else {
            table->horizontalHeader()->setSectionResizeMode(column.index, QHeaderView::Interactive);
        }
    }

    // 不再连接列宽变化信号，而是在程序退出时统一保存

    connect(table, &QTableWidget::cellChanged, this, &InfoMonitor::onItemEnabledChanged);
    connect(table, &QTableWidget::cellDoubleClicked, this, &InfoMonitor::onItemDoubleClicked);

    return table;
}

// todo(wangwenxi)：这里看看能不能用ui文件
void InfoMonitor::populateTableWithItems(QTableWidget* table, const QList<MonitorItem>& items) {
    table->setRowCount(items.size());

    for (int row = 0; row < items.size(); ++row) {
        const MonitorItem& item = items[row];

        // 启用复选框
        QCheckBox* enabledCheckBox = new QCheckBox();
        enabledCheckBox->setChecked(item.enabled);
        table->setCellWidget(row, 0, enabledCheckBox);

        // 类型下拉框
        QComboBox* typeCombo = new QComboBox();
        typeCombo->addItem(QString::fromStdWString(L"注册表"), static_cast<int>(MonitorItem::Registry));
        typeCombo->addItem(QString::fromStdWString(L"文件"), static_cast<int>(MonitorItem::File));
        typeCombo->setCurrentIndex(static_cast<int>(item.getType()));
        table->setCellWidget(row, 1, typeCombo);

        // 其他列
        table->setItem(row, 2, new QTableWidgetItem(item.name));
        table->setItem(row, 3, new QTableWidgetItem(item.path));
        table->setItem(row, 4, new QTableWidgetItem(item.lastStatus));

        QString timeStr = item.lastCheckTime.isValid() ?
            item.lastCheckTime.toString("yyyy-MM-dd hh:mm:ss") : "--";
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
                    pages[pageIndex].getItems()[row].enabled = checked;
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
                    pages[pageIndex].getItems()[row].type = type;
                    saveConfiguration();
                }
            }
        });
    }
}

QWidget* InfoMonitor::createPageWidget(int nPageIndex, const QString& pageName, bool enabled, const QList<MonitorItem>& items) {
    if (nPageIndex < 0) {
        return nullptr;
    }

    // 创建基于UI文件的页面部件
    MonitorPageWidget* pPageWidget = new MonitorPageWidget(nPageIndex, pageName, enabled, items, this);

    // todo(wangwenxi)：这里槽函数挪到page里面去
    // 连接页面信号到主窗口槽函数
    connect(pPageWidget, &MonitorPageWidget::pageNameChanged, this, &InfoMonitor::onPageNameChanged);
    connect(pPageWidget, &MonitorPageWidget::pageEnabledChanged, this, &InfoMonitor::onPageEnabledChanged);
    connect(pPageWidget, &MonitorPageWidget::refreshRequested, this, &InfoMonitor::onRefreshPage);
    connect(pPageWidget, &MonitorPageWidget::addItemRequested, this, &InfoMonitor::onAddItem);

    // 获取表格控件并连接相关信号
    QTableWidget* tableWidget = pPageWidget->getTableWidget();
    if (tableWidget) {
        connect(tableWidget, &QTableWidget::cellChanged, this, &InfoMonitor::onItemEnabledChanged);
        connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &InfoMonitor::onItemDoubleClicked);
    }

    return pPageWidget;
}

void InfoMonitor::createNewPage(const QString& name) {
    // 如果没有提供名称，使用默认名称
    QString pageName = name.isEmpty() ? QString::fromStdWString(L"新页面") : name;

    // 创建页面数据
    MonitorPage page("", pageName);
    m_configManager->addPage(page);

    // 创建页面UI
    QWidget* pPageWidget = createPageWidget(0, pageName, true, QList<MonitorItem>());

    // 添加到标签页
    int tabIndex = m_tabWidget->addTab(pPageWidget, pageName);
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
        pages[pageIndex].name = newName;
        m_tabWidget->setTabText(pageIndex, newName);
        saveConfiguration();
    }
}

void InfoMonitor::onPageEnabledChanged(bool enabled) {
    int pageIndex = getCurrentPageIndex();
    if (pageIndex >= 0) {
        auto& pages = m_configManager->getPages();
        pages[pageIndex].enabled = enabled;
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
        pages[pageIndex].getItems()[row].name = item->text();
    } else if (column == 3) { // 路径列
        pages[pageIndex].getItems()[row].path = item->text();
    }

    saveConfiguration();
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
            enabledCheck->setChecked(item.enabled);
        }

        // 更新类型
        QComboBox* typeCombo = qobject_cast<QComboBox*>(table->cellWidget(row, 1));
        if (typeCombo) {
            typeCombo->setCurrentIndex(static_cast<int>(item.type));
        }

        // 更新其他列
        if (table->item(row, 2)) table->item(row, 2)->setText(item.name);
        if (table->item(row, 3)) table->item(row, 3)->setText(item.path);
        if (table->item(row, 4)) table->item(row, 4)->setText(item.lastStatus);
        if (table->item(row, 5)) {
            QString timeStr = item.lastCheckTime.isValid() ?
                item.lastCheckTime.toString("yyyy-MM-dd hh:mm:ss") : "--";
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
    for (int nIndex = 0; nIndex < pages.size(); ++nIndex) {
        createPageUI(nIndex, pages[nIndex]);
    }

    // 如果没有页面，创建默认页面
    if (m_tabWidget->count() == 0) {
        createNewPage(QString::fromStdWString(L"默认页面"));
    }
}

void InfoMonitor::saveConfiguration() {
    if (!m_configManager) {
        return;
    }

    m_configManager->saveConfiguration();
}

QTableWidget* InfoMonitor::getCurrentTable() {
    int currentIndex = m_tabWidget->currentIndex();
    if (currentIndex < 0) return nullptr;

    QWidget* currentWidget = m_tabWidget->widget(currentIndex);
    if (!currentWidget) return nullptr;

    return currentWidget->findChild<QTableWidget*>();
}

int InfoMonitor::getCurrentPageIndex() const {
    if (m_tabWidget) {
        return m_tabWidget->currentIndex();
    }
    return -1;
}

void InfoMonitor::createPageUI(int nPageIndex, const MonitorPage& page) {
    QWidget* pPageWidget = createPageWidget(nPageIndex, page.name, page.enabled, page.getItems());
    if (!pPageWidget) {
        return;
    }

    m_tabWidget->addTab(pPageWidget, page.name);
}

void InfoMonitor::saveColumnWidthsOnExit()
{
    // 遍历所有页面的表格，保存列宽配置
    auto& pages = m_configManager->getPages();
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QWidget* widget = m_tabWidget->widget(i);
        QTableWidget* table = widget->findChild<QTableWidget*>();
        if (table) {
            // 保存当前表格的所有列宽
            for (int col = 0; col < table->columnCount(); ++col) {
                int width = table->columnWidth(col);
                if (i < pages.size()) {
                    pages[i].setColumnWidth(col, width);
                }
            }
        }
    }
}
*/

InfoMonitor::InfoMonitor() : QMainWindow(nullptr) {
    m_pConfig = new UIConfigManager(this);
    m_windowManager = new WindowManager(this, m_pConfig, this);
    m_trayIcon = new IMTray(this);
    m_pActionBar = new IMActionBar(this, this);
    m_pInfoMonitorCore = new InfoMonitorCoreWrapper(this);
    m_pInfoMonitorCore->Init();

    m_pPageManager = new MonitorPageManager(m_pInfoMonitorCore, this);

    InitUI();
    InitConnect();
}

InfoMonitor::~InfoMonitor() {
    if (m_pInfoMonitorCore) {
        m_pInfoMonitorCore->UnInit();
    }
}

void InfoMonitor::InitUI() {
    ui.setupUi(this);
    setWindowTitle(QString::fromStdWString(L"InfoMonitor - 系统监控工具"));
    setWindowIcon(QIcon(":/InfoMonitor/res/main.png"));
    setMinimumSize(1000, 600);

    // 设置窗口大小
    m_windowManager->setupWindowSize();

    ui.tabWidget->setTabsClosable(true);
    ui.tabWidget->setMovable(true);

    m_pActionBar->InitUI();

    InitTray();
    InitPage();
}

void InfoMonitor::InitTray() {
    // 设置托盘图标和提示
    QIcon trayIcon(":/InfoMonitor/res/main.png");
    m_trayIcon->setIcon(trayIcon);
    m_trayIcon->setToolTip(QString::fromStdWString(L"InfoMonitor - 系统监控工具"));

    if (m_trayIcon->isSystemTrayAvailable()) {
        m_trayIcon->show();
    }
}

void InfoMonitor::InitPage() {
    if (!m_pPageManager) {
        return;
    }

    QVector<MonitorPageWidget*> vecPageWidgets = m_pPageManager->CreateMonitorPages();
    if (vecPageWidgets.isEmpty()) {
        assert(false && "vecPage shouldn't be empty");
        return;
    }
    
    for (auto pPageWidget : vecPageWidgets) {
        if (!pPageWidget) {
            continue;
        }

        ui.tabWidget->addTab(pPageWidget,  pPageWidget->GetTitle());
    }
}

void InfoMonitor::InitConnect() {
    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &InfoMonitor::onTabChanged);
    connect(ui.tabWidget, &QTabWidget::tabCloseRequested, this, &InfoMonitor::onTabCloseRequest);

    connect(m_trayIcon, &IMTray::showWindowRequested, this, &InfoMonitor::onShowWindow);
    connect(m_trayIcon, &IMTray::exitApplicationRequested, this, &InfoMonitor::onExitApplication);
}

void InfoMonitor::closeEvent(QCloseEvent* event) {
    // 保存当前窗口大小
    if (m_windowManager) {
        m_windowManager->saveCurrentSize();
    }

    // 在程序退出时保存所有表格的列宽
    //saveColumnWidthsOnExit();

#ifndef _DEBUG
    // 如果系统支持托盘，隐藏窗口而不是退出程序
    if (m_trayIcon && m_trayIcon->isSystemTrayAvailable()) {
        hide();
        m_trayIcon->showMessage(QString::fromStdWString(L"InfoMonitor"),
                                QString::fromStdWString(L"程序已最小化到托盘"),
                                QSystemTrayIcon::Information,
                                2000);
        event->ignore();
    } else {
        // 如果托盘不可用，正常退出
        event->accept();
    }
#else
    event->accept();
#endif
}

void InfoMonitor::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    // 通知窗口管理器处理大小变化
    if (m_windowManager) {
        m_windowManager->handleResize();
    }
}

void InfoMonitor::onTabChanged(int nIndex) {
    //updateCurrentPageUI();
    //updateStatusBar();
}

void InfoMonitor::onTabCloseRequest(int nIndex) {
    if (ui.tabWidget->count() > 1) { // 至少保留一个标签页
        ui.tabWidget->removeTab(nIndex);
    }
}

void InfoMonitor::onShowWindow() {
    // 显示窗口并激活
    show();
    raise();
    activateWindow();
}

void InfoMonitor::onExitApplication() {
    // 关闭应用程序
    QApplication::exit(0);
}
