#include "stdafx.h"
#include "CardModuleMonitorSvc.h"


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardModuleMonitorSvc::CCardModuleMonitorSvc()
:CMonitorSvc(MONITOR_CARD_ISSUE_MODULE_SVC,ROOT_SVC,theTVM_INFO.GetTHMonitorInterval())
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
CCardModuleMonitorSvc::~CCardModuleMonitorSvc()
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
bool CCardModuleMonitorSvc::Monitor()
{
	try	{
		//CTicketBoxExchangeSvc pTicketBoxSvc;
		//pTicketBoxSvc.RefreshTicketboxStatus();
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