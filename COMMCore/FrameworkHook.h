#pragma once
#include "stdafx.h"

class CFrameworkHook
{
private:
	static CFrameworkHook instance;
	static bool processing;
	static HHOOK m_MainWndGetMessageHook;
	static HHOOK m_MainWndWndProcHook;
	static void HookProcess(UINT message,WPARAM wParam,LPARAM lParam);

	static LRESULT CALLBACK MainWndGetMessageCallBack(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MainWndWndProcCallBack(int code, WPARAM wParam, LPARAM lParam);

	CFrameworkHook();
public:
	virtual ~CFrameworkHook();
};