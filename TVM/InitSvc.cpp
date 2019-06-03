#include "stdafx.h"
#include "InitSvc.h"
#include "UpgradeHelper.h"
#include "RechargeRWMonitorSvc.h"
#include "CardModuleMonitorSvc.h"
#include "IOMonitorSvc.h"
#include "tvmsetting.h"
#include "CardBoxManageSvc.h"
#include "shieldkeyhelper.h"
#include "LanguageInfo.h"
#include "TVM.h"
#include "SCAuditManager.h"
#include "CoinModuleMonitorSvc.h"
#include "BanknoteCollectionModuleMonitorSvc.h"
#include "CDiskException.h"
#include "MainSvc.h"
#include "PrinterMonitorSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CInitSvc,CTVMForeService)
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInitSvc::CInitSvc(void)
	:CTVMForeService(INIT_SVC,MAIN_SVC)
{
	// 进入初始化状态
	theSERVICE_MGR.SetState(INIT);
	//theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CInitSvc::OnExceptionNotify);

	// 初始化内容标题
	m_InitialInfo[ROW_DISK_CHECK].title				= _opl(TXT_INIT_DISK_DETECTION);
	m_InitialInfo[ROW_DATA_INIT].title				= _opl(TXT_INIT_DATA_INITIALIZATION);
	m_InitialInfo[ROW_IO_MODULE].title				= _opl(TXT_INIT_IO_MODULE);
	m_InitialInfo[ROW_CHARGE_RW].title				= _opl(TXT_INIT_RECHARGE_RW);
	//m_InitialInfo[ROW_TOKEN_RW].title				= _opl(TXT_INIT_TOKEN_RW);
	m_InitialInfo[ROW_CARD_RW].title				= _opl(TXT_INIT_CARD_RW);
	//m_InitialInfo[ROW_TOKEN_MODULE].title			= _opl(TXT_INIT_TOKEN_MODULE);
	m_InitialInfo[ROW_CARD_MODULE].title			= _opl(TXT_INIT_CARD_MODULE);
	m_InitialInfo[ROW_COIN_MODULE].title			= _opl(TXT_INIT_COIN_MODULE);
	m_InitialInfo[ROW_BANKNOTE_MODULE].title		= _opl(TXT_INIT_BANKNOTE_MODULE);
	//m_InitialInfo[ROW_BANKNOTE_CHANGE_MODULE].title = _opl(TXT_INIT_BANKNOTE_CHANGE_MODULE);
	m_InitialInfo[ROW_PRINT].title					= _opl(TXT_INIT_RECEIPT_PRINTER_MODULE);
	//m_InitialInfo[ROW_BANKCARD_MODULE].title		= _opl(TXT_INIT_BANKCARD_MODULE);
	//m_InitialInfo[ROW_PINPAD_MODULE].title		= _opl(TXT_INIT_PINPAD_MODULE);
	m_InitialInfo[ROW_DISPLAY].title				= _T("状态显示屏");
	// 是否初始化，对初始化内容重新排序
	int number = 0;
	bool is_tvm = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_TVM;
	m_order[ROW_DISK_CHECK]				= ++number;
	m_order[ROW_DATA_INIT]				= ++number;
	m_order[ROW_IO_MODULE]				= ++number;
	m_order[ROW_CHARGE_RW]				= ++number;
	//m_order[ROW_TOKEN_RW]				= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_CARD_RW]				= is_tvm ? ++number : ROW_INVALID;
	//m_order[ROW_TOKEN_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_CARD_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_COIN_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_BANKNOTE_MODULE]		= ++number;
	//m_order[ROW_BANKNOTE_CHANGE_MODULE] = ++number;
	m_order[ROW_PRINT]					= ++number;
	//m_order[ROW_BANKCARD_MODULE]		= ++number;
	//m_order[ROW_PINPAD_MODULE]		= ++number;
	m_order[ROW_DISPLAY]				= ++number;
	// 重置初始化结果
	for (int i = 0; i < ROW_MAX; ++i) {
		m_InitialInfo[i].result = UNKNOWN;
	}

	// 设置操作员画面
	m_pDialogFlow->AddDialog(CInitDlg::IDD, new CInitDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CInitDlg::IDD);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInitSvc::~CInitSvc(void)
{
	//theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CInitSvc::OnExceptionNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::OnStart()
{
	// 显示初始画面组
	__super::OnStart();

	// 强制刷新屏幕
	m_pDialogFlow->GetDialog(CInitDlg::IDD)->UpdateWindow();		

	// 显示等待光标
	theCURSOR_MGR.BeginWaitCursor();

	// 设置日志目录
	CString tracePath = theTVM_INFO.GetLogPath();
	CLog::Initialize(tracePath);
	LOG("----------------------- TVM Start run-------------------");
	bool bInitOK = Initialize();

	// 暂停，保留画面上的显示结果
	Sleep(2000);

	// 结束等待光标
	theCURSOR_MGR.EndWaitCursor();

	/*theSERVICE_MGR.SetState(MAINTENANCE);
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
	theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);*/
	//if (bInitOK && (!theEXCEPTION_MGR.HasException())) { 
	//	// 启动Login画面
	//	theSERVICE_MGR.StartService(LOGIN_SVC);
	//}
	//// 初期化失败
	//else {          
	//	// 显示主菜单
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);
	//	//theMENU_MGR.SetMenuKind(CMenuMgr::MENU_INIT_ABNORMAL);
	//}

	// 进入终止中状态
	theSERVICE_MGR.SetState(OUT_OF);
	// 启动默认服务
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();	
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
}

//////////////////////////////////////////////////////////////////////////
/**	mei_xiaowei
@brief       锁定数字键盘

@param     BOOL TRUE 锁顶
FALSE 解锁

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
static void SetNumLock( BOOL bState )
{
	BYTE keyState[256];

	GetKeyboardState((LPBYTE)&keyState);
	if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
		(!bState && (keyState[VK_NUMLOCK] & 1)) )
	{
		keybd_event( VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0 );

		keybd_event( VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
			0);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化

@param      无

@retval     bool, true:OK; false:FAIL

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::Initialize()
{
	bool ret = true;
	int DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// 获取设备类型

	// 初始化控制器
	if(!InitializeController()){
		return false;
	}

	// 启动后台service
	StartBackService();

	// 创建目录之类初始化
	LOG("[Initialize] CheckConsistent");
	if (!CheckConsistent()) {
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
		ret = false;
	}
	else {

	}

	/*int id = 100898;
	CString pwd(_T("000000"));
	theWEBSERVER_HELPER.ServerLogin(id,pwd,theMAINTENANCE_INFO.GetDeviceType(),theMAINTENANCE_INFO.GetStationCodeWithLine());
	*/

	//MessageBoxA(NULL, "", "", 0);

	bool bIsInitParameterOk = false;
	// 磁盘检测
	LOG("[Initialize] CheckDiskFreeSpace");
	if(!CheckDiskFreeSpace()){
		ret = false;
	}
	else{
		SetInitRessult(ROW_DISK_CHECK, INIT_RESULT::SUCCESS);   // 设置初始化结果（磁盘检测）
	}
	// 数据初始化
	LOG("[Initialize] InitializeParameter");
	if (!InitializeParameter()) {
		ret = false;
	}
	else {
		bIsInitParameterOk = true;
		SetInitRessult(ROW_DATA_INIT, INIT_RESULT::SUCCESS);   // 设置初始化结果（数据一致性检查）
	}
	// 显示数据初始化结果
	if(bIsInitParameterOk){
		SetInitRessult(ROW_DATA_INIT,INIT_RESULT::SUCCESS);		
	}


	//MessageBoxA(NULL,NULL,NULL,1);
	// IO模块
	
	LOG("[Initialize] InitializeIO");
	if(!InitializeIO()) {
		SetInitRessult(ROW_IO_MODULE, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_IO_MODULE, INIT_RESULT::SUCCESS);
	}
	

	//充值读写器
	LOG("[Initialize] InitializeChargeRW");
	if(!InitializeChargeRW()) {
		SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
		ret = false;
	}
	else {
		//// 待确认
		//CheckSAMidChange(RECHARGE_RW_YPT_ISAM);
		//CheckSAMidChange(RECHARGE_RW_YPT_PSAM);
		//CheckSAMidChange(RECHARGE_RW_YKT_ISAM);
		//CheckSAMidChange(RECHARGE_RW_YKT_PSAM);
		SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::SUCCESS);
	}

	// Card读写器
	LOG("[Initialize] InitializeCardRW");
	if(!InitializeCardRW()) {
		SetInitRessult(ROW_CARD_RW, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
		ret = false;
	}
	else {
		// 待确认
		CheckSAMidChange(CARD_RW_YPT_ISAM);
		CheckSAMidChange(CARD_RW_YPT_PSAM);
		CheckSAMidChange(CARD_RW_YKT_ISAM);
		CheckSAMidChange(CARD_RW_YKT_PSAM);
		SetInitRessult(ROW_CARD_RW, INIT_RESULT::SUCCESS);
	}

	// Card模块
	if(!InitializeCardModule()) {
		SetInitRessult(ROW_CARD_MODULE, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_CARD_MODULE, INIT_RESULT::SUCCESS);
	}
	
	// 硬币模块
	LOG("[Initialize] InitializeCoinModule");
	if(!InitializeCoinModule()) {
		SetInitRessult(ROW_COIN_MODULE, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_COIN_MODULE, INIT_RESULT::SUCCESS);
	}
	// 纸币模块
	LOG("[Initialize] InitializeBanknoteModule");
	if(!InitializeBanknoteModule()) {
		SetInitRessult(ROW_BANKNOTE_MODULE, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_BANKNOTE_MODULE, INIT_RESULT::SUCCESS);
	}
	
	// 凭证打印机
	LOG("[Initialize] InitializePrinter");
	if (!InitializePrinter()) {
		theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_ERR);

		SetInitRessult(ROW_PRINT, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_PRINT_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_PRINT, INIT_RESULT::SUCCESS);
	}
	
	//初始化状态显示屏
	LOG("[Initialize] InitializeStatusDisplay");
	if(!InitializeStatusDisplay())
	{
		SetInitRessult(ROW_DISPLAY, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_STATUS_DISPLAY_ERROR);
		ret = false;
	}else
	{
		SetInitRessult(ROW_DISPLAY, INIT_RESULT::SUCCESS);
	}
	//// 初始化完毕，设置初次运行标识为false.
	//LOG("[Initialize] Initialize finish");
	//theAPP_SESSION.SetIsFirstRun(false);
	m_pDialogFlow->GetDialog(CInitDlg::IDD)->UpdateWindow();

	// 开启设备监控
	StartDeviceMonitor();

	// 若存在更新程序则关机更新程序
	bool needUpgrade = theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade();
	if(needUpgrade)	{
		LOG("[Initialize] TPU need update, restart!");
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);	
		return false;
	}

	// 初始化上位
	LOG("[Initialize] link SC!");
	if (InitializeCPS() != true){
		ret = false;
	}

	// 启动二维码订单本地处理定时器
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_WEB_IDLE_PROCESS,NULL,NULL);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RECHARGE_SERVER_TIMER,NULL,NULL);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_PARAMETER_UPDATE_TIMER,NULL,NULL);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动设备监控线程

