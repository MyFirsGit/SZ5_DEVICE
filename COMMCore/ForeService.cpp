#include "StdAfx.h"
#include "ForeService.h"
#include "DebugLog.h"
#include "InnerException.h"
#include "Command.h"
#include "coreapi.h"

/**
@brief	前台服务基类
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

BEGIN_MESSAGE_MAP(CForeService, CService)
	ON_SERVICE_MESSAGE(ST_START_TIMER,OnStartTimer)
	ON_SERVICE_MESSAGE(ST_STOP_TIMER,OnStopTimer)
END_MESSAGE_MAP()

bool CForeService::m_isWaitingCommand = false;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      上级Service ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CForeService::CForeService(UINT serviceID, UINT parentServiceID,CDialogFlow* dialogFlow)
:CService(serviceID, parentServiceID, false)
{
	// 初始化成员变量
	m_pDialogFlow = dialogFlow;             // 对话框流转控制
    
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CForeService::~CForeService()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::OnStart()
{
	try {
		if(m_pDialogFlow == NULL)
		{
			return;
		}
		// 显示初始画面组
		m_pDialogFlow->ShowFirstDialogGroup();

		// 隐藏父Service的Dialog
		if (GetServiceID() != 0 && GetParentServiceID() != 0) {
			CDialogFlow* dialogFlow = ((CForeService*)GetParent())->GetDialogFlow();
			if(dialogFlow!=NULL)
			{
				dialogFlow->HideDialogGroup(dialogFlow->GetCurDialogGroupID());
			}
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service停止前处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CForeService::OnStop()
{
	if(m_pDialogFlow == NULL)
	{
		return true;
	}
	try {
		// 显示父Service的当前Dialog组
		if (GetServiceID() != 0 && GetParentServiceID() != 0) {
			CDialogFlow* dialogFlow = ((CForeService*)GetParent())->GetDialogFlow();
			if(dialogFlow!=NULL)
			{
				dialogFlow->ShowDialogGroup(dialogFlow->GetCurDialogGroupID());
			}
		}

		return true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      邮寄消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CForeService::PostMessage(UINT message,WPARAM wParam, LPARAM lParam)
{
	// 邮寄到窗口
	ServiceMsg* innerMsg = new ServiceMsg;
	innerMsg->pService = this;
	innerMsg->message = message;
	innerMsg->wParam = wParam;
	innerMsg->lParam = lParam;
	CWnd*  mainWnd = AfxGetApp()->m_pMainWnd;
	BOOL success = FALSE;
	if(mainWnd!=NULL && IsWindow(mainWnd->m_hWnd))
	{
		success = mainWnd->PostMessage(SP_CONVERT, 0, (LPARAM)innerMsg);
	}
	if(!success)
	{
		delete innerMsg;
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     发送消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CForeService::SendMessage(UINT message,WPARAM wParam, LPARAM lParam)
{
	ServiceMsg* innerMsg = new ServiceMsg;
	innerMsg->pService = this;
	innerMsg->message = message;
	innerMsg->wParam = wParam;
	innerMsg->lParam = lParam;
	CWnd*  mainWnd = AfxGetApp()->m_pMainWnd;
	if(mainWnd!=NULL && IsWindow(mainWnd->m_hWnd))
	{
		return mainWnd->SendMessage(SP_CONVERT, 0, (LPARAM)innerMsg);
	}
	delete innerMsg;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否在等待命令结束中

@param      无

@retval     bool    true:等待命令结束中;  false:不在等待命令结束中

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CForeService::IsWaitingCommand()
{
	return m_isWaitingCommand;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置等待命令结束中

@param      bool    true:等待命令结束中;  false:不在等待命令结束中

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::SetWaitingCommand(bool isWaiting)
{
	m_isWaitingCommand = isWaiting;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取画面流转控制器

@param      无

@retval     CDialogFlow* 画面流转控制器

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow* CForeService::GetDialogFlow()
{
	return m_pDialogFlow;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理画面跳转按钮消息

@param      (i)const char* pButtonMsg 按钮按下发送的附加消息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::DoDialogFlow(const TCHAR* pButtonMsg)
{
	if(m_pDialogFlow==NULL)
	{
		return;
	}
	if (m_pDialogFlow->IsFlowButton(pButtonMsg)) {
		m_pDialogFlow->ShowNextDialogGroup(pButtonMsg);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动定时器

@param      (i)UINT timerID 
@param      (i)int seconds  等待时间(秒)

@retval     bool true:成功 false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CForeService::StartTimer(UINT timerID, int seconds)
{
	SendMessage(ST_START_TIMER,timerID,seconds);
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      停止定时器

@param      (i)UINT timerID 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::StopTimer(UINT timerID)
{
	SendMessage(ST_STOP_TIMER,timerID,NULL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开启计时器消息响应函数（计时器在当前服务的Thread中开启）

@param     WPARAM wParam TimerID
@param	LPARAM lParam Interval

@retval     LRESULT

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CForeService::OnStartTimer(WPARAM wParam,LPARAM lParam)
{
	__super::StartTimer(wParam, lParam);
	return TRUE;
}


/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CForeService::OnStopTimer(WPARAM wParam,LPARAM lParam)
{
	__super::StopTimer(wParam);
	return TRUE;
}