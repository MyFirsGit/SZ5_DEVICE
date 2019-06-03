#include "stdafx.h"
#include "UPSMonitorSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSMonitorSvc::CUPSMonitorSvc()
:CMonitorSvc(MONITOR_UPS_SVC,ROOT_SVC,theBOM_INFO.GetUPSMonitorInterval())
{
	m_Alerted = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSMonitorSvc::~CUPSMonitorSvc()
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
@brief      UPS������ʱ���ô˺���

@param      ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////	
void CUPSMonitorSvc::BatteryLowAlert()
{
	CMessageBox::Show(_T(""),_T("UPS�������㣬�����ӽ�����Դ"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_60);
	m_Alerted = true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      UPS����������Ҫ�ػ�ʱ���ô˺���	

@param      ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUPSMonitorSvc::BatteryNeedClose()
{
	CMessageBox::Show(_T(""),_T("UPS�������㣬BOM���Զ��ػ�"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}