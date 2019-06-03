#include "stdafx.h"
#include "TWMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTWMonitorSvc::CTWMonitorSvc()
:CMonitorSvc(MONITOR_TW_SVC,ROOT_SVC,theBOM_INFO.GetRWMonitorInterval(false))
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
CTWMonitorSvc::~CTWMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ض�������

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTWMonitorSvc::Monitor()
{
	RW_RSP_STATUS status;
	try
	{
		m_TWHelper->GetStatus(status);
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