#include "StdAfx.h"
#include "BackService.h"
#include "DebugLog.h"
#include "CoreAPI.h"
#include "timermgr.h"

/**
@brief	��̨�������
*/

BEGIN_MESSAGE_MAP(CBackService, CService)
	ON_SERVICE_MESSAGE(ST_START_TIMER,OnStartTimer)
	ON_SERVICE_MESSAGE(ST_STOP_TIMER,OnStopTimer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      �ϼ�Service ID
@param      bool hasThread                  �Ƿ�����̣߳��ں�̨���У�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBackService::CBackService(UINT serviceID, UINT parentServiceID, bool hasThread)
    :CService(serviceID, parentServiceID, true)
{
    m_pServiceThread = NULL;                            // Service�߳�

    // ���ں�̨���У��򴴽�Service�߳�
    if (hasThread) {
        m_pServiceThread = (CServiceThread*) CUtilThread::StartThread(RUNTIME_CLASS(CServiceThread), "CServiceThread");
        m_pServiceThread->SetService(this);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBackService::~CBackService()
{
    // ���ں�̨���У���ر�Service�߳�
    if (m_pServiceThread != NULL) {
		// �ػ������е�4����פservice׷�Ӵ�log
		CUtilThread::ShutdownThread(m_pServiceThread,3*1000);
        m_pServiceThread = NULL;
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
bool CBackService::OnStop()
{
    MSG msg;

    // �ȴ���̨�߳̿���
    while (m_pServiceThread != NULL && m_pServiceThread->IsBusy()) {
        while (m_pServiceThread->IsBusy() && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::DispatchMessage(&msg);
        }
    }

    return true;
}


CServiceThread* CBackService::GetServiceThread()
{
	return m_pServiceThread;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBackService::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if(m_pServiceThread!=NULL)
	{
		return m_pServiceThread->SendThreadServiceMsg(Msg,wParam,lParam);
	}
	return SP_ERR_INTERNAL_ERROR;
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
BOOL CBackService::PostMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	// ��Service�����߳�, ���ʼĵ��߳�
	if (m_pServiceThread != NULL) {
		return m_pServiceThread->PostThreadServiceMsg(message,wParam,lParam);
	}
	return FALSE;
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
bool CBackService::StartTimer(UINT timerID, int seconds)
{
	PostMessage(ST_START_TIMER,timerID,seconds);
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
void CBackService::StopTimer(UINT timerID)
{
	PostMessage(ST_STOP_TIMER,timerID,NULL);
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
LRESULT CBackService::OnStartTimer(WPARAM wParam,LPARAM lParam)
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
LRESULT CBackService::OnStopTimer(WPARAM wParam,LPARAM lParam)
{
	__super::StopTimer(wParam);
	return TRUE;
}