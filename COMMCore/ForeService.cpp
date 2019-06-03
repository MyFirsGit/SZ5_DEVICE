#include "StdAfx.h"
#include "ForeService.h"
#include "DebugLog.h"
#include "InnerException.h"
#include "Command.h"
#include "coreapi.h"

/**
@brief	ǰ̨�������
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
@brief      ���캯��

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      �ϼ�Service ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CForeService::CForeService(UINT serviceID, UINT parentServiceID,CDialogFlow* dialogFlow)
:CService(serviceID, parentServiceID, false)
{
	// ��ʼ����Ա����
	m_pDialogFlow = dialogFlow;             // �Ի�����ת����
    
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CForeService::~CForeService()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

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
		// ��ʾ��ʼ������
		m_pDialogFlow->ShowFirstDialogGroup();

		// ���ظ�Service��Dialog
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
@brief      Serviceֹͣǰ����

@param      ��

@retval     ��

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
		// ��ʾ��Service�ĵ�ǰDialog��
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
@brief      �ʼ���Ϣ

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CForeService::PostMessage(UINT message,WPARAM wParam, LPARAM lParam)
{
	// �ʼĵ�����
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
@brief     ������Ϣ

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

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
@brief      �Ƿ��ڵȴ����������

@param      ��

@retval     bool    true:�ȴ����������;  false:���ڵȴ����������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CForeService::IsWaitingCommand()
{
	return m_isWaitingCommand;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õȴ����������

@param      bool    true:�ȴ����������;  false:���ڵȴ����������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::SetWaitingCommand(bool isWaiting)
{
	m_isWaitingCommand = isWaiting;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������ת������

@param      ��

@retval     CDialogFlow* ������ת������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDialogFlow* CForeService::GetDialogFlow()
{
	return m_pDialogFlow;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ת��ť��Ϣ

@param      (i)const char* pButtonMsg ��ť���·��͵ĸ�����Ϣ

@retval     ��

@exception  ��
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
@brief      ������ʱ��

@param      (i)UINT timerID 
@param      (i)int seconds  �ȴ�ʱ��(��)

@retval     bool true:�ɹ� false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CForeService::StartTimer(UINT timerID, int seconds)
{
	SendMessage(ST_START_TIMER,timerID,seconds);
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��ʱ��

@param      (i)UINT timerID 

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CForeService::StopTimer(UINT timerID)
{
	SendMessage(ST_STOP_TIMER,timerID,NULL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʱ����Ϣ��Ӧ��������ʱ���ڵ�ǰ�����Thread�п�����

@param     WPARAM wParam TimerID
@param	LPARAM lParam Interval

@retval     LRESULT

@exception  ��
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