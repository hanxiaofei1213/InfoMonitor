#pragma once

#include <vector>
#include <map>
#include <atlstr.h>
#include "../include/InfoMonitorCore.h"

class InfoMonitorPrivate
{
public:
    InfoMonitorPrivate() = default;
    ~InfoMonitorPrivate() = default;

    // 初始化与配置
    int Init();
    int Uninit();

    // 监控项管理
    int AddRegistryMonitor(const CString& strRegPath, const CString& strValueName, int nGroupId);
    int AddFileMonitor(const CString& strFilePath, int nGroupId);
    int AddDirectoryMonitor(const CString& strDirPath, bool bRecursive, int nGroupId);
    int RemoveMonitor(const CString& strPath);
    int ClearAllMonitors();
    int EnableMonitor(const CString& strPath, bool bEnable);

    // 注册表操作
    int ReadRegistry(const CString& strRegPath, const CString& strValueName, CString& strValue);
    int WriteRegistry(const CString& strRegPath, const CString& strValueName, const CString& strValue);
    int DeleteRegistry(const CString& strRegPath, const CString& strValueName);

    // 文件操作
    int ReadFile(const CString& strFilePath, CString& strContent);
    int WriteFile(const CString& strFilePath, const CString& strContent);
    int DeleteFile(const CString& strFilePath);
    int CopyFile(const CString& strSrcPath, const CString& strDstPath);

    // 查询与获取
    int GetMonitorList(std::vector<MonitorItem>& vecItems);
    int GetMonitorItemsByGroup(int nGroupId, std::vector<MonitorItem>& vecItems);
    int GetAllGroups(std::vector<GroupInfo>& vecGroups);
    int SetMonitorGroup(const CString& strPath, int nGroupId);
    int SetGroupName(int nGroupId, const CString& strGroupName);
    int RemoveGroup(int nGroupId);
    int EnableGroup(int nGroupId, bool bEnable);
    int GetChangeRecords(std::vector<ChangeRecord>& vecRecords);
    int ClearChangeRecords();
    bool IsMonitored(const CString& strPath);

private:
    int m_nState = 0;                                       // 初始化状态：0-未初始化，1-已初始化
    int m_nMonitorRunning = 0;                              // 监控运行状态：0-未运行，1-运行中
    std::vector<MonitorItem> m_vecMonitorItems;             // 监控项列表
    std::vector<ChangeRecord> m_vecChangeRecords;           // 变更记录
    std::map<int, CString> m_mapGroupNames;                 // 分组名称（组号->组名）
    std::map<CString, CString> m_mapRegistrySnapshots;      // 注册表快照（路径->值）
    std::map<CString, FILETIME> m_mapFileSnapshots;         // 文件快照（路径->修改时间）
};
