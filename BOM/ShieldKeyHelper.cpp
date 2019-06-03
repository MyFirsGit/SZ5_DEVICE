#include "stdafx.h"
#include "ShieldKeyHelper.h"
#include <Tlhelp32.h>

HHOOK CShieldKeyHelper::hookBOMShield(NULL);



bool CShieldKeyHelper::SetFunctionKeysEnable(bool enabled)
{
	if(enabled)
	{
		BOOL success = ::UnhookWindowsHookEx(hookBOMShield);
		if(success)
		{
			hookBOMShield = NULL;
		}
		return success;
	}
	else
	{
		hookBOMShield = ::SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)HookProc,AfxGetApp()->m_hInstance,NULL);
		return hookBOMShield == NULL;
	}
}

LRESULT CShieldKeyHelper::HookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	LPKBDLLHOOKSTRUCT pKB = (LPKBDLLHOOKSTRUCT)lParam;
	BOOL needShield = FALSE;
	if(nCode == HC_ACTION)
	{
		switch(wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			needShield = IsNeedShield(pKB);
			break;
		}
	}
	if(needShield)
	{
		return TRUE;
	}
	return CallNextHookEx(hookBOMShield,nCode,wParam,lParam);
}


BOOL CShieldKeyHelper::IsNeedShield(LPKBDLLHOOKSTRUCT pKB)
{
	if(pKB->vkCode == VK_RWIN)
	{
		return TRUE;
	}
	if(pKB->vkCode == VK_LWIN)
	{
		return TRUE;
	}
	if((pKB->flags & LLKHF_ALTDOWN)!=0)
	{
		return TRUE;
	}
	if((GetAsyncKeyState(VK_CONTROL) & 0X8000 )!=0)
	{
		
		if(pKB->vkCode == VK_LCONTROL || pKB->vkCode == VK_RCONTROL || pKB->vkCode == VK_LSHIFT || pKB->vkCode == VK_RSHIFT )//输入法切换,允许
		{
			return FALSE;
		}
		else
		{
			
			return TRUE;
		}
	}
	return FALSE;
}


bool CShieldKeyHelper::SetTaskManagerEnable(bool enabled)
{
	TCHAR lpszDllPath[MAX_PATH] = {0};
	// 设置Dll路径
	if (!GetFullPathName(_T("BOMShield.dll"), MAX_PATH, lpszDllPath, NULL))
	{
		return false;
	}
	HKEY hKey;
	CString strKeyName = _T("Software\\Founder\\AFC\\BOM\\Control");
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName,0,KEY_WRITE,&hKey) == ERROR_SUCCESS){
		DWORD dwEnabled = enabled ? 0 : 1;
		RegSetValueEx(hKey, _T("ShieldTaskManager"),0,REG_DWORD,(LPBYTE)&dwEnabled,4);
		RegCloseKey(hKey);
	}
	CString strBOMShieldDllPath;
	strBOMShieldDllPath.Format(_T("%s"),lpszDllPath);
	if(!enabled)
	{
		Eject(strBOMShieldDllPath);
		return Inject(strBOMShieldDllPath);
	}
	else
	{
		return Eject(strBOMShieldDllPath);
	}
}



bool CShieldKeyHelper::SetHostProgarmEnable(bool enabled)
{
	CString strGuardianExe = _T("BOMGuardian.exe");
	if(enabled)
	{
		DWORD guardianProcessID = GetProcessIdFromName(strGuardianExe);
		if(guardianProcessID<=0)
		{
			return ComExecuteApp(strGuardianExe,NULL);
		}
		return TRUE;
	}
	else
	{
		return ComKillAppEx(strGuardianExe);
	}
}

bool CShieldKeyHelper::SetDesktopEnable(bool enabled)
{
	CString strDesktop = _T("explorer.exe");
	if(enabled)
	{
		DWORD desktopProcessID = GetProcessIdFromName(strDesktop);
		if(desktopProcessID<=0)
		{
			return ComExecuteApp(strDesktop,NULL);
		}
		return TRUE;
	}
	else
	{
		return ComKillAppEx(strDesktop);

	}
}

