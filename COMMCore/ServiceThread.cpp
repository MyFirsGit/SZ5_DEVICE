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
@brief	Service��ʹ�õ��߳���                                                                   
*/

IMPLEMENT_DYNCREATE(CServiceThread, CUtilThread)

BEGIN_MESSAGE_MAP(CServiceThread, CUtilThread)
	ON_THREAD_MESSAGE(SP_CONVERT,OnPostServiceMsg)
	ON_THREAD_MESSAGE(SP_SENDMESSAGE,OnSendServiceMsg)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceThread::CServiceThread(void)
    :m_pService(NULL)
    ,m_pendMsgCount(0)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceThread::~CServiceThread(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ʼķ�����Ϣ

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CServiceThread::PostThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam)
{
	// �ʼĵ�����
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
@brief      ������Ϣ

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     ��

@exception  ��
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
@brief      ��SendServiceMsg

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     ��

@exception  ��
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
@brief      ����Thread������Service

@param      (i)WPARAM wParam
@param      (i)LPARAM wParam

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceThread::SetService(CService* pService)
{
	m_pService = pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�æµ

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CServiceThread::IsBusy()
{
    return (m_pendMsgCount > 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡδ������Ϣ��

@param      ��

@retval     int

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CServiceThread::GetPendMsgCount()
{
	return m_pendMsgCount;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CPS������Ӧ����

@param      (i)WPARAM wParam    �������
@param      (i)LPARAM lParam    �������

@retval     ��

@exception  ��
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
@brief      ����Service������Ӧ����

@param      (i)UINT message     The message ID.
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceThread::DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam)
{

        // δ�������Ϣ��+1
    m_pendMsgCount++;

	// ����������־�ر�󣬶���ûʲô�ã����԰���־ȥ���ˡ�20180801 wangfeng
    //theDEBUG_LOG->WriteData(_T("CServiceThread"), _T("DispachCmdMsg"), __LINE__, _T("ServiceID=%d, MessageID=0x%X, wParam=0x%X"), m_pService->GetServiceID(), message, (UINT)wParam);

     //ת������Ӧ��Service���д���
    if (! m_pService->DispachCmdMsg(message, wParam, lParam)) {     // ����Ӧ����
        //CString errMsg;
        //errMsg.Format("����Ӧ���� message=0x%x", message);
        //CInnerException* pException = new CInnerException(m_pService->GetServiceID(), CInnerException::FUNC_ERR_RETURN, _T(__FILE__), __LINE__, errMsg);
        //::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), SP_EXCEPTION, NULL, (LPARAM)pException);
    }
    //theDEBUG_LOG->WriteData(_T("CServiceThread"), _T("DispachCmdMsg"), __LINE__, _T("ServiceID=%d, MessageID=0x%X, wParam=0x%X END"), m_pService->GetServiceID(), message, (UINT)wParam);
    // δ�������Ϣ��-1
    m_pendMsgCount--;
}
