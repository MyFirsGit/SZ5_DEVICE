#include "stdafx.h"
#include "THMonitorSvc.h"
#include "TicketBoxExchangeSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHMonitorSvc::CTHMonitorSvc()
:CMonitorSvc(MONITOR_TH_SVC,ROOT_SVC,theBOM_INFO.GetTHMonitorInterval())
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
CTHMonitorSvc::~CTHMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控动作

@param      无

@retval     bool 是否有异常发生

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTHMonitorSvc::Monitor()
{
	try	{
		CTicketBoxExchangeSvc pTicketBoxSvc;
		pTicketBoxSvc.RefreshTicketboxStatus();
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		return false;
	}
	return true;
}