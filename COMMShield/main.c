#include <windows.h>
#include <Winwlx.h>
#include <wtypes.h>
#include <winuser.h>
#include <winnt.h>
#include <winbase.h>


long lOldSASWindowProc;
HANDLE hSASWnd;
HDESK hDeskWnd;

LRESULT  SASWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL  EnumWindowProc(HWND hWnd, LPARAM lParam);
BOOL  NeedShield();

BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,
  DWORD fdwReason,
  LPVOID lpvReserved
  )
{
	switch (fdwReason){
	case DLL_PROCESS_ATTACH:
		{
			hDeskWnd = OpenDesktop("winlogon", 0, FALSE, MAXIMUM_ALLOWED);
			EnumDesktopWindows(hDeskWnd, (WNDENUMPROC)EnumWindowProc, 0);
			if (hSASWnd)
			{
				lOldSASWindowProc = (long)SetWindowLong(hSASWnd, GWL_WNDPROC, (long)SASWndProc);
			}
			CloseDesktop(hDeskWnd);
		}

		break;
	case DLL_PROCESS_DETACH:
		{
			if (lOldSASWindowProc)
			{
				SetWindowLong(hSASWnd, GWL_WNDPROC, (long)lOldSASWindowProc);
			}
		}
		break;
	}

	return TRUE;
};

BOOL  EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	TCHAR lpszWinText[256] = {0};
	const int MAXCOUNT = 256;  
	BOOL found = FALSE;
	GetWindowText(hWnd, lpszWinText, MAXCOUNT);
	found = (0 == lstrcmpi(lpszWinText, "SAS Window"));
	if (found)
	{
		hSASWnd = hWnd;
		return FALSE;
	}

	return TRUE;
};


LRESULT  SASWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_HOTKEY == uMsg)
	{
		DWORD dwKey = HIWORD(lParam);
		DWORD dwModify = LOWORD(lParam);
		BOOL isCtrlDown  = ((dwModify & VK_CONTROL) != 0);
		BOOL isAltDown   = ((dwModify & VK_MENU) != 0);
		BOOL isShiftDown = ((dwModify & VK_SHIFT)!=0);
		if ((VK_DELETE == dwKey) && isCtrlDown && isAltDown)
		{
			BOOL needShield = NeedShield();
			if(needShield)
			{
				return TRUE; 
			}
		}
		if((VK_ESCAPE == dwKey) & isCtrlDown & isShiftDown)
		{
			BOOL needShield = NeedShield();
			if(needShield)
			{
				return TRUE; 
			}
		}
	}

	return CallWindowProc((WNDPROC)lOldSASWindowProc, hWnd, uMsg, wParam, lParam);
}

BOOL  NeedShield()
{
	BOOL needShield = TRUE;
	HKEY hKey;
	LPCSTR strKeyName = "Software\\Founder\\AFC\\Device\\Control";
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName,0,KEY_READ,&hKey) == ERROR_SUCCESS){
		DWORD dwShield = 0;
		DWORD dwDataType, dwBufSize;
		dwBufSize = 4;
		if(RegQueryValueEx(hKey, "ShieldTaskManager", 0, &dwDataType, (LPBYTE)&dwShield, &dwBufSize) == ERROR_SUCCESS)
		{
			needShield = dwShield>0;
		}
		RegCloseKey(hKey);
	}
	return needShield;
}

