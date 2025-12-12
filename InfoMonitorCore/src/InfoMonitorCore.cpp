// InfoMonitorCore.cpp : 定义静态库的函数。
//

#include "include/InfoMonitorCore.h"
#include "InfoMonitorPrivate.h"

CInfoMonitorCore::CInfoMonitorCore()
{
    m_pImpl = new InfoMonitorPrivate();
}

CInfoMonitorCore::~CInfoMonitorCore()
{
    if (m_pImpl != nullptr)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

// ========== 初始化与配置 ==========

int CInfoMonitorCore::Init()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->Init();
}

int CInfoMonitorCore::Uninit()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->Uninit();
}

int CInfoMonitorCore::LoadConfig(const CString& strConfigPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->LoadConfig(strConfigPath);
}

int CInfoMonitorCore::SaveConfig(const CString& strConfigPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SaveConfig(strConfigPath);
}

// ========== 监控项管理 ==========

int CInfoMonitorCore::AddRegistryMonitor(const CString& strRegPath, const CString& strValueName, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddRegistryMonitor(strRegPath, strValueName, nGroupId);
}

int CInfoMonitorCore::AddFileMonitor(const CString& strFilePath, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddFileMonitor(strFilePath, nGroupId);
}

int CInfoMonitorCore::AddDirectoryMonitor(const CString& strDirPath, bool bRecursive, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddDirectoryMonitor(strDirPath, bRecursive, nGroupId);
}

int CInfoMonitorCore::RemoveMonitor(const CString& strPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->RemoveMonitor(strPath);
}

int CInfoMonitorCore::ClearAllMonitors()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ClearAllMonitors();
}

int CInfoMonitorCore::EnableMonitor(const CString& strPath, bool bEnable)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->EnableMonitor(strPath, bEnable);
}

// ========== 注册表操作 ==========

int CInfoMonitorCore::ReadRegistry(const CString& strRegPath, const CString& strValueName, CString& strValue)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ReadRegistry(strRegPath, strValueName, strValue);
}

int CInfoMonitorCore::WriteRegistry(const CString& strRegPath, const CString& strValueName, const CString& strValue)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->WriteRegistry(strRegPath, strValueName, strValue);
}

int CInfoMonitorCore::DeleteRegistry(const CString& strRegPath, const CString& strValueName)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->DeleteRegistry(strRegPath, strValueName);
}

// ========== 文件操作 ==========

int CInfoMonitorCore::ReadFile(const CString& strFilePath, CString& strContent)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ReadFile(strFilePath, strContent);
}

int CInfoMonitorCore::WriteFile(const CString& strFilePath, const CString& strContent)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->WriteFile(strFilePath, strContent);
}

int CInfoMonitorCore::DeleteFile(const CString& strFilePath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->DeleteFile(strFilePath);
}

int CInfoMonitorCore::CopyFile(const CString& strSrcPath, const CString& strDstPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->CopyFile(strSrcPath, strDstPath);
}

// ========== 查询与获取 ==========

int CInfoMonitorCore::GetMonitorList(std::vector<MonitorItem>& vecItems)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetMonitorList(vecItems);
}

int CInfoMonitorCore::GetMonitorItemsByGroup(int nGroupId, std::vector<MonitorItem>& vecItems)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetMonitorItemsByGroup(nGroupId, vecItems);
}

int CInfoMonitorCore::GetAllGroups(std::vector<GroupInfo>& vecGroups)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetAllGroups(vecGroups);
}

int CInfoMonitorCore::SetMonitorGroup(const CString& strPath, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SetMonitorGroup(strPath, nGroupId);
}

int CInfoMonitorCore::SetGroupName(int nGroupId, const CString& strGroupName)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SetGroupName(nGroupId, strGroupName);
}

int CInfoMonitorCore::RemoveGroup(int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->RemoveGroup(nGroupId);
}

int CInfoMonitorCore::EnableGroup(int nGroupId, bool bEnable)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->EnableGroup(nGroupId, bEnable);
}

int CInfoMonitorCore::GetChangeRecords(std::vector<ChangeRecord>& vecRecords)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetChangeRecords(vecRecords);
}

int CInfoMonitorCore::ClearChangeRecords()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ClearChangeRecords();
}

bool CInfoMonitorCore::IsMonitored(const CString& strPath)
{
    if (m_pImpl == nullptr)
    {
        return false;
    }
    return m_pImpl->IsMonitored(strPath);
}

// ========== 监控控制 ==========

int CInfoMonitorCore::StartMonitor()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->StartMonitor();
}

int CInfoMonitorCore::StopMonitor()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->StopMonitor();
}

int CInfoMonitorCore::ScanNow()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ScanNow();
}

// ========== 其他 ==========

unsigned int CInfoMonitorCore::GetVersion()
{
    return (INFOMONITORCORE_VERSION_MAJOR << 16) |
           (INFOMONITORCORE_VERSION_MINOR << 8) |
           (INFOMONITORCORE_VERSION_PATCH);
}
