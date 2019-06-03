#include "stdafx.h"
#include "MonitorSvc.h"


/************************************************************************/
/* ��ط������                                             */
/************************************************************************/

#define theMonitorControl_LOG CFileLog::GetInstance(FILTER_MONITOR_CONTROL_LOG)

#define SM_START_MONITOR_TIMER (SP_USER + 0x200)

BEGIN_MESSAGE_MAP(CMonitorSvc,CBackService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SM_START_MONITOR_TIMER,StartMonitorTimer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param     UINT  serviceID ������
@param     UINT  parentServiceID ��������
@param     int  interval ��ؼ��ʱ��
@param	bool puase ����ʱ�Ƿ���ͣ���

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMonitorSvc::~CMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::OnStart()
{
	__super::OnStart();
	//������߳��п�����ʱ��
	PostMessage(SM_START_MONITOR_TIMER,NULL,NULL);
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֹͣ�ص�����

@param      ��

@retval     bool �Ƿ�����ֹͣ

@exception  ��
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
@brief      ������ؼ�ʱ��

@param      WPARAM wParam δ��
@param      LPARAM lParam δ��

@retval    LRESULT δ�� 

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
@brief      ��ʱ��Ӧ����

@param   UINT iEventID ��ʱ��ID   

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::OnTimer(UINT iEventID)
{
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
	if(m_PauseFlag)	{
		return;
	}
	bool success = Monitor();
	//m_PauseFlag = !success;//��ط��ִ�����ͣ���
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ͣ��ض���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMonitorSvc::Pause()
{
	m_PauseFlag = true;
	theMonitorControl_LOG->WriteData(_T("CMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceID = :%d") ,this->GetServiceID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ָ����

@param      ��

@retval     ��

@exception  ��
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
@brief      ȡ�ü���Ƿ���ͣ

@param      ��

@retval     bool �Ƿ���ͣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CMonitorSvc::IsPause()
{
	return m_PauseFlag;
}