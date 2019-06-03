#include "stdafx.h"
#include "InitSvc.h"
#include "UpgradeHelper.h"
#include "RWMonitorSvc.h"
#include "TWMonitorSvc.h"
#include "THMonitorSvc.h"
#include "UPSMonitorSvc.h"
#include "LPTMonitorSvc.h"
#include "setting.h"
#include "TicketBoxExchangeSvc.h"
#include "TxnAuditManager.h"
#include "shieldkeyhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CInitSvc,CBOMForeService)
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
    :CBOMForeService(INIT_SVC,MAIN_SVC)
{
    // 进入初始化状态
    theSERVICE_MGR.SetState(INIT);

	//theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CInitSvc::OnExceptionNotify);

    // 初期化显示文本
    m_InitialInfo[ROW_DATA_CHECK].title = "数据一致性检查:";
    m_InitialInfo[ROW_PARAMETER].title  = "检测并初始化参数:";
    m_InitialInfo[ROW_RW].title         = "检测并初始化外部读写器:";
	m_InitialInfo[ROW_TW].title         = "检测并初始化内部读写器:";
	m_InitialInfo[ROW_TH].title         = "检测并初始化出票机:";
    m_InitialInfo[ROW_PRINT].title      = "检测并初始化打印机:";

    for (int i = 0; i < MAX_INITIAL_INFO_ROW; i++) {
        m_InitialInfo[i].result = UNDOWN;
    }

    // 设置操作员画面
    m_pDialogFlow->AddDialog(CInitDlg::IDD, new CInitDlg(this));

    // 设置乘客画面

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
		
		bool bInitOK = Initialize();

		 // 暂停，保留画面上的显示结果
		 Sleep(2000);

		 // 结束等待光标
		 theCURSOR_MGR.EndWaitCursor();

		 // 进入终止中状态
		 theSERVICE_MGR.SetState(OUT_OF);
		 if (bInitOK && (!theEXCEPTION_MGR.HasException())) { 
			 // 启动Login画面
			 theSERVICE_MGR.StartService(LOGIN_SVC);
		 }
		 // 初期化失败
		 else {          
			 // 显示主菜单
				theMENU_MGR.SetMenuKind(CMenuMgr::MENU_INIT_ABNORMAL);
		 }
		 theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
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

	if(!InitializeController()){
		return false;
	}
	
	// 数据一致性检查
	if (!CheckConsistent()) {
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_DATA_CHECK, INIT_RESULT::SUCCESS);   // 设置初始化结果（数据一致性检查）
	}

	//// 初始化参数
	//if (!InitializeParameter()) {		
	//	ret = false;
	//}
	//else {
	//	SetInitRessult(ROW_PARAMETER, INIT_RESULT::SUCCESS);   // 设置初始化结果（数据一致性检查）
	//}

	////初始化外部读写器
	//if(!InitializeRW()) {
	//	SetInitRessult(ROW_RW, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
	//	ret = false;
	//}
	//else {
	//	// 待确认
	//	CheckSAMidChange(RW_YPT_ISAM);
	//	CheckSAMidChange(RW_YPT_PSAM);
	//	CheckSAMidChange(RW_YKT_ISAM);
	//	CheckSAMidChange(RW_YKT_PSAM);
	//	SetInitRessult(ROW_RW, INIT_RESULT::SUCCESS);
	//}

	////初始化内部读写器
	//if (!InitializeTW()) {
	//	SetInitRessult(ROW_TW, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
	//	ret = false;
	//}
	//else {
	//	// 待确认
	//	CheckSAMidChange(TH_YPT_ISAM);
	//	CheckSAMidChange(TH_YPT_PSAM);
	//	CheckSAMidChange(TH_YKT_ISAM);
	//	CheckSAMidChange(TH_YKT_PSAM);
	//	SetInitRessult(ROW_TW, INIT_RESULT::SUCCESS);
	//}

	//// 初始化出票机
	//// 初始化出票机门禁
	//if(!InitializeLPT()){
	//	SetInitRessult(ROW_TH, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
	//	ret = false;
	//}
	//// 正常模式
	//if(theSETTING.GetTHMode() == TH_MODE_NORMAL){
	//	if(!InitializeTH()) {
	//		SetInitRessult(ROW_TH, INIT_RESULT::FAIL);
	//		theAPP_SESSION.SetInitCheckError(true);
	//		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
	//		ret = false;
	//	}
	//	else {
	//		SetInitRessult(ROW_TH, INIT_RESULT::SUCCESS);
	//	}
	//}
	//// 缩退模式
	//else{
	//	SetInitRessult(ROW_TH, INIT_RESULT::UNDOWN);
	//}

	//// 初始化打印机
	//if (!InitializePrinter()) {
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_PRINT_ERROR);
	//	ret = false;
	//}
	//else {
	//	SetInitRessult(ROW_PRINT, INIT_RESULT::SUCCESS);
	//}
	//// 初始化UPS
	//if(!InitializeUPS()){
	//	theAPP_SESSION.SetInitCheckError(true);
	//	//theAPP_SESSION.SetInitDetailErrorFlag(INIT_UPS_ERROR);
	//	ret = false;
	//}

	// 若存在更新程序则关机更新程序
	bool needUpgrade = theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade();
	if(needUpgrade)	{
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);	
		return false;
	}

	// 启用设备监控
	if(theBOM_INFO.IsRWMonitorOpened(true)){
		theSERVICE_MGR.StartService(MONITOR_RW_SVC);
		if(!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID)){
			theSERVICE_MGR.GetService<CRWMonitorSvc>(MONITOR_RW_SVC)->Resume();
		}
	}
	if(theBOM_INFO.IsRWMonitorOpened(false)){
		theSERVICE_MGR.StartService(MONITOR_TW_SVC);
		if(!theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID)){
			theSERVICE_MGR.GetService<CTWMonitorSvc>(MONITOR_TW_SVC)->Resume();
		}
	}
	if(theBOM_INFO.IsTHMonitorOpened()){
		theSERVICE_MGR.StartService(MONITOR_TH_SVC);
		if(!theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID)){
			theSERVICE_MGR.GetService<CTHMonitorSvc>(MONITOR_TH_SVC)->Resume();
		}
	}
	if(theBOM_INFO.IsUPSMonitorOpened()){
		theSERVICE_MGR.StartService(MONITOR_UPS_SVC);
		if(!theEXCEPTION_MGR.HasException(CUPSException::MODULE_ID)){
			theSERVICE_MGR.GetService<CUPSMonitorSvc>(MONITOR_UPS_SVC)->Resume();
		}
	}
	if(theBOM_INFO.IsLPTMonitorOpened()){
		theSERVICE_MGR.StartService(MONITOR_LPT_SVC);
		if(!theEXCEPTION_MGR.HasException(CLPTHardwareException::MODULE_ID)){
			theSERVICE_MGR.GetService<CLPTMonitorSvc>(MONITOR_LPT_SVC)->Resume();
		}
	}

	// 初始化上位
	if (InitializeCPS() != true){
		ret = false;
	}

    return ret;
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

	// create FTP control Instance
	g_pFTPControl = new CFTPControl();
	ASSERT(g_pFTPControl != NULL);
	g_pFTPControl->Initialize();

	
	// create CPS control Instance
	g_pSCControl = new CSCControl();
	ASSERT(g_pSCControl != NULL);
	g_pSCControl->Initialize();


	// create MIMControl control instance
	g_pMIMControl = new CMIMControl();
	ASSERT(g_pMIMControl != NULL);
	g_pMIMControl->Initialize();

	//g_pMemControl = new CMemController(100);
	//ASSERT(g_pMemControl != NULL);

	//g_pMIMControl->SetMemController(g_pMemControl);


	// create RW control Instance
	g_pRWControl = new CRWControl();
	g_pRWControl->Initialize();

	// create Printer control Instance
	g_pPrinterControl = new CPrinterControl();
	g_pPrinterControl->Initialize();

	// 初始化后台service
	theSERVICE_MGR.StartService(SC_SVC,false);
	theSERVICE_MGR.StartService(BUSINESS_INTERVAL_SVC,false);
	theSERVICE_MGR.StartService(EVENT_MSG_SVC,false);
	theSERVICE_MGR.StartService(TRANSMISSION_INTERVAL_SVC,false);

    return true;
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
	try {
		// 初期化INI文件
		int initResult = 0;
		initResult = theBOM_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _T("请联系维修人员重新安装程序");
			CString stm = _T("系统文件错误");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		initResult = theSETTING.Initialize();
		if (initResult != 0) {
			CString msg = _T("请联系维修人员重新安装程序");
			CString stm = _T("系统文件错误");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		initResult = theMAINTENANCE_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _T("请联系维修人员重新安装程序");
			CString stm = _T("系统文件错误");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		//initResult = theTEMP_BACKUP_INFO.Initialize();
		//if (initResult != 0) {
		//	CString msg = "请联系维修人员重新安装程序";
		//	CString stm = "系统文件错误";
		//	m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
		//	m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
		//	return false;
		//}

		// 检查创建数据主目录
		if (!CheckCreateHomeFolder()) {
			CString msg = _T("请联系维修人员检查硬盘");
			CString stm = _T(",创建目录错误");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}

		// 检查创建备份目录
		if (!CheckCreateBackupFolder()) {
			CString msg = _T("请联系维修人员检查CF卡");
			CString stm = _T("创建目录错误");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		CShieldKeyHelper::SetFunctionKeysEnable(theBOM_INFO.IsFunctionKeyEnabled());
		CShieldKeyHelper::SetTaskManagerEnable(theBOM_INFO.IsTaskManagerEnabled());
		CShieldKeyHelper::SetDesktopEnable(theBOM_INFO.IsDesktopEnabled());
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_DATA_CHECK);
		//theEXCEPTION_MGR.ProcessException(e);
		
	}
	catch (...) {
		return false;
	}
    return false;
}

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
	try
	{
		CString szDataPath = theBOM_INFO.GetDataPath();
		CString szBackePath = theBOM_INFO.GetBackupPath();
		// 初始化日志
		try{
			// 设置日志目录
			CString tracePath = theBOM_INFO.GetLogPath();
			CLog::Initialize(tracePath);
			//MIMTrs_Initialize(tracePath);       // Th Trace (单独处理)
			// 是否记录Debug消息
			theDEBUG_LOG->SetWriteDebug(theBOM_INFO.IsWriteDebug());

			// 删除BOM日志 
			DeleteLog(theBOM_INFO.GetStoreDay());

			// 删除充值单据文件 
			DeleteChargeReceiptFile(7);

			//删除TPU日志		
			DeleteTpuLog(theBOM_INFO.GetStoreDay());

			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("-----------------------------------------------"));
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("--------------------BOM Start------------------"));
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("-----------------------------------------------"));
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("-----------------------------------------------"));
		}
		catch(CSysException& e){
			bRes = false;
			DoException(e,ROW_PARAMETER);
		}
		catch(...){
			bRes = false;
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("初始化日志文件发生异常"));
			DoException(e,ROW_PARAMETER);
		}
		
		// 初始化异常
		int ret = theGlobalParameter.GetExceptionDetail();
		if(ret != 0){
			bRes = false;
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("初始化异常文件发生异常"));
			DoException(e,ROW_PARAMETER);
		}
		// 初始化MASTER文件	
		try{			
			CString masterPath = theBOM_INFO.GetDataPath();
			CString masterBakPath = theBOM_INFO.GetBackupPath();
			theCOUNT_INFO.Internalize(masterPath, masterBakPath);
			thePWD_ERR.Internalize(masterPath,masterBakPath);	
			thePRE_TK.Internalize(masterPath, masterBakPath);
			theREGI_INFO.Internalize(masterPath, masterBakPath);
			theDeviceInfo.Internalize(masterPath, masterBakPath);
			theSOFT_VERSION.Internalize(masterPath, masterBakPath);
			thePEKG_AUDIT.Internalize(masterPath, masterBakPath);
			theTVMRefund.Internalize(masterPath, masterBakPath);
			theREFUND_APPLY_RRSULT.Internalize(masterPath, masterBakPath);
			theNAMED_CARD_APPLY_RRSULT.Internalize(masterPath, masterBakPath);
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(DEVICE_MASTER_ERROR);
			DoException(e,ROW_PARAMETER);

		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(DEVICE_MASTER_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("初始化MASTER文件异常"));
			DoException(e,ROW_PARAMETER);

		}
	
		// 初始化交易数据
		try{
			theACC_TXN_MGR.Internalize(szDataPath,szBackePath);
			theAFC_TXN_MGR.Internalize(szDataPath,szBackePath);
			theECT_TXN_MGR.Internalize(szDataPath,szBackePath);
			theBUS_TXN_MGR.Internalize(szDataPath,szBackePath);
			theEVN_TXN_MGR.Internalize(szDataPath,szBackePath);
			theACC_TXN_MGR.InitTxnData();
			theAFC_TXN_MGR.InitTxnData();
			theECT_TXN_MGR.InitTxnData();
			theBUS_TXN_MGR.InitTxnData();
			theEVN_TXN_MGR.InitTxnData();
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(DEVICE_TXNDATA_ERROR);
			DoException(e,ROW_PARAMETER);

		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(DEVICE_TXNDATA_ERROR);			
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("初始化交易数据文件异常"));
			DoException(e,ROW_PARAMETER);
		}

		//初始化参数文件
		try
		{
		
			thePARAM_HELPER.InitAllParam();

			// 设置营业开始时间
			CAfcDeviceTimeParam::OPRATION_TIME operationTime;
			theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
			ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);

			// 判断当前车站是否存在			
			// 取得当前车站编码
			WORD stationCode = 0;
			stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
			

			CAccLineInfoParam::STATION_INFO stationgInfo;
			if(!theACC_LINE.GetStationInfobyCode(stationCode,stationgInfo)) {
				// 参数错误，抛异常，程序中断
				bRes = false;
				theAPP_SESSION.SetInitCheckError(true);
				theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
				throw  CParamException(CParamException::STATION_NOT_IN_VFARE, _T(__FILE__), __LINE__, _T("函数参数异常:沒有相应的车站代码转换"));
			}
			else {
				// 设置当前车站名称
				CString station_name_cn = stationgInfo.station_Chinese_Name;
				CString station_name_en = stationgInfo.station_Eng_Name;
				theAPP_SESSION.SetStationName(station_name_cn,station_name_en);
			}	
	    }
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
			DoException(e,ROW_PARAMETER);
		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("初始化Param文件异常"));
			DoException(e,ROW_PARAMETER);

		}



		//初始化寄存器数据
		try{
			theACCAUDIT_MGR.Initialize();
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			DoException(e,ROW_PARAMETER);

		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("初始化AR文件异常"));
			DoException(e,ROW_PARAMETER);

		}
		//初始化审计文件
		try{
			vector<PurchaseData> unsendLoginPurchase;
			theSCAudit_MGR.LoadUnSendLoginPurchase(unsendLoginPurchase);
			for(int i=0;i<unsendLoginPurchase.size();i++)
			{
				PurchaseData& purchaseData = unsendLoginPurchase[i];
				USER_INFO tempUser;
				tempUser.sUserID = purchaseData.staffID;
				theAPP_SESSION.SetUserInfo(tempUser);
				purchaseData.auditTimeTo = ComGetCurTime();
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(purchaseData,OPERATER_LOGOUT);
				theSCAudit_MGR.SetLoginPurchaseSended(purchaseData.staffID,purchaseData.loginTime);
			}
			theAPP_SESSION.SetUserInfo(USER_INFO());
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
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(purchaseData,END_OF_DAY);
				theSCAudit_MGR.SetDayPurchaseSended(purchaseData.businessDate);
			}
			theSCAudit_MGR.Initilize();
			theSCAudit_MGR.SetCurrentBusinessDay(currentBusinessDate);
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			DoException(e,ROW_PARAMETER);

		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("初始化SC审计文件异常"));
			DoException(e,ROW_PARAMETER);

		}

		//初始化交易审核文件
		try{
			vector<TxnAuditData> unsendTxnAuditData;
			theTxnAudit_MGR.LoadUnSendTxnAudit(unsendTxnAuditData);
			_DATE currentBusinessDate = ComGetBusiDate();
			for(int i=0;i<unsendTxnAuditData.size();i++)
			{
				TxnAuditData& txnAuditData = unsendTxnAuditData[i];
				if(txnAuditData.businessDate == currentBusinessDate)
				{
					continue;
				}
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertTransactionAudit(txnAuditData);
				theTxnAudit_MGR.SetTxnAuditSended(txnAuditData.businessDate);
			}
			theTxnAudit_MGR.Initilize();
			theTxnAudit_MGR.SetCurrentBusinessDay(currentBusinessDate);
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			DoException(e,ROW_PARAMETER);

		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(INIT_DATACOUNT_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("初始化交易审核文件异常"));
			DoException(e,ROW_PARAMETER);

		}
		theMENU_MGR.InitializeIssueMenu();

		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("ok"));
		// 通过NTP同步本地时间
		try
		{
			theSNTP_HELPER.TimeSynchronize();
		}
		catch (CSysException& e) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(NTP_ERROR);
			DoException(e,ROW_PARAMETER);
		}
		catch (...) {
			bRes = false;
			theAPP_SESSION.SetInitCheckError(true);
			theAPP_SESSION.SetInitDetailErrorFlag(NTP_ERROR);
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("时钟同步错误"));
			DoException(e,ROW_PARAMETER);

		}
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_NTP_TIMER,NULL,NULL);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER,NULL,NULL);

	}
	catch(CSysException& e)	{
		bRes = false;
		DoException(e,ROW_PARAMETER);
	}
	catch(...)	{
		bRes = false;
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("参数初始化错误"));
		DoException(e,ROW_PARAMETER);
	}
	return bRes;
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化读写器(RW)

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeRW()
{
	try {
		m_pRWHelper->InitCheck();
		m_pRWHelper->CheckDeviceExchange();
	}
	catch (CSysException& e){
		DoException(e,ROW_RW);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("外部读写器初始化错误"));
		return false;
		DoException(e,ROW_RW);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化读写器(TW)

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTW()
{
	try {
		m_pTWHelper->InitCheck();
		m_pTWHelper->CheckDeviceExchange();
	}
	catch (CSysException& e){
		DoException(e,ROW_TW);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("内部读写器初始化错误"));
		return false;
		DoException(e,ROW_TW);
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
	long ret;
	try {
		m_pPRTHelper->PrinterConnect(theBOM_INFO.GetPrinterCommPort(), theBOM_INFO.GetPrinterBaudRate());
		m_pPRTHelper->CheckStatus();	
		m_pPRTHelper->CheckDeviceExchange();
	}
	catch (CSysException& e){
		DoException(e,ROW_PRINT);
		return false;
	}
	catch(...)	{
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("打印机初始化错误"));
		DoException(e,ROW_PRINT);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化出票机(TH)

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTH()
{
	try {
		// 初期化
		m_pTHHelper->Initialize();

		//m_pTHHelper->CheckDeviceExchange();
		//
		//if(theSETTING.GetRFIDWriteStatus() == 1){
		//	CTicketBoxExchangeSvc pSvc;
		//	pSvc.CheckTicketBoxIsInvalidExchanged();	
		//}
		//// RFID禁用
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

		// 票箱更换检查（需要打印帐票，放在打印机初期化之后处理）
		//CTicketBoxHelper ticketBoxHelper(*this);
		//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_TH);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("出票机初始化错误"));
		DoException(e,ROW_TH);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化出票机门禁

@param       无

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeLPT()
{
	try {
		// 初期化
		m_LPTHelper->LPTOpenLPT();
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_TH);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("出票机初始化错误"));
		DoException(e,ROW_TH);
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
bool CInitSvc::InitializeUPS()
{
	try {
		m_UPSHelper->UPSOpen();
		return true;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("UPS初始化错误"));
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}

	return false;
}

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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("初始化上位失败"))); ;
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
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath())) {    // not exist
		return false;
	}
	// 检查并创建交易数据目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建master文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建ACC参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建ECT参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ECT_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ECT_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建AFC参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_AFC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_AFC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建收益文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + BOM_PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	
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
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath())) {    // not exist
		return false;
	}
	// 检查并创建交易数据目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建master文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建ACC参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建ECT参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ECT_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ECT_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建AFC参数文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_AFC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_AFC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建收益文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + BOM_PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	// 检查并创建审计文件目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + AR_MANAGE_FOLDER)) {    // if not exist, create it
		return false;
	}

	// 检查并创建trace目录
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetLogPath())) {    // if not exist, create it
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化BRContext变量

@param      无

@retval     无

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
// void CInitSvc::InitialBRContext()
// {
// }

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
		CString sDir = theBOM_INFO.GetLogPath();

		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".log"));

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
				dateStr = (sFileName.Right(12)).Left(8);

				// 如果日期小于指定日期则删除
				if(dateStr < dayLimitStr) {
					// 删除该文件
					DeleteFile(sDir + sFileName);
				}
				else {
					continue;
				}
			}
		}

		finder.Close();
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
@brief      删除指定天数前的充值单据文件

