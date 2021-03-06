#include "StdAfx.h"
#include "MainSvc.h"
#include "AppSession.h"
#include "MessageBox.h"
#include "MessagePictureBox.h"
#include "LogoffHelper.h"
#include "InnerException.h"
#include "ServiceMgr.h"
#include "ExceptionMgr.h"
#include "tvmdef.h"
#include "UpgradeHelper.h"
#include "TVM.h"
#include "tvmStatusMgr.h"
#include "SNTPHelper.h"
#include "SCAuditManager.h" 
#include "InitSvc.h"
#include "CommonDefine.h"
#include "EventDataSvc.h"
#include "audithelper.h"
#include "guideinfo.h"
#include "tvmsetting.h"
#include "LogHelper.h"
#include "shieldkeyhelper.h"
#include "NavigationDlg.h"
#include "CDiskException.h"
#include "common.h"
#include "CLOGLoginAndLogoff.h"
#include "RechargeRWMonitorSvc.h"
#include "CoinModuleMonitorSvc.h"
#include "BanknoteCollectionModuleMonitorSvc.h"
#include "PassengerOperationLog.h"
#include "IssueSvc.h"
#include "ChargeSvc.h"
#include "WebServerHelper.h"
#include "IOMonitorSvc.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static const int INTERVAL_SWITH_BUSINESS_DAY=1;
#define UNKNOWN_ID	_T("FFFFFFFF")



BEGIN_MESSAGE_MAP(CMainSvc,CTVMForeService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SM_NET_STATUS,OnNetStatusChange)
	ON_SERVICE_MESSAGE(SM_FORCE_LOGOUT,OnForceLogout)
	ON_SERVICE_MESSAGE(SM_MAIN_SHUT_DOWN,OnShutDown)
	ON_SERVICE_MESSAGE(SM_MAIN_CLOSE_OPERATION,OnCloseOperation)
	ON_SERVICE_MESSAGE(SM_MAIN_START_NTP_TIMER,OnStartNtpTimer)
	ON_SERVICE_MESSAGE(SM_MAIN_NEW_WORK_MODE,OnNewWorkMode)
	ON_SERVICE_MESSAGE(SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER,OnStartSwithBusinessDayTimer)
	ON_SERVICE_MESSAGE(SM_MAIN_NEW_OPERATION_MODE,OnNewOperationMode)
	ON_SERVICE_MESSAGE(SM_MAIN_DEVICE_CONTROL,OnDeviceControl)
	ON_SERVICE_MESSAGE(SM_MAIN_IO_MONITOR,OnStartIOMonitor)				// IO监控消息对应函数
	ON_SERVICE_MESSAGE(SM_MAIN_RETURN_INDICATOR,OnStartReturnIndicator) // 找零指示灯
	ON_SERVICE_MESSAGE(SM_MAIN_RETURN_LIGHT,OnStartReturnLight)			// 找零照明灯
	ON_SERVICE_MESSAGE(SM_MAIN_READY_TO_REST,OnStartReadyToRest)		// 等待到待机
	ON_SERVICE_MESSAGE(SM_DOOR_CLOSED,OnFrontDoorClose)					// 关前维护门
	ON_SERVICE_MESSAGE(SM_MAIN_DOOR_OPEN_WITH_OUT_LOGIN,OnFrontDoorOpenWithoutLogin)			// 门开启登陆超时
	ON_SERVICE_MESSAGE(SM_MAIN_DOOR_OPEN_OUT_OF_TIME_OPERATION,OnFrontDoorOpenTimeOutOperation)	// 门开启操作超时
	ON_SERVICE_MESSAGE(SM_MAIN_ON_SWITCH_BUSINESS_DAY_OPERATION,OnSwitchBusinessDayAndCloseOperation)// 执行运营日切换及日结
	ON_SERVICE_MESSAGE(SM_MAIN_TIMER_CONTROL,OnServiceTimerControl)				// 服务控制定时器处理
	ON_SERVICE_MESSAGE(SM_MAIN_WEB_IDLE_PROCESS,OnWebIdleProcess)				// 二维码订单本地处理
	ON_SERVICE_MESSAGE(SM_MAIN_RECHARGE_SERVER_TIMER,OnRechargeServerMonitor)	// 充值服务器检测处理
	ON_SERVICE_MESSAGE(SM_MAIN_PARAMETER_UPDATE_TIMER,OnParameterUpdateProcess)	// SC下载参数加载处理
END_MESSAGE_MAP()

CTVMRWHelper* RECHARGERW_HELPER=NULL;
CTVMRWHelper* CARDRW_HELPER=NULL;

