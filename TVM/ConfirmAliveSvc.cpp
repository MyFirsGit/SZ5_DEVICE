#include "stdafx.h"
#include "ConfirmAliveSvc.h"


BEGIN_MESSAGE_MAP(CConfirmAliveSvc,CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      确认存活服务构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CConfirmAliveSvc::CConfirmAliveSvc(void)
:CBackService(CONFIRM_ALIVE_SVC,ROOT_SVC)
{
	m_hConfirmAlive = INVALID_HANDLE_VALUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CConfirmAliveSvc::~CConfirmAliveSvc(void)
{
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动时的响应函数

@param      无

@retval     无

@exception  无
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
@brief      服务停止时响应函数

@param      无

@retval     bool 允许停止

@exception  无
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
@brief      定时器响应处理
@param      UINT nTimerID  identifier of a timer
@retval     无
@exception  无
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
