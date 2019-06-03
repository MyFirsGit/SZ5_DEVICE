#include "stdafx.h"
#include "RechargeRWMonitorSvc.h"

#define theMonitorRechargeRW_LOG CFileLog::GetInstance(FILTER_MONITOR_RECHARGE_RW_LOG)

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRechargeRWMonitorSvc::CRechargeRWMonitorSvc()
:CMonitorSvc(MONITOR_RECHARGE_RW_SVC,ROOT_SVC,theTVM_INFO.GetRWMonitorInterval(TVM_RECHARGE_RW))
{
	m_bIsStartedByMonitor = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRechargeRWMonitorSvc::~CRechargeRWMonitorSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控函数

@param      无

@retval     bool 是否有异常发生

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CRechargeRWMonitorSvc::Monitor()
{
//	theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(" <<") );
//	long lRWRet = RW_RSP_OK;
//	bool isStartAnalyze = false;
//	OPERATION_TYPE operationtype = DO_NOTHING;
//	try{
//		// 如果TVM为等待中或待机中状态
//		if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST){
//			//Do fallow
//		}
//		else {
//			// 判断是否是分析服务
//			if (theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//				// 判断是否自动启用
//				if (!m_bIsStartedByMonitor){
//					return true;
//				}
//				// 执行此监控
//				else{
//					// Do fallow
//				}
//			}
//			else{
//				return true;
//			}
//		}
//		/*if(!(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST) && theSERVICE_MGR.GetCurService()->GetServiceID() != ANALYZE_SVC ){
//			theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("Service状态为：%d"),theSERVICE_MGR.GetState());
//			return true;
//		}*/
//		// 如果读写器没有切离
//		if(theTVM_INFO.IsRWUsed(TVM_RECHARGE_RW)){
//			// 如果TVM充值读写器没有异常
//			if(!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) && (!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))){
//				// TVM业务状态支持查询业务
//				if((theTVM_STATUS_MGR.GetWorkMode()&DEVICE_CONTRL_CODE_NO_COIN_QUERY) == DEVICE_CONTRL_CODE_NO_COIN_QUERY){
//					// 读卡
//					RW_READ_CARD_RESPONSE rspReadCard;
//					lRWRet = RECHARGERW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),rspReadCard,false);
//					// 读卡失败
//					if(lRWRet != RW_RSP_OK){						
//						theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("读卡错误"));
//						// 无卡
//						if(rspReadCard.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("无卡错误"));
//							// 当前是查询业务
//							if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//								theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("当前为查询业务"));
//								operationtype = DO_START_DEFAULT_SERVICE;
//							}
//							// 当前不是查询业务
//							else{
//								theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("当前不是查询业务"));
//								// 启动查询业务
//								operationtype = DO_NOTHING;
//							}
//						}
//						// 有卡
//						else{
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("有卡错误"));
//								operationtype = DO_START_ANALYZE_SERVICE;
//						}
//					}
//					// 读卡成功
//					else{
//						theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("读卡成功"));
//						// 当前是查询业务
//						if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("当前为查询业务"));
//							operationtype = DO_NOTHING;
//						}
//						// 当前不是查询业务
//						else{
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("当前不是查询业务"));
//							// 启动查询业务
//							operationtype = DO_START_ANALYZE_SERVICE;
//						}
//					}
//					// 启动查询业务
//					if(operationtype == DO_START_ANALYZE_SERVICE){
//						// 暂停充值读写器监控
//						__super::Pause();
//						// 如果TVM为等待中或待机中状态
//						if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST){					
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("Service为等待中或待机中"));
//							// 启动查询业务
//							theSERVICE_MGR.StartService(ANALYZE_SVC);
//							CMessagePictureBox::End(); 
//							m_bIsStartedByMonitor = true;
//						}
//					}
//					// 启动默认业务
//					else if(operationtype == DO_START_DEFAULT_SERVICE){
//						// 当前是查询业务
//						if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC && m_bIsStartedByMonitor){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("启动默认Service"));
//							// 发生消息给MainService，启动默认业务
//							CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
//							pMainSvc->StartDefaultService();
//							m_bIsStartedByMonitor = false;
//// 							// 等待2秒
//// 							Sleep(2000);
//						}
//					}
//				}
//				else{
//					theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("查询业务降级"));
//				}
//			}
//			else{
//				theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("读写器异常"));
//			}
//		}
//		else{
//			theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("读写器切离") );
//		}
//	}
//	catch (CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	catch(...){
//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
//		return false;
//	}
//	theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(">>") );
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重置启动状态，为默认状态

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRechargeRWMonitorSvc::ResetStartedByMonitorStatus(){
	// 置为初始状态
	m_bIsStartedByMonitor = false;
}