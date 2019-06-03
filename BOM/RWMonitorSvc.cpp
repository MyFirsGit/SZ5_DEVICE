#include "stdafx.h"
#include "RWMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWMonitorSvc::CRWMonitorSvc()
:CMonitorSvc(MONITOR_RW_SVC,ROOT_SVC,theBOM_INFO.GetRWMonitorInterval(true))
{
	m_RWHelper = new CRWHelper(*this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWMonitorSvc::~CRWMonitorSvc()
{
	delete m_RWHelper;
	m_RWHelper = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��غ���

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CRWMonitorSvc::Monitor()
{
	RW_RSP_STATUS status;
	try
	{
		m_RWHelper->GetStatus(status);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		return false;
	}
	return true;
}