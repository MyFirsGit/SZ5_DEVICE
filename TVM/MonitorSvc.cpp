#include "stdafx.h"
#include "MonitorSvc.h"


/************************************************************************/
/* 监控服务基类                                             */
/************************************************************************/

#define theMonitorControl_LOG CFileLog::GetInstance(FILTER_MONITOR_CONTROL_LOG)

#define SM_START_MONITOR_TIMER (SP_USER + 0x200)

BEGIN_MESSAGE_MAP(CMonitorSvc,CBackService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SM_START_MONITOR_TIMER,StartMonitorTimer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param     UINT  serviceID 服务编号
@param     UINT  parentServiceID 父服务编号
@param     int  interval 监控间隔时间
@param	bool puase 启动时是否暂停监控

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMonitorSvc::CMonitorSvc(UINT serviceID, UINT parentServiceID, int interval,bool puase)
:CBackService(serviceID,parentServiceID,true)
,m_Interval(interval)
,m_PauseFlag(puase)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMonitorSvc::~CMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::OnStart()
{
	__super::OnStart();
	//服务的线程中开启计时器
	PostMessage(SM_START_MONITOR_TIMER,NULL,NULL);
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务停止回调函数

@param      无

@retval     bool 是否允许停止

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMonitorSvc::OnStop()
{
	StopTimer(MONITOR_TIMER);
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
	return __super::OnStop();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      开启监控计时器

@param      WPARAM wParam 未用
@param      LPARAM lParam 未用

@retval    LRESULT 未用 

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMonitorSvc::StartMonitorTimer(WPARAM wParam,LPARAM lParam)
{
	StartTimer(MONITOR_TIMER,m_Interval);
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      计时响应函数

@param   UINT iEventID 计时器ID   

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::OnTimer(UINT iEventID)
{
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
	if(m_PauseFlag)	{
		return;
	}
	bool success = Monitor();
	//m_PauseFlag = !success;//监控发现错误，暂停监控
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      暂停监控动作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::Pause()
{
	m_PauseFlag = true;
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      恢复监控

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::Resume()
{
	m_PauseFlag = false;
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
	OnResume();
}

void CMonitorSvc::OnResume()
{
	ASSERT(TRUE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得监控是否暂停

@param      无

@retval     bool 是否暂停

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMonitorSvc::IsPause()
{
	return m_PauseFlag;
}