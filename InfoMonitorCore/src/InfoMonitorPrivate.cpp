#include "InfoMonitorPrivate.h"
#include <atlbase.h>




/*
// ========== 初始化与配置 ==========

int InfoMonitorPrivate::Init()
{
    if (m_nState != 0)
    {
        // 已经初始化过
        return -1;
    }

    // TODO(wangwenxi): 在这里添加初始化逻辑
    // 例如：创建监控线程、初始化数据结构等

    m_nState = 1;
    return 0;
}

int InfoMonitorPrivate::Uninit()
{
    if (m_nState == 0)
    {
        // 尚未初始化
        return -1;
    }


    // TODO(wangwenxi): 在这里添加反初始化逻辑
    // 例如：释放资源、停止线程等

    m_nState = 0;
    return 0;
}

// ========== 监控项管理 ==========

int InfoMonitorPrivate::AddRegistryMonitor(const CString& strRegPath, const CString& strValueName, int nGroupId)
{
    if (m_nState == 0)
    {
        return -1;
    }

    // 检查是否已存在
    for (const auto& item : m_vecMonitorItems)
    {
        if (item.type == MonitorType::Registry && item.strPath == strRegPath)
        {
            return -2; // 已存在
        }
    }

    MonitorItem item;
    item.nGroupId = nGroupId;
    item.type = MonitorType::Registry;
    item.strPath = strRegPath;
    item.strName = strValueName;
    item.bEnabled = true;

    m_vecMonitorItems.push_back(item);

    // TODO(wangwenxi): 记录当前注册表值作为快照

    return 0;
}

int InfoMonitorPrivate::AddFileMonitor(const CString& strFilePath, int nGroupId)
{
    if (m_nState == 0)
    {
        return -1;
    }

    // 检查是否已存在
    for (const auto& item : m_vecMonitorItems)
    {
        if (item.type == MonitorType::File && item.strPath == strFilePath)
        {
            return -2; // 已存在
        }
    }

    MonitorItem item;
    item.nGroupId = nGroupId;
    item.type = MonitorType::File;
    item.strPath = strFilePath;
    item.bEnabled = true;

    m_vecMonitorItems.push_back(item);

    // TODO(wangwenxi): 记录当前文件修改时间作为快照

    return 0;
}

int InfoMonitorPrivate::AddDirectoryMonitor(const CString& strDirPath, bool bRecursive, int nGroupId)
{
    if (m_nState == 0)
    {
        return -1;
    }

    // 检查是否已存在
    for (const auto& item : m_vecMonitorItems)
    {
        if (item.type == MonitorType::Directory && item.strPath == strDirPath)
        {
            return -2; // 已存在
        }
    }

    MonitorItem item;
    item.nGroupId = nGroupId;
    item.type = MonitorType::Directory;
    item.strPath = strDirPath;
    item.strName = bRecursive ? L"Recursive" : L"";
    item.bEnabled = true;

    m_vecMonitorItems.push_back(item);

    // TODO(wangwenxi): 记录当前目录状态作为快照

    return 0;
}

int InfoMonitorPrivate::RemoveMonitor(const CString& strPath)
{
    if (m_nState == 0)
    {
        return -1;
    }

    for (auto it = m_vecMonitorItems.begin(); it != m_vecMonitorItems.end(); ++it)
    {
        if (it->strPath == strPath)
        {
            m_vecMonitorItems.erase(it);
            return 0;
        }
    }

    return -2; // 未找到
}

int InfoMonitorPrivate::ClearAllMonitors()
{
    if (m_nState == 0)
    {
        return -1;
    }

    m_vecMonitorItems.clear();
    m_mapRegistrySnapshots.clear();
    m_mapFileSnapshots.clear();

    return 0;
}

int InfoMonitorPrivate::EnableMonitor(const CString& strPath, bool bEnable)
{
    if (m_nState == 0)
    {
        return -1;
    }

    for (auto& item : m_vecMonitorItems)
    {
        if (item.strPath == strPath)
        {
            item.bEnabled = bEnable;
            return 0;
        }
    }

    return -2; // 未找到
}

// ========== 注册表操作 ==========

int InfoMonitorPrivate::ReadRegistry(const CString& strRegPath, const CString& strValueName, CString& strValue)
{
    // TODO(wangwenxi): 使用 CRegKey 实现注册表读取
    // 需要解析 strRegPath，分离出根键和子键路径

    return 0;
}

int InfoMonitorPrivate::WriteRegistry(const CString& strRegPath, const CString& strValueName, const CString& strValue)
{
    // TODO(wangwenxi): 使用 CRegKey 实现注册表写入

    return 0;
}

int InfoMonitorPrivate::DeleteRegistry(const CString& strRegPath, const CString& strValueName)
{
    // TODO(wangwenxi): 使用 CRegKey 实现注册表删除

    return 0;
}

// ========== 文件操作 ==========

int InfoMonitorPrivate::ReadFile(const CString& strFilePath, CString& strContent)
{
    // TODO(wangwenxi): 实现文件读取
    // 建议使用 CFile 或 _wfopen_s

    return 0;
}

int InfoMonitorPrivate::WriteFile(const CString& strFilePath, const CString& strContent)
{
    // TODO(wangwenxi): 实现文件写入

    return 0;
}

int InfoMonitorPrivate::DeleteFile(const CString& strFilePath)
{
    // TODO(wangwenxi): 实现文件删除
    // 使用 ::DeleteFile(strFilePath)

    return 0;
}

int InfoMonitorPrivate::CopyFile(const CString& strSrcPath, const CString& strDstPath)
{
    // TODO(wangwenxi): 实现文件复制
    // 使用 ::CopyFile(strSrcPath, strDstPath, FALSE)

    return 0;
}

// ========== 查询与获取 ==========

int InfoMonitorPrivate::GetMonitorList(std::vector<MonitorItem>& vecItems)
{
    if (m_nState == 0)
    {
        return -1;
    }

    vecItems = m_vecMonitorItems;
    return 0;
}

int InfoMonitorPrivate::GetChangeRecords(std::vector<ChangeRecord>& vecRecords)
{
    if (m_nState == 0)
    {
        return -1;
    }

    vecRecords = m_vecChangeRecords;
    return 0;
}

int InfoMonitorPrivate::ClearChangeRecords()
{
    if (m_nState == 0)
    {
        return -1;
    }

    m_vecChangeRecords.clear();
    return 0;
}

int InfoMonitorPrivate::GetMonitorItemsByGroup(int nGroupId, std::vector<MonitorItem>& vecItems)
{
    if (m_nState == 0)
    {
        return -1;
    }

    vecItems.clear();
    for (const auto& item : m_vecMonitorItems)
    {
        if (item.nGroupId == nGroupId)
        {
            vecItems.push_back(item);
        }
    }

    return 0;
}

int InfoMonitorPrivate::GetAllGroups(std::vector<GroupInfo>& vecGroups)
{
    if (m_nState == 0)
    {
        return -1;
    }

    vecGroups.clear();

    // 统计每个组的监控项数量
    std::map<int, int> mapGroupCounts;
    for (const auto& item : m_vecMonitorItems)
    {
        mapGroupCounts[item.nGroupId]++;
    }

    // 构建 GroupInfo 列表
    for (const auto& pair : mapGroupCounts)
    {
        GroupInfo info;
        info.nGroupId = pair.first;
        info.nItemCount = pair.second;
        
        // 从分组名称映射中获取组名
        auto it = m_mapGroupNames.find(pair.first);
        if (it != m_mapGroupNames.end())
        {
            info.strGroupName = it->second;
        }
        
        vecGroups.push_back(info);
    }

    return 0;
}

int InfoMonitorPrivate::SetMonitorGroup(const CString& strPath, int nGroupId)
{
    if (m_nState == 0)
    {
        return -1;
    }

    for (auto& item : m_vecMonitorItems)
    {
        if (item.strPath == strPath)
        {
            item.nGroupId = nGroupId;
            return 0;
        }
    }

    return -2; // 未找到
}

int InfoMonitorPrivate::SetGroupName(int nGroupId, const CString& strGroupName)
{
    if (m_nState == 0)
    {
        return -1;
    }

    m_mapGroupNames[nGroupId] = strGroupName;
    return 0;
}

int InfoMonitorPrivate::RemoveGroup(int nGroupId)
{
    if (m_nState == 0)
    {
        return -1;
    }

    // 删除该组的所有监控项
    auto it = m_vecMonitorItems.begin();
    while (it != m_vecMonitorItems.end())
    {
        if (it->nGroupId == nGroupId)
        {
            it = m_vecMonitorItems.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 删除组名
    m_mapGroupNames.erase(nGroupId);

    return 0;
}

int InfoMonitorPrivate::EnableGroup(int nGroupId, bool bEnable)
{
    if (m_nState == 0)
    {
        return -1;
    }

    int nCount = 0;
    for (auto& item : m_vecMonitorItems)
    {
        if (item.nGroupId == nGroupId)
        {
            item.bEnabled = bEnable;
            nCount++;
        }
    }

    return nCount > 0 ? 0 : -2; // 未找到任何该组的监控项
}

bool InfoMonitorPrivate::IsMonitored(const CString& strPath)
{
    for (const auto& item : m_vecMonitorItems)
    {
        if (item.strPath == strPath)
        {
            return true;
        }
    }
    return false;
}

// ========== 监控控制 ==========

int InfoMonitorPrivate::StartMonitor()
{
    if (m_nState == 0)
    {
        return -1;
    }

    if (m_nMonitorRunning != 0)
    {
        return -2; // 已经在运行
    }

    // TODO(wangwenxi): 启动监控线程或定时器

    m_nMonitorRunning = 1;
    return 0;
}

int InfoMonitorPrivate::StopMonitor()
{
    if (m_nState == 0)
    {
        return -1;
    }

    if (m_nMonitorRunning == 0)
    {
        return 0; // 未运行，直接返回成功
    }

    // TODO(wangwenxi): 停止监控线程或定时器

    m_nMonitorRunning = 0;
    return 0;
}

int InfoMonitorPrivate::ScanNow()
{
    if (m_nState == 0)
    {
        return -1;
    }

    // TODO(wangwenxi): 立即扫描所有监控项，检测变化
    // 遍历 m_vecMonitorItems，对比当前状态与快照，记录变更到 m_vecChangeRecords

    return 0;
}

*/
