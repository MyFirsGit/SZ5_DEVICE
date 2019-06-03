#include "stdafx.h"
#include "TWMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTWMonitorSvc::CTWMonitorSvc()
:CMonitorSvc(MONITOR_TW_SVC,ROOT_SVC,theBOM_INFO.GetRWMonitorInterval(false))
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
CTWMonitorSvc::~CTWMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控动作函数

@param      无

@retval     bool 是否有异常发生

@exception  无
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