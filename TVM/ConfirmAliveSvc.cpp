#include "stdafx.h"
#include "ConfirmAliveSvc.h"


BEGIN_MESSAGE_MAP(CConfirmAliveSvc,CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȷ�ϴ������캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CConfirmAliveSvc::CConfirmAliveSvc(void)
:CBackService(CONFIRM_ALIVE_SVC,ROOT_SVC)
{
	m_hConfirmAlive = INVALID_HANDLE_VALUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CConfirmAliveSvc::~CConfirmAliveSvc(void)
{
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ʱ����Ӧ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CConfirmAliveSvc::OnStart()
{
	__super::OnStart();
	m_hConfirmAlive = CreateEvent(NULL,FALSE, TRUE, _T("CONFIRM_ALIVE")); 
	StartTimer(CONFIRM_ALIVE_TIMER,1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֹͣʱ��Ӧ����

@param      ��

@retval     bool ����ֹͣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CConfirmAliveSvc::OnStop()
{
	StopTimer(CONFIRM_ALIVE_TIMER);
	if(m_hConfirmAlive!=NULL && m_hConfirmAlive!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hConfirmAlive);
		m_hConfirmAlive = NULL;
	}
	return __super::OnStop();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ����Ӧ����
@param      UINT nTimerID  identifier of a timer
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CConfirmAliveSvc::OnTimer(UINT nTimerID)
{
	if(CONFIRM_ALIVE_TIMER == nTimerID)
	{
		if(m_hConfirmAlive!=NULL && m_hConfirmAlive!=INVALID_HANDLE_VALUE)
		{
			SetEvent(m_hConfirmAlive);
		}
	}
}
