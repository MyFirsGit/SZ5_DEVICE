#include "stdafx.h"
#include "UPSMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSMonitorSvc::CUPSMonitorSvc()
:CMonitorSvc(MONITOR_UPS_SVC,ROOT_SVC,theBOM_INFO.GetUPSMonitorInterval())
{
	m_Alerted = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSMonitorSvc::~CUPSMonitorSvc()
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
bool CUPSMonitorSvc::Monitor()
{
	DWORD leftCapacity;
	try
	{
		leftCapacity = m_UPSHelper->GetUPSBatteryCapacity();
		if(leftCapacity <= 10){
			BatteryNeedClose();
		}	
		else if(leftCapacity <= 15){
			if(!m_Alerted){
				BatteryLowAlert();
			}				
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

//////////////////////////////////////////////////////////////////////////
/*
@brief      UPS电量低时调用此函数

@param      无

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////	
void CUPSMonitorSvc::BatteryLowAlert()
{
	CMessageBox::Show(_T(""),_T("UPS电量不足，请连接交流电源"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_60);
	m_Alerted = true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      UPS电量不足需要关机时调用此函数	

@param      无

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUPSMonitorSvc::BatteryNeedClose()
{
	CMessageBox::Show(_T(""),_T("UPS电量不足，BOM将自动关机"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}