#include "MonitorPageWidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QHeaderView>

MonitorPageWidget::MonitorPageWidget(const MonitorPageInfo& info, QWidget* pParent) : QWidget(pParent), m_pageInfo(info) {
    ui.setupUi(this);

    InitUI();
}

MonitorPageWidget::~MonitorPageWidget() {
    // 清理资源
}

void MonitorPageWidget::SetTitle(const QString& strTitle) {
    m_pageInfo.strTitle = strTitle;
}

QString MonitorPageWidget::GetTitle() const {
    return m_pageInfo.strTitle;
}

void MonitorPageWidget::InitUI() {

}



/*
MonitorPageWidget::MonitorPageWidget(int pageIndex, const QString& pageName, bool enabled, const QList<MonitorItem>& items, InfoMonitor* parent) 
    : QWidget(parent), m_pageIndex(pageIndex), m_pParent(parent) {
    ui.setupUi(this);
    
    initUI();
    setPageData(pageIndex, pageName, enabled, items);
}

MonitorPageWidget::~MonitorPageWidget() {
    // 清理资源
}

void MonitorPageWidget::setPageData(int pageIndex, const QString& pageName, bool enabled, const QList<MonitorItem>& items) {
    m_pageIndex = pageIndex;
    ui.nameEdit->setText(pageName);
    ui.enabledCheck->setChecked(enabled);
    
    // 设置列宽
    setupColumnWidths();
    
    // 填充表格数据
    populateTableWithItems(items);
}

void MonitorPageWidget::initUI() {
    connect(ui.nameEdit, &QLineEdit::textChanged, this, &MonitorPageWidget::onNameEdit_textChanged);
    connect(ui.enabledCheck, &QCheckBox::toggled, this, &MonitorPageWidget::onEnabledCheck_toggled);
    connect(ui.refreshBtn, &QPushButton::clicked, this, &MonitorPageWidget::onRefreshBtn_clicked);
    connect(ui.addItemBtn, &QPushButton::clicked, this, &MonitorPageWidget::onAddItemBtn_clicked);
    
    connect(ui.tableWidget, &QTableWidget::cellChanged, this, &MonitorPageWidget::onTableWidget_cellChanged);
    connect(ui.tableWidget, &QTableWidget::cellDoubleClicked, this, &MonitorPageWidget::onTableWidget_cellDoubleClicked);
    
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setAlternatingRowColors(true);
    ui.tableWidget->horizontalHeader()->setStretchLastSection(false);
}

void MonitorPageWidget::setupColumnWidths() {
    if (!m_pParent || m_pageIndex < 0) {
        return;
    }
    
    //ConfigManager* configManager = m_pParent->m_configManager;
    ConfigManager* configManager = nullptr;
    if (!configManager) {
        return;
    }
    
    auto& pages = configManager->getPages();
    if (m_pageIndex >= pages.size()) {
        return;
    }
    
    // 默认列配置
    const ColumnConfig defaultColumns[] = {
        {InfoMonitor::COLUMN_ENABLED, 60, false},     // 启用列
        {InfoMonitor::COLUMN_TYPE, 120, true},         // 类型列（固定宽度）
        {InfoMonitor::COLUMN_NAME, 150, false},        // 名称列
        {InfoMonitor::COLUMN_PATH, 400, false},        // 路径列
        {InfoMonitor::COLUMN_STATUS, 200, false},      // 状态列
        {InfoMonitor::COLUMN_LAST_CHECK, 150, false},  // 上次检查时间列
        {InfoMonitor::COLUMN_ACTION, 80, true}         // 操作列（固定宽度）
    };
    
    // 配置所有列
    for (const auto& column : defaultColumns) {
        // 从配置中获取列宽，如果没有配置则使用默认值
        int width = column.width;
        width = pages[m_pageIndex].getColumnWidth(column.index, column.width);
        
        ui.tableWidget->horizontalHeader()->resizeSection(column.index, width);
        if (column.fixed) {
            ui.tableWidget->horizontalHeader()->setSectionResizeMode(column.index, QHeaderView::Fixed);
        } else {
            ui.tableWidget->horizontalHeader()->setSectionResizeMode(column.index, QHeaderView::Interactive);
        }
    }
}

// 填充表格数据
void MonitorPageWidget::populateTableWithItems(const QList<MonitorItem>& items) {
    ui.tableWidget->setRowCount(items.size());

    for (int row = 0; row < items.size(); ++row) {
        const MonitorItem& item = items[row];

        // 启用复选框
        QCheckBox* enabledCheckBox = new QCheckBox();
        enabledCheckBox->setChecked(item.isEnabled());
        ui.tableWidget->setCellWidget(row, InfoMonitor::COLUMN_ENABLED, enabledCheckBox);

        // 类型下拉框
        QComboBox* typeCombo = new QComboBox();
        typeCombo->addItem(QString::fromStdWString(L"注册表"), static_cast<int>(MonitorItem::Registry));
        typeCombo->addItem(QString::fromStdWString(L"文件"), static_cast<int>(MonitorItem::File));
        typeCombo->setCurrentIndex(static_cast<int>(item.getType()));
        ui.tableWidget->setCellWidget(row, InfoMonitor::COLUMN_TYPE, typeCombo);

        // 其他列
        ui.tableWidget->setItem(row, InfoMonitor::COLUMN_NAME, new QTableWidgetItem(item.getName()));
        ui.tableWidget->setItem(row, InfoMonitor::COLUMN_PATH, new QTableWidgetItem(item.getPath()));
        ui.tableWidget->setItem(row, InfoMonitor::COLUMN_STATUS, new QTableWidgetItem(item.getLastStatus()));

        QString timeStr = item.getLastCheckTime().isValid() ?
            item.getLastCheckTime().toString("yyyy-MM-dd hh:mm:ss") : "--";
        ui.tableWidget->setItem(row, InfoMonitor::COLUMN_LAST_CHECK, new QTableWidgetItem(timeStr));

        // todo(wangwenxi)：完善一下
        // 操作按钮
        //QPushButton* deleteBtn = new QPushButton(QString::fromStdWString(L"删除"));
        //connect(deleteBtn, &QPushButton::clicked, m_pParent, &InfoMonitor::onDeleteItem);
        //ui.tableWidget->setCellWidget(row, InfoMonitor::COLUMN_ACTION, deleteBtn);

        // 连接信号
        //connect(enabledCheckBox, &QCheckBox::toggled, m_pParent, [m_pParent = this->m_pParent, row](bool checked) {
        //    int pageIndex = m_pParent->getCurrentPageIndex();
        //    if (pageIndex >= 0) {
        //        ConfigManager* configManager = m_pParent->m_configManager;
        //        if (configManager) {
        //            auto& pages = configManager->getPages();
        //            if (pageIndex < pages.size() && row < pages[pageIndex].getItems().size()) {
        //                pages[pageIndex].getItems()[row].setEnabled(checked);
        //                m_pParent->saveConfiguration();
        //            }
        //        }
        //    }
        //});

        //connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), m_pParent, [m_pParent = this->m_pParent, row](int index) {
        //    int pageIndex = m_pParent->getCurrentPageIndex();
        //    if (pageIndex >= 0) {
        //        ConfigManager* configManager = m_pParent->m_configManager;
        //        if (configManager) {
        //            auto& pages = configManager->getPages();
        //            if (pageIndex < pages.size() && row < pages[pageIndex].getItems().size()) {
        //                MonitorItem::Type type = static_cast<MonitorItem::Type>(index);
        //                pages[pageIndex].getItems()[row].setType(type);
        //                m_pParent->saveConfiguration();
        //            }
        //        }
        //    }
        //});
    }
}

// 页面名称变更槽函数
void MonitorPageWidget::onNameEdit_textChanged(const QString& text) {
    emit pageNameChanged(text);
}

// 页面启用状态变更槽函数
void MonitorPageWidget::onEnabledCheck_toggled(bool checked) {
    emit pageEnabledChanged(checked);
}

// 刷新按钮点击槽函数
void MonitorPageWidget::onRefreshBtn_clicked() {
    emit refreshRequested();
}

// 添加项目按钮点击槽函数
void MonitorPageWidget::onAddItemBtn_clicked() {
    emit addItemRequested();
}

// 表格单元格变更槽函数
void MonitorPageWidget::onTableWidget_cellChanged(int row, int column) {
    //if (m_pParent && column != InfoMonitor::COLUMN_ENABLED) {  // 启用列的变更已经单独处理
    //    m_pParent->onItemEnabledChanged(row, column);
    //}
}

// 表格单元格双击槽函数
void MonitorPageWidget::onTableWidget_cellDoubleClicked(int row, int column) {
    //if (m_pParent) {
    //    m_pParent->onItemDoubleClicked(row, column);
    //}
}

*/