@param      (i)int index
@param      (i)INIT_RESULT result

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::StartDeviceMonitor()
{
	// 启用充值读写器设备监控
	if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
		theSERVICE_MGR.StartService(MONITOR_RECHARGE_RW_SVC,false);
		if(!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)){
			theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();
		}
	}
	// 启用读写器设备监控
	if(theTVM_INFO.IsRWMonitorOpened(TVM_CARD_RW)){
		theSERVICE_MGR.StartService(MONITOR_CARD_RW__SVC,false);
		if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
			theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_CARD_RW__SVC)->Resume();
		}
	}

	// 启用密码键盘读写器设备监控
	// 	if(theTVM_INFO.IsPinPadMonitorOpened()){
	// 		theSERVICE_MGR.StartService(MONITOR_PINPAD_MODULE_SVC,false);
	// 		if(!theEXCEPTION_MGR.HasException(CTVMPinPadException::MODULE_ID)){
	// 			theSERVICE_MGR.GetService<CPinPadMonitorSvc>(MONITOR_PINPAD_MODULE_SVC)->Resume();
	// 		}
	// 	}
	/*if(theTVM_INFO.IsRWMonitorOpened(false)){
	theSERVICE_MGR.StartService(MONITOR_TW_SVC);
	if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
	theSERVICE_MGR.GetService<CTWMonitorSvc>(MONITOR_TW_SVC)->Resume();
	}
	}*/
	/*if(theTVM_INFO.IsTHMonitorOpened()){
	theSERVICE_MGR.StartService(MONITOR_TH_SVC);
	if(!theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID)){
	theSERVICE_MGR.GetService<CTHMonitorSvc>(MONITOR_TH_SVC)->Resume();
	}
	}*/
	/*if(theTVM_INFO.IsUPSMonitorOpened()){
	theSERVICE_MGR.StartService(MONITOR_UPS_SVC);
	if(!theEXCEPTION_MGR.HasException(CUPSException::MODULE_ID)){
	theSERVICE_MGR.GetService<CUPSMonitorSvc>(MONITOR_UPS_SVC)->Resume();
	}
	}*/
	// 如果允许开启Banknote监控，则启动Banknote监控计时器
	if(theTVM_INFO.IsBHMonitorOpened())
	{
		theSERVICE_MGR.StartService(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC,false);
		if(!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID))
		{
			theSERVICE_MGR.GetService<CBanknoteCollectionModuleMonitorSvc>(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC)->Resume();
		}
	}

	if(DEVICE_TYPE_TVM == theMAINTENANCE_INFO.GetDeviceType())
	{
		// 如果允许开启Coin监控，则启动Coin监控计时器
		if(theTVM_INFO.IsCHMonitorOpened())
		{
			theSERVICE_MGR.StartService(MONITOR_COIN_MODULE_SVC,false);
			if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID))
			{
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Resume();
			}
		}
	}

	// 如果允许开启IO监控，则启动IO监控计时器
	if(theTVM_INFO.IsIOMonitorOpened()){
		if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			theSERVICE_MGR.StartService(MONITOR_IO_SVC,false);
			theSERVICE_MGR.GetService<CIOMonitorSvc>(MONITOR_IO_SVC)->Resume();
			theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);

		}
	}
	// 如果允许开启打印机监控，则启动RP监控计时器
	if (theTVM_INFO.IsPrinterMonitorOpened())
	{
		theSERVICE_MGR.StartService(MONITOR_PRINTER_MODULE_SVC,false);
		if (!theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID))
		{
			theSERVICE_MGR.GetService<CPrinterMonitorSvc>(MONITOR_PRINTER_MODULE_SVC)->Resume();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置初期化结果

@param      (i)int index
@param      (i)INIT_RESULT result

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::SetInitRessult(int index, INIT_RESULT result)
{
	m_InitialInfo[index].result = result;

	// 刷新Dialog
	CInitDlg* pInitialDlg = (CInitDlg*)(m_pDialogFlow->GetDialog(CInitDlg::IDD));
	pInitialDlg->ShowInitRessult(index);
	//Sleep(100);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化Controller

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeController()
{

	// 创建FTP控制器
	g_pFTPControl = new CFTPControl();
	ASSERT(g_pFTPControl != NULL);
	g_pFTPControl->Initialize();

	// 创建数据库控制器
	g_pDataControl = new CDataControl();
	ASSERT(g_pDataControl != NULL);
	g_pDataControl->Initialize();

	// 创建SC通信控制器
	g_pSCControl = new CSCControl();
	ASSERT(g_pSCControl != NULL);
	g_pSCControl->Initialize();

	// 创建卡式出票机控制器
	g_pMIMControl = new CMIMControl();
	ASSERT(g_pMIMControl != NULL);
	g_pMIMControl->Initialize();
	//	g_pMemControl = new CMemController(100);
	//	ASSERT(g_pMemControl != NULL);
	//	g_pMIMControl->SetMemController(g_pMemControl);

	// 创建读写器控制器
	g_pTVMRWControl = new CRWControl();
	g_pTVMRWControl->Initialize();

	//// 创建售卡读写器控制器
	//g_pCardRWControl = new CRWControl();
	//g_pCardRWControl->Initialize();

	//// 创建TOKEN读写器控制器
	//g_pTokenRWControl = new CRWControl();
	//g_pTokenRWControl->Initialize();

	//// 创建充值读写器控制器
	//g_pChargeRWControl = new CRWControl();
	//g_pChargeRWControl->Initialize();

	// 创建打印机控制器
	//g_pPrinterControl = new CTVMPrinterControl();
	//g_pPrinterControl->Initialize();

	// 创建硬币模块控制器
	g_pCHControl = new CCHControl();
	g_pCHControl->Initialize();

	// 创建纸币模块控制器
	g_pBHControl = new CBRHControl();
	g_pBHControl->Initialize();

	// 创建纸币找零模块控制器
	g_pBHChangeControl = new CBHChangeControl();
	g_pBHChangeControl->Initialize();
	// 创建Token出票机控制器
	//g_pTokenControl = new CTokenControl();
	//g_pTokenControl->Initialize();

	//// 创建IO控制器
	//g_pIOControl = new CIOControl();
	//g_pIOControl->Initialize();

	// 创建密码键盘控制器
	//g_pPinpadControl = new CTVMPinPadControl();
	//g_pPinpadControl->Initialize();

	// 创建银行卡处理模块控制器
	//g_pBankCardControl = new CTVMBankCardControl();
	//g_pBankCardControl->Initialize();

	// 创建状态显示屏模块控制器
	g_pStatusDisplayControl = new CStatusDisplayControl();
	g_pStatusDisplayControl->Initialize();

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化后台service

@param    none

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::StartBackService()
{
	// 启动后台service
	theSERVICE_MGR.StartService(SC_SVC,false);
	theSERVICE_MGR.StartService(BUSINESS_INTERVAL_SVC,false);
	theSERVICE_MGR.StartService(EVENT_MSG_SVC,false);
	theSERVICE_MGR.StartService(TRANSMISSION_INTERVAL_SVC,false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       数据一致性检查

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckConsistent()
{
	bool bRet = false;
	try {
		// 初期化系统配置文件
		if(InitializeSystemConfig()){
			// 初始化系统目录
			if(InitializeSystemFolder()){
				// 初始化系统设置
				if(InitializeOSSetting()){
					//if(CheckDiskFreeSpace())
					//{
					bRet = true;
					//}
				}
			}
		}
	}
	catch (CSysException& e) {
		//DoException(e,ROW_DISK_CHECK);
	}
	catch (...) {
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查检查磁盘空间

@param      无

@retval     无

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckDiskFreeSpace()
{
	try{
		// 检查磁盘空间
		ULARGE_INTEGER freeBytes,totalBytes,totalFreeBytes;

		// 主数据目录
		if(::GetDiskFreeSpaceEx(theTVM_INFO.GetDataPath(),&freeBytes,&totalBytes,&totalFreeBytes)){
			if(totalFreeBytes.QuadPart/(1024*1024) > theAFC_TVM_OPERATION.GetSystemDiskFullValue()/*theTVM_INFO.GetDataDirectoryStop()*/ 
				&& totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetSystemDiskNearFullValue()/*theTVM_INFO.GetDataDirectoryWarn()*/){
					LOG("[Initialize] CheckDiskFreeSpace:Low Space!(%s)", theTVM_INFO.GetDataPath());
					CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
					CString stm = _opl(TXT_INI_DISK_WARN);
					m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
					m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
					SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
					throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_LOWSPACE,
						_T(__FILE__),__LINE__);
					return false;
			}
			else if(totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetSystemDiskFullValue()/*theTVM_INFO.GetDataDirectoryStop()*/){
				LOG("[Initialize] CheckDiskFreeSpace:No Space!(%s)", theTVM_INFO.GetDataPath());
				CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
				CString stm = _opl(TXT_INI_DISK_STOP);
				m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
				m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
				SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
				theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
				throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,
					_T(__FILE__),__LINE__);
				return false;
			}
		}
		else{
			LOG("[Initialize] CheckDiskFreeSpace:Error---%d!(%s)", GetLastError(), theTVM_INFO.GetDataPath());
			CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
			CString stm = _opl(TXT_INI_DISK_STOP);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
			theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
			throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,
				_T(__FILE__),__LINE__);
			return false;
		}

		// 备份数据目录
		if(::GetDiskFreeSpaceEx(theTVM_INFO.GetBackupPath(),&freeBytes,&totalBytes,&totalFreeBytes)){
			if(totalFreeBytes.QuadPart/(1024*1024) > theAFC_TVM_OPERATION.GetDataDiskFullValue()/*theTVM_INFO.GetBackupDirectoryStop()*/ 
				&& totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetDataDiskNearFullValue()/*theTVM_INFO.GetBackupDirectoryWarn()*/){
					LOG("[Initialize] CheckDiskFreeSpace:Low Space!(%s)", theTVM_INFO.GetBackupPath());
					CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
					CString stm = _opl(TXT_INI_DISK_WARN);
					m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
					m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
					SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
					throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_LOWSPACE,
						_T(__FILE__),__LINE__);
					return false;
			}
			else if(totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetDataDiskFullValue()/*theTVM_INFO.GetBackupDirectoryStop()*/){
				LOG("[Initialize] CheckDiskFreeSpace:No Space!(%s)", theTVM_INFO.GetBackupPath());
				CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
				CString stm = _opl(TXT_INI_DISK_STOP);
				m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
				m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
				SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
				theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
				throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,
					_T(__FILE__),__LINE__);
				return false;
			}
		}
		else{
			LOG("[Initialize] CheckDiskFreeSpace:Error---%d!(%s)", GetLastError(), theTVM_INFO.GetBackupPath());
			CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
			CString stm = _opl(TXT_INI_DISK_STOP);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
			theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
			throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,
				_T(__FILE__),__LINE__);
			return false;
		}

		// 日志文件目录
		if(::GetDiskFreeSpaceEx(theTVM_INFO.GetLogPath(),&freeBytes,&totalBytes,&totalFreeBytes)){
			if(totalFreeBytes.QuadPart/(1024*1024) > theAFC_TVM_OPERATION.GetLogDiskFullValue()/*theTVM_INFO.GetLogDirectoryStop()*/ 
				&& totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetLogDiskNearFullValue()/*theTVM_INFO.GetLogDirectoryWarn()*/){
					LOG("[Initialize] CheckDiskFreeSpace:Low Space!(%s)", theTVM_INFO.GetLogPath());
					CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
					CString stm = _opl(TXT_INI_DISK_WARN);
					m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
					m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
					SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
					throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_LOWSPACE,_T(__FILE__),__LINE__);
					return false;
			}
			else if(totalFreeBytes.QuadPart/(1024*1024) <= theAFC_TVM_OPERATION.GetLogDiskFullValue()/*theTVM_INFO.GetLogDirectoryStop()*/){
				LOG("[Initialize] CheckDiskFreeSpace:No Space!(%s)", theTVM_INFO.GetLogPath());
				CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
				CString stm = _opl(TXT_INI_DISK_STOP);
				m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
				m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
				SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
				theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
				throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,_T(__FILE__),__LINE__);
				return false;
			}
		}
		else{
			LOG("[Initialize] CheckDiskFreeSpace:Error---%d!(%s)", GetLastError(), theTVM_INFO.GetLogPath());
			CString msg = _opl(TXT_INI_DISK_NEEDCHECK);
			CString stm = _opl(TXT_INI_DISK_STOP);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			SetInitRessult(ROW_DISK_CHECK,INIT_RESULT::FAIL);
			theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
			throw CDiskException((CDiskException::DETAIL_ERROR_CODE)CDiskException::UE_DISK_NOSPACE,_T(__FILE__),__LINE__);
			return false;
		}
	}
	catch (CSysException& e){
		DoException(e,ROW_DISK_CHECK);
		return false;
	}
	return true;
}

