// InfoMonitorCore.cpp : 定义静态库的函数。
//

#include "include/InfoMonitorCore.h"

/*
InfoMonitorCore::InfoMonitorCore()
{
    m_pImpl = new InfoMonitorPrivate();
}

InfoMonitorCore::~InfoMonitorCore()
{
    if (m_pImpl != nullptr)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

// ========== 初始化与配置 ==========

int InfoMonitorCore::Init()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->Init();
}

int InfoMonitorCore::Uninit()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->Uninit();
}

int InfoMonitorCore::LoadConfig(const CString& strConfigPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->LoadConfig(strConfigPath);
}

int InfoMonitorCore::SaveConfig(const CString& strConfigPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SaveConfig(strConfigPath);
}

// ========== 监控项管理 ==========

int InfoMonitorCore::AddRegistryMonitor(const CString& strRegPath, const CString& strValueName, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddRegistryMonitor(strRegPath, strValueName, nGroupId);
}

int InfoMonitorCore::AddFileMonitor(const CString& strFilePath, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddFileMonitor(strFilePath, nGroupId);
}

int InfoMonitorCore::AddDirectoryMonitor(const CString& strDirPath, bool bRecursive, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->AddDirectoryMonitor(strDirPath, bRecursive, nGroupId);
}

int InfoMonitorCore::RemoveMonitor(const CString& strPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->RemoveMonitor(strPath);
}

int InfoMonitorCore::ClearAllMonitors()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ClearAllMonitors();
}

int InfoMonitorCore::EnableMonitor(const CString& strPath, bool bEnable)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->EnableMonitor(strPath, bEnable);
}

// ========== 注册表操作 ==========

int InfoMonitorCore::ReadRegistry(const CString& strRegPath, const CString& strValueName, CString& strValue)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ReadRegistry(strRegPath, strValueName, strValue);
}

int InfoMonitorCore::WriteRegistry(const CString& strRegPath, const CString& strValueName, const CString& strValue)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->WriteRegistry(strRegPath, strValueName, strValue);
}

int InfoMonitorCore::DeleteRegistry(const CString& strRegPath, const CString& strValueName)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->DeleteRegistry(strRegPath, strValueName);
}

// ========== 文件操作 ==========

int InfoMonitorCore::ReadFile(const CString& strFilePath, CString& strContent)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ReadFile(strFilePath, strContent);
}

int InfoMonitorCore::WriteFile(const CString& strFilePath, const CString& strContent)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->WriteFile(strFilePath, strContent);
}

int InfoMonitorCore::DeleteFile(const CString& strFilePath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->DeleteFile(strFilePath);
}

int InfoMonitorCore::CopyFile(const CString& strSrcPath, const CString& strDstPath)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->CopyFile(strSrcPath, strDstPath);
}

// ========== 查询与获取 ==========

int InfoMonitorCore::GetMonitorList(std::vector<MonitorItem>& vecItems)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetMonitorList(vecItems);
}

int InfoMonitorCore::GetMonitorItemsByGroup(int nGroupId, std::vector<MonitorItem>& vecItems)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetMonitorItemsByGroup(nGroupId, vecItems);
}

int InfoMonitorCore::GetAllGroups(std::vector<GroupInfo>& vecGroups)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetAllGroups(vecGroups);
}

int InfoMonitorCore::SetMonitorGroup(const CString& strPath, int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SetMonitorGroup(strPath, nGroupId);
}

int InfoMonitorCore::SetGroupName(int nGroupId, const CString& strGroupName)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->SetGroupName(nGroupId, strGroupName);
}

int InfoMonitorCore::RemoveGroup(int nGroupId)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->RemoveGroup(nGroupId);
}

int InfoMonitorCore::EnableGroup(int nGroupId, bool bEnable)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->EnableGroup(nGroupId, bEnable);
}

int InfoMonitorCore::GetChangeRecords(std::vector<ChangeRecord>& vecRecords)
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->GetChangeRecords(vecRecords);
}

int InfoMonitorCore::ClearChangeRecords()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ClearChangeRecords();
}

bool InfoMonitorCore::IsMonitored(const CString& strPath)
{
    if (m_pImpl == nullptr)
    {
        return false;
    }
    return m_pImpl->IsMonitored(strPath);
}

// ========== 监控控制 ==========

int InfoMonitorCore::StartMonitor()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->StartMonitor();
}

int InfoMonitorCore::StopMonitor()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->StopMonitor();
}

int InfoMonitorCore::ScanNow()
{
    if (m_pImpl == nullptr)
    {
        return -1;
    }
    return m_pImpl->ScanNow();
}

// ========== 其他 ==========

unsigned int InfoMonitorCore::GetVersion()
{
    return (INFOMONITORCORE_VERSION_MAJOR << 16) |
           (INFOMONITORCORE_VERSION_MINOR << 8) |
           (INFOMONITORCORE_VERSION_PATCH);
}
*/

InfoMonitorCore::InfoMonitorCore() {

}

InfoMonitorCore::~InfoMonitorCore() {

}

int InfoMonitorCore::Init() {

    return 0;
}

int InfoMonitorCore::UnInit() {

    return 0;
}
