#include "stdafx.h"
#include "Hook.h"
#include "CoreAPI.h"
#include "Service.h"

std::map<UINT, CHook::LPHOOKSTRUCT> CHook::s_ThreadHooks;

LRESULT CHook::WaitForThreadMessage(MSG& msg,UINT threadID, Func<BOOL(MSG)> *condition, UINT timeOutMilSecond, BOOL remove)
{
	if(s_ThreadHooks.find(threadID)==s_ThreadHooks.end())
	{
		HHOOK hook =SetWindowsHookEx(WH_GETMESSAGE,ThreadHookCallBack,AfxGetApp()->m_hInstance,threadID);
		LPHOOKSTRUCT hookStruct = new HOOKSTRUCT;
		hookStruct->hook = hook;
		s_ThreadHooks[threadID]=hookStruct;
	}
	LPHOOKSTRUCT pHookStruct = s_ThreadHooks[threadID];
	LPHOOKITEMSTRUCT hookItem = new HOOKITEMSTRUCT;
	hookItem->condition = condition;
	hookItem->handle = CreateEvent(NULL,FALSE,FALSE,NULL);
	hookItem->remove = remove;
	pHookStruct->callbacks.push_back(hookItem);
	DWORD waitResult = WaitForSingleObject(hookItem->handle,timeOutMilSecond);
	msg = hookItem->msg;
	pHookStruct->callbacks.remove(hookItem);
	CloseHandle(hookItem->handle);
	delete hookItem;
	hookItem = NULL;
	if(pHookStruct->callbacks.size()==0)
	{
		UnhookWindowsHookEx(pHookStruct->hook);
	}
	s_ThreadHooks.erase(threadID);
	if(waitResult == WAIT_OBJECT_0)
	{
		return SP_SUCCESS;
	}
	if(waitResult == WAIT_TIMEOUT)
	{
		return SP_ERR_TIMEOUT;
	}
	return SP_ERR_INTERNAL_ERROR;
}

LRESULT CHook::ThreadHookCallBack(int code, WPARAM wParam, LPARAM lParam)
{
	LPHOOKSTRUCT hookStruct =  s_ThreadHooks[GetCurrentThreadId()];
	HHOOK hook = hookStruct->hook;
	if(code == 0)
	{
		LPMSG lpMsg = (LPMSG)lParam;
		if(lpMsg!=NULL && lpMsg->message>WM_USER)
		{
			for (list<LPHOOKITEMSTRUCT>::reverse_iterator iterItem = hookStruct->callbacks.rbegin();iterItem!=hookStruct->callbacks.rend();iterItem++ )
			{
				LPHOOKITEMSTRUCT hookItemStruct = *iterItem;
				MSG msg = *lpMsg;
				if(lpMsg->message == SP_CONVERT)
				{
					ServiceMsg* serviceMsg= (ServiceMsg*)lpMsg->lParam;
					msg.message = serviceMsg->message;
					msg.wParam = serviceMsg->wParam;
					msg.lParam = serviceMsg->lParam;
				}
				if(hookItemStruct->condition->Invoke(msg))
				{
					hookItemStruct->msg = msg;
					SetEvent(hookItemStruct->handle);
					if(hookItemStruct->remove)
					{
						return TRUE;
					}
				}
			}
		}
	}
	return CallNextHookEx(hook,code,wParam,lParam);
}