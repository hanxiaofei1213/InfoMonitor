# InfoMonitor 系统监控工具设计方案

## 1. 项目概述

InfoMonitor 是一个基于 Qt 开发的 Windows 系统监控工具，用于定时监控注册表键值和文件路径状态，提供分页管理和持久化配置功能。

## 2. 功能需求

### 2.1 核心监控功能
- **注册表监控**：监控指定注册表键的具体值内容，使用Windows标准格式路径
- **文件路径监控**：监控文件是否存在
- **定时查询**：全局统一1分钟间隔自动查询
- **手动刷新**：支持立即强制刷新
- **自动启动**：程序启动即开始监控

### 2.2 界面管理功能
- **分页管理**：支持多个监控页面，每页可容纳几十个监控项
- **表格显示**：使用表格形式展示监控项信息
- **页面控制**：每页可自定义名称，有独立的总开关
- **项目控制**：每个监控项有独立开关控制是否监控
- **直接编辑**：注册表路径可直接在界面上编辑输入
- **状态显示**：
  - 注册表：显示具体键值内容，不存在显示"不存在"
  - 文件：显示"存在"/"不存在"
  - 上次查询时间

### 2.3 配置管理功能
- **持久化存储**：配置保存为JSON文件在程序目录
- **自动恢复**：程序重启后自动加载之前的配置继续监控
- **错误处理**：直接显示具体错误信息

## 3. 技术架构

### 3.1 主要模块
```
InfoMonitor/
├── UI层
│   ├── MainWindow（主窗口）
│   ├── QTabWidget（分页管理）
│   ├── MonitorTableWidget（监控表格）
│   └── AddItemDialog（添加项目对话框）
├── 业务逻辑层
│   ├── MonitorManager（监控管理器）
│   ├── RegistryMonitor（注册表监控）
│   ├── FileMonitor（文件监控）
│   └── TimerManager（定时器管理）
├── 数据层
│   ├── ConfigManager（配置管理）
│   ├── MonitorItem（监控项数据模型）
│   └── MonitorPage（页面数据模型）
└── 工具层
    ├── JsonHelper（JSON处理）
    └── RegistryHelper（注册表访问）
```

### 3.2 数据结构设计

#### MonitorItem（监控项）
```json
{
  "id": "unique_id",
  "type": "registry|file",
  "path": "监控路径",
  "name": "显示名称",
  "enabled": true,
  "lastCheckTime": "2025-01-01T10:00:00",
  "lastStatus": "状态信息",
  "errorMessage": ""
}
```

#### MonitorPage（监控页面）
```json
{
  "id": "page_id",
  "name": "页面名称",
  "enabled": true,
  "items": [MonitorItem...]
}
```

#### 配置文件结构（config.json）
```json
{
  "version": "1.0",
  "globalSettings": {
    "checkInterval": 60000,
    "autoStart": true
  },
  "pages": [MonitorPage...]
}
```

## 4. 界面设计

### 4.1 主窗口布局
```
[菜单栏] 文件 编辑 视图 帮助
[工具栏] [添加页面] [设置] [开始/停止监控]
[标签页] 页面1 | 页面2 | 页面3 | +
[页面内容区域]
  [页面控制栏] [页面名称编辑] [总开关] [强制刷新] [添加项目] [删除页面]
  [监控项表格]
    | ☑启用 | 类型 | 名称 | 路径 | 状态 | 上次检查时间 | 操作 |
    |-------|------|------|------|------|-------------|------|
    | ☑     | 注册表| 系统版本 | HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProductName | Windows 11 Pro | 2025-01-01 10:00:00 | 编辑/删除 |
    | ☑     | 文件  | 系统文件 | C:\Windows\System32\kernel32.dll | 存在 | 2025-01-01 10:00:00 | 编辑/删除 |
[状态栏] 监控状态：运行中 | 下次检查：30秒后 | 总项目数：15个
```

### 4.2 表格列定义
1. **启用**：复选框，控制单个项目是否启用
2. **类型**：图标+文字，区分注册表/文件
3. **名称**：用户自定义的显示名称（可编辑）
4. **路径**：完整的注册表路径或文件路径（可编辑）
5. **状态**：当前监控结果
6. **上次检查时间**：具体的检查时间
7. **操作**：编辑/删除按钮

