#include "stdafx.h"
#include "CoinModuleMonitorSvc.h"

#define theMonitorCoin_LOG CFileLog::GetInstance(FILTER_MONITOR_COIN_MODULE_LOG)

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleMonitorSvc::~CCoinModuleMonitorSvc()
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
bool CCoinModuleMonitorSvc::Monitor()
{
	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(" <<") );
	try{	
	
		// 获取硬币回收箱ID
		CString strCoinCollectionBoxAID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();		// 硬币回收箱A ID
		CString strCoinCollectionBoxBID = theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();		// 硬币回收箱B ID

		// 回收箱A和B均已卸载
		if(_T("FFFFFFFF") == strCoinCollectionBoxAID && _T("FFFFFFFF") == strCoinCollectionBoxBID)
		{
			theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱A和B均已卸载") );
			return false;
		}
		// 存在已安装的回收箱
		else
		{
			//long errCode = COIN_HELPER->ReadStatus(chcommonrsp);							// 读取回收箱硬件安装状态
			//// 箱子A未到位
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX1_SET_STATUS)
			//{
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱A未到位") );
			//	// 已安装
			//	if(_T("FFFFFFFF") != strCoinCollectionBoxAID)
			//	{					
			//		DO_OPEN_ALARM_FOR_A = TRUE;	
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱A非法卸载") );
			//	}		
			//}
			//// 箱子A已到位
			//else
			//{
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱A已到位") );
			//	DO_OPEN_ALARM_FOR_A = FALSE;		
			//}
			//// 箱子B未到位
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX2_SET_STATUS)
			//{	
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱B未到位") );
			//	if(_T("FFFFFFFF") != strCoinCollectionBoxBID)
			//	{					
			//		DO_OPEN_ALARM_FOR_B = TRUE;	
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱B非法卸载") );
			//	}			
			//}
			//// 箱子B已到位
			//else
			//{
			//	DO_OPEN_ALARM_FOR_B = FALSE;		
			//	theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("回收箱B已到位") );
			//}

			//// 打开报警器
			//if(DO_OPEN_ALARM_FOR_A | DO_OPEN_ALARM_FOR_B)
			//{				
			//	if(!m_isAlarmOpend)
			//	{
			//		IO_HELPER->OpenAlarm();
			//		m_isAlarmOpend = true;
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("打开报警器") );
			//	}	
			//}
			//// 关闭报警器
			//if(!(DO_OPEN_ALARM_FOR_A | DO_OPEN_ALARM_FOR_B))
			//{				
			//	/*if(m_isAlarmOpend)
			//	{
			//		IO_HELPER->CloseAlarm();
			//		m_isAlarmOpend = false;		
			//		theMonitorCoin_LOG->WriteData(_T("CCoinModuleMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("关闭报警器") );
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