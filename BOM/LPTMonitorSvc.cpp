#include "stdafx.h"
#include "LPTMonitorSvc.h"
#include "TicketBoxExchangeSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLPTMonitorSvc::CLPTMonitorSvc()
:CMonitorSvc(MONITOR_LPT_SVC,ROOT_SVC,theBOM_INFO.GetLPTMonitorInterval())
{
	m_DoorStatus = DOORSTATUS::CLOSED;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLPTMonitorSvc::~CLPTMonitorSvc()
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
bool CLPTMonitorSvc::Monitor()
{
	try{
		CLPTHelper lptHelper(*this);
		DOORSTATUS currentDoorStatus =lptHelper.GetTicketboxDoorStatus();
		if(currentDoorStatus!=m_DoorStatus){
			CTicketBoxExchangeSvc pTocketBoxSvc;
			pTocketBoxSvc.CheckTicketBoxIsInvalidExchanged();
			m_DoorStatus = currentDoorStatus;
		}
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