@param      (i)int days    指定天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeleteChargeReceiptFile(int days)
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
		CString sDir = theBOM_INFO.GetDataPath() + _T("PrintHistory\\ChargePrint");

		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".txt"));

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

				// 如果日期小于指定日期则删除
				if(dateStr < dayLimitStr) {
					// 删除该文件
					DeleteFile(sDir + sFileName);
				}
				else {
					continue;
				}
			}
		}

		finder.Close();
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
		CString sDir = theBOM_INFO.GetLogPath();

		// 查找文件
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".log"));

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
				dateStr = (sFileName.Right(12)).Left(8);
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
// 	CString stm = "";
// 	//stm.Format("%s%08X%s%s", "(", e.GetActualMTC(), "),", e.GetDescription());
// 	//stm = "," + e.GetDescription() + "," + e.GetPrompt();
// 	stm.Format("%s%08X%s%s%s%s", "(", e.GetActualMTC(), "),", e.GetDescription(),",",e.GetPrompt());
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
	Sleep(2000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查SamId变化

@param      (i)bool, true:判断TH的ISAM变化;  false:判断RW的ISAM变化

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::CheckSAMidChange(SAM_POS_ samPos)
{
	// SAMid发生改变
	DWORD samIdNo = 0;
	// ISAM如果发生改变
	SAM_ID_INFO samId;
	samId = theAPP_SESSION.GetSAMInfo();
	BYTE nosam[6] = {0};
	CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
	theDeviceInfo.GetDeviceInfo(deviceInfo);
	//theDEBUG_LOG.WriteData(_T(__FILE__),__FUNCTION__,__LINE__,"");
	switch (samPos) {
		case RW_YPT_ISAM:
			//外部一票通ISAM
			if (memcmp(samId.rw_acc_isam_id,nosam,6) != 0 && memcmp(samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW1_ISAM(samId.rw_acc_isam_id);
			}	
			break;
		case RW_YPT_PSAM:
			//外部一票通PSAM
			if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW1_PSAM(samId.rw_acc_psam_id);
			}	
			break;
		case TH_YPT_ISAM:
			// 内部一票通ISAM
			if (memcmp(samId.th_acc_isam_id,nosam,6) != 0 && memcmp(samId.th_acc_isam_id,deviceInfo.ACC_RW2_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.th_acc_isam_id,deviceInfo.ACC_RW2_ISAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW2_ISAM(samId.th_acc_isam_id);
			}	
			break;
		case TH_YPT_PSAM:
			// 内部一票通PSAM
			if (memcmp(samId.th_acc_psam_id,nosam,6) != 0 && memcmp(samId.th_acc_psam_id,deviceInfo.ACC_RW2_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.th_acc_psam_id,deviceInfo.ACC_RW2_PSAM);
				// 更新master文件
				theDeviceInfo.SetACC_RW2_PSAM(samId.th_acc_psam_id);
			}	
			break;
		case RW_YKT_ISAM:
			// 外部一卡通ISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW1_ISAM(samId.rw_ect_isam_id);
			}
			break;
		case RW_YKT_PSAM:
			// 外部一卡通PSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW1_PSAM(samId.rw_ect_psam_id);
			}
			break;
		case TH_YKT_ISAM:
			// 内部一卡通ISAM
			if (memcmp(samId.th_ect_isam_id,nosam,6) != 0 && memcmp(samId.th_ect_isam_id,deviceInfo.ECT_RW2_ISAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.th_ect_isam_id,deviceInfo.ECT_RW2_ISAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW2_ISAM(samId.th_ect_isam_id);
			}
			break;
		case TH_YKT_PSAM:
			// 内部一卡通PSAM
			if (memcmp(samId.th_ect_psam_id,nosam,6) != 0 && memcmp(samId.th_ect_psam_id,deviceInfo.ECT_RW2_PSAM,6) != 0) {
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.th_ect_psam_id,deviceInfo.ECT_RW2_PSAM);
				// 更新master文件
				theDeviceInfo.SetETC_RW2_PSAM(samId.th_ect_psam_id);
			}
			break;
		default:
			break;
	}
}


