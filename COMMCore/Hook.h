#pragma once;
#include "stdafx.h"
#include "CoreExp.h"
#include <list>
#include <map>
#include "Func.h"


using namespace  std;
class CORE_API CHook
{
	typedef struct _HookItemStruct
	{
		Func<BOOL(MSG)>* condition;
		HANDLE handle;
		BOOL remove; 
		MSG msg;
		_HookItemStruct()
		{
			condition = NULL;
			handle = INVALID_HANDLE_VALUE;
			remove = FALSE;
		}
	}HOOKITEMSTRUCT,*LPHOOKITEMSTRUCT;

	typedef struct _HookStruct
	{
		HHOOK hook;
		list<LPHOOKITEMSTRUCT> callbacks;
		_HookStruct()
		{
			hook = NULL;
		}
	} HOOKSTRUCT,*LPHOOKSTRUCT;

public:
	static LRESULT WaitForThreadMessage(MSG& msg,UINT threadID,Func<BOOL(MSG)>* condition,UINT timeOutMilSecond=INFINITE,BOOL remove=TRUE);
private:
	static map<UINT, LPHOOKSTRUCT> s_ThreadHooks;
	static LRESULT CALLBACK ThreadHookCallBack(int code, WPARAM wParam, LPARAM lParam);
};
