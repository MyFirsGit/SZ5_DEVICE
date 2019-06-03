#include "StdAfx.h"
#include "ServiceThread.h"
#include "DebugLog.h"
#include "coreapi.h"
#include "service.h"
#include "SysException.h"
#include "InnerException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	Service中使用的线程类                                                                   
*/

IMPLEMENT_DYNCREATE(CServiceThread, CUtilThread)

BEGIN_MESSAGE_MAP(CServiceThread, CUtilThread)
	ON_THREAD_MESSAGE(SP_CONVERT,OnPostServiceMsg)
	ON_THREAD_MESSAGE(SP_SENDMESSAGE,OnSendServiceMsg)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CServiceThread::CServiceThread(void)
    :m_pService(NULL)
    ,m_pendMsgCount(0)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CServiceThread::~CServiceThread(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      邮寄服务消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CServiceThread::PostThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam)
{
	// 邮寄到窗口
	ServiceMsg* innerMsg = new ServiceMsg;
	innerMsg->pService = m_pService;
	innerMsg->message = msg;
	innerMsg->wParam = wParam;
	innerMsg->lParam = lParam;
	BOOL success = this->PostThreadMessage(SP_CONVERT, 0, (LPARAM)innerMsg);
	if(!success)
	{
		delete innerMsg;
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送消息

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServiceThread::SendThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(m_pService == NULL)
	{
		return SP_ERR_INTERNAL_ERROR;
	}
	if(GetCurrentThreadId()==m_nThreadID)
	{
		return m_pService->DispachCmdMsg(msg,wParam,lParam);
	}
	LPTHREADSENDMESSAGESTRUCT sendMessageStruct = new THREADSENDMESSAGESTRUCT;
	sendMessageStruct->message = msg;
	sendMessageStruct->wParam = wParam;
	sendMessageStruct->lParam = lParam;
	sendMessageStruct->handle = CreateEvent(NULL,FALSE,FALSE,NULL);
	BOOL success = this->PostThreadMessage(SP_SENDMESSAGE,NULL,(LPARAM)sendMessageStruct);
	if(!success)
	{
		CloseHandle(sendMessageStruct->handle);
		delete sendMessageStruct;
		sendMessageStruct = NULL;
		return SP_ERR_INTERNAL_ERROR;
	}
	DWORD waitResult = WaitForSingleObject(sendMessageStruct->handle,10*60*1000);
	LRESULT lResult = SP_ERR_INTERNAL_ERROR;
	if(waitResult == WAIT_OBJECT_0)
	{
		lResult = sendMessageStruct->lResult;
	}
	CloseHandle(sendMessageStruct->handle);
	delete sendMessageStruct;
	sendMessageStruct = NULL;
	return lResult;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      当SendServiceMsg

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CServiceThread::OnSendServiceMsg(WPARAM wParam,LPARAM lParam)
{
	LPTHREADSENDMESSAGESTRUCT sendMessageStruct = (LPTHREADSENDMESSAGESTRUCT)lParam;
	if(m_pService == NULL)
	{
		SetEvent(sendMessageStruct->handle);
		return;
	}
	sendMessageStruct->lResult = m_pService->DispachCmdMsg(sendMessageStruct->message,sendMessageStruct->wParam,sendMessageStruct->lParam);
	SetEvent(sendMessageStruct->handle);
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Thread所属的Service

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CServiceThread::SetService(CService* pService)
{
	m_pService = pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否忙碌

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CServiceThread::IsBusy()
{
    return (m_pendMsgCount > 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取未处理信息数

@param      无

@retval     int

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CServiceThread::GetPendMsgCount()
{
	return m_pendMsgCount;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CPS命令响应函数

@param      (i)WPARAM wParam    命令代码
@param      (i)LPARAM lParam    命令对象

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////
void CServiceThread::OnPostServiceMsg(WPARAM wParam, LPARAM lParam)
{
	ServiceMsg* lpMsg = (ServiceMsg*)lParam;
	ServiceMsg msg = *lpMsg;
	delete lpMsg;
	lpMsg = NULL;
	DispachCmdMsg(msg.message, msg.wParam, msg.lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      调用Service命令响应函数

@param      (i)UINT message     The message ID.
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CServiceThread::DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam)
{

        // 未处理的信息数+1
    m_pendMsgCount++;

	// 这里会造成日志特别大，而且没什么用，所以把日志去掉了。20180801 wangfeng
    //theDEBUG_LOG->WriteData(_T("CServiceThread"), _T("DispachCmdMsg"), __LINE__, _T("ServiceID=%d, MessageID=0x%X, wParam=0x%X"), m_pService->GetServiceID(), message, (UINT)wParam);

     //转发到相应的Service进行处理
    if (! m_pService->DispachCmdMsg(message, wParam, lParam)) {     // 无响应函数
        //CString errMsg;
        //errMsg.Format("无响应函数 message=0x%x", message);
        //CInnerException* pException = new CInnerException(m_pService->GetServiceID(), CInnerException::FUNC_ERR_RETURN, _T(__FILE__), __LINE__, errMsg);
        //::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), SP_EXCEPTION, NULL, (LPARAM)pException);
    }
    //theDEBUG_LOG->WriteData(_T("CServiceThread"), _T("DispachCmdMsg"), __LINE__, _T("ServiceID=%d, MessageID=0x%X, wParam=0x%X END"), m_pService->GetServiceID(), message, (UINT)wParam);
    // 未处理的信息数-1
    m_pendMsgCount--;
}