CPrinterHelper* PRINTER_HELPER=NULL;
CTOKENHelper* TOKEN_HELPER=NULL;
CTHHelper* CARD_HELPER=NULL;
CIOModuleHelper* IO_HELPER=NULL;
//CBanknoteModuleHelper* BANKNOTE_HELPER=NULL;
CBRHModuleHelper* BANKNOTE_HELPER = NULL;
//CBHChangeHelper * BH_CHANGE_HELPER=NULL;
CBRHModuleHelper * BH_CHANGE_HELPER=NULL;
CCoinModuleHelper* COIN_HELPER=NULL;
CDataHelper* DATA_HELPER=NULL;
CAudioHelper* AUDIO_HELPER=NULL;
//CBHRfidHelper* BH_RFID_HELPER = NULL;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainSvc::CMainSvc(void) :CTVMForeService(MAIN_SVC, ROOT_SVC)
{
	m_pRWHelper	= new CTVMRWHelper(TVM_CARD_RW);
	m_pRechargeHelper = new CTVMRWHelper(TVM_RECHARGE_RW);
	m_pPrinterHelper = new CPrinterHelper();
	m_pTokenHelper = new CTOKENHelper(*this);
	m_pCardModuleHelper = new CTHHelper(*this);                 // 出票机Helper
	m_pIOHelper = new CIOModuleHelper();
	//m_pStatusDisplayModuleHelper=new CStatusDisplayModuleHelper(*this);
	//m_pBanknoteModuleHelper=new CBanknoteModuleHelper();
	m_pBanknoteModuleHelper=new CBRHModuleHelper();
	//m_pBHChangeHelper=new CBHChangeHelper();
	m_pBHChangeHelper=new CBRHModuleHelper();
	m_pCoinModuleHelper=new CCoinModuleHelper();
	m_pDataHelper=new CDataHelper();
	m_pAudioHelper = new CAudioHelper();
	//BH_RFID_HELPER = new CBHRfidHelper();
	CARDRW_HELPER	  = m_pRWHelper;
	RECHARGERW_HELPER = m_pRechargeHelper;
	/*TOKENRW_HELPER=m_pTokenRWHelper;*/

	PRINTER_HELPER=m_pPrinterHelper;
	TOKEN_HELPER=m_pTokenHelper;
	CARD_HELPER=m_pCardModuleHelper;
	IO_HELPER=m_pIOHelper;
	BANKNOTE_HELPER=m_pBanknoteModuleHelper;
	//BH_CHANGE_HELPER=m_pBHChangeHelper;
	BH_CHANGE_HELPER=BANKNOTE_HELPER;
	COIN_HELPER=m_pCoinModuleHelper;
	DATA_HELPER=m_pDataHelper;
	AUDIO_HELPER=m_pAudioHelper;

	m_CurrentBusiDate = ComGetBusiDate();

	// 设置操作员画面
	m_pDialogFlow->AddDialog(CNavigationDlg::IDD, new CNavigationDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _T(""));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_NAVI, _T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_NAVI, CNavigationDlg::IDD);
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_NAVI);
	m_pDialogFlow->ShowFirstDialogGroup();

	theMENU_MGR.Initialize();
	CXButton::Clicking.AddHandler(this,&CMainSvc::OnButtonClicking);
	CGEdit::EditTextChanged.AddHandler(this,&CMainSvc::OnEditChanged);

	// IO Module
	UPORDOWN     = 1;
	OPENORCLOSE  = 1;
	SOMEONECLOSE = 0;
	WHENSHOCKACTIVE = 1;
	m_bIsAramOpen = false;
	m_bIsBeginRunning = false;
	m_bIsEndRunning   = false;
	m_bIsCallHelpRunning = false;

	// 上位命令状态
	m_bIsSvcMsgComeSystemBusy = false;
	m_device_contrl_code = DEVICE_CONTRL_CODE_SC_UNKNOW;
	m_bIsOperaArryBusy = false;
	m_bBeforeRunCommandArrial = false;
	m_bEndRuningCommandArrial = false;
	m_bIsServerMsgArrial	  = false;
	m_bIsSwitchBussinessDay	  = false;
	m_bIsStartOperation = theTVM_SETTING.IsStartOpeartion();
	m_bIsReadyStartOperation = false;
	m_bIsStartOperationWhenOpen = false;
	m_bIsMenualRepare = false;
	m_bPuseCheckOperation = false;

	m_timerInterval = 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainSvc::~CMainSvc(void)
{
	CXButton::Clicking.RemoveHandler(this,&CMainSvc::OnButtonClicking);
	CGEdit::EditTextChanged.RemoveHandler(this,&CMainSvc::OnEditChanged);
	theMENU_MGR.ShutDown();

	delete m_pRechargeHelper;
	m_pRechargeHelper = NULL;
	delete m_pRWHelper;
	m_pRWHelper = NULL;
	delete m_pCardModuleHelper;
	m_pCardModuleHelper = NULL;
	delete m_pPrinterHelper;
	m_pPrinterHelper = NULL;
	delete m_pIOHelper;
	m_pIOHelper = NULL;

	delete m_pCardModuleHelper;
	m_pCardModuleHelper = NULL;
	delete m_pBanknoteModuleHelper;
	m_pBanknoteModuleHelper = NULL;
	delete m_pBHChangeHelper;
	m_pBHChangeHelper = NULL;
	delete m_pCoinModuleHelper;
	m_pCoinModuleHelper = NULL;
	delete m_pDataHelper;
	m_pDataHelper = NULL;
	delete m_pAudioHelper;
	m_pAudioHelper = NULL;

	//delete BH_RFID_HELPER;
	//BH_RFID_HELPER = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CMainSvc::OnButtonClicking(CXButton* pButton,bool*)
{
	if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == FINISH || theSERVICE_MGR.GetState() == RECEPT)
	{
		theSERVICE_MGR.SetState(RECEPT);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnEditChanged(CGEdit*,CString)
{
	if(theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == FINISH || theSERVICE_MGR.GetState() == RECEPT)
	{
		theSERVICE_MGR.SetState(RECEPT);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnStart()
{
	__super::OnStart();
	//m_pDialogFlow->GetDialog(CMenuDlg::IDD)->Invalidate();
	//m_pDialogFlow->GetDialog(CMenuDlg::IDD)->UpdateWindow();
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      启动默认服务

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::StartDefaultService()
{
#if 1
	// 4.3初始化获取支付方式代码
	WORD device_pay_status1	  = theTVM_SETTING.GetPayMentMode();
	WORD device_change_status1 = theTVM_SETTING.GetChangeMode();
	WORD device_service_mode1  = theTVM_SETTING.GetServiceMode();
	WORD device_print_mode1    = theTVM_SETTING.GetPrintMode();
	// 9.设置支付状态
	theTVM_STATUS_MGR.SetPaymentMode(device_pay_status1);
	// 10.设置找零状态
	theTVM_STATUS_MGR.SetChangeMode(device_change_status1);
	// 11.设置打印模式
	theTVM_STATUS_MGR.SetPrintMode(device_print_mode1);
	// 12.设置业务模式
	theTVM_STATUS_MGR.SetWorkMode(device_service_mode1);

	// 显示屏状态为正常（无法判断是否正常）
	theTVM_STATUS_MGR.SetTouchDisplayStatus(TOUCH_SCREEN_STATUS::TOUCH_SCREEN_STATUS_NORMAL);

	// 13.设置功能菜单
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);	
	// 14.启动默认业务：发票业务
	theSERVICE_MGR.StartService(ISSUE_SVC);
	// 15.设置状态
	if (theSERVICE_MGR.GetState() != MAINTENANCE) {
		theSERVICE_MGR.SetState(READY);
	}
	//theSERVICE_MGR.SetForeServiceBusy(false);
	LOG("End Start Default Service");
	m_bIsStartingDefaultServic = false;
	//NOTIFY_PROCESSING_WINDOW_CLOSE();
	return;

#endif
	

	LOG("Start Default --- begin");
	CMessagePictureBox::End();
	// 首先判断状态
	if(REST == theSERVICE_MGR.GetState()){// 如果不经过传感器，点击屏幕需要改变状态
		SOMEONECLOSE ^= 1;
	}
	//// 启动默认业务证明已经完成,防止开门，乘客操作点取消不返回。
	//theSERVICE_MGR.SetState(FINISH);

	// 上位命令优先	
	if(!CheckSvcStatus()){
		LOG("上位命令优先！");
		return;
	}

	// 正在启动默认业务
	m_bIsStartingDefaultServic = true;

	bool bIsAllowDegrade = theTVM_SETTING.IsAllowDegrade();// 是否支持自动降级？

	try
	{
		IO_HELPER->CloseReturnLight();
		IO_HELPER->ReturnRechargeCard();

	}
	catch (CException* e)
	{
	}
	// 如果有硬盘异常，直接暂停服务
	if(theEXCEPTION_MGR.HasException(CDiskException::MODULE_ID,CDiskException::UE_DISK_NOSPACE)){
		LOG("硬盘异常，暂停服务。");
		goto START_OUT_OF_SERVICE;
	}

	// 如果有最大离线天数异常，直接暂停服务
	if(theEXCEPTION_MGR.HasException(CSCException::MODULE_ID,CSCException::NOT_CONNECT_SERVER_OUT_OF_TIME)){
		LOG("最大离线天数异常，暂停服务。");
		goto START_OUT_OF_SERVICE;
	}

	// 如果参数异常，直接暂停服务
	if(theEXCEPTION_MGR.HasException(CParamException::MODULE_ID)){
		LOG("参数异常，暂停服务。");
		goto START_OUT_OF_SERVICE;
	}

	LOG("Start Default Service");
	// 1.隐藏取消按钮
	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_HIDE);
	// 2.判断维护门是否开启，如果开启，则不能启用默认服务
	CIOMonitorSvc* psvr = (CIOMonitorSvc*)(theSERVICE_MGR.GetService(SERVICE_ID::MONITOR_IO_SVC));

	if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
		try{
			GetIOModuleHelper()->CloseBKCardIndicator();
			//GetIOModuleHelper()->GetIOStatus(m_IOStatusRsp);
			m_IOStatusRsp = psvr->ioStatus;
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}

		if (m_IOStatusRsp.isBackDoorOpen || m_IOStatusRsp.isFrontDoorOpen){
			LOG("维护门打开，暂停服务。");
			goto START_OUT_OF_SERVICE;
		}
	}
	// 3.判断上位模式代码
	SERVICE_STATUS status = theTVM_STATUS_MGR.GetServiceStatus();
	OperationalMode_t operation_mode = theTVM_STATUS_MGR.GetOperationMode();
	// 3.1判断是否是暂停服务？启动暂停服务
	if(theTVM_SETTING.IsSCServiceOutOfService()){
		LOG("上位下发，暂停服务。");
		goto START_OUT_OF_SERVICE;
	}
	// 3.2判断是否是停止服务？（运营结束）
	if(STOP_SERVICE == status){
		LOG("运营结束，停止服务。");
		goto START_OUT_OF_SERVICE;
	}
	// 3.3判断是否是紧急服务？
	if (OperationalMode_t::EMERGENCY_MODE == operation_mode || TRAIN_SERVICE_DISRUPTION_MODE == operation_mode){
		LOG("紧急服务，暂停服务。");
		goto START_OUT_OF_SERVICE;
	}
	// 3.4招援按钮是否按下？
	if(theTVM_INFO.IsIOUsed()){
		// 招援按钮按下，直接返回，在招援按钮响应函数中去响应启动招援业务
		if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
			if(m_IOStatusRsp.isAssittentButtonPressed){
				//goto ERROR_RETURN;
			}
		}
	}

	// 4.判断硬件状态
	// 4.1进行一次异常解除
	if (theFunction_INFO.GetTimesRepareException() > 0 && theEXCEPTION_MGR.HasException()){
		//启用默认业务是否需要进行异常解除？
		theEXCEPTION_MGR.RepareException(m_bIsMenualRepare);
		m_bIsMenualRepare = false;
	}

	// 取IO状态
	IO_RSP_STATUS_INFO ioStatus;
	try{
		//IO_HELPER->GetIOStatus(ioStatus);
		ioStatus = psvr->ioStatus;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

	// 4.2如果有NTP对时、FTP、IO异常，直接进入暂停服务
	if(theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID) ||
		theEXCEPTION_MGR.HasException(CFtpException::MODULE_ID) ||
		theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			goto START_OUT_OF_SERVICE;
	}
	// 4.3初始化获取支付方式代码
	WORD device_pay_status	  = theTVM_SETTING.GetPayMentMode();
	WORD device_change_status = theTVM_SETTING.GetChangeMode();
	WORD device_service_mode  = theTVM_SETTING.GetServiceMode();
	WORD device_print_mode    = theTVM_SETTING.GetPrintMode();
	SERVICE_STATUS service_status = IN_SERVICE;//theTVM_STATUS_MGR.GetServiceStatus();
	LOG("device_pay_status = %04x, device_change_status = %04x, device_service_mode = %04x, device_print_mode = %04x",
		device_pay_status, device_change_status, device_service_mode, device_print_mode);

	// 4.4 获取当前支持的支付模式代码
	GetPaymentInfoFromFuncIni(device_pay_status);

	// 检测配置文件合法性
	CheckIniValue(device_service_mode,device_pay_status,device_change_status);

	bool bIsBoxError = false;//钱票箱故障为true
	// 4.5 判断硬币模块状态:是否使用？
	if (theTVM_INFO.IsCHUsed()){	
		if (theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
			// 4.5.1不支持自动降级，直接进入暂停服务
			if(!bIsAllowDegrade)
				goto START_OUT_OF_SERVICE;
			// 4.5.2支持自动降级
			else{
				LOG("硬币模块故障，降级。");
				// 当前是否支持硬币支付？
				if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
					device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
				}
				// 当前是否支持硬币找零？
				if ((device_change_status & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
					device_change_status = (device_change_status ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
				}
			}

		}
		// 4.5.3没有异常
		else{
			// 判断当前硬币模块是否有异常，如果有异常判断是否可以降级，可以降级直接降级！	
			tCHADevStatus rsp;
			try{
				GetCoinModuleHelper()->CH_GetStatus(&rsp);
				// 模块没有到到位，不能找零与接收
				if(!rsp.bIsCoinModuleStandby){
					LOG("硬币模块未到位。");
					throw CCHHardwareException(CCHHardwareException::ERROR_CH_MODULE_NOT_STANDBY,_T(__FILE__),__LINE__);
				}
			}
			// 获取硬币状态产生异常，可能当前硬币模块断开连接
			catch(CSysException& e){
				LOG("硬币模块获取状态异常。");
				// 不允许降级，直接启用暂停服务
				if (!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				// 降级为不可使用此硬币模块
				else{
					// 当前是否支持硬币支付？
					if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
						device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
					}
					// 当前是否支持硬币找零？
					if ((device_change_status & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
						device_change_status = (device_change_status ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
					}
				}
				theEXCEPTION_MGR.ProcessException(e);
			}

			// 1）获取各箱子的软计数信息
			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
			//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionB = theCOIN_COUNT.GetCollectionCoinboxBInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBox1 = theCOIN_COUNT.GetChangeCoinboxAInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBox2 = theCOIN_COUNT.GetChangeCoinboxBInfo();
			// 循环找零箱
			CTCoinCountInfo::COIN_BOX_INFO hopper1Info = theCOIN_COUNT.GetCoinHopperboxAInfo();
			CTCoinCountInfo::COIN_BOX_INFO hopper2Info = theCOIN_COUNT.GetCoinHopperboxBInfo();

			// 2）判断各箱子的安装状态
			bool bIsInstallErrorA = false;
			//// 回收箱A状态
			if(collectionA.coinBoxID.ToString() == UNKNOWN_ID && rsp.bIsMoneyBoxExist ){
				LOG("硬币回收箱非法安装。");
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				bIsInstallErrorA = true;
				bIsBoxError = true;
			}
			else if(collectionA.coinBoxID.ToString() != UNKNOWN_ID && !rsp.bIsMoneyBoxExist){
				LOG("硬币回收箱不到位。");
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
				bIsInstallErrorA = true;
			}
			else if(collectionA.coinBoxID.ToString() != UNKNOWN_ID && rsp.bIsMoneyBoxExist){
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
			else{
				LOG("硬币回收箱未安装。");
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}

			//3）判断各箱子的数量状态
			// -------------------找零箱1状态
			if (changeBox1.ulCount  <= theAFC_TVM_OPERATION.GetChangeCoinBoxNearEmptyCount() && changeBox1.ulCount >0){
				// 向上位发送将空信息
				theTVM_STATUS_MGR.SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
			}
			// 已空
			else if(changeBox1.ulCount == 0){
				// 向上位发送将空信息
				theTVM_STATUS_MGR.SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			// ------------------找零箱2状态
			if(changeBox2.ulCount <= theAFC_TVM_OPERATION.GetChangeCoinBoxNearEmptyCount() && changeBox2.ulCount > 0){
				theTVM_STATUS_MGR.SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
			}
			// 已空
			else if(changeBox2.ulCount == 0){
				// 向上位发送将空信息
				theTVM_STATUS_MGR.SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			//------------------循环找零箱1状态
			if(hopper1Info.ulCurCount <= theAFC_TVM_OPERATION.GetChangeCoinBoxNearEmptyCount() && hopper1Info.ulCurCount > 0){
				LOG("硬币循环箱1将空。");
				theTVM_STATUS_MGR.SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
			}
			else if(hopper1Info.ulCurCount == 0){
				LOG("硬币循环箱1已空。");
				theTVM_STATUS_MGR.SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			//else if(hopper1Info.ulCurCount >= theTVM_INFO.GetCHHopper1MaxCount()){
			//	LOG("Coin hopper1 num status --- Full!");
			//	theTVM_STATUS_MGR.SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
			//}
			else{
				theTVM_STATUS_MGR.SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			//------------------循环找零箱2状态
			if(hopper2Info.ulCurCount <= theAFC_TVM_OPERATION.GetChangeCoinBoxNearEmptyCount() && hopper2Info.ulCurCount > 0){
				LOG("硬币循环箱2将空。");
				theTVM_STATUS_MGR.SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
			}
			else if(hopper2Info.ulCurCount == 0){
				LOG("硬币循环箱2已空。");
				theTVM_STATUS_MGR.SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			//else if(hopper2Info.ulCurCount >= theTVM_INFO.GetCHHopper2MaxCount()){
			//	LOG("Coin hopper2 num status --- Full!");
			//	theTVM_STATUS_MGR.SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
			//}
			else{
				theTVM_STATUS_MGR.SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			//------------------回收箱状态
			long ulCountA = collectionA.GetCurCount();//collectionA.ulCount_1yuan + collectionA.ulCount_2yuan + collectionA.ulCount_5jiao + collectionA.ulCount_5yuan;// 左边的硬币数
			//long ulCountB = collectionB.GetCurCount();//collectionB.ulCount_1yuan + collectionB.ulCount_2yuan + collectionB.ulCount_5jiao + collectionB.ulCount_5yuan;// 其他非一元
			if (ulCountA< theAFC_TVM_OPERATION.GetRecycleBoxFullCount() && ulCountA >= theAFC_TVM_OPERATION.GetRecycleBoxNearFullCount()){
				LOG("硬币回收箱将满。");
				// 向上位发送回收箱将满信息
				theTVM_STATUS_MGR.SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
			}
			// 已满
			else if(ulCountA >= theAFC_TVM_OPERATION.GetRecycleBoxFullCount()){
				LOG("硬币回收箱已满。");
				theTVM_STATUS_MGR.SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
			}
			// 正常
			else{
				theTVM_STATUS_MGR.SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}

			// 4）安装状态的业务判断
			if(bIsInstallErrorA ){// 判断当前是否支持自动降级?
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				else{// 设置不可使用硬币支付
					if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
						device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
					}
				}
			}
			// 判断是否有回收箱箱子没有安装?
			if(collectionA.coinBoxID.ToString() == UNKNOWN_ID /*|| collectionB.coinBoxID.ToString() == UNKNOWN_ID*/){
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				else if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
					LOG("硬币回收箱未安装降级。");

					device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
				}
			}

			// 5）数量状态的业务判断
			// 判断是否已空
			long lAllCount = changeBox1.ulCount + changeBox2.ulCount + hopper1Info.ulCurCount + hopper2Info.ulCurCount;
			if (lAllCount == 0){
				// 判断是否支持自动降级？
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				else{
					LOG("硬币钱箱空降级。");

					// 设置不可找零
					if ((device_change_status & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
						device_change_status = (device_change_status ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
					}
				}
			}
			// 判断当前是否不能够接收硬币？(循环找零箱+回收箱A+待收最大接收）>=回收箱A已满)
			if((ulCountA + hopper2Info.ulCurCount + hopper1Info.ulCurCount + theTVM_INFO.GetCHTemporary1Max()) >= theAFC_TVM_OPERATION.GetRecycleBoxFullCount()){
				LOG("循环找零箱加回收箱A的数量大于回收箱已满枚数，暂不接收硬币。");
				if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
					device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
				}
			}

			// 6）参数的业务判断
			// 判断当前支持的硬币种类
			bool bSupportOneYuan = true,bSupportHalfYuan = true;
			vector<WORD> vec_acceptTable;
			// 售票支持的硬币类型
			theAFC_TVM_OPERATION.GetAcceptCoinFaceOfIssue(vec_acceptTable);
			for(auto i = vec_acceptTable.begin();i != vec_acceptTable.end();i++){
				if(*i == 100){
					bSupportOneYuan = true;
				}

				if(*i == 50){
					bSupportHalfYuan = true;
				}
			}
			// 不接收硬币
			if(!bSupportHalfYuan && !bSupportOneYuan){
				if(!bIsAllowDegrade)
					goto START_OUT_OF_SERVICE;
				else if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
					device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
				}
			}

			// 回收箱到位
			if(rsp.bIsMoneyBoxExist){
				// 判断一元及五角Hopper状态
				if(ulCountA >= theAFC_TVM_OPERATION.GetRecycleBoxFullCount()													// 回收箱已满
					&& (!bSupportOneYuan || bSupportOneYuan && hopper1Info.ulCurCount >= theTVM_INFO.GetCHHopper1MaxCount()) 	// 一元Hopper已满（如果可以接收）
					&& (!bSupportHalfYuan || bSupportHalfYuan && hopper2Info.ulCurCount >= theTVM_INFO.GetCHHopper2MaxCount())	// 五角Hopper已满（如果可以接收）
					){
						if(!bIsAllowDegrade){
							goto START_OUT_OF_SERVICE;
						}
						else if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
							LOG("回收箱已满且循环箱数量异常，支付模式降级。");
							device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
						}
				}
			}
			// 右边不到位，只判断Hopper数量状态
			else {
				// 添加异常信息
				theEXCEPTION_MGR.ProcessException(CCHHardwareException(CCHHardwareException::ERROR_CH_COLLECT_BOX_A_NOT_OPEN,_T(__FILE__),__LINE__));
				if((!bSupportOneYuan || bSupportOneYuan && hopper1Info.ulCurCount >= theTVM_INFO.GetCHHopper1MaxCount()) 	// 一元Hopper已满（如果可以接收）
					&&(!bSupportHalfYuan || bSupportHalfYuan && hopper2Info.ulCurCount >= theTVM_INFO.GetCHHopper2MaxCount())		// 五角Hopper已满（如果可以接收）
					){
						if(!bIsAllowDegrade){
							goto START_OUT_OF_SERVICE;
						}
						else if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
							device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
						}
				}
			}
		}
	}
	// 切离设定：不适用硬币模块
	else{
		// 判断是否支持自动降级？
		if(!bIsAllowDegrade){
			goto START_OUT_OF_SERVICE;
		}
		else{
			// 当前是否支持硬币支付？
			if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
				device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
			}
			// 当前是否支持硬币找零？
			if ((device_change_status & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
				device_change_status = (device_change_status ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
			}
		}
	}
	// 4.5.0判断硬币接收口是否使用？(IO 处理)

	// 4.6 判断纸币循环状态
	if(theTVM_INFO.IsBHUsed()){
		// 4.6.1纸币箱有无异常
		if (theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)){
			// 4.6.2不支持自动降级，直接进入暂停服务
			if(!bIsAllowDegrade){
				goto START_OUT_OF_SERVICE;
			}
			// 4.6.3支持自动降级
			else{
				// 4.6.4设置不可纸币支付、找零
				if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
					device_pay_status = device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE | DEVICE_CONTRL_CODE_PAYMENT;
				}
				if((device_change_status & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) == DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
					device_change_status = device_change_status ^ DEVICE_CONTRL_CODE_NO_COIN_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE;
				}
			}
		}
		// 4.6.5无异常
		else{
			// Waring:判断钱箱状态
			bool bIsBanknoteBoxAttached = false;
			bool bIsChangeBoxAttached = false;
			map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> mapItems;
			try{
				bIsBanknoteBoxAttached = BANKNOTE_HELPER->IsCashBoxAttached();
				bIsChangeBoxAttached   = BANKNOTE_HELPER->IsCashChangeBoxAttached();
				// 纸币回收箱不在位
				if(!bIsBanknoteBoxAttached){
					throw CBRHHardwareException(CBRHHardwareException::ERR_CASHBOX_NOT_INSTALLED,_T(__FILE__),__LINE__);
				}

				// 3. 纸币模块不到位
				if(!m_IOStatusRsp.isBanknoteModeReady){
					throw CBRHHardwareException(CBRHHardwareException::BANKNOTE_ERROR_MODULE_NOT_STANDBY,_T(__FILE__),__LINE__);
				}

				//tBncDevReturn bh_status;
				//GetBanknoteModuleHelper()->GetStatus(bh_status);

				//// 设备需要复位
				//if(bh_status.iType == 2){
				//	BANKNOTE_HELPER->Reset();
				//}
				//// 根据下面的方式，判断纸币模块是否处于忙碌状态
				//do{
				//	try{
				//		mapItems = BANKNOTE_HELPER->GetEmptyFull();
				//	}
				//	catch(CSysException&e)
				//	{
				//		if (e.GetDetailID()!=CBRHHardwareException::ERR_BUSY){
				//			throw e;
				//		}
				//		else{
				//			Sleep(1000);//等纸币模块退出忙碌状态
				//		}
				//	}
				//}while(1);


				//if (bh_status.intermediateStackerStatus == XFS_S_CDR_IS_NOT_EMPTY)
				//{
				//	
				//	throw CBRHHardwareException(CBRHHardwareException::ERR_CDR_CASH_UNIT_ERROR,_T(__FILE__),__LINE__);

				//}
				theTVM_STATUS_MGR.SetBanknoteModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_NORMAL);
			}
			catch(CSysException& e){
				// 不支持自动降级，直接进入暂停服务
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				// 支持自动降级
				else{
					//设置不可纸币支付
					if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
						device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
					}
					// 设置不可纸币找零
					if((device_change_status & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) == DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
						device_change_status = device_change_status ^ DEVICE_CONTRL_CODE_NO_COIN_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE;
					}
					// 设置不充值
					if ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE)
					{
						device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
				theEXCEPTION_MGR.ProcessException(e);
			}


			// 1）获取各箱子的软计数信息
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& banknote = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			bool bIsChangeBoxEmpty = false,bIsRecyclerBoxAEmpty = false,bIsRecyclerBoxBEmpty = false,bIsRecyclerBoxCEmpty = false,bIsRecyclerDEmpty = false;
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& cashBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& changeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& recyclerBoxAInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& recyclerBoxBInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& recyclerBoxCInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& recyclerBoxDInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

			// 2）判断各箱子的安装状态
			// ---------------回收箱
			bool bIsIllegalInstall = false;
			// 纸币箱非法卸载
			if (theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() != UNKNOWN_ID && !bIsBanknoteBoxAttached){
				LOG("纸币回收箱未到位。");
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
				bIsIllegalInstall = true;
				bIsBoxError = true;
			}
			// 非法安装
			else if(theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() == UNKNOWN_ID && bIsBanknoteBoxAttached){
				LOG("纸币回收箱非法安装。");
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				bIsIllegalInstall = true;
				bIsBoxError = true;

			}
			// 未安装
			else if(theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() == UNKNOWN_ID && !bIsBanknoteBoxAttached){
				LOG("纸币回收箱未安装。");
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
				bIsIllegalInstall = true;
			}
			// 正常安装
			else{
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
			// ---------------找零箱
			bool bIsChangeBoxInstallIllegal = false;
			if(changeBoxInfo.banknoteBoxID.ToString() == UNKNOWN_ID && bIsChangeBoxAttached){
				LOG("纸币补充箱非法安装。");
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				bIsChangeBoxInstallIllegal = true;
				bIsBoxError = true;
			}
			else if(changeBoxInfo.banknoteBoxID.ToString() != UNKNOWN_ID && !bIsChangeBoxAttached){
				LOG("纸币补充箱未到位。");
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
				bIsChangeBoxInstallIllegal = true;
				bIsBoxError = true;
			}
			else if(changeBoxInfo.banknoteBoxID.ToString() == UNKNOWN_ID && !bIsChangeBoxAttached){
				LOG("纸币补充箱未安装。");
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}

			// 3）判断各箱子的数量状态
			// ---------------回收箱
			bool bIsCashBoxFull = false;
			long ulCount = banknote.BankntoeTotalCount();//banknote.ulCount_10 + banknote.ulCount_100 + banknote.ulCount_1 + banknote.ulCount_20 + banknote.ulCount_50 + banknote.ulCount_5;
			// 纸币箱将满
			if (ulCount > theAFC_TVM_OPERATION.GetBanknotesNearFullCount() && ulCount < theAFC_TVM_OPERATION.GetBanknotesFullConut()){
				LOG("纸币回收箱将满。");
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
			}
			// 纸币箱已满
			else if(ulCount >= theAFC_TVM_OPERATION.GetBanknotesFullConut()){
				LOG("纸币回收箱已满。");
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
				bIsCashBoxFull = true;
			}
			// 纸币箱数量正常
			else{
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			// ---------------找零箱		
			long lChangeCount = changeBoxInfo.BanknotesTotalCount();
			if(lChangeCount > 0 && lChangeCount <= theAFC_TVM_OPERATION.GetBanknoteChangeNearEmptyCount()){
				LOG("纸币补充箱将空。");
				theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
			}
			else if(lChangeCount == 0 ){
				LOG("纸币补充箱已空。");
				theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);bIsChangeBoxEmpty = true;
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}


			map<BNR_MODULE_ID,T_XfsPhysicalCashUnit>::iterator it; 
			long lFullCount = 0;
			long lNearFullCount = 0;
			long lRECount = 0;
			// ---------------循环鼓1
			lRECount =recyclerBoxAInfo.ulCurCount;
			it = mapItems.find(BNR_RECYCLER_BOX_A);
			if(it != mapItems.end()){
				lFullCount = it->second.threshold.full;
				lNearFullCount = it->second.threshold.high;
			}
			if(lRECount >= 0 && lRECount < lNearFullCount){
				LOG("循环箱1将空。");
				theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			else if(lRECount >= lNearFullCount && lRECount < lFullCount){
				LOG("循环箱1已空。");
				theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			// ---------------循环鼓2
			lRECount =recyclerBoxBInfo.ulCurCount;
			it = mapItems.find(BNR_RECYCLER_BOX_B);
			if(it != mapItems.end()){
				lFullCount = it->second.threshold.full;
				lNearFullCount = it->second.threshold.high;
			}
			if(lRECount >= 0 && lRECount < lNearFullCount){
				LOG("循环箱2将空。");
				theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			else if(lRECount >= lNearFullCount && lRECount < lFullCount){
				LOG("循环箱2已空。");
				theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			// ---------------循环鼓3
			lRECount =recyclerBoxCInfo.ulCurCount;
			it = mapItems.find(BNR_RECYCLER_BOX_C);
			if(it != mapItems.end()){
				lFullCount = it->second.threshold.full;
				lNearFullCount = it->second.threshold.high;
			}
			if(lRECount >= 0 && lRECount < lNearFullCount){
				LOG("循环箱3将空。");
				theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			else if(lRECount >= lNearFullCount && lRECount < lFullCount){
				LOG("循环箱3已空。");
				theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			// ---------------循环鼓4
			lRECount =recyclerBoxDInfo.ulCurCount;
			it = mapItems.find(BNR_RECYCLER_BOX_D);
			if(it != mapItems.end()){
				lFullCount = it->second.threshold.full;
				lNearFullCount = it->second.threshold.high;
			}
			if(lRECount >= 0 && lRECount < lNearFullCount){
				LOG("循环箱4将空。");
				theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}
			else if(lRECount >= lNearFullCount && lRECount < lFullCount){
				LOG("循环箱4已空。");
				theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			}
			else{
				theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}

			// 4）业务判断		
			// 循环鼓空、找零箱空需要降级
			if(bIsChangeBoxEmpty || !bIsChangeBoxAttached){
				if(recyclerBoxAInfo.ulCurCount == 0 &&
					recyclerBoxBInfo.ulCurCount == 0 &&
					recyclerBoxCInfo.ulCurCount == 0 && 
					recyclerBoxDInfo.ulCurCount == 0){
						LOG("不支持纸币找零。");
						// 设置不可纸币找零
						if((device_change_status & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) == DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
							device_change_status = device_change_status ^ DEVICE_CONTRL_CODE_NO_COIN_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE;
						}
				}
			}
			// 非法安装，未安装，已满都需要降级为不收纸币
			if(bIsIllegalInstall || !bIsBanknoteBoxAttached || bIsCashBoxFull){
				if(!bIsAllowDegrade){// 是否支持自动降级？
					goto START_OUT_OF_SERVICE;
				}
				else {
					LOG("不收纸币，不充值。");
					if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
						device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
					}
					// 设置不充值
					if ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
						device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
			}
		}
	}
	// 切离设定不支持纸币
	else{
		// 是否支持自动降级？
		if(!bIsAllowDegrade)
			goto START_OUT_OF_SERVICE;
		else{
			// 设置不可纸币支付
			if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
				device_pay_status = (device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
			// 设置不可纸币找零
			if((device_change_status & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) == DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
				device_change_status = device_change_status ^ DEVICE_CONTRL_CODE_NO_COIN_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE;
			}
			// 设置不充值
			if ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
				device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
			}

		}
	}

	// 4.8 判断票卡
	if(theTVM_INFO.IsTHUsed() && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_CARD_RW)){
		// 4.8.1判断是否有异常？
		if (theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) ||
			theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) ||
			theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) ||
			theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
				// 4.8.1.1判读是否支持自动降级?
				if(!bIsAllowDegrade)
					goto START_OUT_OF_SERVICE;
				// 4.8.1.2支持自动降级，设置票卡模块不可用
				else{
					LOG("票卡读写器故障或卡模块故障降级");
					if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD)
						device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD | DEVICE_CONTRL_CODE_BUSINESS;
				}
				theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_FAILURE);
		}
		// 4.8.2票卡模块正常(无异常)
		else{

			// 获取票卡模块状态
			TH_RSP_GET_DEV_STATUS deviceStatus;
			try{
				GetCardModuleHelper()->GetDevStatus(&deviceStatus);

				//// 有故障，或者出票口卡票尝试修复出票机
				//if(deviceStatus.uiType != 0){
				//	// 修复成功，再次读取状态
				//	if(CARD_HELPER->TryRepareTH()){
				//		CARD_HELPER->GetDevStatus(&deviceStatus);
				//	}
				//}

				//// 如果无工作票箱，需要设置工作
				//if(!deviceStatus.ReadAreaHasCard){
				//	CARD_HELPER->SetWorkMode(CMD_WORK_MODE_NORMAL);
				//}

				// 发卡模块未推到位
				IO_RSP_STATUS_INFO io_state;
				//IO_HELPER->GetIOStatus(io_state);
				io_state = psvr->ioStatus;
				if(!ioStatus.isCardModeReady){
					theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_UNSET);
					throw CTHHardwareException(CTHHardwareException::ERROR_TH_NOT_STANDBY,_T(__FILE__),__LINE__);
				}

				//// 出票口卡票（初始化也没有修复，只能人工处理）
				//if(deviceStatus.bOutShutterDetect){
				//	throw CTHHardwareException(CTHHardwareException::ERROR_TH_CARD_OUT_JAM,_T(__FILE__),__LINE__);
				//}
				// 废票箱不在位
				if(!deviceStatus.bBoxCArrive){
					throw CTHHardwareException(CTHHardwareException::ERROR_TH_WASTE_BOX_NOT_INSTALL,_T(__FILE__),__LINE__);
				}
				// 票箱1盖板未打开　票箱2盖板未打开　支持单个票箱　王峰
				if(!deviceStatus.bBoxACoverArrive && !deviceStatus.bBoxBCoverArrive){
					//此处只为抛出异常，暂时未定义新的异常信息 王峰
					throw CTHHardwareException(CTHHardwareException::ERROR_TH_BOX1_COVER_CLOSE,_T(__FILE__),__LINE__);
				}
				//// 票箱2盖板未打开
				//if(!deviceStatus.bBoxBCoverArrive){
				//	throw CTHHardwareException(CTHHardwareException::ERROR_TH_BOX2_COVER_CLOSE,_T(__FILE__),__LINE__);
				//}
				//// 废票箱盖板未打开
				//if(!deviceStatus.bBoxCCoverArrive){
				//	throw CTHHardwareException(CTHHardwareException::ERROR_TH_WASTE_BOX_COVER_CLOSE,_T(__FILE__),__LINE__);
				//}

				// 废票箱满
				if(deviceStatus.bBoxCFull){
					throw CTHHardwareException(CTHHardwareException::ERROR_TH_WASTE_BOX_FULL,_T(__FILE__),__LINE__);
				}

				// 非法安装
				if(theCARD_COUNT.GetCardboxAInfo().ticketBoxID.ToString() == UNKNOWN_ID || 
					theCARD_COUNT.GetCardboxBInfo().ticketBoxID.ToString() == UNKNOWN_ID ||
					theCARD_COUNT.GetCardboxWastedInfo().ticketBoxID.ToString() == UNKNOWN_ID){
						//lichao						throw CTHHardwareException(CTHHardwareException::ERROR_TH_ILLGIAL_INSTALL,_T(__FILE__),__LINE__);
						bIsBoxError = true;
				}

				// 实际计数与逻辑计数不符合（A、B都空）
				if(deviceStatus.bBoxAEmpty && (theCARD_COUNT.GetCardboxAInfo().ulCurCount > 0)&& deviceStatus.bBoxBEmpty && (theCARD_COUNT.GetCardboxBInfo().ulCurCount > 0)){
					throw CTHHardwareException(CTHHardwareException::ERROR_TH_ILLGIAL_COUNT,_T(__FILE__),__LINE__);
				}

				// 设备故障
				if(deviceStatus.uiType > 1){
					throw CTHHardwareException(CTHHardwareException::TH_HARDWERE_ERROR,_T(__FILE__),__LINE__);
				}

				// 设备正常
				theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_NORMAL);
			}
			catch(CSysException& e){
				theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_FAILURE);
				theEXCEPTION_MGR.ProcessException(e);
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				else{
					if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
						device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
			}
			catch(...){
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}

			bool bCardBoxAEmpty = false,bCardBoxBEmpty = false,bWastedFull = false;
			// 硬件满足售票需求
			//if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
			CTCardCountInfo::CARD_BOX_INFO cardBoxA = theCARD_COUNT.GetCardboxAInfo();
			CTCardCountInfo::CARD_BOX_INFO cardBoxB = theCARD_COUNT.GetCardboxBInfo();
			CTCardCountInfo::CARD_BOX_INFO wastedBox= theCARD_COUNT.GetCardboxWastedInfo();

			// --------------------------------------------票箱数量状态---------------------
			//if (theTVM_STATUS_MGR.GetCardBox1NumStatus() != MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY)
			{
				// ------------------------票箱A数量状态
				if(cardBoxA.ulCurCount <= theAFC_TVM_OPERATION.GetTicketEmptyCount() && 0 <= cardBoxA.ulCurCount || deviceStatus.bBoxAEmpty){
					theTVM_STATUS_MGR.SetCardBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);bCardBoxAEmpty = true;
				}
				else if(cardBoxA.ulCurCount <= theAFC_TVM_OPERATION.GetTicketNearEmptyCount() && theAFC_TVM_OPERATION.GetTicketEmptyCount() < cardBoxA.ulCurCount){
					theTVM_STATUS_MGR.SetCardBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
				}
				else{
					theTVM_STATUS_MGR.SetCardBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
				}

			}
			//if (theTVM_STATUS_MGR.GetCardBox2NumStatus()!= MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY)
			{
				// ------------------------票箱B数量状态
				if(cardBoxB.ulCurCount <= theAFC_TVM_OPERATION.GetTicketEmptyCount() && 0 <= cardBoxB.ulCurCount || deviceStatus.bBoxBEmpty){
					theTVM_STATUS_MGR.SetCardBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);bCardBoxBEmpty = true;
				}
				else if(cardBoxB.ulCurCount <= theAFC_TVM_OPERATION.GetTicketNearEmptyCount() && theAFC_TVM_OPERATION.GetTicketEmptyCount() < cardBoxB.ulCurCount){
					theTVM_STATUS_MGR.SetCardBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_EMPTY);
				}
				else{
					theTVM_STATUS_MGR.SetCardBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
				}

			}


			// ------------------------废票箱数量状态
			if(wastedBox.ulCurCount < theAFC_TVM_OPERATION.GetTicketInvalidFullCount() && theAFC_TVM_OPERATION.GetTicketInvalidNearFullCount() <= wastedBox.ulCurCount){
				theTVM_STATUS_MGR.SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
			}
			else if(theAFC_TVM_OPERATION.GetTicketInvalidFullCount() <= wastedBox.ulCurCount){
				theTVM_STATUS_MGR.SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);bWastedFull = true;
			}
			else{
				theTVM_STATUS_MGR.SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			}

			bool bInstallIllagelA = false, bInstallIllagelB = false,bInstallIllagelC = false;
			// ------------------------------------------票箱安装状态
			// ------------------------票箱A
			if(cardBoxA.ticketBoxID.ToString() == UNKNOWN_ID && deviceStatus.bBoxACoverArrive){
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);bInstallIllagelA = true;
				bIsBoxError = true;

			}
			else if(cardBoxA.ticketBoxID.ToString() != UNKNOWN_ID && !deviceStatus.bBoxACoverArrive){
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);bInstallIllagelA = true;
				bIsBoxError = true;

			}
			else if(cardBoxA.ticketBoxID.ToString() == UNKNOWN_ID && !deviceStatus.bBoxACoverArrive){
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}

			// ------------------------票箱B
			if(cardBoxB.ticketBoxID.ToString() == UNKNOWN_ID && deviceStatus.bBoxBCoverArrive == SET){
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);bInstallIllagelB = true;
				bIsBoxError = true;
			}
			else if(cardBoxB.ticketBoxID.ToString() != UNKNOWN_ID && !deviceStatus.bBoxBCoverArrive){
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);bInstallIllagelB = true;
				bIsBoxError = true;

			}
			else if(cardBoxB.ticketBoxID.ToString() == UNKNOWN_ID && !deviceStatus.bBoxBCoverArrive){
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}

			// ------------------------废票箱
			if(wastedBox.ticketBoxID.ToString() == UNKNOWN_ID && deviceStatus.bBoxCCoverArrive){
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);bInstallIllagelC = true;
			}
			else if(wastedBox.ticketBoxID.ToString() != UNKNOWN_ID && !deviceStatus.bBoxCCoverArrive){
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);bInstallIllagelC = true;
			}
			else if(wastedBox.ticketBoxID.ToString() == UNKNOWN_ID && !deviceStatus.bBoxCCoverArrive){
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}

			// 判断是否为空
			if((bInstallIllagelA && bInstallIllagelB) || (bCardBoxAEmpty && bCardBoxBEmpty) || bWastedFull || bInstallIllagelC){
				if(!bIsAllowDegrade){
					goto START_OUT_OF_SERVICE;
				}
				else{
					if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
						device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
			}

			//// 如果A与B都空了，需要提示
			//if(bCardBoxAEmpty && bCardBoxBEmpty){
			//	try{
			//		throw CTHHardwareException(CTHHardwareException::ERROR_TH_CARD_BOX_EMPTY,_T(__FILE__),__LINE__,_T("票箱A和B都空了。"));
			//	}
			//	catch(CSysException& e){
			//		theEXCEPTION_MGR.ProcessException(e);
			//	}
			//	catch(...){
			//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			//	}
			//}
			//}
		}
	}
	else{
		// 不支持自动降级，直接进入暂停服务
		if (!bIsAllowDegrade)
			goto START_OUT_OF_SERVICE;
		// 设置不可售Card
		else{
			if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD)== DEVICE_CONTRL_CODE_BUSINESS_CARD)
				device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
		}
	}

	// 5. 判断充值
	if (theTVM_INFO.IsRWUsed(RW_TYPE::TVM_RECHARGE_RW) /*&& theFunction_INFO.IsMenuVisibled(RECHARGE_ID)*/){
		// 5.1 有读卡异常
		if (theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID)|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
			// 5.1.1 如果不支持自动降级，直接进入暂停服务状态
			if (!bIsAllowDegrade)
				goto START_OUT_OF_SERVICE;
			// 5.1.2 否则禁用充值、售产品、查询业务
			else{
				// 禁用充值
				if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
					device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE | DEVICE_CONTRL_CODE_BUSINESS;
				}
			}
		}
		// 5.2 无异常，根据上位下发状态自动判断状态，这里不做处理
		else{
			RW_RSP_TPU_INFO rsp;
			RW_RSP_TPU_INFO rsp_compare;
			try{
				GetRWHelper()->GetTPUInfo(rsp);
				// 判断否有SMA卡在位
				if(memcmp(rsp.accISAMid,rsp_compare.accISAMid,sizeof(rsp_compare.accISAMid)) == 0){
					//throw CRWHardwareException(RW_TYPE::TVM_RECHARGE_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
				}
				theTVM_STATUS_MGR.SetRW2Status(STATUS_RW::RW_NORMAL);

			}
			catch(CSysException& e){
				theTVM_STATUS_MGR.SetRW2Status(STATUS_RW::RW_ERROR);
				// 如果不支持自动降级，直接进入暂停服务状态
				if(!bIsAllowDegrade)
					goto START_OUT_OF_SERVICE;
				else{
					// 禁用充值
					if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
						device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
				theEXCEPTION_MGR.ProcessException(e);
			}
		}
	}
	else{
		theTVM_STATUS_MGR.SetRW2Status(STATUS_RW::RW_ERROR);

		// 如果不支持自动降级，直接进入暂停服务状态
		if(!bIsAllowDegrade)
			goto START_OUT_OF_SERVICE;
		else{
			// 禁用充值
			if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
				device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
			}
		}
	}

	// 判断读卡器状态，是否正常？
	if(theTVM_INFO.IsRWUsed(RW_TYPE::TVM_CARD_RW)){
		//  有读卡异常,直接暂停服务
		if (theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) ||
			theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
				theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_ERROR);
				// 5.1.1 如果不支持自动降级，直接进入暂停服务状态
				if (!bIsAllowDegrade)
					goto START_OUT_OF_SERVICE;
				// 5.1.2 否则禁用充值、售产品、查询业务
				else{
					// 禁用充值
					if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
						device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD | DEVICE_CONTRL_CODE_BUSINESS;
					}
				}
		}
		// 无异常，根据上位下发状态自动判断状态，这里不做处理
		else{
			RW_RSP_TPU_INFO tpuInfo;
			RW_RSP_TPU_INFO rsp_compare;
			try{
				GetRWHelper()->GetTPUInfo(tpuInfo);
				//// 判断否有SMA卡在位
				//RW_RSP_TPU_INFO rsp_compare;
				//if(memcmp(tpuInfo.accISAMid,rsp_compare.accISAMid,sizeof(rsp_compare.accISAMid)) == 0){
				//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
				//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
				//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
				//}
				//if(memcmp(tpuInfo.accPSAMid,rsp_compare.accPSAMid,sizeof(rsp_compare.accPSAMid)) == 0){
				//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
				//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
				//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
				//}
				//if(memcmp(tpuInfo.bmacPSAMid,rsp_compare.bmacPSAMid,sizeof(rsp_compare.bmacPSAMid)) == 0){
				//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
				//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
				//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
				//}	

				// 上报SAM卡状态
				theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
				// 上报读写器状态
				theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_NORMAL);
			}
			catch(CSysException& e){
				theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_ERROR);
				theEXCEPTION_MGR.ProcessException(e);
				goto START_OUT_OF_SERVICE;
			}
		}
	}
	else{
		// 如果不支持自动降级，直接进入暂停服务状态
		if(!bIsAllowDegrade)
			goto START_OUT_OF_SERVICE;
		else{
			// 禁用充值
			if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
				device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
			}
		}
	}

	// 判断当前是否支持打印模式？
	if(theTVM_INFO.IsPrinterUsed()){
		// 判断是否有异常？
		if(theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
			if(!bIsAllowDegrade)
				goto START_OUT_OF_SERVICE;
			else{
				if(device_print_mode == 0){
					device_print_mode = DEVICE_CONTRL_CODE_SC_NO_PRINT;
				}
			}
		}
		// 无异常
		else{
			// 再次判断是否异常？
			try{
				// 是否少纸?
				if(PRINTER_HELPER->isLessPaper() || PRINTER_HELPER->isNoPaper()){
					theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NOPAPER);
				}
				else{
					// 设置状态
					theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NORMAL);
				}

				//PRINTER_HELPER->CheckStatus();

			}
			catch(CSysException& e){
				theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_ERR);
				theEXCEPTION_MGR.ProcessException(e);
				if(!bIsAllowDegrade)
					goto START_OUT_OF_SERVICE;
				else{
					if(device_print_mode == 0){
						device_print_mode = DEVICE_CONTRL_CODE_SC_NO_PRINT;
					}
				}
			}
		}
	}
	else{
		if(!bIsAllowDegrade)
			goto START_OUT_OF_SERVICE;
		else{
			if(device_print_mode == 0)
				device_print_mode = DEVICE_CONTRL_CODE_SC_NO_PRINT;
		}
	}
	// 6.判断无硬币找零时是否允许售票?(运营参数)
	if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) ==  DEVICE_CONTRL_CODE_BUSINESS_CARD){
		// 无硬币找零
		if((device_change_status & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) != DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
			if(!theAFC_TVM_OPERATION.SupportSaleWithoutCoinChange()){
				LOG("无硬币找零且运营参数限定该情况不售票。");

				// 禁用售票
				if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
					device_service_mode = device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD | DEVICE_CONTRL_CODE_BUSINESS;
				}
			}
		}
	}

	// 7. 判断当前模式是否支持服务？无产品可售，无法支付
	if(device_service_mode == DEVICE_CONTRL_CODE_BUSINESS){
		LOG("无可售产品/无法支付，暂停服务。");

		goto START_OUT_OF_SERVICE;
	}
	// 8. 没有任何支付方式
	if(device_pay_status == DEVICE_CONTRL_CODE_PAYMENT){
		LOG("没有任何支付方式，暂停服务。");

		goto START_OUT_OF_SERVICE;
	}
	if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
	{
		if ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE)
		{
			device_service_mode = (device_service_mode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
	}

	////钱票箱非法安装或非法卸载去提醒
	//if (bIsBoxError)
	//{
	//	PlayEventAudio(1007,true);
	//}

	LOG("device_pay_status = %04x, device_change_status = %04x, device_service_mode = %04x, device_print_mode = %04x",
		device_pay_status, device_change_status, device_service_mode, device_print_mode);
	//if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED)//设备连线才有意义
		theTVM_STATUS_MGR.SetServiceStatus(service_status);
	// 9.设置支付状态
	theTVM_STATUS_MGR.SetPaymentMode(device_pay_status);
	// 10.设置找零状态
	theTVM_STATUS_MGR.SetChangeMode(device_change_status);
	// 11.设置打印模式
	theTVM_STATUS_MGR.SetPrintMode(device_print_mode);
	// 12.设置业务模式
	theTVM_STATUS_MGR.SetWorkMode(device_service_mode);

	// 显示屏状态为正常（无法判断是否正常）
	theTVM_STATUS_MGR.SetTouchDisplayStatus(TOUCH_SCREEN_STATUS::TOUCH_SCREEN_STATUS_NORMAL);

	// 13.设置功能菜单
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);	
	// 14.启动默认业务：发票业务
	if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) ==  DEVICE_CONTRL_CODE_BUSINESS_CARD){
		theSERVICE_MGR.StartService(ISSUE_SVC);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);

	}
	else if ((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) ==  DEVICE_CONTRL_CODE_BUSINESS_CHARGE)
	{
		theSERVICE_MGR.StartService(CHARGE_SVC);
	}
	// 15.设置状态
	if (theSERVICE_MGR.GetState() != MAINTENANCE) {
		theSERVICE_MGR.SetState(READY);
	}
	//theSERVICE_MGR.SetForeServiceBusy(false);
	LOG("End Start Default Service");
	m_bIsStartingDefaultServic = false;
	//NOTIFY_PROCESSING_WINDOW_CLOSE();
	return;
