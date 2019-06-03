#include "stdafx.h"
#include "BanknoteCollectionModuleMonitorSvc.h"

#define theMonitorBanknote_LOG CFileLog::GetInstance(FILTER_MONITOR_BANKNOTE_MODULE_LOG)
//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectionModuleMonitorSvc::~CBanknoteCollectionModuleMonitorSvc()
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
bool CBanknoteCollectionModuleMonitorSvc::Monitor()
{
	try	{
		
		if(false == BANKNOTE_HELPER->IsCashBoxAttached())
		{
			if(_T("FFFFFFFF") == theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString())
			{
				DO_OPEN_ALARM_FOR_Banknotebox = FALSE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ֽ������ж��") );
			}
			else
			{
				DO_OPEN_ALARM_FOR_Banknotebox = TRUE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ֽ����Ƿ�ж��") );
			}	
		}
		else
		{
			if(_T("FFFFFFFF") == theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString())
			{
				DO_OPEN_ALARM_FOR_Banknotebox = TRUE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ֽ����Ƿ���װ") );
			}
			else	
			{
				DO_OPEN_ALARM_FOR_Banknotebox = FALSE;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ֽ�����Ѱ�װ") );
			}
		}
		if(DO_OPEN_ALARM_FOR_Banknotebox)
		{
			if(!m_isAlarmOpend)
			{
				IO_HELPER->OpenAlarm();
				m_isAlarmOpend = true;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�򿪱�����") );
			}
			
		}
		else
		{
			/*if(m_isAlarmOpend)
			{
				IO_HELPER->CloseAlarm();
				m_isAlarmOpend = false;
				theMonitorBanknote_LOG->WriteData(_T("CBanknoteCollectionModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�رձ�����") );
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