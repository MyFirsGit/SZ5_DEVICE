#include "stdafx.h"
#include "THMonitorSvc.h"
#include "TicketBoxExchangeSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHMonitorSvc::CTHMonitorSvc()
:CMonitorSvc(MONITOR_TH_SVC,ROOT_SVC,theBOM_INFO.GetTHMonitorInterval())
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
CTHMonitorSvc::~CTHMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ض���

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
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