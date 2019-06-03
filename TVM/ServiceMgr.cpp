#include "StdAfx.h"
#include "ServiceMgr.h"
#include "InnerException.h"
#include "exceptionmgr.h"
#include "CursorMgr.h"
#include "messageiddefinition.h"
#include "foreservice.h"
#include "mainsvc.h"
#include "initsvc.h"
#include "servicedefinition.h"
#include "loginsvc.h"
#include "tvmforeservice.h"
#include "CoinModuleTestSvc.h"
#include "networktestsvc.h"
#include "printertestsvc.h"
#include "rwtestsvc.h"
#include "devicemodeswitchsvc.h"
#include "communicationsetsvc.h"
#include "stationsetsvc.h"
#include "timesetsvc.h"
#include "SmartCardTestSvc.h"
#include "TokenModuleTestSvc.h"
#include "DataInputSvc.h"
#include "DataOutputSvc.h"
#include "removeexceptionsvc.h"
#include "ChargeSvc.h"
#include "ProductIssueSvc.h"
#include "analyzesvc.h"
#include "serverclientsvc.h"
#include "rootsvc.h"
#include "IssueSvc.h"
#include "BusinessTranslateSvc.h"
#include "EventDataSvc.h"
#include "TranscationTranslateSvc.h"
#include "CardModuleMonitorSvc.h"
#include "CardRWMonitorSvc.h"
#include "RechargeRWMonitorSvc.h"
#include "IOMonitorSvc.h"
#include "rfidsetsvc.h"
#include "MessageBox.h"
#include "MessagePictureBox.h"
#include "confirmalivesvc.h"
#include "MaintenanceSvc.h"
#include "TransactionHistorySvc.h"
#include "SelectLanguageSvc.h"
#include "CardBoxManageSvc.h"
//#include "CTokenManageSvc.h"
//#include "CoinBoxManageSvc.h"
//#include "BanknoteBoxManageSvc.h"
#include "CoinAddingBoxSetSvc.h"
#include "SwitchOffSetSvc.h"
#include "DeviceStatusSvc.h"
#include "BanknoteAcceptModuleTestSvc.h"
#include "BanknoteChangeModuleTestSvc.h"
#include "IOModuleTestSvc.h"
#include "ReceipPrintSvc.h"
#include "SaleStaticSvc.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxSvc.h"
#include "CoinBoxManageAddCoinToCycleChangeBoxSvc.h"
#include "CoinBoxManageAttachCoinCollectionBoxSvc.h"
#include "CoinBoxManageCollectCoinSvc.h"
#include "CoinBoxManageDetachCoinCollectionBoxSvc.h"
#include "CoinBoxManageDetachCoinAddingBoxSvc.h"
#include "AssistSvc.h"
#include "StandbySvc.h"
#include "ScreenBrightnessSetSvc.h"
#include "BalanceSvc.h"
#include "ModeSettingsSvc.h"
#include "CoinModuleMonitorSvc.h"
#include "BanknoteCollectionModuleMonitorSvc.h"
#include "VersionInfoSvc.h"
#include "AllDeviceStatusSvc.h"
#include "ChangeAndInvalidBanknoteboxManageSvc.h"
#include "BanknoteChangeAndInvalidBoxRemovalSvc.h"
#include "BanknoteCollectingBoxInstallationSvc.h"
#include "BanknoteCollectingBoxRemovalSvc.h"
#include "InstallCoinChangeBoxSvc.h"
#include "RemoveCoinChangeBoxSvc.h"
#include "OneKeySettlementSvc.h"
#include "shieldkeyhelper.h"
#include "ComportAndBaudrateSetSvc.h"
#include "BanknoteBoxCollectionSvc.h"
#include "PrinterMonitorSvc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServiceMgr CServiceMgr::theInstance;   // ʵ����CServiceMgr


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr::CServiceMgr()
{
    m_pRootService = NULL;
    m_pCurService = NULL;
    m_serviceState = INIT;
	m_ServiceBusy = false;

	CMessageBox::OnShowing.AddHandler(this,&CServiceMgr::OnShowingMessageBox);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr::~CServiceMgr()
{
    StopService(m_pRootService);
	CMessageBox::OnShowing.RemoveHandler(this,&CServiceMgr::OnShowingMessageBox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡCServiceMgr���ʵ��

@param      ��

@retval     ��

@exception  CServiceMgr&    CServiceMgr���ʵ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr& CServiceMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)SERVICE_ID serviceID	    Service���
@param      (i)bool post                true: ͨ���ʵ���Ϣ����, false:ֱ������

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StartService(SERVICE_ID serviceID, bool post)
{
	try {
		if (post) 
		{
			AfxGetMainWnd()->PostMessage(WM_START_SERVICE, serviceID, NULL);
		} 
		else if (CMessageBox::Exist()) 
		{
			CMessageBox::End(IDABORT);                 // ���������Ϣ����������
			AfxGetMainWnd()->PostMessage(WM_START_SERVICE, serviceID, NULL);
		}
		else 
		{
			CService* pService=GetService(serviceID);
			if(NULL==pService)
			{
				pService = CreateService(serviceID);
				if(NULL != pService){
					CService* pParentService = SearchService((SERVICE_ID)pService->GetParentServiceID());
					if(NULL!=pParentService)
					{//��ʱ�´�����Service�϶�֮ǰû�д�������ֱ����ӵ���Ӧ�ĸ�Service��ȥ
						pParentService->AddChild(pService);
					}
					StartService(pService);
				}
			}else
			{
				StartService(pService);
			}
		}
	}
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)CService* pService   ��������Service����

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StartService(CService* pService)
{
    try {
        if (pService == NULL) {
            throw CInnerException(ROOT_SVC, CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("����Serviceʱ��Service����ΪNULL"));
        }
        // �����ϼ�Service
        CService* pParentService = SearchService((SERVICE_ID)pService->GetParentServiceID());

        // �����趨��ǰService
        if (pService->GetServiceID() == ROOT_SVC || pService->IsForeground()) 
		{
            m_pCurService = pService;
        }

        if (pParentService != NULL)  // �ϼ�Service����
		{ 
            if (pParentService->GetServiceID() != ROOT_SVC) 
			{
				pParentService->StopChildren();
                //pParentService->DeleteChildren();             // ɾ��ͬ����Service
            }
			StartingService.Invoke(pService);			//�����¼�
            //pParentService->AddChild(pService);    // ���뵽�ϼ�Service
			pService->Start();
			StartedService.Invoke(pService);				//�����¼�
        }
        else  // �ϼ�Service�����ڣ���Root Service�� 
		{    
            m_pRootService = pService;
        }
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ǰService

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::RestartCurService()
{
    if (GetCurService() != NULL) 
	{
        StartService((SERVICE_ID)GetCurService()->GetServiceID());
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰService

@param      ��

@retval     CService*   ��ǰService

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::GetCurService()
{
    return m_pCurService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣService

@param      (i)SERVICE_ID serviceID	    Service���
@param      (i)bool post                true: ͨ���ʵ���Ϣ�ر�, false:ֱ�ӹر�

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StopService(SERVICE_ID serviceID, bool post)
{
    try {
        if (post) 
		{
            AfxGetMainWnd()->PostMessage(WM_STOP_SERVICE, serviceID, NULL);
        }
        else 
		{
            CService* pService = SearchService(serviceID);
            StopService(pService);
        }
    }
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣService

@param      (i)CService* pService

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StopService(CService* pService)
{
    try {
        if (pService != NULL) {
            CService* pParentService = pService->GetParent();

            // �����趨��ǰService
            if (pService->IsForeground())
			{
                m_pCurService = pParentService;
            }
			StopingService.Invoke(pService);
			pService->Stop();
			StopedService.Invoke(pService);
			/*
            if (pParentService != NULL) 
			{
                // �Ӹ�Service��ɾ����Service
                pParentService->DeleteChild(pService);
            }
            else 
			{
                // ɾ��ROOT_SVC, ��Service��Ϊ��
                pService->DeleteChildren();
                delete pService;
                m_pRootService = NULL;
            }
			*/
        }
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service ID��ȡService����

@param      (i)SERVICE_ID serviceID	        Service���

@retval     CService*   NULL��δ�ҵ���������Service����

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::GetService(SERVICE_ID serviceID)
{
    try 
	{
        return SearchService(serviceID);
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ServiceTree�и���Service ID����Service

@param      (i)SERVICE_ID serviceID	        Service���

@retval     SeriverNode     NULL��δ�ҵ���������Serivere

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::SearchService(SERVICE_ID serviceID)
{
    try 
	{
        CService* pService = NULL;
        if (m_pRootService != NULL) 
		{
            pService = m_pRootService->SearchService(serviceID);
        }
        return pService;
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)SERVICE_ID       Service���

@retval     CService*   fail:NULL

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::CreateService(SERVICE_ID serviceID)
{
    try{
        CService* pService = NULL;
        switch(serviceID) {
			case RUN_MODE_SETTING_SVC:
				pService = new CModeSettingsSvc;
				break;
			case OPERATION_CLOSE_SVC:
				pService = new CBalanceSvc;
				break;
		    case CALL_HELP_SVC:
			     pService = new CAssistSvc;
			    break;
			case ANALYZE_SVC:
				pService = new CAnalyzeSvc;
				break;
			case BUSINESS_INTERVAL_SVC:
				pService = new CBusinessTranslateSvc;
				break;
			case CARD_BOX_MANAGE_SVC:
				pService = new CTicketBoxExchangeSvc();
				break;
			case CARD_MODULE_TEST_SVC:
				pService = new CSmardCardTestSvc;
				break;
			case CHARGE_SVC:
				pService = new CChargeSvc;
				break;
			//case ISSUE_PRODUCT_SVC:
			//	pService = new CProductIssueSvc;
			//	break;
			case ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC:
				pService = new CCoinChangeBoxAddingSvc;	// Ӳ��ר��������ӱ�
				break;
			//case ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC:
			//	pService = new CCoinBoxManageAddCoinToCycleChangeBoxSvc;		// Ӳ��ѭ��������ӱ�
			//	break;
			case ATTACH_COLLECTION_COIN_BOX_SVC:
				pService = new CCoinBoxManageAttachCoinCollectionBoxSvc;		// ��װӲ�һ�����
				break;
			case DETACH_COLLECTION_COIN_BOX_SVC:
				pService = new CCoinBoxManageDetachCoinCollectionBoxSvc;		// ж��Ӳ�һ�����
				break;
			//case DETACH_ADDING_COIN_BOX_SVC:
				//pService = new CCoinBoxManageDetachCoinAddingBoxSvc;			// ж��Ӳ�Ҳ�����
				//break;
			case COLLECTION_COIN_SVC:
				pService = new CCoinBoxManageCollectCoinSvc;					// Ӳ�һ���
				break;

			case INSTALL_COIN_CHANGE_BOX_SVC:									// ��װӲ��������
				pService = new CInstallCoinChangeBoxSvc;
				break;

			case REMOVE_COIN_CHANGE_BOX_SVC:									// ж��Ӳ��������
				pService = new CRemoveCoinChangeBoxSvc;
				break;

			case ONE_KEY_SETTLEMENT_SVC:										// һ���ջ�
				pService = new COneKeySettlementSvc;
				break;

			/*case BANKNOTE_BOX_MANAGE_SVC:
				pService = new CBanknoteBoxManageSvc;
				break;	*/
			case INSTALL_BANKNOTE_COLLECTION_BOX_SVC:							// ��װֽ�һ�����
				pService = new CBanknoteCollectingBoxInstallationSvc;
				break;
			case REMOVE_BANKNOTE_COLLECTION_BOX_SVC:
				pService = new CBanknoteCollectingBoxRemovalSvc;				// ж��ֽ�һ�����
				break;
			case INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC:
				pService = new CChangeAndInvalidBanknoteboxManageSvc;			// ��װֽ�������䣬�ϱ���
				break;
			case REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC:						// ж��ֽ�������䣬�ϱ���
				pService = new CBanknoteChangeAndInvalidBoxRemovalSvc;
				break;
			case BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC:							// ֽ�һ���
				pService = new CBanknoteBoxCollectionSvc;
				break;
			case COMMUNICATION_SET_SVC:
				pService = new CCommunicationSetSvc;
				break;
			case CONFIRM_ALIVE_SVC:
				pService = new CConfirmAliveSvc;
				break;
			case DATA_INPUT_SVC:
				pService = new CDataInputSvc;
				break;
			case DATA_OUTPUT_SVC:
				pService = new CDataOutputSvc;
				break;
			case VERSION_INFO_SVC:
				pService = new CVersionInfoSvc;
				break;
			case DEVICE_STATUS_SVC:
				pService = new CDeviceStatusSvc;
				break;
			case EVENT_MSG_SVC:
				pService = new CEventDataSvc;
				break;
			case INIT_SVC:								// ��ʼ��
				pService = new CInitSvc;
				break;
			case ISSUE_SVC:								// ��Ʊ
				pService = new CIssueSvc;
				break;
			case LOGIN_SVC:								// ��¼
				pService = new CLoginSvc;
				break;
			case MAIN_SVC:								// ����
				pService = new CMainSvc;
				break;
			case MAINTENANCE_SVC:
				pService = new CMaintenanceSvc;
				break;
			case MODE_SWITCH_SVC:
				pService = new CCoinAddingBoxSetSvc;
				break;
			case MONITOR_RECHARGE_RW_SVC:
				pService = new CRechargeRWMonitorSvc;
				break;
			case MONITOR_CARD_RW__SVC:
				pService = new CCardRWMonitorSvc;
				break;
			case MONITOR_CARD_ISSUE_MODULE_SVC:
				pService = new CCardModuleMonitorSvc;
				break;
			case MONITOR_COIN_MODULE_SVC:
				pService = new CCoinModuleMonitorSvc;
				break;
			case MONITOR_BANKNOTE_ACCEPT_MODULE_SVC:
				pService = new CBanknoteCollectionModuleMonitorSvc;
				break;
			case MONITOR_IO_SVC:
				pService = new CIOMonitorSvc;
				break;
			//case MONITOR_PINPAD_MODULE_SVC:
			//	pService = new CPinPadMonitorSvc;  // �������
			//	break;
			case NETWORK_TEST_SVC:
				pService = new CNetworkTestSvc;
				break;
			case RECEIPT_RINTER_TEST_SVC:
				pService = new CPrinterTestSvc;
				break;
			case COIN_MODULE_TEST_SVC:					// Ӳ���Լ�-Ӳ��ģ��
				pService = new CCoinModuleTestSvc;
				break;
			case BANKNOTE_ACCEPT_MODULE_TEST_SVC:		// Ӳ���Լ�-ֽ�ҽ���
				pService = new CBanknoteAcceptModuleTestSvc;
				break;
			case BANKNOTE_CHANGE_MODULE_TEST_SVC:       // Ӳ���Լ�-ֽ������
				pService = new CBanknoteChangeModuleTestSvc;
				break;
			case TOKEN_MODULE_TEST_SVC:					// Ӳ���Լ�-Tokenģ��
				pService = new CTokenModuleTestSvc;
				break;
			case IO_MODULE_TEST_SVC:					// Ӳ���Լ�-IOģ��
				pService = new CIOModuleTestSvc;    
				break;
			//case BANKCARD_MODULE_TEST_SVC:				// Ӳ���Լ�-���п�ģ��
			//	pService = new CBankCardModuleTestSvc;
			//	break;
			//case PIN_PAD_TEST_SVC:						// Ӳ���Լ�-�������ģ��
			//	pService = new CPinPadTestSvc;
			//	break;
			case MONITOR_PRINTER_MODULE_SVC:			// ��ӡ�����
				pService = new CPrinterMonitorSvc;
				break;
			case ALL_DEVICE_STATUS_SVC:
				pService = new CAllDeviceStatusSvc;
				break;
			case REMOVE_EXCEPTION_SVC:					// �쳣���
				pService = new CRemoveExceptionSvc;
				break;
			case RFID_MODE_SET_SVC:
				pService = new CRFIDModeSetSvc;
				break;
			case ROOT_SVC:								// ��
				pService = new CRootSvc;
				break;						
			case RW_TEST_SVC:
				pService = new CRWTestSvc;
				break;			
			case SALE_STATIC_SVC:          
				pService = new CSaleStaticSvc;
				break;
			case SC_SVC:
				pService = new CServerClientSvc;
				break;			
			case STATION_SET_SVC:
				pService = new CStationSetSvc;
				break;
			case SWITCH_OFF_SVC:
				pService = new CSwitchOffSetSvc;
				break;
			case SYSTEM_TIME_SET_SVC:
				pService = new CTimeSetSvc;
				break;
			case SCREEN_BRIGHTNESS_SET:
				pService = new CScreenBrightnessSetSvc;
				break;
			case TOKEN_BOX_MANAGE_SVC:
				//pService = new CTokenManageSvc();
				break;
			case TRANSACTION_HISTORY_SVC:
				pService = new CTransactionHistorySvc;
				break;
			case TRANSMISSION_INTERVAL_SVC:
				pService = new CTransSvc;
				break;									
			/*case WAIT_MAINTENANCE_SVC:
				pService = new CSelectLanguageSvc;
				break;*/
			case STANDBY_SVC:
				pService = new CStandbySvc;
				break;
			case RECEIPT_REPRINT_SVC:
				pService = new CReceipPrintSvc;
				break;
			case DEVICE_COMPORT_SET_SVC:
				pService = new CComportAndBaudrateSetSvc;		// ��������
				break;
			case SHUTDOWN_SVC:
				{
					theSERVICE_MGR.SetForeServiceBusy(true);
					theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
					theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
					theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
				}
				break;;
			case RESTART_SVC:
				{
					theSERVICE_MGR.SetForeServiceBusy(true);
					theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
					theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
					theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
				}
				break;
			case EXIT_SVC:
				{
					CShieldKeyHelper::SetDesktopEnable(true);		// ��ʾ����
					CShieldKeyHelper::SetHostProgarmEnable(false);	// �˳��ػ�����
					theAPP_SESSION.SetCloseStartFlag(CS_EXIT);		// �����˳�
					theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
				}
				break;
			default:
				break;
        }
        return pService;
    }
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��Service, ����Ĭ��Service

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::NotifyFinish()
{
    // ����ǰ����ҵ��������Ĭ��ҵ��������ҵ����ֹͣ��ҵ��
    if (GetCurService() != NULL && GetCurService()->GetParentServiceID() == MAIN_SVC) 
	{
		GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,0);
    }
    else  if (GetCurService() != NULL) 
	{
        StopService((SERVICE_ID)GetCurService()->GetServiceID(), true);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֪ͨǰ̨Serviceģʽ�����ı�

@param      (i)UINT mode                ģʽ���ࣨSP_MODE_SYSTEM������
@param      (i)CForeService* pService   ��֪ͨ��Service��NULL: MAIN_SVC��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CServiceMgr::NotifyModeChange(UINT mode, CTVMForeService* pService)
//{
//    // pServiceΪ��ʱ��ȡMAIN_SVC
//    if (pService == NULL) 
//	{
//        pService = (CTVMForeService*)theSERVICE_MGR.GetService(MAIN_SVC);
//        if (pService == NULL) 
//		{
//            return;
//        }
//    }
//    // ����Service��OnModeChange
//    pService->OnModeChange(mode);
//    // �ݹ������Service��OnModeChange
//    CService::ServiceChildren children = pService->GetChildren();
//    for (size_t i = 0; i < children.size(); i++) 
//	{
//        ((CTVMForeService*)children[i])->OnModeChange(mode);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ѵ�ǰ�����Ϊæµ

@param      (i)bool bBusy \n
true :��Ϊæµ ; false: ��Ϊ��æµ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetForeServiceBusy(bool bBusy)
{
	if(bBusy == m_ServiceBusy)
	{
		return;
	}
	m_ServiceBusy = bBusy;
	if(GetCurService()->GetServiceID()!=MAIN_SVC)
	{
		GetService<CMainSvc>(MAIN_SVC)->OnBusy(bBusy);
	}
	if (GetCurService()->IsForeground()) 
	{
		CTVMForeService* pService = (CTVMForeService*)GetCurService();
		if (pService != NULL) 
		{
			pService->OnBusy(bBusy);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�жϵ�ǰ�����Ƿ�Ϊæµ  

@param      (o)bool m_ServiceBusy
            true:æµ  false:��æµ

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CServiceMgr::IsForeServiceBusy(){
	return m_ServiceBusy;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡҵ��״̬

@param      ��

@retval     SERVICE_STATE, ҵ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATE CServiceMgr::GetState()
{
    return m_serviceState;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ҵ��״̬

@param      (i)SERVICE_STATE state, ҵ��״̬

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetState(SERVICE_STATE state)
{
	//LOG("m_state = %d, state = %d", m_serviceState, state);
	// �쳣��
    if (state == ABNORMAL) {
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("��������Ϊ�쳣״̬"));
    }
    try{
		// ����״̬��ԭ״̬��ͬ
		if (state == m_serviceState) {
			// �������������ü�ʱ��
			if(state == RECEPT){
				int interval = theAFC_TVM_OPERATION.GetCancelDealTime();/*theTVM_INFO.GetReceptToReadyTimerInterval()*/;
				if (0 == interval){
					interval = 60;//��λ����
				}
				GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
			}
			return;
		}
		// �豸������
		if(state == ISSUEING){
			SetForeServiceBusy(true);
		}
		else{
			SetForeServiceBusy(false);
		}
		// �����м�ʱ��
		if ((FINISH == m_serviceState || COUNTING == m_serviceState || CANCELING == m_serviceState) && RECEPT == state){
			// ������ڼ����л�����У���ǰ����Ϊ�����У�������״̬��ֱ�ӷ���
			return;
		}

		if(state == FINISH || state == RECEPT || state == CANCELING){
			int interval = state == RECEPT ? theAFC_TVM_OPERATION.GetCancelDealTime():theTVM_INFO.GetFinishToReadyTimerInterval()/*theTVM_INFO.GetReceptToReadyTimerInterval()*/;
			GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
		}
		else{
			GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_DEFAULT_SERVICE);
		}

		//if(state == RECEPT){
		//	int interval = theAFC_TVM_OPERATION.GetCancelDealTime()/*theTVM_INFO.GetReceptToReadyTimerInterval()*/;
		//	GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
		//}
		//else{
		//	GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_DEFAULT_SERVICE);
		//}

		//// FINISH״̬�Ƚ����⣬FINISH״ֻ̬���ڶ�ʱ��������ˣ��л���RECEIPT״̬
		//if(state == FINISH){
		//	int interval = theTVM_INFO.GetFinishToReadyTimerInterval();
		//	GetService(MAIN_SVC)->PostMessage(SM_MAIN_TIMER_CONTROL, 1, (LPARAM)(new int(interval)));
		//}
		//else{
		//	// do nothing
		//}

		// ֻ�еȴ�����Ϣ״̬�Ż����ö�д�����
		if(state == READY || state == REST){
			// �������RW���
			if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
				// ��������״̬����ǰΪĬ��Serviceʱ
				theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->ResetStartedByMonitorStatus();
			}
			//if(theTVM_INFO.IsPinPadMonitorOpened() && !theEXCEPTION_MGR.HasException(CTVMPinPadException::MODULE_ID)){
			//	//����������̼��
			//	GetService<CPinPadMonitorSvc>(MONITOR_PINPAD_MODULE_SVC)->Resume();
			//}
		}
		else{
			// ֹͣ׼���е������м�ʱ��
			theSERVICE_MGR.GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_READY_TO_REST);
			// ����˿͵����Ļ��ť��ֹͣ������̼��
			/*if(theTVM_INFO.IsPinPadMonitorOpened() && !theAPP_SESSION.IsBlindManOperation()){
				GetService<CPinPadMonitorSvc>(MONITOR_PINPAD_MODULE_SVC)->StopWaitPinPadInput();
			}*/
		}
		// ������ͣ����ʱ��״̬תΪ��ͣ����
		if (OUT_OF == state){
			theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		}
		// ά����
		if(state == MAINTENANCE){
			DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
			if (dwOutOfTime > 0)
			{
				// 0.�Զ��ǳ�ʱ�����0ʱ�����Զ��ǳ���ʱ��
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);
			}
		}

		SERVICE_STATE oldState = m_serviceState;
        m_serviceState = state;
		StateChanged.Invoke(oldState,m_serviceState);
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����ʾ

@param      (i)SERVICE_STATE state, ҵ��״̬

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::OnShowingMessageBox()
{
	//if(m_serviceState!=ISSUEING && m_serviceState!=MAINTENANCE)
	//{
	//	SetState(ISSUEING);
	//}
}