### 4.3 注册表路径格式
支持标准Windows注册表路径格式：
- `HKEY_LOCAL_MACHINE\SOFTWARE\...` 或 `HKLM\SOFTWARE\...`
- `HKEY_CURRENT_USER\SOFTWARE\...` 或 `HKCU\SOFTWARE\...`
- `HKEY_CLASSES_ROOT\...` 或 `HKCR\...`
- `HKEY_USERS\...` 或 `HKU\...`
- `HKEY_CURRENT_CONFIG\...` 或 `HKCC\...`

## 5. 实现细节

### 5.1 注册表监控
- 使用 Windows Registry API（RegOpenKeyEx, RegQueryValueEx）
- 支持所有注册表根键和数据类型
- 错误处理：权限不足、路径不存在、键值不存在等
- 返回具体的键值内容或"不存在"

### 5.2 文件监控
- 使用 QFileInfo::exists() 检查文件存在性
- 支持相对路径和绝对路径
- 错误处理：路径无效、权限不足等
- 返回"存在"或"不存在"

### 5.3 定时器管理
- 使用 QTimer 实现全局定时检查（默认60秒）
- 支持暂停/恢复监控
- 每个页面的总开关控制该页面所有项目
- 单个项目开关控制具体项目的监控

### 5.4 配置持久化
- JSON格式存储在程序目录的 config.json
- 程序启动时自动加载配置并开始监控
- 配置变更时自动保存
- 支持配置文件版本管理

### 5.5 表格交互
- 双击单元格直接编辑（名称、路径列）
- 复选框直接控制启用状态
- 右键菜单支持复制、删除等操作
- 支持多选批量操作

## 6. 核心类设计

### 6.1 MonitorItem 类
```cpp
class MonitorItem {
public:
    enum Type { Registry, File };
    
    QString id;
    Type type;
    QString name;
    QString path;
    bool enabled;
    QDateTime lastCheckTime;
    QString lastStatus;
    QString errorMessage;
    
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    QString checkStatus(); // 执行实际检查
};
```

### 6.2 MonitorPage 类
```cpp
class MonitorPage {
public:
    QString id;
    QString name;
    bool enabled;
    QList<MonitorItem> items;
    
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    void refreshAll(); // 刷新所有启用的项目
};
```

### 6.3 MonitorManager 类
```cpp
class MonitorManager : public QObject {
    Q_OBJECT
public:
    void startMonitoring();
    void stopMonitoring();
    void refreshAll();
    void refreshPage(const QString& pageId);
    
private slots:
    void onTimerTimeout();
    
private:
    QTimer* m_timer;
    QList<MonitorPage> m_pages;
    ConfigManager* m_configManager;
};
```

## 7. 开发计划

### 阶段1：基础框架（1-2天）
1. 创建主窗口和基本UI布局
2. 实现分页管理（QTabWidget）
3. 创建监控项表格（QTableWidget）
4. 实现基本的数据模型类

### 阶段2：核心功能（2-3天）
1. 实现注册表监控功能
2. 实现文件监控功能
3. 实现定时器管理和监控逻辑
4. 实现配置的JSON序列化

### 阶段3：界面完善（1-2天）
1. 完善表格的编辑功能
2. 实现添加/删除监控项
3. 实现页面管理功能
4. 完善开关控制逻辑

### 阶段4：优化完善（1天）
1. 错误处理和异常情况
2. 界面美化和用户体验优化
3. 测试和bug修复
4. 添加必要的提示和帮助信息

## 8. 技术要点

### 8.1 Windows注册表访问
- 需要包含 `<windows.h>` 和相关注册表API
- 注意权限问题，某些注册表项可能需要管理员权限
- 正确处理不同数据类型（REG_SZ, REG_DWORD等）

### 8.2 Qt表格控件
- 使用 QTableWidget 实现可编辑表格
- 自定义委托（QItemDelegate）处理特殊编辑需求
- 实现复选框列和按钮列

### 8.3 JSON配置管理
- 使用 Qt 的 QJsonDocument, QJsonObject, QJsonArray
- 实现配置的自动保存和加载
- 处理配置文件损坏的情况

这个设计方案涵盖了所有确定的需求，可以作为开发的详细指导文档。
