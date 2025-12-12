#pragma once

#include <vector>
#include <atlstr.h>

// InfoMonitorCore 版本定义
#define INFOMONITORCORE_VERSION_MAJOR 1
#define INFOMONITORCORE_VERSION_MINOR 0
#define INFOMONITORCORE_VERSION_PATCH 0

// 监控项类型
enum class MonitorType
{
    Registry,   // 注册表
    File,       // 文件
    Directory   // 目录
};

// 监控项信息
struct MonitorItem
{
    int nGroupId = 0;               // 组号（0表示未分组）
    MonitorType type = MonitorType::File;
    CString strPath;                // 注册表路径或文件路径
    CString strName;                // 监控项名称（可选）
    bool bEnabled = true;           // 是否启用
};

// 分组信息
struct GroupInfo
{
    int nGroupId = 0;               // 组号
    CString strGroupName;           // 组名
    int nItemCount = 0;             // 该组包含的监控项数量
};

// 变更记录
struct ChangeRecord
{
    MonitorType type = MonitorType::File;
    CString strPath;
    CString strOldValue;
    CString strNewValue;
    SYSTEMTIME timeChanged = {};
};

// 前置声明内部实现类（仅指针类型暴露给外部，不暴露实现细节）
class InfoMonitorPrivate;

/// <summary>
/// InfoMonitorCore 核心类（pImpl 模式）
/// 用于批量管理注册表、文件的监控与操作
/// </summary>
class CInfoMonitorCore
{
public:
    CInfoMonitorCore();
    ~CInfoMonitorCore();

    // ========== 初始化与配置 ==========

    /// <summary>
    /// 初始化监控核心
    /// </summary>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int Init();

    /// <summary>
    /// 反初始化监控核心
    /// </summary>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int Uninit();

    // ========== 监控项管理 ==========

    /// <summary>
    /// 添加注册表监控项
    /// </summary>
    /// <param name="strRegPath">注册表路径（如 HKEY_LOCAL_MACHINE\\Software\\XXX）</param>
    /// <param name="strValueName">值名称（空表示监控整个键）</param>
    /// <param name="nGroupId">组号（0表示未分组）</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int AddRegistryMonitor(const CString& strRegPath, const CString& strValueName = L"", int nGroupId = 0);

    /// <summary>
    /// 添加文件监控项
    /// </summary>
    /// <param name="strFilePath">文件完整路径</param>
    /// <param name="nGroupId">组号（0表示未分组）</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int AddFileMonitor(const CString& strFilePath, int nGroupId = 0);

    /// <summary>
    /// 添加目录监控项
    /// </summary>
    /// <param name="strDirPath">目录路径</param>
    /// <param name="bRecursive">是否递归监控子目录</param>
    /// <param name="nGroupId">组号（0表示未分组）</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int AddDirectoryMonitor(const CString& strDirPath, bool bRecursive = false, int nGroupId = 0);

    /// <summary>
    /// 删除监控项
    /// </summary>
    /// <param name="strPath">路径</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int RemoveMonitor(const CString& strPath);

    /// <summary>
    /// 清空所有监控项
    /// </summary>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int ClearAllMonitors();

    /// <summary>
    /// 启用/禁用监控项
    /// </summary>
    /// <param name="strPath">路径</param>
    /// <param name="bEnable">是否启用</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int EnableMonitor(const CString& strPath, bool bEnable);

    // ========== 注册表操作 ==========

    /// <summary>
    /// 读取注册表值
    /// </summary>
    /// <param name="strRegPath">注册表路径</param>
    /// <param name="strValueName">值名称</param>
    /// <param name="strValue">输出：读取的值</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int ReadRegistry(const CString& strRegPath, const CString& strValueName, CString& strValue);

    /// <summary>
    /// 写入注册表值
    /// </summary>
    /// <param name="strRegPath">注册表路径</param>
    /// <param name="strValueName">值名称</param>
    /// <param name="strValue">要写入的值</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int WriteRegistry(const CString& strRegPath, const CString& strValueName, const CString& strValue);

    /// <summary>
    /// 删除注册表值
    /// </summary>
    /// <param name="strRegPath">注册表路径</param>
    /// <param name="strValueName">值名称</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int DeleteRegistry(const CString& strRegPath, const CString& strValueName);

    // ========== 文件操作 ==========

    /// <summary>
    /// 读取文件内容
    /// </summary>
    /// <param name="strFilePath">文件路径</param>
    /// <param name="strContent">输出：文件内容</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int ReadFile(const CString& strFilePath, CString& strContent);

    /// <summary>
    /// 写入文件内容
    /// </summary>
    /// <param name="strFilePath">文件路径</param>
    /// <param name="strContent">要写入的内容</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int WriteFile(const CString& strFilePath, const CString& strContent);

    /// <summary>
    /// 删除文件
    /// </summary>
    /// <param name="strFilePath">文件路径</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int DeleteFile(const CString& strFilePath);

    /// <summary>
    /// 复制文件
    /// </summary>
    /// <param name="strSrcPath">源文件路径</param>
    /// <param name="strDstPath">目标文件路径</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int CopyFile(const CString& strSrcPath, const CString& strDstPath);

    // ========== 查询与获取 ==========

    /// <summary>
    /// 获取所有监控项列表
    /// </summary>
    /// <param name="vecItems">输出：监控项列表</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int GetMonitorList(std::vector<MonitorItem>& vecItems);

    /// <summary>
    /// 按组号获取监控项列表
    /// </summary>
    /// <param name="nGroupId">组号</param>
    /// <param name="vecItems">输出：监控项列表</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int GetMonitorItemsByGroup(int nGroupId, std::vector<MonitorItem>& vecItems);

    /// <summary>
    /// 获取所有分组信息
    /// </summary>
    /// <param name="vecGroups">输出：分组信息列表</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int GetAllGroups(std::vector<GroupInfo>& vecGroups);

    /// <summary>
    /// 设置监控项的组号
    /// </summary>
    /// <param name="strPath">路径</param>
    /// <param name="nGroupId">组号</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int SetMonitorGroup(const CString& strPath, int nGroupId);

    /// <summary>
    /// 设置分组名称
    /// </summary>
    /// <param name="nGroupId">组号</param>
    /// <param name="strGroupName">组名</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int SetGroupName(int nGroupId, const CString& strGroupName);

    /// <summary>
    /// 删除整个分组（及其所有监控项）
    /// </summary>
    /// <param name="nGroupId">组号</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int RemoveGroup(int nGroupId);

    /// <summary>
    /// 启用/禁用整个分组
    /// </summary>
    /// <param name="nGroupId">组号</param>
    /// <param name="bEnable">是否启用</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int EnableGroup(int nGroupId, bool bEnable);

    /// <summary>
    /// 获取变更记录
    /// </summary>
    /// <param name="vecRecords">输出：变更记录</param>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int GetChangeRecords(std::vector<ChangeRecord>& vecRecords);

    /// <summary>
    /// 清空变更记录
    /// </summary>
    /// <returns>成功返回 0，失败返回错误码</returns>
    int ClearChangeRecords();

    /// <summary>
    /// 检查路径是否在监控中
    /// </summary>
    /// <param name="strPath">路径</param>
    /// <returns>是否在监控中</returns>
    bool IsMonitored(const CString& strPath);

    // ========== 其他 ==========

    /// <summary>
    /// 获取版本号
    /// </summary>
    /// <returns>版本号（高16位：主版本，中8位：次版本，低8位：补丁版本）</returns>
    unsigned int GetVersion();

protected:
    InfoMonitorPrivate* m_pImpl = nullptr;
};