START_OUT_OF_SERVICE:
	StartOutOfService();
ERROR_RETURN:
	m_bIsStartingDefaultServic = false;
	NOTIFY_PROCESSING_WINDOW_CLOSE();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检测上位下发命令状态，是否允许继续启动默认业务

@param      无

@retval     bool: true(继续启动默认业务)  false(不启动默认业务)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMainSvc::CheckSvcStatus(){
	bool bIsReturn = false;
	if(m_bIsSvcMsgComeSystemBusy){
		theSERVICE_MGR.SetState(READY);//20180129,lichao:解决业务中收到模式后，业务完成返回时流程中断，卡在那里。
		PostMessage(SM_MAIN_DEVICE_CONTROL,m_device_contrl_code,NULL);
	}
	else if(m_bIsOperaArryBusy){
		theSERVICE_MGR.SetState(READY);//20180129,lichao:解决业务中收到模式后，业务完成返回时流程中断，卡在那里。
		PostMessage(SM_MAIN_NEW_OPERATION_MODE,m_operational_mode,NULL);
	}
	else{
		bIsReturn = true;
	}
	return bIsReturn;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       释放Controller

@param       无

@retval      无

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::ReleaseController()
{
	try {
		theSERVICE_MGR.StopService(SC_SVC,false);
		theSERVICE_MGR.StopService(BUSINESS_INTERVAL_SVC,false);
		theSERVICE_MGR.StopService(EVENT_MSG_SVC,false);
		theSERVICE_MGR.StopService(TRANSMISSION_INTERVAL_SVC,false);
		theSERVICE_MGR.StopService(CONFIRM_ALIVE_SVC,false);

		// 释放FTP控制器
		if (g_pFTPControl != NULL) {
			g_pFTPControl->Shutdown();
			delete g_pFTPControl;
			g_pFTPControl = NULL;
		}

		// 释放数据库控制器
		if(g_pDataControl!=NULL)	{
			g_pDataControl->Shutdown();
			delete g_pDataControl;
			g_pDataControl = NULL;
		}

		// 释放SC通信控制器
		if (g_pSCControl != NULL) {
			g_pSCControl->Shutdown();
			delete g_pSCControl;
			g_pSCControl = NULL;
		}

		// delete RW control
		//if (g_pChargeRWControl != NULL) {
		//	g_pChargeRWControl->Shutdown();
		//	delete g_pChargeRWControl;
		//	g_pChargeRWControl = NULL;
		//}

		//// delete RW control
		//if (g_pCardRWControl != NULL) {
		//	g_pCardRWControl->Shutdown();
		//	delete g_pCardRWControl;
		//	g_pCardRWControl = NULL;
		//}

		//// delete RW control
		//if (g_pTokenRWControl != NULL) {
		//	g_pTokenRWControl->Shutdown();
		//	delete g_pTokenRWControl;
		//	g_pTokenRWControl = NULL;
		//}

		// 释放卡式出票机控制器
		if (g_pMIMControl != NULL) {
			g_pMIMControl->Shutdown();
			delete g_pMIMControl;
			g_pMIMControl = NULL;
		}
		// 释放卡式出票机内存管理控制器
		/*if (g_pMemControl != NULL) {
		delete []g_pMemControl;
		g_pMemControl = NULL;
		}*/

		// 释放卡式出票机内存管理控制器
		if (g_pTVMRWControl != NULL) {
			g_pTVMRWControl->Shutdown();
			delete g_pTVMRWControl;
			g_pTVMRWControl = NULL;
		}

		// 释放打印机控制器
		if (g_pPrinterControl != NULL) {
			g_pPrinterControl->Shutdown();
			delete g_pPrinterControl;
			g_pPrinterControl = NULL;
		}

		// 释放硬币模块控制器
		if (g_pCHControl != NULL) {
			g_pCHControl->Shutdown();
			delete g_pCHControl;
			g_pCHControl = NULL;
		}

		// 释放纸币模块控制器
		if (g_pBHControl != NULL) {
			g_pBHControl->Shutdown();
			delete g_pBHControl;
			g_pBHControl = NULL;
		}

		// 释放纸币找零模块控制器
		if(NULL != g_pBHChangeControl){
			g_pBHChangeControl->Shutdown();
			delete g_pBHChangeControl;
			g_pBHChangeControl = NULL;
		}

		//// 释放IO控制器
		//if (g_pIOControl != NULL) {
		//	g_pIOControl->Shutdown();
		//	delete g_pIOControl;
		//	g_pIOControl = NULL;
		//}


		// 创建状态显示屏模块控制器
		if (g_pStatusDisplayControl != NULL) {
			g_pStatusDisplayControl->Shutdown();
			delete g_pStatusDisplayControl;
			g_pStatusDisplayControl = NULL;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      开始运营

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::StartOperation(){

	if(theTVM_STATUS_MGR.GetServiceStatus() == OUTOF_SERVICE){
		if(!m_bIsReadyStartOperation){
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
		}
	}
	// 停止服务
	else if(theTVM_STATUS_MGR.GetServiceStatus() ==  STOP_SERVICE){
		m_bIsServerMsgArrial = true;
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessageW(SC_MSG_STARTOPERATION,NULL,NULL);
	}
	else if(theTVM_STATUS_MGR.GetServiceStatus() == IN_SERVICE){
		//theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         业务结束

@param      无

@retval        无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::CloseOperation(void)
{
	CString comment(_T("CloseOperation"));
	// 重启--操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BALANCE_PROCESSING,true,comment);
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theTVM_STATUS_MGR.SetRunStatus(CLOSEING);
	PostMessage(SM_MAIN_CLOSE_OPERATION,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			开始服务

@param       无

@retval			无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::ResumeService()
{
	if(theTVM_SETTING.IsSCServiceOutOfService()){
		// 设置正常服务
		theTVM_SETTING.SetSCServiceStatus(0);
		StartDefaultService();
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			暂停服务

@param       无

@retval			无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::PauseService()
{
	// 1.如果是暂停服务，判断上位设置服务状态，设置服务状态为暂停服务
	if(theTVM_STATUS_MGR.GetServiceStatus() == OUTOF_SERVICE){
		// 1.1判断是否是暂停服务？如果不是，设置为是暂停服务模式
		if(!theTVM_SETTING.IsSCServiceOutOfService()){
			theTVM_SETTING.SetSCServiceStatus(1);
		}
	}
	// 2.如果是正在服务状态
	else if(theTVM_STATUS_MGR.GetServiceStatus() == IN_SERVICE || WARN_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
		// 2.1theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
		theTVM_SETTING.SetSCServiceStatus(1);
		// 2.2如果是等待或者休息状态，直接启用默认服务，切换为暂停服务
		if (READY == theSERVICE_MGR.GetState() || REST == theSERVICE_MGR.GetState() || FINISH == theSERVICE_MGR.GetState()){
			StartDefaultService();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			重启TVM

@param      无

@retval			无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::RestartTVM()
{
	CString comment(_T("RestartTVM"));
	// 重启--操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RESTART,true,comment);
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
	theTVM_STATUS_MGR.SetRunStatus(START_RUNING);
	//OnShutDown(NULL,NULL);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief			关机

@param       无

@retval			无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::Shutdown(void)
{
	CString comment(_T("Shutdown"));
	// 关机--操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_SHUTDOWN,true,comment);
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			退出TVM程序

@param		 无

@retval			无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::ExitTVM()
{
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	CLogoffHelper logoffHelper(*this);
	logoffHelper.Logoff();
	theAPP_SESSION.SetCloseStartFlag(CS_EXIT);
	theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		设置降级运行模式  

@param      bool : true(开启) false(关闭)

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::SetDegradedMode(bool bOnOrOff){
	theTVM_SETTING.SetDeradedMode(bOnOrOff ? 1 : 0 );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      强制登出

@param      WPARAM wParam 强制登出原因

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnForceLogout(WPARAM wParam,LPARAM lParam)
{	
	CString forceLogoutMessage = _T("");
	switch(wParam)
	{
	case 	FORCE_LOGOUT_SERVER_REQUEST:
		forceLogoutMessage = _tl(TXT_FORCE_LOGOUT_SERVER_REQUEST);
		break;
	case FORCE_LOGOUT_DEVICE_CONTROL:
		forceLogoutMessage = _tl(TXT_FORCE_LOGOUT_DEVICE_CONTROL);
		break;
	case FORCE_LOGOUT_EMERGENCY_MODE:
		forceLogoutMessage = _tl(TXT_FORCE_LOGOUT_EMERGENCY_MODE);
		break;
	case FORCE_LOGOUT_NEWPARAMETER:
		forceLogoutMessage = _tl(TXT_FORCE_LOGOUT_NEWPARAMETER);
		break;
	case FORCE_LOGOUT_DEVICE_CONTROL_SHUTDOWN:
		forceLogoutMessage = _tl(TXT_UPSPOWEROFF_SHUTDOWN);
		break;
	default:
		forceLogoutMessage = TXT_NOTHING;
	}
	if(wParam == FORCE_LOGOUT_DEVICE_CONTROL_SHUTDOWN&&theTVM_STATUS_MGR.GetLoginStatus() != LOGIN_TYPE::LOGIN_OFF){
		CMessageBox::Show(theAPP_SESSION.GetServiceName(SHUTDOWN_SVC),forceLogoutMessage,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	}	
	if(theTVM_STATUS_MGR.GetLoginStatus() == LOGIN_TYPE::LOGIN_ON){
		CMessageBox::Show(_tl(TXT_FORCE_LOGOUT),forceLogoutMessage	,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
		CLogoffHelper logoffHelper(*this);
		logoffHelper.Logoff();		
	}	
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      与上位连接状态改变

@param      WPARAM wParam 无意义

@param      LPARAM lParam 当前状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnNetStatusChange(WPARAM wParam,LPARAM lParam)
{
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,lParam == SC_CONNECTION_ON ? _opl(TXT_ID_STATUES_CONNECTED):_opl(TXT_ID_STATUES_DISCONNECTED));
	theAPP_SESSION.GetChildView()->m_pStatusDlg->SetNetWorkStatus(lParam == SC_CONNECTION_ON ? TXT_ID_STATUES_CONNECTED:TXT_ID_STATUES_DISCONNECTED);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启NTP计时器

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartNtpTimer(WPARAM wParam,LPARAM lParam)
{
	CAfcDeviceCommonParam::COMMUNICATION comunication;
	theAFC_COMMON.GetCommunication(comunication);
	WORD interval = comunication.ntpInterval==0?30:comunication.ntpInterval;
	StartTimer(TIMER_NTP,interval * 60);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartSwithBusinessDayTimer(WPARAM wParam,LPARAM lParam)
{
	m_CurrentBusiDate = ComGetBusiDate();
	StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      工作模式变更响应函数

@param   WPARAM wParam   
@param   LPARAM lParam 

@retval     LRESULT

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnNewWorkMode(WPARAM wParam, LPARAM lParam)
{
	theMENU_MGR.SetWorkMode((BYTE)wParam);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnNewOperationMode(WPARAM wParam,LPARAM lParam)
{
	OperationalMode_t mode = (OperationalMode_t)wParam;
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("<<%02x"),mode);
	// 乘客忙碌，暂时不对应
	if(IsServiceBusyOnPassger()){
		m_bIsOperaArryBusy = true;
		m_operational_mode = mode;
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(">>"));
		return FALSE;
	}
	else{
		m_bIsOperaArryBusy = false;
		m_operational_mode = OperationalMode_t::NORMAL_SERVICE_MODE;
	}
	switch(mode){
		// 正常
	case NORMAL_SERVICE_MODE:
		{
			StartDefaultService();
		}
		break;
		// 关闭模式
		/*case DEVICE_MODE_CLOSED:
		{
		StartOutOfService();
		}
		break;*/
		// 列车故障模式
	case TRAIN_SERVICE_DISRUPTION_MODE:
		// 紧急模式
	case EMERGENCY_MODE:
		{
			StartOutOfService();
		}
		break;
	default:
		{
			StartDefaultService();

			theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("设备不对应此运营模式"));
		}
		break;
	}
	//theMENU_MGR.SetWorkMode((OperationalMode_t)wParam);
	//CString systemMode = _T("--");
	// 	if (wParam == DEVICE_MODE_NORMAL) {      // 正常服务模式
	// 		systemMode = TXT_STATUES_NORMAL_MODE;
	// 	}
	//else if (wParam == DEVICE_MODE_EMERGENCY_EXIT) {       // 紧急模式
	//	systemMode = TXT_STATUES_EMENGERCY_MODE;
	//}
	// 	else {
	// 		systemMode = TXT_STATUES_DEGRADE_MODE;                       //降級模式
	// 	}
	//theAPP_SESSION.GetChildView()->m_pStatusDlg->SetOperatingMode(systemMode);
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(">>"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      运行控制命令响应函数

@param   WPARAM wParam   
@param   LPARAM lParam 

@retval     LRESULT

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnDeviceControl(WPARAM wParam,LPARAM lParam)
{
	DEVICE_CONTRL_CODE controlCode = (DEVICE_CONTRL_CODE)wParam;
	if(IsServiceBusyOnPassger()){// 乘客正在操作，此时不响应命令，待操作完后再去响应
		m_bIsSvcMsgComeSystemBusy = true;
		m_device_contrl_code = controlCode;
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(">>"));
		return FALSE;
	}
	else{
		m_bIsSvcMsgComeSystemBusy = false;
		m_device_contrl_code = DEVICE_CONTRL_CODE_SC_UNKNOW;
	}
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("<<%02x"),controlCode);
	switch(controlCode)
	{
		// 电源关闭
	case DEVICE_CONTRL_CODE_SC_POWER_OFF:
		Shutdown();
		break;
		// 设备重启
	case DEVICE_CONTRL_CODE_SC_RESTART_TVM:
		RestartTVM();
		break;
		// 软件重启
	case DEVICE_CONTRL_CODE_SC_RESTART_SOFTWARE:
		ExitTVM();
		break;
		// 睡眠模式
	case DEVICE_CONTRL_CODE_SC_SLEEP:
		Shutdown();
		break;
		// 远程唤醒
	case DEVICE_CONTRL_CODE_SC_WAKEUP:	
		// 无处理
		break;
		// 运营开始
	case DEVICE_CONTRL_CODE_SC_OPERATION_START:		
		StartOperation();
		if(m_bPuseCheckOperation)
		{
			theSERVICE_MGR.SetForeServiceBusy(false);
			m_bPuseCheckOperation = false;
			theTVM_STATUS_MGR.SetRunStatus(RUN_STATUS::RUNING);
		}
		break;
		// 运营结束
	case DEVICE_CONTRL_CODE_SC_OPERATION_END:		
		CloseOperation();
		break;
		// 开始服务
	case DEVICE_CONTRL_CODE_SC_START_SERVICE:
		ResumeService();
		if(m_bPuseCheckOperation)
		{
			theSERVICE_MGR.SetForeServiceBusy(false);
			m_bPuseCheckOperation = false;
			theTVM_STATUS_MGR.SetRunStatus(RUNING);
		}
		break;
		// 暂停服务
	case DEVICE_CONTRL_CODE_SC_OUT_OF_SERVICE:
		PauseService();
		break;
		// 降级运行开
	case DEVICE_CONTRL_CODE_SC_DE_OPER_ON:
		SetDegradedMode(true);
		break;
		// 降级运行关
	case DEVICE_CONTRL_CODE_SC_DE_OPER_OFF:
		SetDegradedMode(false);
		break;
		// 恢复正常模式
	case DEVICE_CONTRL_CODE_SC_RECOVER_NORMAL_MODE:
		{
			// 服务模式
			//theTVM_STATUS_MGR.SetWorkMode(DEVICE_CONTRL_CODE_BUSINESS_ALL);	
			theTVM_SETTING.SetServiceMode(DEVICE_CONTRL_CODE_BUSINESS_ALL);
			theTVM_SETTING.SetSCServiceStatus(0);
			// 支付模式
			//theTVM_STATUS_MGR.SetPaymentMode(DEVICE_CONTRL_CODE_PAYMENT_ALL);
			theTVM_SETTING.SetPayMentMode(DEVICE_CONTRL_CODE_PAYMENT_ALL);
			// 找零模式
			//theTVM_STATUS_MGR.SetChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE);
			theTVM_SETTING.SetChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE);
			// 打印模式
			//theTVM_STATUS_MGR.SetPrintMode(0);
			theTVM_SETTING.SetPrintMode(0);
			if(!m_bIsStartingDefaultServic){
				StartDefaultService();
			}
		}
		break;
	default:
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(""));
		break;
	}
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(">>"));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计时器响应

@param      UINT nEventID 计时器ID


@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnTimer(UINT nEventID)
{
	// 1.NTP对时计时器响应
	if(nEventID == TIMER_NTP)	{
		try{
			// 没在关机中
			if(theTVM_STATUS_MGR.GetRunStatus() != SHUTDOWNING){
				// 无NTP异常时执行NTP对时。
				if (!theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID)){
					theSNTP_HELPER.TimeSynchronize();
				}
			}
		}
		catch (CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
	}
	// 2.启动默认业务定时器响应
	else if(nEventID == TIMER_DEFAULT_SERVICE){
		if (theSERVICE_MGR.GetState()!=FINISH){
			if(theAPP_SESSION.IsBlindManOperation()){
				if(theSERVICE_MGR.GetState() == FINISH){
					StopTimer(TIMER_DEFAULT_SERVICE);
					if(!m_bIsStartingDefaultServic)
						StartDefaultService();
				}
			}
			else{
				StopTimer(TIMER_DEFAULT_SERVICE);
				//if(RECEPT != theSERVICE_MGR.GetState())
				//	hideAllCompoment.Invoke();
				if(!m_bIsStartingDefaultServic)
					StartDefaultService();	
			}	
		}
		else{
			if (--m_timerInterval<0){
				StopTimer(TIMER_DEFAULT_SERVICE);
				//if(RECEPT != theSERVICE_MGR.GetState())
				//	hideAllCompoment.Invoke();
				if(!m_bIsStartingDefaultServic)
					StartDefaultService();	
			}
			else{
				if(theSERVICE_MGR.GetCurService()->GetServiceID()==ISSUE_SVC){
					CString strShowTxt;
					strShowTxt.Format(_tl(TXT_COUNT_DOWN) + _T("%d"), m_timerInterval);
					((CIssueSvc*)theSERVICE_MGR.GetCurService())->UpdateReceptDlg(strShowTxt);
				}
				else if(theSERVICE_MGR.GetCurService()->GetServiceID()==CHARGE_SVC){
					CString strShowTxt;
					strShowTxt.Format(_tl(TXT_COUNT_DOWN) + _T("%d"), m_timerInterval);
					((CChargeSvc*)theSERVICE_MGR.GetCurService())->UpdateReceptDlg(strShowTxt);
				}
			}
		}

	}
	// 3.自动登出定时器
	else if(nEventID == TIMER_AUTOMATIC_LOGOFF){
		LOGIN_STATUS login = theTVM_STATUS_MGR.GetLoginStatus();
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		// 维护中并且已登录状态则自动登出且不为忙碌状态
		if(serviceState == MAINTENANCE && login == LOGIN&&!theSERVICE_MGR.IsForeServiceBusy()) {
			// 停止定时器
			StopTimer(TIMER_AUTOMATIC_LOGOFF);
			// 登出
			if (theAPP_SESSION.GetMaintainMenu() != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC)
			{
				CLogoffHelper logoffHelper(*(theSERVICE_MGR.GetCurService()));
				logoffHelper.Logoff();	
			}
			//theAPP_SESSION.ShowStatusWaringMsg(_T("警告：登录超时，自动登出！"));
		}
		else{
			// do nothing
		}
	}
	// 4.运营日切换定时器响应
	if(nEventID == TIMER_BUSIDAY_SWITH){

		// 是否暂停运营切换检查
		if(m_bPuseCheckOperation){
			return;
		}
		// 读取当前时间
		CAfcDeviceTimeParam::OPRATION_TIME operationTime;
		theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
		_DATE_TIME dtCurDate = ComGetCurTime();
		_TIME CurTime;
		CurTime.bHour = dtCurDate.biHour;
		CurTime.bMin  = dtCurDate.biMinute;
		CurTime.bSec  = dtCurDate.biSecond;

		long lCurTime = ((long)CurTime.bHour)*60*60 + ((long)CurTime.bMin)*60 +(long)CurTime.bSec;
		long lBeginTime = ((long)operationTime.oprationBeginTime.bHour)*60*60 + ((long)operationTime.oprationBeginTime.bMin)*60 +(long)operationTime.oprationBeginTime.bSec;
		long lEndTime   = ((long)operationTime.oprationEndTime.bHour)*60*60 + ((long)operationTime.oprationEndTime.bMin)*60 +(long)operationTime.oprationEndTime.bSec;
		if (lEndTime < lBeginTime){//跨天运营
			if (lCurTime<lEndTime){
				lCurTime = ((long)CurTime.bHour+24)*60*60 + ((long)CurTime.bMin)*60 +(long)CurTime.bSec;
			}
			lEndTime = ((long)operationTime.oprationEndTime.bHour+24)*60*60 + ((long)operationTime.oprationEndTime.bMin)*60 +(long)operationTime.oprationEndTime.bSec;
		}

		// 当前状态
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		if(INIT == serviceState){
			return;
		}

		// 业务结束
		if(CLOSEING == theTVM_STATUS_MGR.GetRunStatus()){
			return ;
		}

		// 判断上位
		if(m_bIsServerMsgArrial){
			if(lCurTime < lBeginTime){// 运营时间没有到，发送了开始运营
				m_bBeforeRunCommandArrial = true;
				LOG("运营时间没有到，发送了开始运营");
			}			
			else if(lEndTime < lCurTime){// 运营时间过去，发送运营开始
				m_bEndRuningCommandArrial = true;
				LOG("运营时间过去，发送运营开始");
			}
			LOG("[TIMER_BUSIDAY_SWITH] SetStartOperation TRUE!");		
			theTVM_SETTING.SetStartOperation(true);// 设置为运营开始
			m_bIsStartOperation = true;
			m_bIsServerMsgArrial = false;
		}

		// 判断是否是跨运营日强制时钟同步
		if(m_bIsSwitchBussinessDay){
			LOG("跨运营日强制时钟同步");
			OnSwitchBusinessDayAndCloseOperation(NULL,NULL);
			return;
		}

		// 判断当前是否已经走运营开始时序？
		// ---------------------------------------已走过运营时序
		if(m_bIsReadyStartOperation){

			// 运营时间范围内
			if((lCurTime >= lBeginTime) && (lCurTime < lEndTime)){
				// 清空上位下发状态
				if(m_bIsStartOperation){
					// 停止服务，怎么可能？临界值的时候会出现，需要判断并设置正常服务
					if(STOP_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
						// 运营时间内
						LOG("[TIMER_BUSIDAY_SWITH] Post SC_MSG_STARTOPERATION to SC_SVC!");
						theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
						if(!m_bIsStartingDefaultServic)	StartDefaultService();
					}
					LOG("[TIMER_BUSIDAY_SWITH] SetStartOperation FALSE!");
					theTVM_SETTING.SetStartOperation(false);
					m_bIsStartOperation = false;
					m_bBeforeRunCommandArrial = false;
				}
			}
			// 运营时间范围外
			else{
				// 24小时
				if(OPERATION_24HOURS_ON == theTVM_STATUS_MGR.Get24HourMode()){
					// 判断运营日切换
					if(m_CurrentBusiDate != ComGetBusiDate()){
						if(!IsServiceBusyOnPassger()){
							if(STOP_SERVICE != theTVM_STATUS_MGR.GetServiceStatus()){
								StartOutOfService();
							}
							// 处理时显示系统正在处理中...
							NOTIFY_PROCESSING_WINDOW();
							LOG("[TIMER_BUSIDAY_SWITH] Switch Bussiness Day when 24HourMode !");
							// 停止计时器
							StopTimer(TIMER_DEFAULT_SERVICE);	// 默认业务定时器
							StopTimer(TIMER_AUTOMATIC_LOGOFF);	// 自动登出计时器
							StopTimer(TIMER_BUSIDAY_SWITH);		// 运营日切换定时器
							StopTimer(TIMER_IO_MONITOR);		// IO监控定时器
							StopTimer(TIMER_READY_TO_REST);		// 等待中到待机中计时器

							// 执行运营日切换
							OnSwitchBussinessDay();


							// 恢复计时器
							StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
							OnStartIOMonitor(NULL,NULL);

							// 启动默认服务
							if(!m_bIsStartingDefaultServic)StartDefaultService();
						}
					}
					return ;
				}
				// 延长运营
				long lDelay = theTVM_STATUS_MGR.GetDelayTime();
				if(0 != lDelay){
					// 过了24点以后
					if(lCurTime < lBeginTime && lCurTime < lEndTime){
						if((lCurTime + 24*60*60) < (lEndTime + lDelay * 60)){
							return ;
						}
						else if((lCurTime + 24*60*60) == (lEndTime + lDelay * 60)){
							m_bIsEndRunning = true;
						}
					}
					// 延长后可以继续运行
					else if(lCurTime < (lEndTime + lDelay * 60)){
						return ;
					}
					else if(lCurTime == (lEndTime + lDelay * 60)){// 延长运营后需要做业结
						m_bIsEndRunning = true;
					}
				}

				// 非上位下发运营开始
				if(!(m_bBeforeRunCommandArrial || m_bEndRuningCommandArrial || m_bIsStartOperation)){
					// 自动运营结束？
					if(lCurTime == lEndTime || m_bIsEndRunning){
						LOG("[TIMER_BUSIDAY_SWITH] EndOperation!");
						// 此时有乘客操作，暂缓业务结束
						if(IsServiceBusyOnPassger()){
							m_bIsEndRunning = true;
							return;
						}
						else{
							theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
							StartOutOfService();
							// 过了24点以后
							if(lCurTime < lBeginTime && lCurTime < lEndTime){
								m_bIsEndRunning = false;
								// 非人工业务结束
								theAPP_SESSION.SetManualCloseOperation(false);
								CloseOperation();
								m_bIsReadyStartOperation = false;
							}
							else if(lCurTime >= lEndTime){
								m_bIsEndRunning = false;
								// 非人工业务结束
								theAPP_SESSION.SetManualCloseOperation(false);
								CloseOperation();
								m_bIsReadyStartOperation = false;
							}
							return;
						}
					}
				}
				// 需要判断设备状态
				else{
					// 清空上位下发状态
					if(m_bIsStartOperation){
						// 停止服务，跨运营日了，需要判断并设置正常服务
						if(STOP_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
							// 运营时间内
							LOG("[TIMER_BUSIDAY_SWITH] Post SC_MSG_STARTOPERATION to SC_SVC!");
							theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
							if(!m_bIsStartingDefaultServic)	StartDefaultService();
						}
						LOG("[TIMER_BUSIDAY_SWITH] SetStartOperation FALSE!");
						theTVM_SETTING.SetStartOperation(false);
						m_bIsStartOperation = false;
						m_bBeforeRunCommandArrial = false;
					}
				}
			}
		}
		else{
			// ---------------------------------------未走过运营时序
			// 停止服务状态
			if(STOP_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
				// 运营时间内
				if((lCurTime >= lBeginTime) && (lCurTime < lEndTime) ){
					m_bIsReadyStartOperation = true;
					LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
					theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
					theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);//lichao-20171025(修复断网不关机，运营日切换后，当时间到运营时间内，一直处于停止服务的问题。）
					if(!m_bIsStartingDefaultServic)	StartDefaultService();
				}
				else{
					// 上位下发命令
					if(m_bIsStartOperation && theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
						m_bIsReadyStartOperation = true;
						LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
						theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
						if(!m_bIsStartingDefaultServic)	StartDefaultService();
					}
					else {
						if(READY == theSERVICE_MGR.GetState()){
							LOG("[TIMER_BUSIDAY_SWITH] StartDefaultService!");

							if(!m_bIsStartingDefaultServic)	{StartDefaultService();m_bIsStartingDefaultServic= true;}//本地调试m_bIsStartingDefaultServic= true，正式版本放开
						}
					}
				}
			}
			// 正常服务状态
			else if(IN_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
				// Do nothing
			}
			// 暂停服务状态
			else if(OUTOF_SERVICE == theTVM_STATUS_MGR.GetServiceStatus() || MANT_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){

				// 运营中，直接返回（中途关开机）
				if((lCurTime > lBeginTime) && (lCurTime < lEndTime)){
					if(!m_bIsStartOperationWhenOpen && theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
						m_bIsStartOperationWhenOpen = true;
						LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
						theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
						m_bIsReadyStartOperation = true;
						if(!m_bIsStartingDefaultServic && !IsServiceBusyOnPassger()) StartDefaultService();
					}
				}
				// 运营结束，或未开始（门开着的状态）
				else/*if(((lCurTime > lEndTime) && (lCurTime > lEndTime))||(( lCurTime < lEndTime) && (lCurTime < lBeginTime)))*/{
					// 24小时
					if(OPERATION_24HOURS_ON == theTVM_STATUS_MGR.Get24HourMode()){
						// 启动运营开始时序
						if(!m_bIsReadyStartOperation && theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
							m_bIsStartOperationWhenOpen = true;
							LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
							theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
							m_bIsReadyStartOperation = true;
							if(!m_bIsStartingDefaultServic && !IsServiceBusyOnPassger()) StartDefaultService();
						}
						return ;
					}
					// 延长运营(武汉没有
					long lDelay = theTVM_STATUS_MGR.GetDelayTime();
					if(0 != lDelay){
						// 过了24点以后
						if(lCurTime < lBeginTime && lCurTime < lEndTime){
							if((lCurTime + 24*60*60) < (lEndTime + lDelay * 60)){
								// 启动运营开始时序
								if(!m_bIsReadyStartOperation && theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
									m_bIsStartOperationWhenOpen = true;
									LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
									theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
									m_bIsReadyStartOperation = true;
									if(!m_bIsStartingDefaultServic && !IsServiceBusyOnPassger()) StartDefaultService();
								}
								return ;
							}
							else if((lCurTime + 24*60*60) == (lEndTime + lDelay * 60)){
								m_bIsEndRunning = true;
							}
						}
						// 延长后可以继续运行
						else if(lCurTime < (lEndTime + lDelay * 60)){
							// 启动运营开始时序
							if(!m_bIsReadyStartOperation && theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
								m_bIsStartOperationWhenOpen = true;
								LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
								theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
								m_bIsReadyStartOperation = true;
								if(!m_bIsStartingDefaultServic && !IsServiceBusyOnPassger()) StartDefaultService();
							}
							return ;
						}
						else if(lCurTime == (lEndTime + lDelay * 60)){// 延长运营后需要做业结
							m_bIsEndRunning = true;
						}
					}

					// 非上位下发运营开始
					if(!(m_bBeforeRunCommandArrial || m_bEndRuningCommandArrial || m_bIsStartOperation)){
						// 自动运营结束？
						if(lCurTime == lEndTime || m_bIsEndRunning){
							LOG("[TIMER_BUSIDAY_SWITH] EndOperation!");
							// 此时有乘客操作，暂缓业务结束
							if(IsServiceBusyOnPassger()){
								m_bIsEndRunning = true;
								return;
							}
							else{
								theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
								StartOutOfService();
								// 过了24点以后
								if(lCurTime < lBeginTime && lCurTime < lEndTime){
									m_bIsEndRunning = false;
									// 非人工业务结束
									theAPP_SESSION.SetManualCloseOperation(false);
									LOG("[TIMER_BUSIDAY_SWITH] Do CloseOperation!");
									CloseOperation();
									m_bIsReadyStartOperation = false;
								}
								else if(lCurTime >= lEndTime){
									m_bIsEndRunning = false;
									// 非人工业务结束
									theAPP_SESSION.SetManualCloseOperation(false);
									LOG("[TIMER_BUSIDAY_SWITH] Do CloseOperation!");
									CloseOperation();
									m_bIsReadyStartOperation = false;
								}
								return;
							}
						}
						else {
							LOG("[TIMER_BUSIDAY_SWITH] SetServiceStatus - stop_service");
							theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
						}
					}
					else{
						if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
							LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
							theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
							m_bIsReadyStartOperation = true;
						}
					}
				}
				//return;// 运营日切换判断
			}
			// 维护状态
			else if(MANT_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
				if((lCurTime >= lBeginTime) && (lCurTime < lEndTime)){
					if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
						LOG("[TIMER_BUSIDAY_SWITH] POST SC_MSG_STARTOPERATION to SC_SVC!");
						theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
						m_bIsReadyStartOperation = true;
					}
				}
				else{

				}
				// 非上位下发运营开始
				if(!(m_bBeforeRunCommandArrial || m_bEndRuningCommandArrial || m_bIsStartOperation)){
					LOG("[TIMER_BUSIDAY_SWITH] SetServiceStatus - stop_service");
					theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
				}
			}
			// 其他
			else{

			}
		}

		// 运营日没有切换，不处理
		if(m_CurrentBusiDate == ComGetBusiDate()){
			return;
		}		


		// 运营日切换处理	
		LOG("[TIMER_BUSIDAY_SWITH] ready to Switch BusiDay");
		m_bEndRuningCommandArrial = false;
		// 设备等待中，待机中，结束，异常，等待维护时处理
		if(!(serviceState == READY || serviceState == ABNORMAL || serviceState == REST || serviceState == FINISH ||  serviceState == WAIT_MAINTENANCE)) {
			LOG("[TIMER_BUSIDAY_SWITH] leave!");
			return;
		}

		LOG("[TIMER_BUSIDAY_SWITH] do Switch BusiDay");
		// 停止计时器
		StopTimer(TIMER_DEFAULT_SERVICE);	// 默认业务定时器
		StopTimer(TIMER_AUTOMATIC_LOGOFF);	// 自动登出计时器
		StopTimer(TIMER_BUSIDAY_SWITH);		// 运营日切换定时器
		StopTimer(TIMER_IO_MONITOR);		// IO监控定时器
		StopTimer(TIMER_READY_TO_REST);		// 等待中到待机中计时器

		// 执行运营日切换
		OnSwitchBussinessDay();


		// 恢复计时器
		StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
		OnStartIOMonitor(NULL,NULL);

		//****************************The follow not need ********************************************/
		// 运营日切换，设置停止服务状态！
		theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
		StartOutOfService();
		// 运营日切换，则提示并登出（等待和结束状态）
		if (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY
			&& (serviceState !=INIT && serviceState!=OUT_OF)) {		
				if (serviceState ==REST){
					busidaySwitch.Invoke();
				}
				Sleep(100);
				// 登出
				CLogoffHelper logoffHelper(*(theSERVICE_MGR.GetCurService()));
				logoffHelper.Logoff();
		}
		// SC审计数据 mzy
		theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
		try{
			CLogHelper::UploadLogFile(m_CurrentBusiDate);//将运营结束切换前的日志发送给服务器。
		}
		catch(CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
		}
		m_CurrentBusiDate = ComGetBusiDate();
		//清空密码锁定
		thePWD_ERR.Internalize(theTVM_INFO.GetDataPath(),theTVM_INFO.GetBackupPath());
		// 再次启动计时器
		StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
		m_bIsBeginRunning = false;
		m_bIsEndRunning   = false;
	}	
	// 5.执行IO的计时器
	if(nEventID == TIMER_IO_MONITOR){
#if 0

		try{
			// 确定IO连接，且没有异常
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				CIOMonitorSvc* psvr = (CIOMonitorSvc*)(theSERVICE_MGR.GetService(SERVICE_ID::MONITOR_IO_SVC));
				m_IOStatusRsp = psvr->ioStatus;
				//GetIOModuleHelper()->GetIOStatus(m_IOStatusRsp);
				// 招援业务、维护门打开、关闭乘客业务和维护业务转换 -- 放开注释即可
				//if(theAFC_TVM_OPERATION.IsEnableAssistanceBtn())
				//	PostAssistanceButtonActionMSG();	// 发送招援按钮动作消息
				PostDoorActionMSG();				// 发送维护门动作消息
				FeelSomeCloseDevice();				// 距离传感器响应函数
				WhenShockActive();					// 震动传感器响应
				JudgeIsOperatorLogin();				// 站员是否登陆:门开
			}
			// 否则需要停止计时器
			else{
				try{
					if(theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
						theEXCEPTION_MGR.RemoveException(CIOException::MODULE_ID);
						GetIOModuleHelper()->RepareIO();
					}
				}
				catch(CSysException& e){
					StopTimer(TIMER_IO_MONITOR);	// IO监控
					//StopTimer(TIMER_READY_TO_REST);	// 停止到等待中
					theEXCEPTION_MGR.ProcessException(e);
				}				
			}
		}
		catch (CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
#endif

	}
	// 6.执行找零指示灯计时器
	if(nEventID == TIMER_RETURN_INDICATOR){
		// 关闭此计时器，一次只执行一次
		StopTimer(TIMER_RETURN_INDICATOR);
		try{
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				// 关闭找零指示灯
				GetIOModuleHelper()->CloseReturnCupIndicator();
			}
		}
		catch (CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
	}
	// 7.执行找零照明灯计时器
	if (nEventID == TIMER_RETURN_LIGHT){
		// 关闭此计时器，一次只执行一次
		StopTimer(TIMER_RETURN_LIGHT);
		try{
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				// 关闭照明灯
				GetIOModuleHelper()->CloseReturnLight();
				GetIOModuleHelper()->CloseReturnCupIndicator();
			}
		}
		catch (CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
	}
	// 8.执行等待中到待机中计时器
	if (nEventID == TIMER_READY_TO_REST){
		StopTimer(TIMER_READY_TO_REST);
		if(!m_IOStatusRsp.isBackDoorOpen && !m_IOStatusRsp.isFrontDoorOpen){
			theSERVICE_MGR.SetState(REST);
			// 启动待机服务
			theSERVICE_MGR.StartService(STANDBY_SVC);
		}
	}
	// 9.执行未登陆超时计时器:报警器响应
	if (nEventID == TIMER_OUT_OF_TIME_LOGIN){
		// 9.1停止计时器
		StopTimer(TIMER_OUT_OF_TIME_LOGIN);
		// 9.2执行报警,如果没有登陆,如果门也开着
		if (LOGIN != theTVM_STATUS_MGR.GetLoginStatus() && (m_IOStatusRsp.isBackDoorOpen || m_IOStatusRsp.isFrontDoorOpen)){
			GetIOModuleHelper()->OpenAlarm();
			theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGETYPE_T::CHINESE);
			PlayEventAudio(1007,true);
			m_bIsAramOpen = true;
			theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_DOOR_OPEN_WITHOUT_LOGIN);
			theTVM_STATUS_MGR.SetAuthorizedAccessStatus(AUTHORIZED_STATUS::AUTHORIZED_STATUS_ABNORMAL);
			theTVM_STATUS_MGR.SetServiceStatus(SERVICE_STATUS::WARN_SERVICE);
			theTVM_STATUS_MGR.SetMaintenanceDoorStatus(DOOR_STATUS::DOOR_ILLEGLE_OPEN);
		}		
	}
	// 10.语音重复播放计时器（盲人）
	if( nEventID == TIMER_REPEAT_EVENT_AUDIO){
		StopTimer(nEventID);
		if (LOGIN != theTVM_STATUS_MGR.GetLoginStatus() && !m_IOStatusRsp.isBackDoorOpen){
			PlayEventAudio(1007,true);
		}		

		//		if(theAPP_SESSION.IsBlindManOperation())
		//			PlayEventAudio(m_dwLastPlayedVoice);
	}
	// 11.盲人购票操作超时时间
	if(nEventID == TIMER_BLIND_OPERATION_OUT_OF_TIME){
		// 		StopTimer(TIMER_REPEAT_EVENT_AUDIO);
		// 		StopTimer(nEventID);
		// 		if(theAPP_SESSION.IsBlindManOperation()){
		// 			// 提示交易因为超时而取消
		// 			PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
		// 			if(m_bIsPinPadRunning){
		// 				m_pPinPadModuleHelper->TurnOffInputTrigger();
		// 			}
		// 			else{
		// 				//m_pPinPadModuleHelper->PowerOff();
		// 				theSERVICE_MGR.SetState(READY);
		// 				theAPP_SESSION.SetBlindManStatus(false);
		// 			}
		// 		}
	}
	// 12.二维码订单本地处理（当前这里已废弃不用，放到CWebServerHelper中处理）
	if(nEventID == TIMER_WEB_LOCAL_PROCESS){
		LOG("二维码订单本地处理定时器开始");
		StopTimer(TIMER_WEB_LOCAL_PROCESS);
		if(theWEBSERVER_HELPER.Web_ConnectCheck()==RSP_CODE_OK){
			if(theWEBSERVER_HELPER.IsEnableIdleProcess()){
				theWEBSERVER_HELPER.Web_IdleProcessOrder();
			}
		}
		StartTimer(TIMER_WEB_LOCAL_PROCESS, 10);
		LOG("二维码订单本地处理定时器结束");
	}
	//// 13.本地下载参数检测生效
	//if(nEventID == TIMER_PARAMETER_UPDATE){
	//	StopTimer(TIMER_PARAMETER_UPDATE);
	//	//1) 检查是否在时序中，是否业务中，是否初始化中，是否响应上位命令中
	//	SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
	//	if(READY == serviceState || WAIT_MAINTENANCE == serviceState || REST == serviceState){
	//		//2) 检查参数升级目录下是否有需要升级的参数文件
	//		CParamHelper::VEC_VERSION_INFO VecVersionInfo;
	//		thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo,false);
	//		if (VecVersionInfo.size()>0){
	//			// 只有立即生效参数，立刻加载
	//			if(VecVersionInfo[0].paramId==AFC_MODE_RESUME_ID){
	//				thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo,true);
	//				thePARAM_HELPER.InitAllParam();
	//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
	//				try {
	//					CARDRW_HELPER->Close();
	//					CARDRW_HELPER->InitCheck();
	//					CARDRW_HELPER->CheckDeviceExchange();

	//					// 硬件参数更新
	//					CARDRW_HELPER->InitData();
	//					CARDRW_HELPER->SynchronismParam();
	//				}
	//				catch (CSysException& e) {
	//					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("读写器同步立即生效参数失败！"));	
	//					theEXCEPTION_MGR.ProcessException(e);
	//				}
	//				catch (...) {
	//					theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//				}
	//				StartTimer(TIMER_PARAMETER_UPDATE, 30);
	//				return;
	//			}
	//		}
	//		//3) 检查当前是否在配置文件指定的时间段
	//		long beginHour = theTVM_INFO.GetParameterUpdateBeginHour();
	//		long beginMin = theTVM_INFO.GetParameterUpdateBeginMin();
	//		long endHour = theTVM_INFO.GetParameterUpdateEndHour();
	//		long endMin = theTVM_INFO.GetParameterUpdateEndMin();
	//		_DATE_TIME dtCurDate = ComGetCurTime();
	//		long lBeginTime = beginHour*60*60 + beginMin*60 + 0;
	//		long lEndTime   = endHour*60*60 + endMin*60 + 0;
	//		long lCurTime = ((long)dtCurDate.biHour)*60*60 + ((long)dtCurDate.biMinute)*60 +(long)dtCurDate.biSecond;
	//		if (lCurTime>=lBeginTime && lCurTime<=lEndTime){
	//			// 若存在升级参数，则进行升级
	//			CParamHelper::VEC_VERSION_INFO VecVersionInfo;
	//			thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
	//			if (VecVersionInfo.size()>0){
	//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
	//			}
	//			// 若存在更新程序则关机更新程序
	//			bool needUpgrade = theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade();
	//			if(needUpgrade)
	//			{
	//				//前台登出
	//				//ForceForegroundLogout(FORCE_LOGOUT_NEWPARAMETER,true);
	//				// 初始化所有参数		
	//				try{
	//					thePARAM_HELPER.InitAllParam();
	//				}
	//				catch(CSysException& e){
	//					theEXCEPTION_MGR.ProcessException(e);
	//				}
	//				catch(...){
	//					theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//				}	
	//				// 解除异常(防止存在参数以及参数引起的读写器异常)
	//				theEXCEPTION_MGR.RepareException();
	//				// 取得当前车站编码
	//				WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();			
	//				// 判断当前车站是否存在			
	//				if(!theACC_LOCATION.IsStationCodeExist(stationCode)) {
	//					theEXCEPTION_MGR.ProcessException(CParamException(CParamException::STATION_NOT_IN_VFARE, _T(__FILE__), __LINE__,_opl(TXT_INI_FUNCTION_EXCEPTION)));
	//				}
	//				else {
	//					//LocationNumber_t locationNum = theACC_LOCATION.GetStationLocationNumber(stationCode);
	//					// 设置当前车站名称
	//					theAPP_SESSION.SetStationName(theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_CHINESE),theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_ENGLISH),theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_PORTUGUESE));
	//				}

	//				// 更新声音文件及模板文件
	//				theUPGRADE_HELPER::UpdateVoiceAndTemplateFolder();
	//				// 更新应用程序及GUI文件
	//				if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade())
	//				{
	//					theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
	//				}
	//			}

	//			if (VecVersionInfo.size()>0 ||needUpgrade){
	//				theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
	//				LOG("Post SM_MAIN_SHUT_DOWN to MAIN_SVC!");
	//				theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
	//			}
	//		}
	//	}

	//	StartTimer(TIMER_PARAMETER_UPDATE, 30);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      响应运营切换

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnSwitchBussinessDay(){
	try{
		// 1.时间同步
		try{
			theSNTP_HELPER.TimeSynchronize();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}

		// 2.清除24小时运营标志
		if(theTVM_STATUS_MGR.Get24HourMode())theTVM_STATUS_MGR.Set24HourMode(OPERATION_24HOURS_OFF);

		// 3.生成日结数据
		try{
			CAuditHelper* auditHelper = new CAuditHelper(*this);
			auditHelper->PrintBalanceBill(false);

			if(NULL != auditHelper){
				delete auditHelper;
				auditHelper = NULL;
			}
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}


		// 4.上传所有未上传数据
		// 上传所有业务数据
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->SendBusinessData();
		// 上传所有交易数据
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendTransactionData();
		// 上传所有事件数据
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendEventData();

		// 5.参数版本切换
		try{
			thePARAM_HELPER.InitAllParam();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
		// 6.删除过期交易
		try{
			bool IsValidateUnsendPackge = false;
			int iSendedDataMaxDay = theFunction_INFO.GetLimitedDaysOfSendData();
			int iUnSendDataMaxDay = theFunction_INFO.GetLimitedDaysOfUnSendData();

			theACC_TXN_MGR.InitTxnData(IsValidateUnsendPackge,iSendedDataMaxDay,iUnSendDataMaxDay);
			theAFC_TXN_MGR.InitTxnData(IsValidateUnsendPackge,iSendedDataMaxDay,iUnSendDataMaxDay);

			theBUS_TXN_MGR.InitTxnData(IsValidateUnsendPackge,iSendedDataMaxDay,iUnSendDataMaxDay);
			theEVN_TXN_MGR.InitTxnData(IsValidateUnsendPackge,iSendedDataMaxDay,iUnSendDataMaxDay);
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
		// 7.删除过期日志
		try{
			DeleteLog();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}

		// 8.转换当前日期
		m_CurrentBusiDate = ComGetBusiDate();

		// 设置运营日
		//theSCAudit_MGR.Initilize();
		theSCAudit_MGR.SetCurrentBusinessDay(m_CurrentBusiDate);

		// 9.清空密码锁定
		thePWD_ERR.Internalize(theTVM_INFO.GetDataPath(),theTVM_INFO.GetBackupPath());

		// 10.初始化数据
		m_bIsBeginRunning = false;
		m_bIsEndRunning   = false;

		// 11.设置TPU运营日时间
		CARDRW_HELPER->InitData();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上位【业务结束】序列完成时，请求关机

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnShutDown(WPARAM wParam,LPARAM lParam)
{
	if( IN_SERVICE   == theTVM_STATUS_MGR.GetServiceStatus() ||
		WARN_SERVICE == theTVM_STATUS_MGR.GetServiceStatus() || 
		SHUTDOWNING  == theTVM_STATUS_MGR.GetRunStatus()     ||
		START_RUNING == theTVM_STATUS_MGR.GetRunStatus() )
		StartOutOfService();
	try{
		CShieldKeyHelper::SetHostProgarmEnable(false);
	}
	catch(...){

	}

	try {

		theSERVICE_MGR.SetForeServiceBusy(true);
		if(theAPP_SESSION.GetCloseStartFlag() != CS_EXIT )theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
		StopTimer(TIMER_READY_TO_REST);
		StopTimer(TIMER_RETURN_INDICATOR);
		StopTimer(TIMER_RETURN_LIGHT);
		StopTimer(TIMER_IO_MONITOR);
		StopTimer(TIMER_NTP);
		StopTimer(TIMER_DEFAULT_SERVICE);
		StopTimer(TIMER_AUTOMATIC_LOGOFF);
		StopTimer(TIMER_BUSIDAY_SWITH);

		// 显示向导提示
		switch (theAPP_SESSION.GetCloseStartFlag()) {
		case CS_CLOSE:
			theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_CLOSE);
			break;
		case CS_RESTART:
		case CS_SHUTDOWN:
			theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_SHUTDOWN);
			break;
		case CS_EXIT:
			theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_EXIT);
			//theDeviceAR.UpdateSerialFile();
			break;
		}
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	//theSERVICE_MGR.StopService(MONITOR_RW_SVC);
	//theSERVICE_MGR.StopService(MONITOR_TW_SVC);
	//theSERVICE_MGR.StopService(MONITOR_TH_SVC);
	//theSERVICE_MGR.StopService(MONITOR_UPS_SVC);
	//theSERVICE_MGR.StopService(MONITOR_LPT_SVC);
	try {
		if (theAPP_SESSION.IsInitErrorOver() == 0) {
			if (theAPP_SESSION.GetCloseStartFlag() == CS_SHUTDOWN) {
				// 设置审计结束时间 mzy
				//theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
				if(theEXCEPTION_MGR.HasException())
				{
					theTVM_STATUS_MGR.SetServiceStatus(ERROR_SERVICE);
				}
			}
		}
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
	// 业务结束（放在运营结束时序中：zhengxianle）
	//try {
	//	if (theAPP_SESSION.GetCloseStartFlag() == CS_CLOSE) {  
	//		// 打印销售统计帐票	mzy
	//		CAuditHelper* m_AuditHelper = new CAuditHelper(*this);
	//		m_AuditHelper->PrintBalanceBill(!m_bIsEndRunning);
	//		theSCAudit_MGR.ClearCurrentBusinessDay();
	//	}
	//}
	//catch (CSysException& e) 
	//{
	//	theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	//}
	//catch (...) {
	//	theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	//}
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(")"));
	try {
		if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXIT)
		{
			// 需要断开连接，否则TH更新无法打开串口
			TOKEN_HELPER->Disconnect();

			theUPGRADE_HELPER::Upgrade();
			// 显示向导提示
			switch (theAPP_SESSION.GetCloseStartFlag()) {
			case CS_CLOSE:
				theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_CLOSE);
				break;
			case CS_RESTART:
			case CS_SHUTDOWN:
				theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_SHUTDOWN);
				break;
			case CS_EXIT:
				theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_EXIT);
				break;

			}
		}

	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));

	try {
		if (theAPP_SESSION.IsInitErrorOver() == 0) {
			// 断开外部读写器
			m_pRWHelper->Close();
			m_pRechargeHelper->Close();
		}
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
	//try {
	//	if (theAPP_SESSION.IsInitErrorOver() == 0) {
	//		// 断开内部读写器
	//		m_pTWHelper->Close();
	//	}
	//}
	//catch (CSysException& e) 
	//{
	//	theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	//}
	//catch (...) {
	//	theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	//}
	//theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));

	// 断开打印机
	try {
		if (theAPP_SESSION.IsInitErrorOver() == 0) {

			m_pPrinterHelper->PrinterClose();
		}
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));

	try
	{
		theSERVICE_MGR.StopService((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID());
	}	
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}
	try {
		// 释放Controller
		ReleaseController();
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
	}

	try {
		//把当天的日志发给服务器
		try{
			// 关闭日志
			//ReleaseTHLog();	// 关闭TH日志连接
			//ReleaseBHLog();		// 关闭BH日志连接
			BHChange_CloseLog();// 关闭BHChange日志连接
			//BHChangeRfid_CloseLog();
			ReleaseCHLog();		// 关闭CH日志连接
			//ReleaseIOLog();	// 关闭IO日志连接lichao
			//CTSPTrs_Close();	// 关闭显示屏日志连接
			CFileLog::ReleaseDebugLog();
			// 关闭密码键盘日志
			// 关闭打印机日志
			// 关闭银行卡日志
			// 关闭CS_Service日志
			// 关闭SC_Client日志
			// 关闭keyboard日志
			theLOGIN_LOGOFF.ReleaseLoginLog();// 关闭登录登出日志
			//theDEBUG_LOG->ReleaseDebugLog();// 关闭Debug日志
			//theTOKEN_TRACE->ReleaseDebugLog();//关闭Token_Module日志
			// 关闭Txn_Data日志
			// 关闭监控日志
			// 关闭3个读写器日志
			// 关闭DB_SQL日志
			CPassengerOperationLog::ReleasePassengerOperationLog();// 关闭乘客操作日志
			theEXCEPTION_LOG.ReleaseExceptionLog();// 关闭异常日志
			// 关闭PassgerFore日志
			if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXIT){
				CLogHelper::PackAndUploadLog();
			}
		}
		catch (...) {
			//
		}

		// 关闭框架窗口
#ifndef _DEBUG
		BOOL exitMainRecWndSuccess = theApp.GetMainWndRec()->PostMessage(WM_CLOSE);
		BOOL exitMainWndSuccess = theApp.GetMainWnd()->PostMessage(WM_CLOSE);		

		CShieldKeyHelper::SetFunctionKeysEnable(true);
		CShieldKeyHelper::SetTaskManagerEnable(true);
		if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXIT)		{
			bool reboot = theAPP_SESSION.GetCloseStartFlag() == CS_RESTART;			
			ComShutDownSystem(reboot);
		}
		// 结束窗体未成功，强制退出
		if(!exitMainRecWndSuccess || !exitMainWndSuccess || theAPP_SESSION.GetCloseStartFlag() == CS_EXIT){
			HANDLE hself = GetCurrentProcess();
			TerminateProcess(hself, 0);
			//ExitProcess(0);
		}
#endif
	}
	catch (...) {
		//
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnCloseOperation(WPARAM wParam,LPARAM lParam)
{
	// 暂停运营切换检测
	m_bPuseCheckOperation = true;
	//设置运营状态为业务结束中
	theTVM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
	theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());

	theAPP_SESSION.SetCloseStartFlag(CS_CLOSE);
	theAPP_SESSION.ShowOfficeGuide(GUIDE_LOGIN_CLOSE);
	theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_CLOSEOPERATION,NULL,NULL);

	// 24小时运营模式有可能需要继续运营
	m_bIsReadyStartOperation = false;
	//m_bPuseCheckOperation = false;

	if(!theAPP_SESSION.IsSCConnected()){
		OnShutDown(NULL,NULL);// 与上位断开连接，需要在这里关机
	}
	else{
		// 24小时需要继续运营
		if(OPERATION_24HOURS_ON == theTVM_STATUS_MGR.Get24HourMode()){
			theSCAudit_MGR.Initilize();
			theSCAudit_MGR.SetCurrentBusinessDay(ComGetBusiDate());
		}
	}

	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      解除异常

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::RepairInitException()
{
	try
	{
		theSERVICE_MGR.SetForeServiceBusy(true);
		theEXCEPTION_MGR.RepareException();
		CInitSvc* pInitSvc = (CInitSvc*)(theSERVICE_MGR.GetService(INIT_SVC));
		if(pInitSvc!=NULL)
		{
			if (!(theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) || theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))) {
				//				pInitSvc->SetInitRessult(CInitSvc::ROW_TOKEN_RW,CInitSvc::SUCCESS);
			}
			// 			if (!(theEXCEPTION_MGR.HasException(CTVMPrinterException::MODULE_ID))) {
			// 				pInitSvc->SetInitRessult(CInitSvc::ROW_PRINT,CInitSvc::SUCCESS);
			// 			}
			//if (!(theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID) || theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID))) {
			//	pInitSvc->SetInitRessult(CInitSvc::ROW_TW,CInitSvc::SUCCESS);
			//}
			//if (!(theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))) {
			//	pInitSvc->SetInitRessult(CInitSvc::ROW_TH,CInitSvc::SUCCESS);
			//}
			if(!(theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID)))
			{
				pInitSvc->SetInitRessult(CInitSvc::ROW_DATA_INIT,CInitSvc::SUCCESS);
			}
			if(!theEXCEPTION_MGR.HasException())
			{
				// 启动Login画面
				theSERVICE_MGR.StartService(LOGIN_SVC);
			}
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
		CString comment(_T("RepairInitException"));
		// 异常解除--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_REMOVE_ERROR,true,comment);
	}
	catch (CSysException& e)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		CString comment(_T("RepairInitException"));
		// 异常解除--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_REMOVE_ERROR,false,comment);
	}
	catch(...)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		CString comment(_T("RepairInitException"));
		// 异常解除--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_REMOVE_ERROR,false,comment);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启IO监控消息响应函数

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartIOMonitor(WPARAM wParam,LPARAM lParam){
	// 定时器响应时间
	int nTimerTime = theTVM_INFO.GetIOMonitorInterval();
	// Set Default time
	if (0 == nTimerTime){
		nTimerTime = 1;
	}
	StartTimer(TIMER_IO_MONITOR,nTimerTime);

	// 如果可以启动默认业务
	if(theAFC_TVM_OPERATION.IsReadyToRestUsed()){
		try{
			//IO_HELPER->OpenPassengerSenser();lichao
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启找零指示灯消息响应函数

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartReturnIndicator(WPARAM wParam,LPARAM lParam){
	// 找零指示灯关闭时间计时器
	int nTimer = theTVM_INFO.GetReturnIndicatorTimerInterval();
	// Set Default time
	if (0 == nTimer){
		nTimer = 10;
	}
	StartTimer(TIMER_RETURN_INDICATOR,nTimer);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启找零照明灯消息响应函数

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartReturnLight(WPARAM wParam,LPARAM lParam){
	// 找零照明灯关闭时间
	int nTimerTime = theTVM_INFO.GetReturnLightTimerInterval();
	// Set Default time
	if (0 == nTimerTime){
		nTimerTime = 60;
	}
	StartTimer(TIMER_RETURN_LIGHT,nTimerTime);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启等待中到待机中消息响应函数

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnStartReadyToRest(WPARAM wParam,LPARAM lParam){
	// 等待到待机时间
	int nTimerTime = theAFC_TVM_OPERATION.GetReadyToRestTime();/*theTVM_INFO.GetReadyToRestTimerInterval()*/;
	// 为零表示不启用。
	if (0 == nTimerTime){
		//nTimerTime = 60;
	}
	else{
		StartTimer(TIMER_READY_TO_REST,nTimerTime);
	}	
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开启等待中到待机中消息响应函数

@param      WPARAM wParam 无意义

@param      LPARAM lParam 无意义

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::PostAssistanceButtonActionMSG(){

	// 招援按钮按下
	if(TRUE == m_IOStatusRsp.isAssittentButtonPressed && UPORDOWN == 1 && !m_bIsCallHelpRunning)
	{
		// 硬件动作时不响应招援按钮
		if(IsServiceBusyOnPassger()){
			return;
		}

		// SC连接时招援按钮生效
		if(theAPP_SESSION.IsSCConnected()/*theTVM_STATUS_MGR.GetCommunicate() == CONNECTED*/){
			// 发送自定义招援按钮按下消息
			ResponseAssistBtn(SM_ASSITANCE_BUTTON_DOWN) ? UPORDOWN ^= 1 : NULL;
			//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_DOWN,NULL,NULL);
			//UPORDOWN ^= 1;
		}
		else{
			//if(!CMessagePictureBox::Exist()){
			UPORDOWN ^= 1;
			theAPP_SESSION.ShowMessageToPassenger(TXT_MSG_GUIDE_NETWORK_ERROR_NOT_FOR_HELP);
			//CMessagePictureBox::Show(_T(""),_tl(TXT_MSG_GUIDE_NETWORK_ERROR_NOT_FOR_HELP),NULL,CMessagePictureBox::BTN_NONE,DT_CENTER,INFINITE,CMessagePictureBox::BTN_NONE);
			//}
		}
		//m_bIsCallHelpRunning = true;
	}
	else if(FALSE == m_IOStatusRsp.isAssittentButtonPressed && UPORDOWN == 0 && !m_bIsCallHelpRunning){
		m_bIsCallHelpRunning = true;
	}
	// 招援按钮再次按下
	else if(TRUE == m_IOStatusRsp.isAssittentButtonPressed && UPORDOWN == 0 && m_bIsCallHelpRunning)
	{		
		// SC连接时招援按钮生效
		if(!theAPP_SESSION.IsSCConnected()/*theTVM_STATUS_MGR.GetCommunicate() != CONNECTED*/){
			theAPP_SESSION.ShowMessageToPassenger(STATUS_CALL_HELP_MESSAGE_SHOW_NONE);
			UPORDOWN ^= 1;
		}
		else {
			// 发送自定义招援按钮弹起消息
			ResponseAssistBtn(SM_ASSITANCE_BUTTON_UP) ? UPORDOWN ^= 1 : NULL;
			//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_UP,NULL,NULL);
			//UPORDOWN ^= 1;
		}
		m_bIsCallHelpRunning = false;
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送维护门动作消息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::PostDoorActionMSG(){
	try{
		// 如果当前是IO测试业务，不进行IO监控业务切换操作。
		if(IO_MODULE_TEST_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
			return;
		}

		// 维护门打开
		if((m_IOStatusRsp.isBackDoorOpen || m_IOStatusRsp.isFrontDoorOpen) && OPENORCLOSE == 1  )
		{
			LOG("维护门打开！");
			//IO_HELPER->CloseBUElectromagnet();
			//IO_HELPER->CloseTUElectromagnet();
			//IO_HELPER->OpenOperatorLight();
			if(IsServiceBusyOnPassger()){
				LOG("提示乘客正在操作中，请关门！");
				// 提示乘客正在操作中，请关门...
				theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_DOOR_OPEN_PASSENGER_OPERATORING);
				GetIOModuleHelper()->OpenAlarm();
				m_bIsAramOpen = true;
				return;
			}
			//if(theSERVICE_MGR.IsForeServiceBusy()){
			//	//return;
			//}
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent();
			StopTimer(TIMER_READY_TO_REST);
			// 门开启后启动未登陆超时计时器
			PostMessage(SM_MAIN_DOOR_OPEN_WITH_OUT_LOGIN,NULL,NULL);
			// 发送自定义维护门打开消息
			//theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_OPENED,NULL,NULL);
			OPENORCLOSE ^= 1;

			m_bIsMenualRepare = true;
			// 维护业务
			//theSERVICE_MGR.SetState(OUT_OF);
			theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
			theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
			theSERVICE_MGR.StartService(LOGIN_SVC);
			theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
			theTVM_STATUS_MGR.TVMPaymentChanged.Invoke();//变相调用设置界面为不显示支付方式。
			theTVM_STATUS_MGR.SetMaintenanceDoorStatus(DOOR_STATUS::DOOR_NORMAL_OPEN);
		}
		// 维护门关闭
		else if((!m_IOStatusRsp.isBackDoorOpen && !m_IOStatusRsp.isFrontDoorOpen) && OPENORCLOSE == 0)
		{
			LOG("维护门关闭！");
			//IO_HELPER->CloseOperatorLight();
			// 1.发送自定义维护门关闭消息
			//theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_CLOSED,NULL,NULL);
			if(theSERVICE_MGR.IsForeServiceBusy()){// 如果后维护乘客正在操作，不允许关门
				LOG("禁止关门！");
				return;
			}
			OPENORCLOSE ^= 1;

			// 2.判断报警是否响应（未登陆报警）
			if (m_bIsAramOpen){
				GetIOModuleHelper()->CloseAlarm();
				m_bIsAramOpen = false;
			}
			else{
				StopTimer(TIMER_OUT_OF_TIME_LOGIN);
			}
			// 3.判断是否登出，否则立即自动登出
			if (LOGIN == theTVM_STATUS_MGR.GetLoginStatus()){
				CLogoffHelper logoffHelper(*this);
				logoffHelper.Logoff();
				// 停止登陆超时计时
				StopTimer(TIMER_OUT_OF_TIME_LOGIN);
			}
			if (theAPP_SESSION.GetMaintainMenu() == SERVICE_ID::ONE_KEY_SETTLEMENT_SVC)
			{
				theAPP_SESSION.SetMaintainMenu(MAINTENANCE_SVC);
			}
			// 禁用所有按键
			theAPP_SESSION.SetKeyboard(KEY_NOBODY);
			// 4.清除提示信息
			theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_DOOR_OPEN_PASSENGER_OPERATORING,false);
			// 5.乘客业务(需要修改，暂时如此写：zhengxianle)
			/*theSERVICE_MGR.SetState(OUT_OF);
			theSERVICE_MGR.SetState(ISSUEING);
			theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);
			theSERVICE_MGR.StartService(ISSUE_SVC);
			theTVM_STATUS_MGR.SetWorkMode(0x061f);*/
			// 维护门关闭时，需要将票箱复位
			try
			{
				//TOKEN_RSP_GET_DEV_STATUS status;
				//CARD_HELPER->GetDevStatus(&status);
				//SC_REPLACE_BOX_CMD_INFO replaceSelect;
				//if (!status.bBoxAArrive)
				//{
				//	replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_A;
				//	replaceSelect.actionType = TH_INSTALL;
				//	m_pCardModuleHelper->ReplaceTicketBox(replaceSelect);
				//}
				//if (!status.bBoxBArrive)
				//{
				//	replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_B;
				//	replaceSelect.actionType = TH_INSTALL;
				//	m_pCardModuleHelper->ReplaceTicketBox(replaceSelect);
				//}
			}
			catch(CSysException&e)
			{
				//do nothing
			}
			if(STOP_SERVICE != theTVM_STATUS_MGR.GetServiceStatus()){
				theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
				if(!m_bIsStartingDefaultServic)
					StartDefaultService();
			}
			// 门正常关闭
			theTVM_STATUS_MGR.SetMaintenanceDoorStatus(DOOR_STATUS::DOOR_NORMAL_CLOSE);
		}	
		// 关门时
		else if(m_IOStatusRsp.isBackDoorOpen || m_IOStatusRsp.isFrontDoorOpen){
			//IO_HELPER->CloseOperatorLight();
			if(IsServiceBusyOnPassger()&&m_bIsAramOpen){
				LOG("关门清空警告显示！");
				// 关门清空警告显示
				theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_DOOR_OPEN_PASSENGER_OPERATORING,false);
				GetIOModuleHelper()->CloseAlarm();
				m_bIsAramOpen = false;
				return;
			}
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      响应招援按钮

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMainSvc::ResponseAssistBtn(UINT uMsg)
{
	bool bRet = false;
	SERVICE_STATE state = theSERVICE_MGR.GetState();

	switch(uMsg)
	{
	case SM_ASSITANCE_BUTTON_DOWN:
		switch(state)
		{
		case OUT_OF:
		case READY:
		case RECEPT:
		case ABNORMAL:
		case REST:
		case FINISH:
			theSERVICE_MGR.SetState(OUT_OF);    //设置状态
			theSERVICE_MGR.StartService(CALL_HELP_SVC);
			theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestCallHelp(false);
			bRet = true;
			break;
		default:
			break;
		}

		/*theAPP_SESSION.ShowMessageToPassenger(TXT_MSG_GUIDE_ASSISTANCE_GOING_ON);
		theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestCallHelp(true);*/
		bRet = true;
		break;
	case SM_ASSITANCE_BUTTON_UP:
		switch(state)
		{
		case OUT_OF:
		case READY:
		case RECEPT:
		case ABNORMAL:
		case REST:
		case FINISH:
			//theSERVICE_MGR.SetState(READY);    //设置状态
			//theSERVICE_MGR.StartService(ISSUE_SVC);
			theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestCallHelp(true);
			//启动默认业务
			if(!m_bIsStartingDefaultServic)
				StartDefaultService();
			bRet = true;
			//设置状态 IN_SERVICE
			break;
		default:
			break;
		}
		break;

		/*theAPP_SESSION.ShowMessageToPassenger(STATUS_CALL_HELP_MESSAGE_SHOW_NONE);
		theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestCallHelp(false);*/
		bRet = true;
		break;
	default:
		break;
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      响应当人接近TMV时的动作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::FeelSomeCloseDevice(){

	// 0.判断当门打开时，跳过距离传感器感应
	if (!m_IOStatusRsp.isBackDoorOpen || !m_IOStatusRsp.isFrontDoorOpen)
	{
		StopTimer(TIMER_READY_TO_REST);
		return;
	}
	// 1.当有人靠近设备且招援按钮没有按下时，如果是等待状态停止计时器，如果是待机中，根据配置是否立即返回默认业务？手动点击回到默认业务。
	if ((m_IOStatusRsp.isLeftPersonActive||m_IOStatusRsp.isRightPersonActive) && SOMEONECLOSE == 1 /*&& !m_IOStatusRsp.isAssittentButtonPressed*/){
		// 1.1判断当前状态，如果是等待中，则停止计时器
		if (theSERVICE_MGR.GetState() != REST){
			SOMEONECLOSE ^= 1;
			StopTimer(TIMER_READY_TO_REST);
		}
		// 1.2 如果是待机中，则停止待机（广告），转到等待中
		else if (theSERVICE_MGR.GetState() == REST){
			// 1.2.1如果自动返回默认业务，则自动返回。否则不进行任何操作。
			if (theTVM_INFO.IsAutoReturnDefaultService()){
				//SOMEONECLOSE ^= 1;
				if(!m_bIsStartingDefaultServic)StartDefaultService();
			}			
		}
	}
	// 2.如果没有人靠近，且当前状态为等待中的状态时，则启动计时器
	else if(!(m_IOStatusRsp.isLeftPersonActive||m_IOStatusRsp.isRightPersonActive)&& SOMEONECLOSE == 0 && READY == theSERVICE_MGR.GetState() && theAFC_TVM_OPERATION.IsReadyToRestUsed()){
		SOMEONECLOSE ^= 1;
		// 2.1 没人靠近且是等待状态，开启计时器，计时转到待机状态
		PostMessage(SM_MAIN_READY_TO_REST,NULL,NULL);	
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      响应震动传感器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::WhenShockActive(){
	try{
		// 1.当震动传感器响应时，报警，不切换Service!
		// 		if (m_IOStatusRsp.isRightShockActive && WHENSHOCKACTIVE == 1)
		// 		{
		// 			// 1.1如果开启状态，直接返回 
		// 			if (theAPP_SESSION.IsWarningStart())
		// 			{
		// 				return;
		// 			}
		// 			//1.2 找零中可能会触发此异常，不做处理。后续会去掉这些，硬件不符合要求。！！！
		// 			/*if (theSERVICE_MGR.GetState() == COUNTING	|| 
		// 			theSERVICE_MGR.GetState() == FINISH		||
		// 			theSERVICE_MGR.GetState() == ISSUEING	|| 
		// 			theSERVICE_MGR.GetState() == CANCELING  ||
		// 			theSERVICE_MGR.GetState() == RECEPT)
		// 			{
		// 			WHENSHOCKACTIVE ^= 1;
		// 			return;				
		// 			}*/
		// 
		// 			// 1.3 启动警报
		// 			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
		// 			{
		// 				GetIOModuleHelper()->OpenAlarm();
		// 			}			
		// 			// 1.4 设置状态为报警状态
		// 			theAPP_SESSION.SetIsWarningStart(true);
		// 			// 1.5 向上位发送警告消息
		// 			// Need or not?
		// 			theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_SHOCK_SENSOR_ACTIVE);
		// 			// 1.6 只执行一次操作,设置标记位
		// 			WHENSHOCKACTIVE ^= 1;
		// 		}
		// 		// 2.当震动传感器无响应过后
		// 		else if(!m_IOStatusRsp.isRightShockActive && WHENSHOCKACTIVE == 0)
		// 		{
		// 			// 2.1 如果没有人登陆，直接返回不进行警告处理
		// 			LOGIN_STATUS login = theTVM_STATUS_MGR.GetLoginStatus();
		// 			if (LOGIN != login)
		// 			{
		// 				return;
		// 			}
		// 			// 2.2 置震动传感器状态
		// 			WHENSHOCKACTIVE ^= 1;
		// 			// 2.3 设置当前状态
		// 			theAPP_SESSION.SetIsWarningStart(false);
		// 			// 2.4 关闭报警
		// 			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
		// 			{
		// 				GetIOModuleHelper()->CloseAlarm();
		// 			}
		// 			// 清空显示信息
		// 			theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_SHOCK_SENSOR_ACTIVE,false);
		// 		}
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断当前Service状态是否有乘客正在操作

@param      无

@retval     bool（true：乘客正在操作 false：无乘客操作）

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMainSvc::IsServiceBusyOnPassger(){
	SERVICE_STATE stCur = theSERVICE_MGR.GetState();
	// 取当前的Service状态，根据service状态返回不同的值
	switch(stCur){
	case RECEPT:
	case COUNTING:
	case ISSUEING:
		//case FINISH:
	case CANCELING:
		return true;
	default:
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      维护门关闭消息处理函数(未完成，暂时先不处理zhengxianle)

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnFrontDoorClose(WPARAM wParam,LPARAM lParam){
	// 	try{
	// 		//1.判断硬件模块状态
	// 		//1.1判断门为关闭状态是否正常？
	// 		GetIOModuleHelper()->GetIOStatus(m_IOStatusRsp);
	// 		if (m_IOStatusRsp.isFrontDoorOpen){
	// 			//防止关门后立即开门
	// 			return TRUE;
	// 		}
	// 		//1.2判断硬件切离设定？判断状态时候正常，否则添加到异常队列。
	// 		CheckSetModuleStatus();
	// 		//2.判断票箱、钱箱状态
	// 		if (!m_IOStatusRsp.isBanknoteModeReady){
	// 
	// 		}
	// 		else{
	// 
	// 		}
	// 		if (!m_IOStatusRsp.isCardModeReady){
	// 
	// 		}
	// 		else{
	// 
	// 		}
	// 
	// 		//3.如果状态发生变更，需要发送状态变更数据
	// 		//4.启动默认业务,设置状态 
	// 	}
	// 	catch(CSysException& e)	{
	// 		theEXCEPTION_MGR.ProcessException(e);
	// 	}
	// 	catch(...){
	// 
	// 	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断切离状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::CheckSetModuleStatus(){
	//根据切离状态，修复异常，如果有。
	theEXCEPTION_MGR.RepareException();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		启动门开未登陆计时器 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnFrontDoorOpenWithoutLogin(WPARAM wParam,LPARAM lParam){
	// 1.获取超时时间参数
	DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetDoorUnlockAlarmTime();
	if (0 == dwOutOfTime){
		// 为零，不响应
		return SP_SUCCESS;
		//dwOutOfTime = 120;//设置默认时间为120S，如果为空
	}
	// 2.启动未登陆报警计时器
	StartTimer(TIMER_OUT_OF_TIME_LOGIN,dwOutOfTime);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		启动操作员操作超时计时器  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnFrontDoorOpenTimeOutOperation(WPARAM wParam,LPARAM lParam){
	// 1.获取超时时间参数
	DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
	if (0 == dwOutOfTime){
		// 为零，不启用计时器
		return SP_SUCCESS;
	}
	// 2.启动未登陆报警计时器
	StartTimer(TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);

	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		不断判断是否登陆，登陆时需要解除超时报警

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::JudgeIsOperatorLogin(){
	// 1.获取登陆状态,解除警报,门开时
	if (!m_IOStatusRsp.isBackDoorOpen && !m_IOStatusRsp.isFrontDoorOpen){
		LOGIN_STATUS login = theTVM_STATUS_MGR.GetLoginStatus();
		// 报警响，且登陆，解除报警
		if (m_bIsAramOpen && LOGIN == login){
			m_bIsAramOpen = false;
			GetIOModuleHelper()->CloseAlarm();
			// 登录时清空告警提示信息
			theAPP_SESSION.ShowStatusWaringMsg(TXT_MSG_GUIDE_SHOCK_SENSOR_ACTIVE,false);
		}		
		else{
			// 没有关门且登出，启动未登陆计时器,在登出的位置来启动计时器!!!
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		在Function.ini中获取支付方式  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::GetPaymentInfoFromFuncIni(WORD& device_pay_status/*当前支付模式*/){
	// 信用卡后期再改
	bool bIsSupportCash(false),bIsSupportEP(false),bIsSupportBankcard(false),bIsSupportCreditCard(false);
	// 是否支持现金？判断所有票卡支付模式
	if(theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_UL,PAYMENT_METHOD_CASH/*现金*/)||
		theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CASH) ||
		theFunction_INFO.IsPayTypeEnable(ISSUE_PRODUCT_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CASH) ||
		theFunction_INFO.IsPayTypeEnable(CHARGE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CASH)){
			bIsSupportCash = true;
	}
	// 是否支持EP支付？
	// 	if(theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_UL,PAYMENT_METHOD_EFT/*EP*/)||
	// 		theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_EFT) ||
	// 		theFunction_INFO.IsPayTypeEnable(ISSUE_PRODUCT_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_EFT) ||
	// 		theFunction_INFO.IsPayTypeEnable(CHARGE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_EFT)){
	// 			bIsSupportEP = true;
	// 	}
	// 	// 是否支持信用卡?(预留)
	// 	if(theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_UL,PAYMENT_METHOD_CREDIT_CARD/*信用卡*/)||
	// 		theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CREDIT_CARD) ||
	// 		theFunction_INFO.IsPayTypeEnable(ISSUE_PRODUCT_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CREDIT_CARD) ||
	// 		theFunction_INFO.IsPayTypeEnable(CHARGE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_CREDIT_CARD)){
	// 			bIsSupportCreditCard = true;
	// 	}
	// 是否支持银行卡？
	if(theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_UL,PAYMENT_METHOD_BANK_CARD/*银行卡*/)||
		theFunction_INFO.IsPayTypeEnable(ISSUE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_BANK_CARD) ||
		theFunction_INFO.IsPayTypeEnable(ISSUE_PRODUCT_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_BANK_CARD) ||
		theFunction_INFO.IsPayTypeEnable(CHARGE_SVC,CARD_TYPE_ACC_M1,PAYMENT_METHOD_BANK_CARD)){
			bIsSupportBankcard = true;
	}
	// 如果不支持现金,如果有现金支付，则去掉现金支付
	if (!bIsSupportCash){
		if((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN){
			device_pay_status |= device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}
		if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			device_pay_status |= device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}
	}
	// 如果不支持EP，去掉EP支付(如果有的话)
	//if (!bIsSupportEP){
	//	if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_EP) == DEVICE_CONTRL_CODE_PAYMENT_EP){
	//		device_pay_status |= device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_EP;
	//	}
	//}
	//// 如果不支持信用卡，去掉信用卡支付(如果有的话)
	//if (!bIsSupportCreditCard){
	//	if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD) == DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD){
	//		device_pay_status |= device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD;
	//	}
	//}
	//// 如果不支持银行卡，去掉银行卡支付（如果有的话）
	//if (!bIsSupportBankcard){
	//	if ((device_pay_status & DEVICE_CONTRL_CODE_PAYMENT_BANKCARD) == DEVICE_CONTRL_CODE_PAYMENT_BANKCARD){
	//		device_pay_status |= device_pay_status ^ DEVICE_CONTRL_CODE_PAYMENT_BANKCARD;
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		启用暂停服务业务  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::StartOutOfService(){
	// 20180104,lichao; 增加用户登录的条件判断，防止将已登录的界面重新刷新成登出。
	if (TRUE/*LOGIN != theTVM_STATUS_MGR.GetLoginStatus()*/){
		theTVM_STATUS_MGR.SetWorkMode(DEVICE_CONTRL_CODE_BUSINESS);
		theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
		theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
		theSERVICE_MGR.StartService(LOGIN_SVC);
		theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		theTVM_STATUS_MGR.TVMPaymentChanged.Invoke();
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      密码键盘操作响应函数，盲人购票业务开始

@param      WPARAM wParam 无
LPARAM lParam 无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnPinPadMonitorReturn(WPARAM wParam,LPARAM lParam){
	try{
		// 判断当前是否为暂停服务？因为当暂停服务时密码键盘监控未启用，暂时不提示语言！！！
		//if(theAPP_SESSION.IsBlindManOperation()){
		//	// 1.设置当前状态为操作中
		//	theSERVICE_MGR.SetState(RECEPT);
		//	// 初始化设置为中文
		//	theAPP_SESSION.GetCurrentBlindPlayVoiceLanguageType() == LANGUAGETYPE_T::LANGUAGE_ID_CHINESE ? NULL : theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGE_ID_CHINESE);
		//	m_PinpadInputType = UE_UNKNOWN_KEY;
		//	// 初始化标志位
		//	m_bIsClickButtonWithoutPinPad = false;
		//	// 2.状态：选择语言
		//	m_operation_status = STATUS_BLIND_SELECT_LANGUAGE;
		//	// 3.播放语言：请选择语言
		//	PlayEventAudio(AUDIO_COMMON_PLEASE_SELECT_LANGUAGE);
		//	// 4.密码键盘监控
		//	TurnOnPinPadAndTimer();
		//}		
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断自动降级是否需要提示

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMainSvc::IsAutoDownGradeNeedWarn(){
	// 是否允许降级？
	if(theTVM_SETTING.IsAllowDegrade()){
		bool bIsTvm = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_TVM ? true: false;
		if (theTVM_STATUS_MGR.GetPrintMode() != 0 ||																			// 是否支持打印？
			(theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE ||	// 是否不接收纸币
			(((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN) && bIsTvm)||			// 是否不接收硬币？
			(((theTVM_STATUS_MGR.GetChangeMode() & DEVICE_CONTRL_CODE_NO_BILL_CHANGE) != DEVICE_CONTRL_CODE_NO_BILL_CHANGE) && bIsTvm)){		// 是否不找零？
				// 上面有任意一种都需要提示乘客
				if(!bIsTvm){//AVM如果没有充值业务，直接提示不支持盲人业务
					if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
						PlayEventAudio(AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE,true);// 当前不支持盲人业务
						theAPP_SESSION.SetBlindManStatus(false);
						theSERVICE_MGR.SetState(READY);
						return false;
					}
				}
				PlayEventAudio(AUDIO_COMMON_MSG_DEGRADE);
				//m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_GEDRADE_MSG;
				return true;
		}
		else {
			if(!bIsTvm){
				// AVM只有充值
				if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
					PlayEventAudio(AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE,true);// 当前不支持盲人业务
					theAPP_SESSION.SetBlindManStatus(false);
					theSERVICE_MGR.SetState(READY);
					return false;
				}
				// 启动充值业务
				else if(theSERVICE_MGR.GetCurService()->GetServiceID() != CHARGE_SVC){
					theSERVICE_MGR.StartService(CHARGE_SVC);
					//m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_NORMAL;
					StopTimer(TIMER_REPEAT_EVENT_AUDIO);
					return false;
				}
			}
			else{// 当前不支持任何盲人业务
				if(!((theTVM_STATUS_MGR.GetWorkMode()&DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE ||
					(theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD/* ||
																															(theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_TOKEN) == DEVICE_CONTRL_CODE_BUSINESS_TOKEN*/)){
																																PlayEventAudio(AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE,true);// 当前不支持盲人业务
																																theAPP_SESSION.SetBlindManStatus(false);
																																theSERVICE_MGR.SetState(READY);
																																return false;
				}
				else{
					PlayEventAudio(AUDIO_COMMON_SELECT_BUSINESS);
					//					m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_SELECT_BUSINESS;
				}
			}			
			return false;
		}
	}
	else{
		if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_AVM){
			// AVM只有充值
			if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
				PlayEventAudio(AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE,true);// 当前不支持盲人业务
				theAPP_SESSION.SetBlindManStatus(false);
				theSERVICE_MGR.SetState(READY);
				return false;
			}
			// 启动充值业务
			else if(theSERVICE_MGR.GetCurService()->GetServiceID() != CHARGE_SVC){
				theSERVICE_MGR.StartService(CHARGE_SVC);
				//				m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_NORMAL;
				StopTimer(TIMER_REPEAT_EVENT_AUDIO);
				return false;
			}
		}
		else{
			// 当前不支持任何盲人业务
			if(!((theTVM_STATUS_MGR.GetWorkMode()&DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE ||
				(theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD /*||
																														 (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_TOKEN) == DEVICE_CONTRL_CODE_BUSINESS_TOKEN*/)){
																															 PlayEventAudio(AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE,true);// 当前不支持盲人业务
																															 theAPP_SESSION.SetBlindManStatus(false);
																															 theSERVICE_MGR.SetState(READY);
																															 return false;
			}
			else{
				PlayEventAudio(AUDIO_COMMON_SELECT_BUSINESS);
				//				m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_SELECT_BUSINESS;
			}
		}			
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      托管售票充值业务发票后的剩余工作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::StartManagedBlind(BLIND_OPERATION_STATUS status){
	// 1.设置当前运行的状态
	//m_operation_status = status;
	//// 2.如果密码键盘没有打开，打开密码键盘监控。
	//if(!m_bIsPinPadRunning && theAPP_SESSION.IsBlindManOperation()){
	//	TurnOnPinPadAndTimer(false);
	//}
	//// 3.结束时记得关闭密码键盘
	//else if(STATUS_BLIND_BUY_TOKEN	== status || STATUS_BLIND_BUY_CARD == status ||	STATUS_BLIND_RECHARGE== status ){
	//	theAPP_SESSION.SetBlindManStatus(false);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      语音播放接口

@param      DWORD eventID (语音ID)
@param		bool  bStopRepeat (停止标记)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::PlayEventAudio(DWORD eventID,bool bStopRepeat /* = false */){
	LOG("PlayEventAudio");
	// 1.本句语音ID
	//if(eventID == AUDIO_COMMON_MSG_INPUT_ERROR||eventID == AUDIO_COMMON_MSG_NOT_SUPPORT_BUSINESS){
	//	bStopRepeat = false;
	//}
	//else 
	//		m_dwLastPlayedVoice = eventID;
	// 2.播放前先停止计时器
	StopTimer(TIMER_REPEAT_EVENT_AUDIO);
	unsigned int nPlayTimes = 0;
	m_pAudioHelper->playEventAudio(eventID,nPlayTimes);
	int nTimeSecends = nPlayTimes/1000 + 6;
	// 3.是否重复播放？
	if(!bStopRepeat)
		StartTimer(TIMER_REPEAT_EVENT_AUDIO,nTimeSecends);
	else
		StopTimer(TIMER_REPEAT_EVENT_AUDIO);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动盲人购票超时计时器

@param      bool bIsStartOutOfTimer(是否开启操作超时计时器)

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CMainSvc::TurnOnPinPadAndTimer(bool bIsStartOutOfTimer /* = true */){
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      当购票还在有MainSvc管理时点击了界面的按钮的处理

@param      none

@retval     bool 密码键盘是否是激活状态？

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CMainSvc::OnClickButtonWithoutPinPad(bool bIsStop){
	LOG("Stop");
	bool bIsPinPadRunning = false;
	return bIsPinPadRunning;
}

//CTVMRWHelper* CMainSvc::GetRechargeRWHelper()
//{
//	return m_pRechargeRWHelper;
//}
//
//CTVMRWHelper* CMainSvc::GetCardRWHelper()
//{
//	return m_pCardRWHelper;
//}
//
//CTVMRWHelper* CMainSvc::GetTokenRWHelper()
//{
//	return m_pTokenRWHelper;
//}

CPrinterHelper* CMainSvc::GetPrinterHelper()
{
	return m_pPrinterHelper;
}

CTHHelper* CMainSvc::GetCardModuleHelper()
{
	return m_pCardModuleHelper;
}

CIOModuleHelper* CMainSvc::GetIOModuleHelper()
{
	return m_pIOHelper;
}

CBRHModuleHelper* CMainSvc::GetBanknoteModuleHelper()
{
	return m_pBanknoteModuleHelper;
}

//CBHChangeHelper* CMainSvc::GetBHChangeHelper()
CBRHModuleHelper* CMainSvc::GetBHChangeHelper()
{
	return m_pBHChangeHelper;
}

CCoinModuleHelper* CMainSvc::GetCoinModuleHelper()
{
	return m_pCoinModuleHelper;
}

CDataHelper* CMainSvc::GetDataHelper()
{
	return m_pDataHelper;
}

CAudioHelper* CMainSvc::GetAudioHelper()
{
	return m_pAudioHelper;
}

CTVMRWHelper* CMainSvc::GetRWHelper(){
	return m_pRWHelper;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      删除所有过期日志

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::DeleteLog(){
	// Try to delete log as follow
	int day = theTVM_INFO.GetStoreDay();
	// 删除系统日志
	DeleteLog(day);
	// 删除TPU日志
	DeleteTpuLog(day);
	// 删除打印日志
	DeletePrintHistory(day);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定天数前的FTP上传的日志

@param      (i)int days    指定天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::DeleteLog(int days)
{
	try {
		// 得到指定日数以前的日期结构体以及字符串
		CString dayLimitStr;
		vector<int> logdate;
		GetLimitLogDate(days,logdate);
		if (logdate.size() == 0) {
			return;
		}
		dayLimitStr.Format(_T("%d"),logdate[0]);

		// 取得文件目录
		CString sDir = theTVM_INFO.GetLogPath();		
		sDir +=FILTER_LOG_ZIP_BACKUP_HISTORY;
		// 查找文件
		vector<CString> fileNames;
		SearchFile(sDir + _T("\\*Log*9001*"),fileNames);
		vector<CString>::iterator iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// 取出文件名，用于比较日期
			CString dateStr = _T("");
			dateStr = ((*iter).Right(18)).Left(8);

			// 如果日期小于指定日期则删除
			if(dateStr < dayLimitStr) {
				// 删除该文件
				DeleteFile(*iter);
			}
			iter ++;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定天数前的日志文件

@param      (i)int days    指定天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::DeleteTpuLog(int days)
{
	try {
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + LOG_DIR;

		CString dayLimitStr;
		vector<CString> logdate;
		GetLimitTpuLogDate(days,logdate);
		if (logdate.size() == 0) {
			return;
		}
		vector<CString>::iterator iter = logdate.begin();
		while (iter != logdate.end()) {
			DeleteFolder(sDir + (*iter) +_T("\\"));
			iter ++;
		}		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      删除打印历史记录

@param      (i)int days;（天数）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::DeletePrintHistory(int days){
	CString logPath = theTVM_INFO.GetGetReceiptLogPath();

	vector<CString> vecStaffHistory;

	//Do check staff print history
	vecStaffHistory.clear();
	GetLimitPrintLogDate(days,logPath + FILTER_STAFF_PRINT_HISTORY,vecStaffHistory,false);
	//Do check staff print UnPrint
	vecStaffHistory.clear();
	GetLimitPrintLogDate(days,logPath + FILTER_STAFF_PRINT_UNPRINT,vecStaffHistory,false);
	//Do check Passenger print history
	vecStaffHistory.clear();
	GetLimitPrintLogDate(days,logPath + FILTER_PASSENGER_PRINT_HISTORY,vecStaffHistory,false);
	//Do check Passenger print UnPrint
	vecStaffHistory.clear();
	GetLimitPrintLogDate(days,logPath + FILTER_PASSENGER_PRINT_UNPRINT,vecStaffHistory,false);
	//Do check Audit
	vecStaffHistory.clear();
	GetLimitPrintLogDate(days,logPath + FILTER_AUDIT_LOG,vecStaffHistory,true);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取指定天数的log日志文件日期

@param       (i)int days			  指定天数
@param       (o)vector<int>& logdate  日期列表

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::GetLimitTpuLogDate(int days,vector<CString>& logdate)
{
	try {
		// 取得文件目录
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + LOG_DIR;

		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*.*"));

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return;
		}

		while (bContinue) {
			bContinue = finder.FindNextFile();
			if (finder.IsDots()) {
				continue;
			}
			else if(finder.IsDirectory()){   
				// 取出文件名，用于比较日期
				CString dateStr;
				CTime createTime;
				finder.GetCreationTime(createTime);

				SYSTEMTIME sysTime;
				::GetLocalTime(&sysTime);
				CTime curTime(sysTime);

				CTimeSpan timeSpan = CTimeSpan(days,0,0,0);
				if((curTime-createTime)>timeSpan){
					sFileName = finder.GetFileName();
					logdate.push_back(sFileName);
				}				
			}
			else{
				continue;
			}
		}

		finder.Close();

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取指定天数的log日志文件日期

@param       (i)int days			  指定天数
@param       (o)vector<int>& logdate  日期列表

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::GetLimitLogDate(int days,vector<int>& logdate)
{
	try {
		// 取得文件目录
		CString sDir = theTVM_INFO.GetLogPath();

		sDir += FILTER_LOG_ZIP_BACKUP_HISTORY;
		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("\\*")+ _T("Log*9001*"));//_T(".zip"));

		// 如果没有找到文件，就关闭文件
		if (!bContinue) {
			finder.Close();
			return;
		}

		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取出文件名，用于比较日期
				CString dateStr;
				sFileName = finder.GetFileName();
				dateStr = (sFileName.Right(18)).Left(8);
				logdate.push_back(_ttoi(dateStr));
			}
		}

		finder.Close();

		// 对日志文件的日期排序(从小到大)
		sort(logdate.begin(),logdate.end(),less<int>());

		// 删除其中重复的日期
		vector<int>::iterator end_itor = unique(logdate.begin(),logdate.end());
		logdate.erase(end_itor,logdate.end());

		if (logdate.size() < days) {
			return;
		}
		else {
			logdate.erase(logdate.begin() , logdate.end()-days);
		}
	}catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得指定目录下指定类型应该删除天数之外的文件

@param      (i)int days;					指定天数
(i)CString strPath;				指定路径
(o)vector<CString>& vecNeedDel;	指定天数路径下可删除的文件
(i)bool bIsDir;					指定路径是否是目录

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir){
	// Is empty?
	if(strPath.IsEmpty()){
		return;
	}
	else{
		if(strPath.Right(1) != _T("\\")) strPath += _T("\\");
	}

	vector<CString> vecData;
	vecData.clear();
	CFileFind finder;
	CString sFileName;

	bool bIsFind = finder.FindFile(strPath + _T("*.*"));//*.TXT and *.txt

	while(bIsFind){
		bIsFind = finder.FindNextFile();
		if(finder.IsDots()){
			continue;
		}
		else if(finder.IsDirectory()){
			//Find directory level
			if(bIsDir){
				CString dateStr;
				CTime createTime;
				finder.GetCreationTime(createTime);

				SYSTEMTIME sysTime;
				::GetLocalTime(&sysTime);
				CTime curTime(sysTime);

				CTimeSpan timeSpan = CTimeSpan(days,0,0,0);
				if((curTime - createTime)>timeSpan){
					sFileName = finder.GetFilePath();
					vecNeedDel.push_back(sFileName);
				}	
			}
			else{
				continue;
			}
		}
		else{
			//Find file
			CString dateStr;
			CTime createTime;
			finder.GetCreationTime(createTime);

			SYSTEMTIME sysTime;
			::GetLocalTime(&sysTime);
			CTime curTime(sysTime);

			CTimeSpan timeSpan = CTimeSpan(days,0,0,0);
			if((curTime - createTime)>timeSpan){
				sFileName = finder.GetFilePath();
				vecNeedDel.push_back(sFileName);
			}
		}
	}

	finder.Close();


	// Delete it
	if(vecNeedDel.size() == 0){
		return;
	}

	for(auto it = begin(vecNeedDel); it != end(vecNeedDel);it++){
		if(!it->IsEmpty()){

			if(bIsDir){
				DeleteFolder(*it);//Delete directory
			}
			else{
				DeleteFile(*it);  //Delete file
			}
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      执行运营日切换及日结处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnSwitchBusinessDayAndCloseOperation(WPARAM wParam,LPARAM lParam){
	if(!IsServiceBusyOnPassger()){
		m_bIsSwitchBussinessDay = false;
		if(STOP_SERVICE != theTVM_STATUS_MGR.GetServiceStatus()){
			StartOutOfService();
		}
		// 停止计时器
		StopTimer(TIMER_DEFAULT_SERVICE);	// 默认业务定时器
		StopTimer(TIMER_AUTOMATIC_LOGOFF);	// 自动登出计时器
		StopTimer(TIMER_BUSIDAY_SWITH);		// 运营日切换定时器
		StopTimer(TIMER_IO_MONITOR);		// IO监控定时器
		StopTimer(TIMER_READY_TO_REST);		// 等待中到待机中计时器

		// 执行运营日切换
		OnSwitchBussinessDay();


		// 恢复计时器
		StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
		OnStartIOMonitor(NULL,NULL);

		// 日结处理
		CloseOperation();
	}
	else
		m_bIsSwitchBussinessDay = true;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      检测配置文件的合法性

@param      (io)WORD& wService_Status,WORD& wPayment_Status,WORD& wChange_Status

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::CheckIniValue(WORD& wService_Status,WORD& wPayment_Status,WORD& wChange_Status){
	// 业务模式
	if((wService_Status & DEVICE_CONTRL_CODE_BUSINESS) == DEVICE_CONTRL_CODE_BUSINESS){
		if(wService_Status > DEVICE_CONTRL_CODE_BUSINESS_ALL){
			wService_Status = DEVICE_CONTRL_CODE_BUSINESS_ALL;
			theTVM_SETTING.SetServiceMode(wService_Status);
		}
	}
	else{
		wService_Status = DEVICE_CONTRL_CODE_BUSINESS_ALL;
		theTVM_SETTING.SetServiceMode(wService_Status);
	}

	// 支付模式
	if((wPayment_Status & DEVICE_CONTRL_CODE_PAYMENT) == DEVICE_CONTRL_CODE_PAYMENT ){
		if(wPayment_Status > DEVICE_CONTRL_CODE_PAYMENT_ALL){
			wPayment_Status = DEVICE_CONTRL_CODE_PAYMENT_ALL;
			theTVM_SETTING.SetPayMentMode(wPayment_Status);
		}
	}
	else{
		wPayment_Status = DEVICE_CONTRL_CODE_PAYMENT_ALL;
		theTVM_SETTING.SetPayMentMode(wPayment_Status);
	}

	// 找零模式
	if((wChange_Status & DEVICE_CONTRL_CODE_NO_CHANGE) == DEVICE_CONTRL_CODE_NO_CHANGE){
		if(wChange_Status > DEVICE_CONTRL_CODE_ALL_CHANGE){
			wChange_Status = DEVICE_CONTRL_CODE_ALL_CHANGE;
			theTVM_SETTING.SetChangeMode(wChange_Status);
		}
	}
	else{
		wChange_Status = DEVICE_CONTRL_CODE_ALL_CHANGE;
		theTVM_SETTING.SetChangeMode(wChange_Status);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      服务控制定时器消息处理（固定控制TIMER_DEFAULT_SERVICE定时器）

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnServiceTimerControl(WPARAM wParam,LPARAM lParam)
{
	// 
	int* pInterval = (int *)lParam;
	LOG("OnServiceTimerControl：%s-TIMER_DEFAULT_SERVICE, interval-%d", wParam?_T("Open"):_T("Close"), *pInterval);
	if (wParam!=0){//开启定时器
		m_timerInterval = *pInterval;
		StartTimer(TIMER_DEFAULT_SERVICE, 1);
	}
	else{//启动定时器
		StopTimer(TIMER_DEFAULT_SERVICE);
	}

	LOG("OnServiceTimerControl：leave");
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      二维码订单本地处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnWebIdleProcess(WPARAM wParam,LPARAM lParam)
{
	// 启动WEB订单本地处理定时器
	theWEBSERVER_HELPER.EnableIdleProcess(true);
	//StartTimer(TIMER_WEB_LOCAL_PROCESS, 10);目前定时器不启动，本地处理放到CIssueSvc构造函数中启动一个线程来做。

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      充值服务器检测

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnRechargeServerMonitor(WPARAM wParam,LPARAM lParam)
{
	CARDRW_HELPER->DoRechargeServerMoniter();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      SC下载参数加载

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnParameterUpdateProcess(WPARAM wParam,LPARAM lParam)
{
	//// 启动WEB订单本地处理定时器
	//StartTimer(TIMER_PARAMETER_UPDATE, 30);

	return TRUE;
}