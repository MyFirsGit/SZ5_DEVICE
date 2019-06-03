#include "StdAfx.h"
#include "requesthandler.h"
#include "CoreCommon.h"
#include "CoreAPI.h"
#include "SysException.h"
#include "InnerException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRequestHandler, CUtilThread)

BEGIN_MESSAGE_MAP(CRequestHandler, CUtilThread)
    ON_THREAD_MESSAGE(REQUEST_QUEUE, OnRequestQueue)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRequestHandler::CRequestHandler(void)
{
	m_pRequestItem = NULL;
	//m_dwNotifyThreadID = 0;
	m_hServiceMutex = NULL;
	m_bTimeOut = FALSE;
    m_isPause = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRequestHandler::~CRequestHandler(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线程初期化响应函数

@param      无

@retval     TRUE if succesful.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRequestHandler::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CString sMutexCom = _T("CTCOMFRM");
	m_hServiceMutex = ::CreateMutex(NULL, FALSE, sMutexCom);

    if (m_hServiceMutex == NULL) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线程中止响应函数

@param      无

@retval     TRUE if succesful.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CRequestHandler::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if (m_hServiceMutex != NULL) {
		CloseHandle(m_hServiceMutex);
		m_hServiceMutex = NULL;
	}

    while (!m_requestList.IsEmpty()) {
        CCommand* pRequestItem = (CCommand*)(m_requestList.RemoveHead());

        if (pRequestItem != NULL) {
            delete pRequestItem;
            pRequestItem = NULL;
        }
    }

	return CUtilThread::ExitInstance();
}


void CRequestHandler::OnRequestItemCompleted(CCommand* requestItem)
{
	requestItem->Completed.RemoveHandler(this,&CRequestHandler::OnRequestItemCompleted);
	m_pRequestItem = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Idle 响应函数

@param      (i)LONG lCount

@retval     TRUE if succesful.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRequestHandler::OnIdle(LONG lCount)
{
	Sleep(1);
	//当前的命令没有COMPLETE
	if(m_pRequestItem!=NULL)
	{
		return TRUE;
	}
    // 若处于暂停状态则直接返回
    if (m_isPause) {
        return TRUE;
    }
    // 若命令队列空则直接返回
    if (m_requestList.IsEmpty()) {
        return TRUE;
    }
    // 若命令队列不空则取出第一条命令，进行处理
    try 
	{
        // Get latest item in queue and send it to request handler
		m_pRequestItem = (CCommand*)m_requestList.RemoveHead();
		m_pRequestItem->Completed.AddHandler(this,&CRequestHandler::OnRequestItemCompleted);
        m_pRequestItem->Execute();		
    }
    catch (CSysException& e) 
	{
    }
    catch (...) 
	{
    }

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      REQUEST_QUEUE 消息响应 (从CRequestManager发出)

@param      (i)wParam : unused.
@param      (i)lParam : Execution mode.

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRequestHandler::OnRequestQueue(WPARAM wParam, LPARAM lParam)
{
    m_requestList.AddTail((CCommand*)lParam);	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置被通知的线程ID

@param      (i)DWORD dwNotifyThreadID         被通知的线程ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CRequestHandler::SetNotifyThreadID(DWORD dwNotifyThreadID)
//{
//	m_dwNotifyThreadID = dwNotifyThreadID;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Wait for synchronize the data transfer

@param      (i)DWORD dwTimeOut: Time out value for this waiting

@retval     DWORD

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CRequestHandler::LockCommunication(DWORD dwTimeOut)
{
    if (m_hServiceMutex != NULL) {
		return ::WaitForSingleObject(m_hServiceMutex, dwTimeOut);
    }
    else {
        return WAIT_OBJECT_0;
    }
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      brief unlock for synchronize the data transfer

@param      无

@retval     TRUE: success to release lock coomunication for other thread
            FALSE: fail to release lock coomunication.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRequestHandler::UnLockCommunication()
{
	if (m_hServiceMutex != NULL) {
		return ::ReleaseMutex (m_hServiceMutex);
	}
	else {
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置是否暂停

@param      (i)bool isPause     true:暂停, false:继续

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRequestHandler::SetPause(bool isPause)
{
    m_isPause = isPause;
}