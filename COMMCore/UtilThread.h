#pragma once
#include "CoreExp.h"

// CUtilThread

class CORE_API CUtilThread : public CWinThread
{
	DECLARE_DYNCREATE(CUtilThread)

protected:
	CUtilThread();           // protected constructor used by dynamic creation
	virtual ~CUtilThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL PostThreadMessageEx (DWORD dwThreadID, UINT uiMessage, WPARAM wParam, LPARAM lParam);
	BOOL PostMessageEx (HWND hWnd , UINT uiMessage, WPARAM wParam, LPARAM lParam);
	static BOOL StartThread(CUtilThread *pUtilThread);
	static CUtilThread * StartThread (CRuntimeClass *pRunTimeThread, LPSTR sClassName);
	static LONG ShutdownThread (CUtilThread *pUtilThread,DWORD dwMilliseconds = 0xFFFFFFFF);

};


