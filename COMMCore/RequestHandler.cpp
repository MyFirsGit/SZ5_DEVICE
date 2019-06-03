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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRequestHandler::~CRequestHandler(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �̳߳��ڻ���Ӧ����

@param      ��

@retval     TRUE if succesful.

@exception  ��
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
@brief      �߳���ֹ��Ӧ����

@param      ��

@retval     TRUE if succesful.

@exception  ��
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
@brief      Idle ��Ӧ����

@param      (i)LONG lCount

@retval     TRUE if succesful.

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRequestHandler::OnIdle(LONG lCount)
{
	Sleep(1);
	//��ǰ������û��COMPLETE
	if(m_pRequestItem!=NULL)
	{
		return TRUE;
	}
    // ��������ͣ״̬��ֱ�ӷ���
    if (m_isPause) {
        return TRUE;
    }
    // ��������п���ֱ�ӷ���
    if (m_requestList.IsEmpty()) {
        return TRUE;
    }
    // ��������в�����ȡ����һ��������д���
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
@brief      REQUEST_QUEUE ��Ϣ��Ӧ (��CRequestManager����)

@param      (i)wParam : unused.
@param      (i)lParam : Execution mode.

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRequestHandler::OnRequestQueue(WPARAM wParam, LPARAM lParam)
{
    m_requestList.AddTail((CCommand*)lParam);	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ�֪ͨ���߳�ID

@param      (i)DWORD dwNotifyThreadID         ��֪ͨ���߳�ID

@retval     ��

@exception  ��
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

@exception  ��
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

@param      ��

@retval     TRUE: success to release lock coomunication for other thread
            FALSE: fail to release lock coomunication.

@exception  ��
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
@brief      �����Ƿ���ͣ

@param      (i)bool isPause     true:��ͣ, false:����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRequestHandler::SetPause(bool isPause)
{
    m_isPause = isPause;
}