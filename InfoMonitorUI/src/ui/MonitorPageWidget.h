#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QList>
//#include "MonitorItem.h"
#include "ui_MonitorPageWidget.h"

// todo(wangwenxi)：负责具体展示的内容，具体的内容去业务层查

class MonitorPageWidget : public QWidget {
    Q_OBJECT
public:
    MonitorPageWidget(QWidget* pParent);
    ~MonitorPageWidget();


signals:

protected:
    void InitUI();



private:
    Ui::MonitorPageWidget ui;


};


/*
class InfoMonitor;
class MonitorPage;
class ConfigManager;

class MonitorPageWidget : public QWidget {
    Q_OBJECT

public:
    MonitorPageWidget(int pageIndex, const QString& pageName, bool enabled, 
                      const QList<MonitorItem>& items = QList<MonitorItem>(), 
                      InfoMonitor* parent = nullptr);
    ~MonitorPageWidget();

    // 设置页面数据
    void setPageData(int pageIndex, const QString& pageName, bool enabled, const QList<MonitorItem>& items);
    
    // 获取表格控件
    QTableWidget* getTableWidget() const {
        return ui.tableWidget;
    }

    // 填充表格数据
    void populateTableWithItems(const QList<MonitorItem>& items);

signals:
    // 页面名称变更信号
    void pageNameChanged(const QString& newName);
    
    // 页面启用状态变更信号
    void pageEnabledChanged(bool enabled);
    
    // 刷新按钮点击信号
    void refreshRequested();
    
    // 添加项目按钮点击信号
    void addItemRequested();

protected slots:
    void onNameEdit_textChanged(const QString& text);
    void onEnabledCheck_toggled(bool checked);
    void onRefreshBtn_clicked();
    void onAddItemBtn_clicked();
    
    void onTableWidget_cellChanged(int row, int column);
    void onTableWidget_cellDoubleClicked(int row, int column);

protected:
    void initUI();
    void setupColumnWidths();

private:
    int m_pageIndex = 0;
    InfoMonitor* m_pParent = nullptr;
};
*/