typedef bool (CInitSvc::*INIFUNC)();
typedef struct _tag_param
{
	INIFUNC		pIniFunc;
	bool		bret;
	HANDLE		hEvent;
	CInitSvc*	pIniSvc;
	_tag_param()
	{
		bret=true;
	}
}PARAMINFO;
unsigned int _stdcall iniParameter(void* lparam)
{
	PARAMINFO* ptemp=(PARAMINFO*)lparam;
	ptemp->bret=(ptemp->pIniSvc->*(ptemp->pIniFunc))();
	::SetEvent(ptemp->hEvent);
	_endthreadex(0);
	return 0;
}
void iniProcess(PARAMINFO* paramInfo)
{
	unsigned int ithreadID=0;
	HANDLE hThread=(HANDLE)_beginthreadex(NULL,0,iniParameter,paramInfo,0,&ithreadID);
	CloseHandle(hThread);
	hThread=NULL;
}
#define UD_COUNT 8

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化系统参数

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeParameter()
{
	bool bRes = true;
	try{

		DWORD dw1=GetTickCount();
		// 初始化系统日志
		if(!InitializeSystemLog()){
			LOG("[Initialize] InitializeSystemLog failed!");
			bRes = false;	
		}

		// 初始化异常数据
		if(!InitializeSystemException()){
			LOG("[Initialize] InitializeSystemException failed!");
			bRes = false;	
		}

		// 初始化MASTER
		if(!InitializeSystemMaster()){
			LOG("[Initialize] InitializeSystemMaster failed!");
			bRes = false;	
		}

		// 初始化交易数据
		if(!InitializeTxnData()){
			LOG("[Initialize] InitializeTxnData failed!");
			bRes = false;	
		}

		// 初始化参数文件
		if(!InitializeParamData()){
			LOG("[Initialize] InitializeParamData failed!");
			bRes = false;	
		}

		// 初始化数据库
		if(!InitializeDatabase()){
			LOG("[Initialize] InitializeDatabase failed!");
			bRes = false;	
		}
		// 初始化寄存器数据
		if(!InitializeRegistAudit()){
			LOG("[Initialize] InitializeRegistAudit failed!");
			bRes = false;	
		}
		// 初始化系统时间
		if(!InitializeSystemTime()){
			LOG("[Initialize] InitializeSystemTime failed!");
			bRes = false;	 
		}
		// 初始化审计数据
		if(!InitializeSCAudit()){
			LOG("[Initialize] InitializeSCAudit failed!");
			bRes = false;	
		}

		// 同步交易、日志、已送数据保留天数
		theTVM_INFO.SetStoreDay(theAFC_TVM_OPERATION.GetMaxLogSaveDays());

		theFunction_INFO.SetLimitedDaysOfSendData(theAFC_TVM_OPERATION.GetSendedMsgSaveDays());

		theTVM_SETTING.SetMaxOffLineDay(theAFC_TVM_OPERATION.GetOfflineWorkDays());
		// 判断是否超过最大离线天数
		try{
			_DATE dtDay	= theTVM_SETTING.GetLastOnlineDate();
			CTime lastTime(dtDay.wYear,dtDay.biMonth,dtDay.biDay,0,0,0,-1);
			WORD  wDay	= theTVM_SETTING.GetMaxOffLineDay();//theAFC_TVM_OPERATION.GetMaxOffLineTime();

			CTimeSpan timeSpan = CTime::GetCurrentTime() - lastTime;
			// 超过最大离线天数
			if(wDay < timeSpan.GetDays() && wDay != 0){
				throw CSCException(CSCException::NOT_CONNECT_SERVER_OUT_OF_TIME,_T(__FILE__),__LINE__);
			}
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}

	}
	catch(CSysException& e)	{
		bRes = false;
		DoException(e,ROW_DATA_INIT);
	}
	catch(...)	{
		bRes = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_PARAM_ERROR));
		DoException(e,ROW_DATA_INIT);
	}
	return bRes;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化状态条屏

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeStatusDisplay()
{
	try
	{	
		theSTATUS_DISPLAYER.Connect();
		//if(TRUE/*theAPP_SESSION.IsFirstRun()*/){
		theSTATUS_DISPLAYER.Init();
		//}
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		//DoException(e,ROW_PRINT);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_RECEIPT_PRINTER_MODULE_ERROR));
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化Card读写器

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCardRW()
{
	try {
		CARDRW_HELPER->InitCheck();
		CARDRW_HELPER->CheckDeviceExchange();

		// 硬件参数更新
		CARDRW_HELPER->InitData();
		//CARDRW_HELPER->SynchronismParam();

		//// 同步当前站点到全网最高票价
		//RW_CMD_TICKET_PRICE query;
		//RW_CMD_TICKET_PRICE_RES result;
		//
		//query.CardType = CARD_TYPE_ACC_UL;
		//query.ProductCategory = PURSE;
		//query.ProductType = SJT;
		//query.ProductSubType = 0;
		//query.PriceType = 0x01; 
		//query.Time = ComGetCurTime();

		//WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
		//memcpy(query.BegainStation, &currentStation, 4);
		//WORD uncurrentStation = 0xFFFF;
		//memcpy(query.EndStation,&uncurrentStation, 4);
		//
		//long resultCode = CARDRW_HELPER->GetTicketPrice(query, result);
		//if(resultCode == RW_RSP_OK){
		//	theFunction_INFO.SetTicketMaxAmount(result.TicketPrice);
		//}

		//// 判断卡槽4是否有SAM卡，否则抛异常
		//SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

		//BYTE PSAMid[6];
		//memset(PSAMid,0,sizeof(PSAMid));
		//if(memcmp(samID.th_acc_psam_id,PSAMid,sizeof(PSAMid)) == 0){
		//	theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
		//	//throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
		//}
		//// 上报SAM卡状态
		//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
	}
	catch (CSysException& e){
		DoException(e,ROW_CARD_RW);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_CARD_RW_ERROR));
		return false;
		DoException(e,ROW_CARD_RW);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化Token读写器

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//bool CInitSvc::InitializeTokenRW()
//{
//	try {
//		TOKENRW_HELPER->InitCheck();
//		TOKENRW_HELPER->CheckDeviceExchange();
//
//		// 硬件参数更新
//		TOKENRW_HELPER->InitData();
//		TOKENRW_HELPER->SynchronismParam();
//
//		// 判断卡槽4是否有SAM卡，否则抛异常
//		SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();
//
//		BYTE ISAMid[6];
//		memset(ISAMid,0,sizeof(ISAMid));
//		if(memcmp(samID.rw_acc_isam_id,ISAMid,sizeof(ISAMid)) == 0){
//			theTVM_STATUS_MGR.SetSam3Status(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
//			throw CRWHardwareException(RW_TYPE::TVM_TOKEN_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
//		}
//		theTVM_STATUS_MGR.SetSam3Status(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
//	}
//	catch (CSysException& e){
//		DoException(e,ROW_TOKEN_RW);
//		return false;
//	}
//	catch(...)	{
//		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_TOKEN_RW_ERROR));
//		return false;
//		DoException(e,ROW_TOKEN_RW);
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化充值读写器

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeChargeRW()
{
	try {
		RECHARGERW_HELPER->InitCheck();
		RECHARGERW_HELPER->CheckDeviceExchange();

		// 硬件参数更新
		RECHARGERW_HELPER->InitData();
		//RECHARGERW_HELPER->SynchronismParam();

		//// 判断卡槽4是否有SAM卡，否则抛异常
		//SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

		//BYTE ISAMid[6];
		//memset(ISAMid,0,sizeof(ISAMid));
		//if(memcmp(samID.rw_acc_isam_id,ISAMid,sizeof(ISAMid)) == 0){
		//	theTVM_STATUS_MGR.SetSam1Status(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
		//	throw CRWHardwareException(RW_TYPE::TVM_RECHARGE_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
		//}
		//// 向上位报状态
		//theTVM_STATUS_MGR.SetSam1Status(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
	}
	catch (CSysException& e){
		DoException(e,ROW_CHARGE_RW);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_RECHARGE_RW_ERROR));
		return false;
		DoException(e,ROW_CHARGE_RW);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       充值终端授权（一卡通）认证

@param       无

@retval      INIT_RESULT    认证结果

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
// CInitSvc::INIT_RESULT CInitSvc::BMACAccredit()
// {
//     INIT_RESULT result = CInitSvc::ACCREDIT_FAIL;
// 
//     return result;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief       ACC授权（一票通）认证

@param       无

@retval      INIT_RESULT    认证结果

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
// CInitSvc::INIT_RESULT CInitSvc::ACCAccredit()
// {
//     INIT_RESULT result = ACCREDIT_FAIL;
// 
//     return result;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化打印机(Printer)

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializePrinter()
{
	try {
		//PRINTER_HELPER->PrinterClose();
		PRINTER_HELPER->PrinterConnect();
		//PRINTER_HELPER->CheckStatus();
	}
	catch (CSysException& e){
		DoException(e,ROW_PRINT);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_RECEIPT_PRINTER_MODULE_ERROR));
		DoException(e,ROW_PRINT);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化IO模块

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeIO()
{
	try {
		// 初期化
		IO_HELPER->OpenIO();
		IO_HELPER->InitializeIO();
		// 检测一下IO状态
		IO_RSP_STATUS_INFO ioStatus;
		IO_HELPER->GetIOStatus(ioStatus);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_IO_MODULE);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_IO_MODULE_ERROR));
		DoException(e,ROW_IO_MODULE);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化UPS

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//bool CInitSvc::InitializeUPS()
//{
//	try {
//		//m_UPSHelper->UPSOpen();
//		return true;
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	catch (...) {
//		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_UPS_ERROR));
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//
//	return false;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化上位(CPS)

@param       无

@retval      INIT_RESULT    认证结果

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCPS()
{
	if(TRUE == theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_CONNECT,NULL,NULL)){
		return true;
	}
	else{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_UPPER_FAILURE))); ;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查创建数据主目录

