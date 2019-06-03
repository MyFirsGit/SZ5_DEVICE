#include "stdafx.h"
#include "BanknoteCollectionModuleMonitorSvc.h"

#define theMonitorBanknote_LOG CFileLog::GetInstance(FILTER_MONITOR_BANKNOTE_MODULE_LOG)
//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectionModuleMonitorSvc::CBanknoteCollectionModuleMonitorSvc()
:CMonitorSvc(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC,ROOT_SVC,theTVM_INFO.GetBHMonitorInterval())
{
	DO_OPEN_ALARM_FOR_Banknotebox = FALSE;
	m_isAlarmOpend = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectionModuleMonitorSvc::~CBanknoteCollectionModuleMonitorSvc()
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
bool CBanknoteCollectionModuleMonitorSvc::Monitor()
{
	try	{
		
		if(false == BANKNOTE_HELPER->IsCashBoxAttached())
		{
			if(_T("FFFFFFFF") == theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString())
			{
				DO_OPEN_ALARM_FOR_Banknotebox = FALSE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("纸币箱已卸载") );
			}
			else
			{
				DO_OPEN_ALARM_FOR_Banknotebox = TRUE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("纸币箱非法卸载") );
			}	
		}
		else
		{
			if(_T("FFFFFFFF") == theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString())
			{
				DO_OPEN_ALARM_FOR_Banknotebox = TRUE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("纸币箱非法安装") );
			}
			else	
			{
				DO_OPEN_ALARM_FOR_Banknotebox = FALSE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("纸币箱已安装") );
			}
		}
		if(DO_OPEN_ALARM_FOR_Banknotebox)
		{
			if(!m_isAlarmOpend)
			{
				IO_HELPER->OpenAlarm();
				m_isAlarmOpend = true;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("打开报警器") );
			}
			
		}
		else
		{
			/*if(m_isAlarmOpend)
			{
				IO_HELPER->CloseAlarm();
				m_isAlarmOpend = false;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("关闭报警器") );
			}*/
		}
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