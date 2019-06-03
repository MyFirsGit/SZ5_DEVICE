#include "stdafx.h"
#include "LPTMonitorSvc.h"
#include "TicketBoxExchangeSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLPTMonitorSvc::CLPTMonitorSvc()
:CMonitorSvc(MONITOR_LPT_SVC,ROOT_SVC,theBOM_INFO.GetLPTMonitorInterval())
{
	m_DoorStatus = DOORSTATUS::CLOSED;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLPTMonitorSvc::~CLPTMonitorSvc()
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