@param      无

@retval     无

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckCreateHomeFolder()
{
	// 检查并创建数据主目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath())) {    // not exist
		return false;
	}
	// 检查并创建交易数据目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}

	// 检查并创建master文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建参数文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建收益文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}	
	// 检查并创建图片文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PHOTO_MAIN_DIR)) {
		return false;
	}
	// 检查并创建临时图片文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PHOTO_TEMP_DIR)) {
		return false;
	}
	// 删除临时图片
	DeletePicture();
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查创建备份目录

@param      无

@retval     无

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckCreateBackupFolder()
{
	// 检查并创建数据主目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath())) {    // not exist
		return false;
	}
	// 检查并创建交易数据目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建master文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建ACC参数文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建收益文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建审计文件目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + AR_MANAGE_FOLDER)) {    // if not exist, create it
		return false;
	}
	// 检查并创建trace目录
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetLogPath())) {    // if not exist, create it
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定天数前的FTP上传的日志

@param      (i)int days    指定天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeleteLog(int days)
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
		sDir += _T("LogZipBackup\\History");
		// 查找文件
		vector<CString> fileNames;
		SearchFile(sDir + _T("*.zip"),fileNames);
		vector<CString>::iterator iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// 取出文件名，用于比较日期
			CString dateStr = _T("");
			dateStr = ((*iter).Right(18)).Left(8);

			// 如果日期小于指定日期则删除
			if(dateStr < dayLimitStr) {
				// 删除该文件
				DeleteFile(sDir + *iter);
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
@brief      删除照片

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeletePicture()
{
	try {
		// 取得文件目录
		CString sDir = theTVM_INFO.GetDataPath() + PHOTO_MAIN_DIR;		

		// 查找文件
		vector<CString> fileNames;
		SearchFile(sDir + _T("\\*.*"),fileNames);
		vector<CString>::iterator iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// 删除该文件
			DeleteFile(*iter);
			iter ++;
		}
		sDir = theTVM_INFO.GetDataPath() + PHOTO_TEMP_DIR;	
		// 查找文件
		fileNames.clear();
		SearchFile(sDir + _T("\\*.*"),fileNames);
		iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// 删除该文件
			DeleteFile(*iter);
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
@brief       获取指定天数的log日志文件日期

@param       (i)int days			  指定天数
@param       (o)vector<int>& logdate  日期列表

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::GetLimitLogDate(int days,vector<int>& logdate)
{
	try {
		// 取得文件目录
		CString sDir = theTVM_INFO.GetLogPath();

		sDir += _T("LogZipBackup\\History");
		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".zip"));

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
/**
@brief      删除指定天数前的日志文件

@param      (i)int days    指定天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeleteTpuLog(int days)
{
	try {
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + _T("Log\\");

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
/**
@brief       获取指定天数的log日志文件日期

@param       (i)int days			  指定天数
@param       (o)vector<int>& logdate  日期列表

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::GetLimitTpuLogDate(int days,vector<CString>& logdate)
{
	try {
		// 取得文件目录
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + _T("Log\\");

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
@brief      获取异常显示信息

@param      (i)CSysException e    异常

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// CString CInitSvc::GetInitErrorInfo(CSysException e)
// {
// 	// 显示异常信息
// 	CString stm = _T("");
// 	//stm.Format(_T("%s%08X%s%s"), _T("("), e.GetActualMTC(), _T("),"), e.GetDescription());
// 	//stm = _T(",") + e.GetDescription() + _T(",") + e.GetPrompt();
// 	stm.Format(_T("%s%08X%s%s%s%s"), _T("("), e.GetActualMTC(), _T("),"), e.GetDescription(),_T(","),e.GetPrompt());
// 	return stm;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      追加未送集计

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// void CInitSvc::AddUnsendAudit()
// {
// 	
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief     处理异常

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DoException(CSysException& e,int index)
{
	CString guideMsg = theEXCEPTION_MGR.GetDescription(e);
	CString dealMsg = theEXCEPTION_MGR.GetPrompt(e);
	m_InitialInfo[index].errorInfo = guideMsg;
	m_InitialInfo[index].dealInfo = dealMsg;
	theEXCEPTION_MGR.ProcessException(e);
	SetInitRessult(index, INIT_RESULT::FAIL);
	//Sleep(2000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查SamId变化

@param      (i)bool, true:判断TH的ISAM变化;  false:判断RW的ISAM变化

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::CheckSAMidChange(SAM_POSITION samPos)
{
	// SAMid发生改变
	DWORD samIdNo = 0;
	// ISAM如果发生改变
	SAM_ID_INFO samId;
	samId = theAPP_SESSION.GetSAMInfo();
	BYTE nosam[6] = {0};
	CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
	theDeviceInfo.GetDeviceInfo(deviceInfo);
	//theDEBUG_LOG.WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(""));
	switch (samPos) {
	case RECHARGE_RW_YPT_ISAM:
		//外部一票通ISAM
		if (memcmp(samId.rw_acc_isam_id,nosam,6) != 0 && memcmp(samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM,6) != 0) {
			// 发送设备部件更换电文
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM);
			// 更新master文件
			theDeviceInfo.SetACC_RW1_ISAM(samId.rw_acc_isam_id);
		}	
		break;
	case RECHARGE_RW_YPT_PSAM:
		//外部一票通PSAM
		if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM,6) != 0) {
			// 发送设备部件更换电文
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM);
			// 更新master文件
			theDeviceInfo.SetACC_RW1_PSAM(samId.rw_acc_psam_id);
		}	
		break;
	case RECHARGE_RW_YKT_ISAM:
		{// ECT ISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW1_ISAM(samId.rw_ect_isam_id);
			}
		}
		break;
	case RECHARGE_RW_YKT_PSAM:
		{// ECT PSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW1_PSAM(samId.rw_ect_psam_id);
			}
		}
		break;
		//RW3
	case CARD_RW_YPT_ISAM:
		{//ACC ISAM
			if(memcmp(samId.th_acc_isam_id,nosam,6) != 0 && memcmp(samId.th_acc_isam_id,deviceInfo.ACC_RW3_ISAM,6) != 0){
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_ISAM_RW,samId.th_acc_isam_id,deviceInfo.ACC_RW3_ISAM);
				theDeviceInfo.SetACC_RW3_ISAM(samId.th_acc_isam_id);
			}
		}
		break;
	case CARD_RW_YPT_PSAM:
		{//ACC PSAM
			if(memcmp(samId.th_acc_psam_id,nosam,6) != 0 && memcmp(samId.th_acc_psam_id,deviceInfo.ACC_RW3_PSAM,6) != 0){
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_PSAM_RW,samId.th_acc_psam_id,deviceInfo.ACC_RW3_PSAM);
				theDeviceInfo.SetACC_RW3_PSAM(samId.th_acc_psam_id);
			}
		}
		break;
	case CARD_RW_YKT_ISAM:
		{//ETC ISAM
			if(memcmp(samId.th_ect_isam_id,nosam,6) != 0 && memcmp(samId.th_ect_isam_id,deviceInfo.ECT_RW3_ISAM,6) != 0){
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_ISAM_RW,samId.th_ect_isam_id,deviceInfo.ECT_RW3_ISAM);
				theDeviceInfo.SetETC_RW3_ISAM(samId.th_ect_isam_id);
			}
		}
		break;
	case CARD_RW_YKT_PSAM:
		{//ETC PSAM
			if(memcmp(samId.th_ect_psam_id,nosam,6) != 0 && memcmp(samId.th_ect_psam_id,deviceInfo.ECT_RW3_PSAM,6) != 0){
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_PSAM_RW,samId.th_ect_psam_id,deviceInfo.ECT_RW3_PSAM);
				theDeviceInfo.SetETC_RW3_PSAM(samId.th_ect_psam_id);
			}
		}
		break;
		// RW2
	case TOKEN_RW_YPT_ISAM:
		{//ACC ISAM
			if (memcmp(samId.rw_acc_isam_id,nosam,6) != 0 && memcmp(samId.rw_acc_isam_id,deviceInfo.ACC_RW2_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW2_ISAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW2_ISAM(samId.rw_acc_isam_id);
			}	
		}
		break;
	case TOKEN_RW_YPT_PSAM:
		{//ACC PSAM
			if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW2_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW2_PSAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW2_PSAM(samId.rw_acc_psam_id);
			}	
		}
		break;
	case TOKEN_RW_YKT_ISAM:
		{//ETC ISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW2_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW2_ISAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW2_ISAM(samId.rw_ect_isam_id);
			}
		}
		break;
	case TOKEN_RW_YKT_PSAM:
		{//ETC PSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW2_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW2_PSAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW2_PSAM(samId.rw_ect_psam_id);
			}
		}
		break;
		//case TH_YPT_ISAM:
		//	// 内部一票通ISAM
		//	if (memcmp(samId.th_acc_isam_id,nosam,6) != 0 && memcmp(samId.th_acc_isam_id,deviceInfo.ACC_TH_ISAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.th_acc_isam_id,deviceInfo.ACC_TH_ISAM);
		//		// 更新master文件
		//		theDeviceInfo.SetACC_TH_ISAM(samId.th_acc_isam_id);
		//	}	
		//	break;
		//case TH_YPT_PSAM:
		//	// 内部一票通PSAM
		//	if (memcmp(samId.th_acc_psam_id,nosam,6) != 0 && memcmp(samId.th_acc_psam_id,deviceInfo.ACC_TH_PSAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.th_acc_psam_id,deviceInfo.ACC_TH_PSAM);
		//		// 更新master文件
		//		theDeviceInfo.SetACC_TH_PSAM(samId.th_acc_psam_id);
		//	}	
		//	break;
		//case RW_YKT_ISAM:
		//	// 外部一卡通ISAM
		//	if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW_ISAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW_ISAM);
		//		// 更新master文件
		//		theDeviceInfo.SetECT_RW_ISAM(samId.rw_ect_isam_id);
		//	}
		//	break;
		//case RW_YKT_PSAM:
		//	// 外部一卡通PSAM
		//	if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW_PSAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW_PSAM);
		//		// 更新master文件
		//		theDeviceInfo.SetECT_RW_PSAM(samId.rw_ect_psam_id);
		//	}
		//	break;
		//case TH_YKT_ISAM:
		//	// 内部一卡通ISAM
		//	if (memcmp(samId.th_ect_isam_id,nosam,6) != 0 && memcmp(samId.th_ect_isam_id,deviceInfo.ECT_TH_ISAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.th_ect_isam_id,deviceInfo.ECT_TH_ISAM);
		//		// 更新master文件
		//		theDeviceInfo.SetECT_TH_ISAM(samId.th_ect_isam_id);
		//	}
		//	break;
		//case TH_YKT_PSAM:
		//	// 内部一卡通PSAM
		//	if (memcmp(samId.th_ect_psam_id,nosam,6) != 0 && memcmp(samId.th_ect_psam_id,deviceInfo.ECT_TH_PSAM,6) != 0) {
		//		// 发送设备部件更换电文
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.th_ect_psam_id,deviceInfo.ECT_TH_PSAM);
		//		// 更新master文件
		//		theDeviceInfo.SetECT_TH_PSAM(samId.th_ect_psam_id);
		//	}
		//	break;
	default:
		break;
	}
}


void CInitSvc::OnExceptionNotify(CSysException& e,bool addException)
{
	if(e.GetModuleID() == CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID || e.GetModuleID() == CRWException::TVM_RECHARGE_RW_MODULE_ID)
	{
		if (!(theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) || theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))) {
			//this->SetInitRessult(CInitSvc::ROW_TOKEN_RW,CInitSvc::SUCCESS);
		}
	}
	/*if(e.GetModuleID() == CTVMPrinterException::MODULE_ID)
	{
	if (!(theEXCEPTION_MGR.HasException(CTVMPrinterException::MODULE_ID))) {
	this->SetInitRessult(CInitSvc::ROW_PRINT,CInitSvc::SUCCESS);
	}
	}*/

	if(e.GetModuleID() == CSNTPSException::MODULE_ID)
	{
		if(!(theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID)))
		{
			this->SetInitRessult(CInitSvc::ROW_DATA_INIT,CInitSvc::SUCCESS);
		}
	}
	if(!theEXCEPTION_MGR.HasException())
	{
		// 启动Login画面
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化Smart-Card模块

@param       无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCardModule()
{
	try {
		// 初期化
		CARD_HELPER->Initialize();
		//		CARD_HELPER->CheckDeviceExchange();

		//if(theFunction_INFO.IsCardNormalBoxRFIDWriteable() == 1) {
		//	CTicketBoxExchangeSvc pSvc;
		//	pSvc.CheckTicketBoxIsInvalidExchanged();	
		//}
		// RFID禁用
		//else{
		//	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfo;	
		//	BYTE byBoxIDA[4] = {0x02,0x94,0x00,0x01};
		//	ticketBoxAInfo.ticketBoxID.Deserialize(byBoxIDA);
		//	ticketBoxAInfo.ulCurCount = 500;
		//	ticketBoxAInfo.ulInitCount =  500;
		//	theCOUNT_INFO.SetTicketboxAInfo(ticketBoxAInfo);

		//	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfo;	
		//	BYTE byBoxIDB[4] = {0x02,0x94,0x00,0x02};
		//	ticketBoxBInfo.ticketBoxID.Deserialize(byBoxIDB);
		//	ticketBoxBInfo.ulCurCount = 500;
		//	ticketBoxBInfo.ulInitCount =  500;
		//	theCOUNT_INFO.SetTicketboxBInfo(ticketBoxBInfo);

		//	SC_TICKET_DATA ticketData;
		//	ticketData.wMagazineANumber = 500;
		//	ticketData.wMagazineBNumber = 500;
		//	ticketData.wCollectionBoxNumber = 0;
		//	m_pTHHelper->TicketDataWriteCMD(ticketData);
		//}
		// 初始化票卡模块数量
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxAInfo;		// token箱A
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxBInfo;		// token箱B
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token废票箱
		//CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card箱A
		//CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card废票箱
		//tokenBoxAInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
		//tokenBoxBInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
		//tokenWasteBoxInfo	= theTOKEN_COUNT.GetTokenboxWastedInfo();
		//cardBoxAInfo		= theCARD_COUNT.GetCardboxAInfo();
		//cardWasteBoxInfo	= theCARD_COUNT.GetCardboxWastedInfo();


		//SC_TICKET_DATA ticketData;
		//ticketData.wMagazineANumber = cardBoxAInfo.ulCurCount;
		//ticketData.wMagazineBNumber = 0;
		//ticketData.wCollectionBoxNumber = cardWasteBoxInfo.ulCurCount;
		//CARD_HELPER->TicketDataWriteCMD(ticketData);
		// 票箱更换检查（需要打印帐票，放在打印机初期化之后处理）
		//CTicketBoxHelper ticketBoxHelper(*this);
		//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_CARD_MODULE);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("出票机初始化错误"));
		DoException(e,ROW_CARD_MODULE);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化Token模块

@param       无

@retval      bool  true:成功;  false:失败

@exception   无
*/
////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTokenModule()
{
	//	try {
	//		// 初期化
	//		TOKEN_HELPER->Initialize();
	//		//TOKEN_HELPER->CheckTicketBoxExist();
	//
	//// 		if(theFunction_INFO.IsCardNormalBoxRFIDWriteable() == 1) {
	//// 			//CTicketBoxExchangeSvc pSvc;
	//// 			//pSvc.CheckTicketBoxIsInvalidExchanged();	
	//// 		}
	//		 //RFID禁用
	//// 		else{
	//// 			// 初始化票卡模块数量
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxAInfo;		// token箱A
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxBInfo;		// token箱B
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token废票箱
	//// 			CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card箱A
	//// 			CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card废票箱
	//// 			tokenBoxAInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
	//// 			tokenBoxBInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
	//// 			tokenWasteBoxInfo	= theTOKEN_COUNT.GetTokenboxWastedInfo();
	//// 			cardBoxAInfo		= theCARD_COUNT.GetCardboxAInfo();
	//// 			cardWasteBoxInfo	= theCARD_COUNT.GetCardboxWastedInfo();
	//// 
	//// 			
	//// 			SC_TICKET_DATA ticketData;
	//// 			ticketData.wMagazineANumber = cardBoxAInfo.ulCurCount;
	//// 			ticketData.wMagazineBNumber = 0;
	//// 			ticketData.wCollectionBoxNumber = cardWasteBoxInfo.ulCurCount;
	//// 			CARD_HELPER->TicketDataWriteCMD(ticketData);
	//// 		}
	//
	//		// 票箱更换检查（需要打印帐票，放在打印机初期化之后处理）
	//		//CTicketBoxHelper ticketBoxHelper(*this);
	//		//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
	//		return true;
	//	}
	//	catch (CSysException& e) {
	//		DoException(e,ROW_TOKEN_MODULE);
	//		return false;
	//	}
	//	catch (...) {
	//		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_TOKEN_MODULE_ERROR));
	//		DoException(e,ROW_TOKEN_MODULE);
	//		return false;
	//	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化纸币模块

@param       无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeBanknoteModule()
{
	bool bRet = true;
	try {
		// 初期化
		BANKNOTE_HELPER->Init();
	}
	catch (CSysException& e) {
		bRet = false;
		DoException(e,ROW_BANKNOTE_MODULE);		
	}
	catch (...) {
		bRet = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_BANKNOTE_MODULE_ERROR));
		DoException(e,ROW_BANKNOTE_MODULE);		
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化纸币找零模块

@param      无

@retval     bool true:成功； false：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeBanknoteChangeModule(){
	bool bRet = true;
	// 纸币找零模块RFID
	//if(theFunction_INFO.IsBanknoteChangeBoxIDRead()){
	//	try{
	//		BH_RFID_HELPER->initialize();
	//	}
	//	catch(CSysException& e){
	//		theEXCEPTION_MGR.ProcessException(e);
	//	}
	//	catch(...){

	//	}
	//}

	try{
		//BH_CHANGE_HELPER->initialize();
	}
	catch(CSysException& e){
		bRet = false;
		//DoException(e,ROW_BANKNOTE_CHANGE_MODULE);
	}
	catch(...){
		bRet = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_BANKNOTE_MODULE_ERROR));
		//DoException(e,ROW_BANKNOTE_CHANGE_MODULE);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化硬币模块

@param       无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCoinModule()
{
	try {		
		COIN_HELPER->CH_InitModule();
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_COIN_MODULE);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_COIN_MODULE_ERROR));
		DoException(e,ROW_COIN_MODULE);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统日志

@param       无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemLog()
{
	bool bRet =true;
	// 初始化日志
	try{
		// 设置日志目录
		CString tracePath = theTVM_INFO.GetLogPath();
		//CLog::Initialize(tracePath);20180309,lichao:日志的初始化放到所有初始化之前

		// 初始化硬件模块通信日志
		USES_CONVERSION;
		//MIMTrs_Initialize(T2A(tracePath));        // Th Trace (单独处理)
		//BHTrs_Initialize(T2A(tracePath));			// 纸币接收模块日志
		BHChangeTrs_Initialize(T2A(tracePath));		// 纸币找零模块日志
		//BHChangeRfidTrs_Initialize(T2A(tracePath));
		CHTrs_Initialize(T2A(tracePath));			// 硬币模块日志
		//IOTrs_Initialize(T2A(tracePath));			// IO模块日志
		//CTSPTrs_Initialize(T2A(tracePath));			// 调屏显示器日志

		// 是否记录Debug消息
		theDEBUG_LOG->SetWriteDebug(theTVM_INFO.IsWriteDebug());

		// 删除TVM日志 
		//DeleteLog(theTVM_INFO.GetStoreDay());
		// 删除TPU日志		
		//DeleteTpuLog(theTVM_INFO.GetStoreDay());
		// 删除打印日志
		//DeletePrintHistory(theTVM_INFO.GetStoreDay());
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->DeleteLog();
	}
	catch(CSysException& e){
		bRet = false;
		DoException(e,ROW_DATA_INIT);
	}
	catch(...){
		bRet = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_LOG_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统异常

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemException()
{
	bool bRet = true;
	// 初始化异常数据
	int ret = theExceptionInfo.GetExceptionDetail();
	if(ret != 0){
		bRet = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统配置

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemConfig() 
{
	bool bRet = true;
	try {
		// 初期化主INI文件
		int initResult = 0;
		initResult = theTVM_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _opl(TXT_INI_REINSTALL_PROGRAM);
			CString stm = _opl(TXT_INI_SYSTEM_FILES_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}
		// 初始化系统设置INI文件
		initResult = theTVM_SETTING.Initialize();
		if (initResult != 0) {
			CString msg = _opl(TXT_INI_REINSTALL_PROGRAM);
			CString stm = _opl(TXT_INI_SYSTEM_FILES_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}

		// 初始化维护设置INI文件
		initResult = theMAINTENANCE_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _opl(TXT_INI_REINSTALL_PROGRAM);
			CString stm = _opl(TXT_INI_SYSTEM_FILES_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}		
	}
	catch (CSysException& e) {
		bRet = false;
		DoException(e,ROW_DISK_CHECK);
	}
	catch (...) {
		bRet = false;
	}
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化操作系统配置(键盘功能键，任务管理器，桌面显示，鼠标形式，清理回收站等)

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeOSSetting() 
{
	bool bRet = true;
	try {
		// 功能键设置
		CShieldKeyHelper::SetFunctionKeysEnable(theTVM_INFO.IsFunctionKeyEnabled());
		// 任务管理器设置
		CShieldKeyHelper::SetTaskManagerEnable(theTVM_INFO.IsTaskManagerEnabled());
		// 桌面设置
		CShieldKeyHelper::SetDesktopEnable(theTVM_INFO.IsDesktopEnabled());
		// 清空回收站
		SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|SHERB_NOSOUND);
		// 启动守护进程
		CShieldKeyHelper::SetHostProgarmEnable(theTVM_INFO.IsUsedGuardian());
	}
	catch (CSysException& e) {
		bRet = false;
		DoException(e,ROW_DISK_CHECK);
	}
	catch (...) {
		bRet = false;
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统目录结构

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemFolder()
{
	bool bRet = true;
	try {
		// 检查本地TPU强制更新目录
		TCHAR szAppPath[_MAX_PATH];
		if (ComGetAppPath(szAppPath, _MAX_PATH)>0) {
			CString fileName = CString(szAppPath) + _T("PG\\TPU-APP\\CURRENT");
			ComCheckAndCreateDirectory(fileName);
		}

		// 检查创建数据主目录
		if (!CheckCreateHomeFolder()) {
			CString msg = _opl(TXT_INI_CHECK_HARD_DISK);
			CString stm = _opl(TXT_INI_CREATE_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}

		// 检查创建备份目录
		if (!CheckCreateBackupFolder()) {
			CString msg = _opl(TXT_INI_CHECK_CF);
			CString stm = _opl(TXT_INI_CREATE_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}
	}
	catch (CSysException& e) {
		bRet = false;
		DoException(e,ROW_DISK_CHECK);
	}
	catch (...) {
		bRet = false;
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统Master

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemMaster()
{
	bool bRet = true;
	// 初始化MASTER文件	
	try{			
		CString masterPath = theTVM_INFO.GetDataPath();
		CString masterBakPath = theTVM_INFO.GetBackupPath();

		theCOUNT_INFO.Internalize(masterPath, masterBakPath);
		thePWD_ERR.Internalize(masterPath,masterBakPath);	
		theREGI_INFO.Internalize(masterPath, masterBakPath);
		theDeviceInfo.Internalize(masterPath, masterBakPath);
		theSOFT_VERSION.Internalize(masterPath, masterBakPath);
		thePEKG_AUDIT.Internalize(masterPath, masterBakPath);
		theBANKNOTE_COUNT.Internalize(masterPath, masterBakPath);
		theCARD_COUNT.Internalize(masterPath, masterBakPath);
		theCOIN_COUNT.Internalize(masterPath, masterBakPath);
		//theTOKEN_COUNT.Internalize(masterPath, masterBakPath);
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		//		theAPP_SESSION.SetInitDetailErrorFlag(MASTER_ERROR);
		DoException(e,ROW_DATA_INIT);

	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		//		theAPP_SESSION.SetInitDetailErrorFlag(MASTER_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_MASTER_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化交易数据

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTxnData()
{
	bool bRet = true;
	// 初始化交易数据
	try{
		CString strDataPath = theTVM_INFO.GetDataPath();
		CString strBackUpPath = theTVM_INFO.GetBackupPath();
		theACC_TXN_MGR.Internalize(strDataPath,strBackUpPath);
		theAFC_TXN_MGR.Internalize(strDataPath,strBackUpPath);
		theECT_TXN_MGR.Internalize(strDataPath,strBackUpPath);
		theBUS_TXN_MGR.Internalize(strDataPath,strBackUpPath);
		theEVN_TXN_MGR.Internalize(strDataPath,strBackUpPath);

		bool IsValidateUnSendPackage=false;
		int iSendedDataMaxDay=theFunction_INFO.GetLimitedDaysOfSendData();
		int iUnSendDataMaxDay=theFunction_INFO.GetLimitedDaysOfUnSendData();
		theACC_TXN_MGR.InitTxnData(IsValidateUnSendPackage,iSendedDataMaxDay,iUnSendDataMaxDay);
		theAFC_TXN_MGR.InitTxnData(IsValidateUnSendPackage,iSendedDataMaxDay,iUnSendDataMaxDay);
		theECT_TXN_MGR.InitTxnData(IsValidateUnSendPackage,iSendedDataMaxDay,iUnSendDataMaxDay);
		theBUS_TXN_MGR.InitTxnData(IsValidateUnSendPackage,iSendedDataMaxDay,iUnSendDataMaxDay);
		theEVN_TXN_MGR.InitTxnData(IsValidateUnSendPackage,iSendedDataMaxDay,iUnSendDataMaxDay);
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		//		theAPP_SESSION.SetInitDetailErrorFlag(TXNDATA_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		//		theAPP_SESSION.SetInitDetailErrorFlag(TXNDATA_ERROR);			
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_TRANSACTION_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化参数数据

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeParamData()
{
	bool bRet = true;
	//初始化参数文件
	try{
		thePARAM_HELPER.InitParam();
		thePARAM_HELPER.InitAllParam();
		// 取得当前车站编码
		WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();			
		// 判断当前车站是否存在			
		//if(!theACC_LOCATION.IsStationCodeExist(stationCode)) {
		//	// 参数错误，抛异常，程序中断
		//	theAPP_SESSION.SetInitCheckError(true);
		//	theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
		//	throw  CParamException(CParamException::STATION_NOT_IN_VFARE, _T(__FILE__), __LINE__,_opl(TXT_INI_FUNCTION_EXCEPTION));
		//}
		//else 
		{
			//LocationNumber_t locationNum = theACC_LOCATION.GetStationLocationNumber(stationCode);
			// 设置当前车站名称
			theAPP_SESSION.SetStationName(theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_CHINESE),theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_ENGLISH),theACC_LOCATION.GetLocationName(stationCode,LANGUAGE_ID_PORTUGUESE));
		}
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_PARAM_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化寄存器数据

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeRegistAudit()
{
	bool bRet = true;
	//初始化寄存器数据
	try{
		CString strDataPath = theTVM_INFO.GetDataPath();
		CString strBackUpPath = theTVM_INFO.GetBackupPath();
		theACCAUDIT_MGR.Initialize(strDataPath,strBackUpPath);
		theDeviceRegAR.Initialize(strDataPath,strBackUpPath);
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_AR_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化SC审计数据

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSCAudit()
{
	bool bRet = true;
	//初始化SC审计数据
	try{
		//vector<PurchaseData> unsendLoginPurchase;
		//theSCAudit_MGR.LoadUnSendLoginPurchase(unsendLoginPurchase);
		//for(int i=0;i<unsendLoginPurchase.size();i++)
		//{
		//	PurchaseData& purchaseData = unsendLoginPurchase[i];
		//	USER_INFO tempUser;
		//	tempUser.sUserID = purchaseData.staffID;
		//	theAPP_SESSION.SetUserInfo(tempUser);
		//	purchaseData.auditTimeTo = ComGetCurTime();
		//	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(purchaseData,OPERATER_LOGOUT);
		//	theSCAudit_MGR.SetLoginPurchaseSended(purchaseData.staffID,purchaseData.loginTime);
		//}
		//theAPP_SESSION.SetUserInfo(USER_INFO());

		// 发送没有发送的审计日志
		vector<PurchaseData> unsendDayPurchase;
		theSCAudit_MGR.LoadUnSendDayPurchase(unsendDayPurchase);
		_DATE currentBusinessDate = ComGetBusiDate();
		for(int i=0;i<unsendDayPurchase.size();i++)
		{
			PurchaseData& purchaseData = unsendDayPurchase[i];
			if(purchaseData.businessDate == currentBusinessDate)
			{
				continue;
			}
			purchaseData.auditTimeTo = ComGetCurTime();
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(purchaseData);
			theSCAudit_MGR.SetDayPurchaseSended(purchaseData.businessDate);
		}

		// 设置运营日
		theSCAudit_MGR.Initilize();
		theSCAudit_MGR.SetCurrentBusinessDay(currentBusinessDate);
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_SC_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化系统时间

@param     无

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemTime()
{
	bool bRet = true;
	// 通过NTP同步本地时间
	try{
		theSNTP_HELPER.TimeSynchronize(true);
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(NTP_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(NTP_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_opl(TXT_INI_CLOCK_ERROR));
		DoException(e,ROW_DATA_INIT);
	}
	// 设置营业日切换时间
	CAfcDeviceTimeParam::OPRATION_TIME operationTime;
	theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
	ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);

	// 启动NTP对时计时器
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_NTP_TIMER,NULL,NULL);
	// 启动运营日切换计时器
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER,NULL,NULL);
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化数据库

@param     none

@retval      bool  true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeDatabase()
{
	// 初始化数据库
	bool bRet = false;
	try{
		if (0 == DATA_HELPER->DbInitialize()){
			bRet = true;
		}		
	}
	catch (CSysException& e) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INI_DATABASE_ERROR);
		DoException(e,ROW_DATA_INIT);
	}
	catch (...) {
		bRet = false;
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INI_DATABASE_ERROR);
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_PARAM_EXCEPTION));
		DoException(e,ROW_DATA_INIT);
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      删除打印历史记录

@param      (i)int days;（天数）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeletePrintHistory(int days){
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
void CInitSvc::GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir){
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

	bool bIsFind = finder.FindFile(strPath + _T("*.txt"));

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
