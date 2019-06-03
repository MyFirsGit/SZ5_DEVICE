#include "stdafx.h"
#include "CoinModuleMonitorSvc.h"

#define theMonitorCoin_LOG CFileLog::GetInstance(FILTER_MONITOR_COIN_MODULE_LOG)

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleMonitorSvc::CCoinModuleMonitorSvc()
:CMonitorSvc(MONITOR_COIN_MODULE_SVC,ROOT_SVC,theTVM_INFO.GetCHMonitorInterval())
{
	m_isAlarmOpend = false;
	DO_OPEN_ALARM_FOR_A = FALSE;
	DO_OPEN_ALARM_FOR_B = FALSE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleMonitorSvc::~CCoinModuleMonitorSvc()
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
bool CCoinModuleMonitorSvc::Monitor()
{
	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(" <<") );
	try{	
	
		// ��ȡӲ�һ�����ID
		CString strCoinCollectionBoxAID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();		// Ӳ�һ�����A ID
		CString strCoinCollectionBoxBID = theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();		// Ӳ�һ�����B ID

		// ������A��B����ж��
		if(_T("FFFFFFFF") == strCoinCollectionBoxAID && _T("FFFFFFFF") == strCoinCollectionBoxBID)
		{
			theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������A��B����ж��") );
			return false;
		}
		// �����Ѱ�װ�Ļ�����
		else
		{
			//long errCode = COIN_HELPER->ReadStatus(chcommonrsp);							// ��ȡ������Ӳ����װ״̬
			//// ����Aδ��λ
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX1_SET_STATUS)
			//{
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������Aδ��λ") );
			//	// �Ѱ�װ
			//	if(_T("FFFFFFFF") != strCoinCollectionBoxAID)
			//	{					
			//		DO_OPEN_ALARM_FOR_A = TRUE;	
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������A�Ƿ�ж��") );
			//	}		
			//}
			//// ����A�ѵ�λ
			//else
			//{
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������A�ѵ�λ") );
			//	DO_OPEN_ALARM_FOR_A = FALSE;		
			//}
			//// ����Bδ��λ
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX2_SET_STATUS)
			//{	
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������Bδ��λ") );
			//	if(_T("FFFFFFFF") != strCoinCollectionBoxBID)
			//	{					
			//		DO_OPEN_ALARM_FOR_B = TRUE;	
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������B�Ƿ�ж��") );
			//	}			
			//}
			//// ����B�ѵ�λ
			//else
			//{
			//	DO_OPEN_ALARM_FOR_B = FALSE;		
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("������B�ѵ�λ") );
			//}

			//// �򿪱�����
			//if(DO_OPEN_ALARM_FOR_A | DO_OPEN_ALARM_FOR_B)
			//{				
			//	if(!m_isAlarmOpend)
			//	{
			//		IO_HELPER->OpenAlarm();
			//		m_isAlarmOpend = true;
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�򿪱�����") );
			//	}	
			//}
			//// �رձ�����
			//if(!(DO_OPEN_ALARM_FOR_A | DO_OPEN_ALARM_FOR_B))
			//{				
			//	/*if(m_isAlarmOpend)
			//	{
			//		IO_HELPER->CloseAlarm();
			//		m_isAlarmOpend = false;		
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�رձ�����") );
			//	}*/				
			//}
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
	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(" >>") );
	return true;
}