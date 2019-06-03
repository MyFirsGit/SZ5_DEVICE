#include "stdafx.h"
#include "RechargeRWMonitorSvc.h"

#define theMonitorRechargeRW_LOG CFileLog::GetInstance(FILTER_MONITOR_RECHARGE_RW_LOG)

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRechargeRWMonitorSvc::CRechargeRWMonitorSvc()
:CMonitorSvc(MONITOR_RECHARGE_RW_SVC,ROOT_SVC,theTVM_INFO.GetRWMonitorInterval(TVM_RECHARGE_RW))
{
	m_bIsStartedByMonitor = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRechargeRWMonitorSvc::~CRechargeRWMonitorSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��غ���

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CRechargeRWMonitorSvc::Monitor()
{
//	theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T(" <<") );
//	long lRWRet = RW_RSP_OK;
//	bool isStartAnalyze = false;
//	OPERATION_TYPE operationtype = DO_NOTHING;
//	try{
//		// ���TVMΪ�ȴ��л������״̬
//		if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST){
//			//Do fallow
//		}
//		else {
//			// �ж��Ƿ��Ƿ�������
//			if (theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//				// �ж��Ƿ��Զ�����
//				if (!m_bIsStartedByMonitor){
//					return true;
//				}
//				// ִ�д˼��
//				else{
//					// Do fallow
//				}
//			}
//			else{
//				return true;
//			}
//		}
//		/*if(!(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST) && theSERVICE_MGR.GetCurService()->GetServiceID() != ANALYZE_SVC ){
//			theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("Service״̬Ϊ��%d"),theSERVICE_MGR.GetState());
//			return true;
//		}*/
//		// �����д��û������
//		if(theTVM_INFO.IsRWUsed(TVM_RECHARGE_RW)){
//			// ���TVM��ֵ��д��û���쳣
//			if(!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) && (!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))){
//				// TVMҵ��״̬֧�ֲ�ѯҵ��
//				if((theTVM_STATUS_MGR.GetWorkMode()&DEVICE_CONTRL_CODE_NO_COIN_QUERY) == DEVICE_CONTRL_CODE_NO_COIN_QUERY){
//					// ����
//					RW_READ_CARD_RESPONSE rspReadCard;
//					lRWRet = RECHARGERW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),rspReadCard,false);
//					// ����ʧ��
//					if(lRWRet != RW_RSP_OK){						
//						theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��������"));
//						// �޿�
//						if(rspReadCard.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�޿�����"));
//							// ��ǰ�ǲ�ѯҵ��
//							if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//								theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��ǰΪ��ѯҵ��"));
//								operationtype = DO_START_DEFAULT_SERVICE;
//							}
//							// ��ǰ���ǲ�ѯҵ��
//							else{
//								theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��ǰ���ǲ�ѯҵ��"));
//								// ������ѯҵ��
//								operationtype = DO_NOTHING;
//							}
//						}
//						// �п�
//						else{
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�п�����"));
//								operationtype = DO_START_ANALYZE_SERVICE;
//						}
//					}
//					// �����ɹ�
//					else{
//						theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("�����ɹ�"));
//						// ��ǰ�ǲ�ѯҵ��
//						if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��ǰΪ��ѯҵ��"));
//							operationtype = DO_NOTHING;
//						}
//						// ��ǰ���ǲ�ѯҵ��
//						else{
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��ǰ���ǲ�ѯҵ��"));
//							// ������ѯҵ��
//							operationtype = DO_START_ANALYZE_SERVICE;
//						}
//					}
//					// ������ѯҵ��
//					if(operationtype == DO_START_ANALYZE_SERVICE){
//						// ��ͣ��ֵ��д�����
//						__super::Pause();
//						// ���TVMΪ�ȴ��л������״̬
//						if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST){					
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("ServiceΪ�ȴ��л������"));
//							// ������ѯҵ��
//							theSERVICE_MGR.StartService(ANALYZE_SVC);
//							CMessagePictureBox::End(); 
//							m_bIsStartedByMonitor = true;
//						}
//					}
//					// ����Ĭ��ҵ��
//					else if(operationtype == DO_START_DEFAULT_SERVICE){
//						// ��ǰ�ǲ�ѯҵ��
//						if(theSERVICE_MGR.GetCurService()->GetServiceID() == ANALYZE_SVC && m_bIsStartedByMonitor){
//							theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("����Ĭ��Service"));
//							// ������Ϣ��MainService������Ĭ��ҵ��
//							CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
//							pMainSvc->StartDefaultService();
//							m_bIsStartedByMonitor = false;
//// 							// �ȴ�2��
//// 							Sleep(2000);
//						}
//					}
//				}
//				else{
//					theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��ѯҵ�񽵼�"));
//				}
//			}
//			else{
//				theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��д���쳣"));
//			}
//		}
//		else{
//			theMonitorRechargeRW_LOG->WriteData(_T("CRechargeRWMonitorSvc"),CString(__FUNCTION__), __LINE__,_T("��д������") );
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
@brief      ��������״̬��ΪĬ��״̬

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRechargeRWMonitorSvc::ResetStartedByMonitorStatus(){
	// ��Ϊ��ʼ״̬
	m_bIsStartedByMonitor = false;
}