void CInitSvc::OnExceptionNotify(CSysException& e,bool addException)
{
	//////if(addException)
	//////{
	//////	return;
	//////}
	if(e.GetModuleID() == CRWHardwareException::MODULE_ID || e.GetModuleID() == CRWException::MODULE_ID)
	{
		if (!(theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CRWException::MODULE_ID))) {
			this->SetInitRessult(CInitSvc::ROW_RW,CInitSvc::SUCCESS);
		}
	}
	if(e.GetModuleID() == CPrinterException::MODULE_ID)
	{
		if (!(theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID))) {
			this->SetInitRessult(CInitSvc::ROW_PRINT,CInitSvc::SUCCESS);
		}
	}
	if(e.GetModuleID() == CTWHardwareException::MODULE_ID || e.GetModuleID() == CTWException::MODULE_ID)
	{
		if (!(theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTWException::MODULE_ID))) {
			this->SetInitRessult(CInitSvc::ROW_TW,CInitSvc::SUCCESS);
		}
	}
	if(e.GetModuleID() == CTHHardwareException::MODULE_ID || e.GetModuleID() == CTHException::MODULE_ID)
	{
		if (!(theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))) {
			this->SetInitRessult(CInitSvc::ROW_TH,CInitSvc::SUCCESS);
		}
	}
	if(e.GetModuleID() == CSNTPSException::MODULE_ID)
	{
		if(!(theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID)))
		{
			this->SetInitRessult(CInitSvc::ROW_PARAMETER,CInitSvc::SUCCESS);
		}
	}
	if(!theEXCEPTION_MGR.HasException())
	{
		// 启动Login画面
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
}