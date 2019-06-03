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

CServiceMgr CServiceMgr::theInstance;   // 实例化CServiceMgr


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr::~CServiceMgr()
{
    StopService(m_pRootService);
	CMessageBox::OnShowing.RemoveHandler(this,&CServiceMgr::OnShowingMessageBox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取CServiceMgr类的实例

@param      无

@retval     无

@exception  CServiceMgr&    CServiceMgr类的实例
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr& CServiceMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动Service

@param      (i)SERVICE_ID serviceID	    Service编号
@param      (i)bool post                true: 通过邮递消息启动, false:直接启动

@retval     无

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
			CMessageBox::End(IDABORT);                 // 如果存在消息框，则先销毁
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
					{//此时新创建的Service肯定之前没有创建过，直接添加到对应的父Service中去
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
@brief      启动Service

@param      (i)CService* pService   被启动的Service对象

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StartService(CService* pService)
{
    try {
        if (pService == NULL) {
            throw CInnerException(ROOT_SVC, CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("启动Service时，Service对象为NULL"));
        }
        // 查找上级Service
        CService* pParentService = SearchService((SERVICE_ID)pService->GetParentServiceID());

        // 重新设定当前Service
        if (pService->GetServiceID() == ROOT_SVC || pService->IsForeground()) 
		{
            m_pCurService = pService;
        }

        if (pParentService != NULL)  // 上级Service存在
		{ 
            if (pParentService->GetServiceID() != ROOT_SVC) 
			{
				pParentService->StopChildren();
                //pParentService->DeleteChildren();             // 删除同级的Service
            }
			StartingService.Invoke(pService);			//引发事件
            //pParentService->AddChild(pService);    // 加入到上级Service
			pService->Start();
			StartedService.Invoke(pService);				//引发事件
        }
        else  // 上级Service不存在（是Root Service） 
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
@brief      重新启动当前Service

@param      无

@retval     无

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
@brief      获取当前Service

@param      无

@retval     CService*   当前Service

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::GetCurService()
{
    return m_pCurService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止Service

@param      (i)SERVICE_ID serviceID	    Service编号
@param      (i)bool post                true: 通过邮递消息关闭, false:直接关闭

@retval     无

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
@brief      停止Service

@param      (i)CService* pService

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StopService(CService* pService)
{
    try {
        if (pService != NULL) {
            CService* pParentService = pService->GetParent();

            // 重新设定当前Service
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
                // 从父Service中删除本Service
                pParentService->DeleteChild(pService);
            }
            else 
			{
                // 删除ROOT_SVC, 置Service树为空
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
@brief      根据Service ID获取Service对象

@param      (i)SERVICE_ID serviceID	        Service编号

@retval     CService*   NULL：未找到，其他：Service对象

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
@brief      在ServiceTree中根据Service ID查找Service

@param      (i)SERVICE_ID serviceID	        Service编号

@retval     SeriverNode     NULL：未找到，其他：Serivere

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
@brief      创建Service

@param      (i)SERVICE_ID       Service编号

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
				pService = new CCoinChangeBoxAddingSvc;	// 硬币专用找零箱加币
				break;
			//case ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC:
			//	pService = new CCoinBoxManageAddCoinToCycleChangeBoxSvc;		// 硬币循环找零箱加币
			//	break;
			case ATTACH_COLLECTION_COIN_BOX_SVC:
				pService = new CCoinBoxManageAttachCoinCollectionBoxSvc;		// 安装硬币回收箱
				break;
			case DETACH_COLLECTION_COIN_BOX_SVC:
				pService = new CCoinBoxManageDetachCoinCollectionBoxSvc;		// 卸载硬币回收箱
				break;
			//case DETACH_ADDING_COIN_BOX_SVC:
				//pService = new CCoinBoxManageDetachCoinAddingBoxSvc;			// 卸载硬币补充箱
				//break;
			case COLLECTION_COIN_SVC:
				pService = new CCoinBoxManageCollectCoinSvc;					// 硬币回收
				break;

			case INSTALL_COIN_CHANGE_BOX_SVC:									// 安装硬币找零箱
				pService = new CInstallCoinChangeBoxSvc;
				break;

			case REMOVE_COIN_CHANGE_BOX_SVC:									// 卸载硬币找零箱
				pService = new CRemoveCoinChangeBoxSvc;
				break;

			case ONE_KEY_SETTLEMENT_SVC:										// 一键收机
				pService = new COneKeySettlementSvc;
				break;

			/*case BANKNOTE_BOX_MANAGE_SVC:
				pService = new CBanknoteBoxManageSvc;
				break;	*/
			case INSTALL_BANKNOTE_COLLECTION_BOX_SVC:							// 安装纸币回收箱
				pService = new CBanknoteCollectingBoxInstallationSvc;
				break;
			case REMOVE_BANKNOTE_COLLECTION_BOX_SVC:
				pService = new CBanknoteCollectingBoxRemovalSvc;				// 卸载纸币回收箱
				break;
			case INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC:
				pService = new CChangeAndInvalidBanknoteboxManageSvc;			// 安装纸币找零箱，废币箱
				break;
			case REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC:						// 卸载纸币找零箱，废币箱
				pService = new CBanknoteChangeAndInvalidBoxRemovalSvc;
				break;
			case BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC:							// 纸币回收
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
			case INIT_SVC:								// 初始化
				pService = new CInitSvc;
				break;
			case ISSUE_SVC:								// 售票
				pService = new CIssueSvc;
				break;
			case LOGIN_SVC:								// 登录
				pService = new CLoginSvc;
				break;
			case MAIN_SVC:								// 主控
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
			//	pService = new CPinPadMonitorSvc;  // 密码键盘
			//	break;
			case NETWORK_TEST_SVC:
				pService = new CNetworkTestSvc;
				break;
			case RECEIPT_RINTER_TEST_SVC:
				pService = new CPrinterTestSvc;
				break;
			case COIN_MODULE_TEST_SVC:					// 硬件自检-硬币模块
				pService = new CCoinModuleTestSvc;
				break;
			case BANKNOTE_ACCEPT_MODULE_TEST_SVC:		// 硬件自检-纸币接收
				pService = new CBanknoteAcceptModuleTestSvc;
				break;
			case BANKNOTE_CHANGE_MODULE_TEST_SVC:       // 硬件自检-纸币找零
				pService = new CBanknoteChangeModuleTestSvc;
				break;
			case TOKEN_MODULE_TEST_SVC:					// 硬件自检-Token模块
				pService = new CTokenModuleTestSvc;
				break;
			case IO_MODULE_TEST_SVC:					// 硬件自检-IO模块
				pService = new CIOModuleTestSvc;    
				break;
			//case BANKCARD_MODULE_TEST_SVC:				// 硬件自检-银行卡模块
			//	pService = new CBankCardModuleTestSvc;
			//	break;
			//case PIN_PAD_TEST_SVC:						// 硬件自检-密码键盘模块
			//	pService = new CPinPadTestSvc;
			//	break;
			case MONITOR_PRINTER_MODULE_SVC:			// 打印机监控
				pService = new CPrinterMonitorSvc;
				break;
			case ALL_DEVICE_STATUS_SVC:
				pService = new CAllDeviceStatusSvc;
				break;
			case REMOVE_EXCEPTION_SVC:					// 异常解除
				pService = new CRemoveExceptionSvc;
				break;
			case RFID_MODE_SET_SVC:
				pService = new CRFIDModeSetSvc;
				break;
			case ROOT_SVC:								// 根
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
				pService = new CComportAndBaudrateSetSvc;		// 串口设置
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
					CShieldKeyHelper::SetDesktopEnable(true);		// 显示桌面
					CShieldKeyHelper::SetHostProgarmEnable(false);	// 退出守护进程
					theAPP_SESSION.SetCloseStartFlag(CS_EXIT);		// 程序退出
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
@brief      终止本Service, 启动默认Service

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::NotifyFinish()
{
    // 若当前是主业务，则启动默认业务，若是子业务，则停止子业务
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
@brief      通知前台Service模式发生改变

@param      (i)UINT mode                模式种类（SP_MODE_SYSTEM，，）
@param      (i)CForeService* pService   被通知的Service（NULL: MAIN_SVC）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CServiceMgr::NotifyModeChange(UINT mode, CTVMForeService* pService)
//{
//    // pService为空时，取MAIN_SVC
//    if (pService == NULL) 
//	{
//        pService = (CTVMForeService*)theSERVICE_MGR.GetService(MAIN_SVC);
//        if (pService == NULL) 
//		{
//            return;
//        }
//    }
//    // 调用Service的OnModeChange
//    pService->OnModeChange(mode);
//    // 递归调用子Service的OnModeChange
//    CService::ServiceChildren children = pService->GetChildren();
//    for (size_t i = 0; i < children.size(); i++) 
//	{
//        ((CTVMForeService*)children[i])->OnModeChange(mode);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      把当前画面变为忙碌

@param      (i)bool bBusy \n
true :变为忙碌 ; false: 变为非忙碌

@retval     无

@exception  无
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
@brief		判断当前画面是否为忙碌  

@param      (o)bool m_ServiceBusy
            true:忙碌  false:不忙碌

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CServiceMgr::IsForeServiceBusy(){
	return m_ServiceBusy;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取业务状态

@param      无

@retval     SERVICE_STATE, 业务状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATE CServiceMgr::GetState()
{
    return m_serviceState;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置业务状态

@param      (i)SERVICE_STATE state, 业务状态

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetState(SERVICE_STATE state)
{
	//LOG("m_state = %d, state = %d", m_serviceState, state);
	// 异常中
    if (state == ABNORMAL) {
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("不能设置为异常状态"));
    }
    try{
		// 设置状态与原状态相同
		if (state == m_serviceState) {
			// 操作中重新设置计时器
			if(state == RECEPT){
				int interval = theAFC_TVM_OPERATION.GetCancelDealTime();/*theTVM_INFO.GetReceptToReadyTimerInterval()*/;
				if (0 == interval){
					interval = 60;//单位：秒
				}
				GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
			}
			return;
		}
		// 设备运行中
		if(state == ISSUEING){
			SetForeServiceBusy(true);
		}
		else{
			SetForeServiceBusy(false);
		}
		// 计数中计时器
		if ((FINISH == m_serviceState || COUNTING == m_serviceState || CANCELING == m_serviceState) && RECEPT == state){
			// 如果是在计数中或完成中，当前动作为操作中，不设置状态，直接返回
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

		//// FINISH状态比较特殊，FINISH状态只会在定时器间隔到了，切换到RECEIPT状态
		//if(state == FINISH){
		//	int interval = theTVM_INFO.GetFinishToReadyTimerInterval();
		//	GetService(MAIN_SVC)->PostMessage(SM_MAIN_TIMER_CONTROL, 1, (LPARAM)(new int(interval)));
		//}
		//else{
		//	// do nothing
		//}

		// 只有等待和休息状态才会启用读写器监控
		if(state == READY || state == REST){
			// 如果开启RW监控
			if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
				// 重置启动状态，当前为默认Service时
				theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->ResetStartedByMonitorStatus();
			}
			//if(theTVM_INFO.IsPinPadMonitorOpened() && !theEXCEPTION_MGR.HasException(CTVMPinPadException::MODULE_ID)){
			//	//启动密码键盘监控
			//	GetService<CPinPadMonitorSvc>(MONITOR_PINPAD_MODULE_SVC)->Resume();
			//}
		}
		else{
			// 停止准备中到待机中计时器
			theSERVICE_MGR.GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_READY_TO_REST);
			// 如果乘客点击屏幕按钮，停止密码键盘监控
			/*if(theTVM_INFO.IsPinPadMonitorOpened() && !theAPP_SESSION.IsBlindManOperation()){
				GetService<CPinPadMonitorSvc>(MONITOR_PINPAD_MODULE_SVC)->StopWaitPinPadInput();
			}*/
		}
		// 设置暂停服务时，状态转为暂停服务
		if (OUT_OF == state){
			theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		}
		// 维护中
		if(state == MAINTENANCE){
			DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
			if (dwOutOfTime > 0)
			{
				// 0.自动登出时间大于0时重置自动登出计时器
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
@brief      提示框显示

@param      (i)SERVICE_STATE state, 业务状态

@retval     无

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
