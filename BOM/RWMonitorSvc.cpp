#include "stdafx.h"
#include "RWMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWMonitorSvc::CRWMonitorSvc()
:CMonitorSvc(MONITOR_RW_SVC,ROOT_SVC,theBOM_INFO.GetRWMonitorInterval(true))
{
	m_RWHelper = new CRWHelper(*this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWMonitorSvc::~CRWMonitorSvc()
{
	delete m_RWHelper;
	m_RWHelper = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控函数

@param      无

@retval     bool 是否有异常发生

@exception  无
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