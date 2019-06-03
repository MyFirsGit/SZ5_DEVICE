#pragma once;

class CShieldKeyHelper
{
public:

	static bool SetFunctionKeysEnable(bool enabled);
	static bool SetTaskManagerEnable(bool enabled);
	static bool SetHostProgarmEnable(bool enabled);
	static bool SetDesktopEnable(bool enabled);

private:
	static HHOOK hookBOMShield;
	static LRESULT WINAPI HookProc(int nCode,WPARAM wParam,LPARAM lParam);
	static BOOL IsNeedShield(LPKBDLLHOOKSTRUCT pKB);
	static BOOL WINAPI EnablePrivilege(LPCTSTR lpszPrivilegeName,BOOL bEnable);
	static BOOL Inject(CString& strDllPath);
	static BOOL Eject(CString& strDllPath);
	static DWORD GetDllBaseAddress(DWORD dwPID,CString& strDllPath);

};