#include "stdafx.h"
#include "FrameworkHook.h"
#include "CoreAPI.h"
#include "Service.h"

CFrameworkHook CFrameworkHook::instance;
bool CFrameworkHook::processing = false;
HHOOK CFrameworkHook::m_MainWndGetMessageHook = NULL;
HHOOK CFrameworkHook::m_MainWndWndProcHook = NULL;

CFrameworkHook::CFrameworkHook()
{
	//if(m_MainWndGetMessageHook == NULL)
	//{
	//	m_MainWndGetMessageHook = ::SetWindowsHookEx(WH_GETMESSAGE,MainWndGetMessageCallBack,AfxGetApp()->m_hInstance,AfxGetApp()->m_nThreadID);
	//}
	//if(m_MainWndWndProcHook==NULL)
	//{
	//	m_MainWndWndProcHook =::SetWindowsHookEx(WH_CALLWNDPROC,MainWndWndProcCallBack,AfxGetApp()->m_hInstance,AfxGetApp()->m_nThreadID);
	//}
}

CFrameworkHook::~CFrameworkHook()
{
	if(m_MainWndGetMessageHook!=NULL)
	{
		UnhookWindowsHookEx(m_MainWndGetMessageHook);
		m_MainWndGetMessageHook = NULL;
	}
	if(m_MainWndWndProcHook!=NULL)
	{
		UnhookWindowsHookEx(m_MainWndWndProcHook);
		m_MainWndWndProcHook = NULL;
	}
}

void CFrameworkHook::HookProcess(UINT message,WPARAM wParam,LPARAM lParam)
{
	if(message==SP_CONVERT)
	{
		if(!processing)
		{
			processing = true;
			ServiceMsg* lpMsg = (ServiceMsg*)lParam;
			ServiceMsg msg = *lpMsg;
			delete lpMsg;
			lpMsg = NULL;
			if(msg.pService!=NULL && msg.pService->IsForeground())
			{
				msg.pService->DispachCmdMsg(msg.message,msg.wParam,msg.lParam);
			}
			else
			{
				msg.pService->SendMessage(msg.message,msg.wParam,msg.lParam);
			}
			processing = false;
		}
	}
}

LRESULT CFrameworkHook::MainWndGetMessageCallBack(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == 0)
	{
		LPMSG msg = (LPMSG)lParam;
		if(msg!=NULL && msg->hwnd == AfxGetMainWnd()->m_hWnd)
		{
			HookProcess(msg->message,msg->wParam,msg->lParam);
		}
	}
	return CallNextHookEx(m_MainWndGetMessageHook,code,wParam,lParam);
}

LRESULT CFrameworkHook::MainWndWndProcCallBack(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == 0)
	{
		LPCWPSTRUCT msg = (LPCWPSTRUCT )lParam;
		if(msg!=NULL && msg->hwnd == AfxGetMainWnd()->m_hWnd)
		{
			HookProcess(msg->message,msg->wParam,msg->lParam);
		}
	}
	return CallNextHookEx(m_MainWndWndProcHook,code,wParam,lParam);
}