BOOL WINAPI CShieldKeyHelper::EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY | TOKEN_READ,&hToken))
		return FALSE;

	if(!LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
		return TRUE;

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = (bEnable) ? SE_PRIVILEGE_ENABLED : 0;

	AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL);

	CloseHandle(hToken);

	return (GetLastError() == ERROR_SUCCESS);
}

BOOL CShieldKeyHelper::Inject(CString& strDllPath)
{
	EnablePrivilege(SE_DEBUG_NAME,TRUE);
	HANDLE processWinlogon  =NULL;
	BYTE* pDataRemote = NULL;
	HANDLE hRemoteLoadLibrary = NULL;
	BOOL success = FALSE;
	__try{
		 processWinlogon = OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetProcessIdFromName(_T("winlogon.exe")));
		if(processWinlogon == NULL)
		{
			__leave;
		}
		int cbSize = (strDllPath.GetLength()+1)*sizeof(TCHAR);
		pDataRemote = (BYTE*)VirtualAllocEx(processWinlogon,0,cbSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if(pDataRemote == NULL)
		{
			__leave;
		}
		
		WriteProcessMemory(processWinlogon,pDataRemote,(LPCVOID)strDllPath,cbSize,NULL);
		LPTHREAD_START_ROUTINE lpfnLoadLibary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")),"LoadLibraryA");
		hRemoteLoadLibrary = CreateRemoteThread(processWinlogon,NULL,0,lpfnLoadLibary,pDataRemote,0,NULL);
		if(hRemoteLoadLibrary == NULL)
			__leave;
		WaitForSingleObject(hRemoteLoadLibrary,INFINITE);
		success = TRUE;
	}
	__finally
	{
		if(pDataRemote)
		{
			VirtualFreeEx(processWinlogon,pDataRemote,NULL,MEM_RELEASE);
		}
		if(hRemoteLoadLibrary)
		{
			CloseHandle(hRemoteLoadLibrary);
			hRemoteLoadLibrary = NULL;
		}
		if(processWinlogon)
		{
			CloseHandle(processWinlogon);
		}
	}

	EnablePrivilege(SE_DEBUG_NAME,FALSE);
	
	return success;
}


BOOL CShieldKeyHelper::Eject(CString& strDllPath)
{
	EnablePrivilege(SE_DEBUG_NAME,TRUE);
	HANDLE processWinlogon = NULL;
	HANDLE hRemoteFreeLibrary = NULL;
	BOOL success = FALSE;
	__try{
		processWinlogon = OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetProcessIdFromName(_T("winlogon.exe")));
		if(processWinlogon == NULL)
			__leave;
		DWORD dwDllBaseAddress = GetDllBaseAddress(GetProcessIdFromName(_T("winlogon.exe")),strDllPath);
		if(!dwDllBaseAddress)
			__leave;
		LPTHREAD_START_ROUTINE lpfnFreeLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("Kernel32")),"FreeLibrary");
		if(!lpfnFreeLibrary)
			__leave;
		hRemoteFreeLibrary = CreateRemoteThread(processWinlogon,NULL,0,lpfnFreeLibrary,(LPVOID)dwDllBaseAddress,0,NULL);
		WaitForSingleObject(hRemoteFreeLibrary,INFINITE);
		success = TRUE;
	}
	__finally
	{
		if(hRemoteFreeLibrary)
		{
			CloseHandle(hRemoteFreeLibrary);
		}
		if(processWinlogon)
		{
			CloseHandle(processWinlogon);
		}
	}

	EnablePrivilege(SE_DEBUG_NAME,FALSE);
	return success;
}

DWORD CShieldKeyHelper::GetDllBaseAddress(DWORD dwPID,CString& strDllPath)
{
	
	EnablePrivilege(SE_DEBUG_NAME,TRUE);
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(MODULEENTRY32);
	DWORD dwAdress = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);
	if(hSnapShot == NULL)
	{
		return dwAdress;
	}
	BOOL hasNext = Module32First(hSnapShot,&modEntry);
	while(hasNext)
	{
		if(lstrcmpi((LPCTSTR)strDllPath,modEntry.szExePath) == 0)
		{
			dwAdress = (DWORD)modEntry.modBaseAddr;
			break;
		}
		hasNext = Module32Next(hSnapShot,&modEntry);
	}
	CloseHandle(hSnapShot);
	EnablePrivilege(SE_DEBUG_NAME,FALSE);
	return dwAdress;
}