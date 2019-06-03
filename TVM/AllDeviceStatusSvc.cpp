#include "stdafx.h"
#include "AllDeviceStatusSvc.h"
#include "AllDeviceStatusMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAllDeviceStatusSvc,CTVMForeService)
	//  ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_F2,OnKeyboardF2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAllDeviceStatusSvc::CAllDeviceStatusSvc()
	:CTVMForeService(ALL_DEVICE_STATUS_SVC)
{
	//设置操作员画面
	m_pDialogFlow->AddDialog(IDD_ALL_DEVICE_STATUS,new CAllDeviceStatusMaintainDlg(this));

	//设置画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,_opl(TXT_DEVICE_STATUS_DETECTIONING));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_ALL_DEVICE_STATUS);

	//设置初始画面
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
CAllDeviceStatusSvc::~CAllDeviceStatusSvc(){
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务开始时初始化状态函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllDeviceStatusSvc::OnStart()
{	
	m_bIsFinished = false;
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	__super::OnStart();
	theSERVICE_MGR.SetForeServiceBusy(true);
	InitDataModel();
	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      返回

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CAllDeviceStatusSvc::OnKeyboardF3(WPARAM , LPARAM )
{	
	// 完成时才可以返回
	if(m_bIsFinished)
		theAPP_SESSION.ReturnMaintainMenu();	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      显示初始化结果

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllDeviceStatusSvc::SetInitRessult(int index, INIT_RESULT result)
{
	m_InitialInfo[index].result = result;

	// 刷新Dialog
	CAllDeviceStatusMaintainDlg* pInitialDlg =  dynamic_cast<CAllDeviceStatusMaintainDlg*>(m_pDialogFlow->GetDialog(CAllDeviceStatusMaintainDlg::IDD));
	if(NULL != pInitialDlg)
		pInitialDlg->ShowInitRessult(index);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化检测设备状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllDeviceStatusSvc::InitDataModel(){
	// 清空数据
	for(int i = 0; i< ROW_MAX;i++){
		//m_InitialInfo[i] = INITIAL_INFO();
		m_order[i] = ROW_INVALID;
	}
	m_pDialogFlow->GetDialog(CAllDeviceStatusMaintainDlg::IDD)->UpdateWindow();
	// 初始化内容标题	
	m_InitialInfo[ROW_IO_MODULE].title				= _opl(TXT_INIT_IO_MODULE);
	//m_InitialInfo[ROW_CHARGE_RW].title				= _opl(TXT_INIT_RECHARGE_RW);
	m_InitialInfo[ROW_CARD_RW].title				= _opl(TXT_INIT_CARD_RW);
	//m_InitialInfo[ROW_CARD_MODULE].title			= _opl(TXT_INIT_CARD_MODULE);
	m_InitialInfo[ROW_TOKEN_MODULE].title			= _opl(TXT_INIT_TOKEN_MODULE);
	m_InitialInfo[ROW_COIN_MODULE].title			= _opl(TXT_INIT_COIN_MODULE);
	m_InitialInfo[ROW_BANKNOTE_MODULE].title		= _opl(TXT_INIT_BANKNOTE_MODULE);
	//m_InitialInfo[ROW_BANKNOTE_CHANGE_MODULE].title	= _opl(TXT_INIT_BANKNOTE_CHANGE_MODULE);
	m_InitialInfo[ROW_PRINT].title					= _opl(TXT_INIT_RECEIPT_PRINTER_MODULE);

	int number = 0;
	bool is_tvm = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_TVM;
	m_order[ROW_IO_MODULE]				= ++number;
	//m_order[ROW_CHARGE_RW]				= ++number;
	m_order[ROW_CARD_RW]				= is_tvm ? ++number : ROW_INVALID;
	//m_order[ROW_CARD_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_TOKEN_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_COIN_MODULE]			= is_tvm ? ++number : ROW_INVALID;
	m_order[ROW_BANKNOTE_MODULE]		= ++number;
	//m_order[ROW_BANKNOTE_CHANGE_MODULE]	= ++number;
	m_order[ROW_PRINT]					= ++number;
	// 初始化标题
	CAllDeviceStatusMaintainDlg* pInitialDlg =  dynamic_cast<CAllDeviceStatusMaintainDlg*>(m_pDialogFlow->GetDialog(CAllDeviceStatusMaintainDlg::IDD));
	if(NULL != pInitialDlg)
		pInitialDlg->InitTitle();
	// 检测IO状态
	if(!InitializeIO()){
		SetInitRessult(ROW_IO_MODULE, INIT_RESULT::FAIL);
	}
	else{
		SetInitRessult(ROW_IO_MODULE, INIT_RESULT::SUCCESSFUL);
	}
	//// 检测充值读写器状态
	//if(!InitializeChargeRW()){
	//	SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::FAIL);
	//}
	//else{
	//	SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::SUCCESSFUL);
	//}
	if(is_tvm){
		// 检测票卡读写器状态
		if(!InitializeCardRW()){
			SetInitRessult(ROW_CARD_RW, INIT_RESULT::FAIL);
		}
		else{
			SetInitRessult(ROW_CARD_RW, INIT_RESULT::SUCCESSFUL);
		}
		//// 检测Smart-Card模块状态
		//if(!InitializeCardModule()){
		//	SetInitRessult(ROW_CARD_MODULE,INIT_RESULT::FAIL);
		//}
		//else{
		//	SetInitRessult(ROW_CARD_MODULE,INIT_RESULT::SUCCESSFUL);
		//}
		// 检测TOKEN模块状态
		if(!InitializeTokenModule()){
			SetInitRessult(ROW_TOKEN_MODULE,INIT_RESULT::FAIL);
		}
		else{
			SetInitRessult(ROW_TOKEN_MODULE,INIT_RESULT::SUCCESSFUL);
		}
		// 检测硬币模块状态
		if(!InitializeCoinModule()){
			SetInitRessult(ROW_COIN_MODULE,INIT_RESULT::FAIL);
		}
		else{
			SetInitRessult(ROW_COIN_MODULE,INIT_RESULT::SUCCESSFUL);
		}
	}	
	// 检测纸币模块状态
	if(!InitializeBanknoteModule()){
		SetInitRessult(ROW_BANKNOTE_MODULE,INIT_RESULT::FAIL);
	}
	else{
		SetInitRessult(ROW_BANKNOTE_MODULE,INIT_RESULT::SUCCESSFUL);
	}

	// 检测纸币找零模块
	/*if(!InitializeBanknoteChangeModule()){
	SetInitRessult(ROW_BANKNOTE_CHANGE_MODULE,INIT_RESULT::FAIL);
	}
	else{
	SetInitRessult(ROW_BANKNOTE_CHANGE_MODULE,INIT_RESULT::SUCCESSFUL);
	}*/

	// 检测打印机状态
	if(!InitializePrinter()){
		SetInitRessult(ROW_PRINT,INIT_RESULT::FAIL);
	}
	else{
		SetInitRessult(ROW_PRINT,INIT_RESULT::SUCCESSFUL);
	}	
	m_bIsFinished = true;
	theAPP_SESSION.ShowOfficeGuide(TXT_CONFIRM_THE_HARDWARE_STATUS_INFOMATION);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      充值读写器

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeChargeRW()
{
	//try {
	//	RECHARGERW_HELPER->Close();
	//	RECHARGERW_HELPER->InitCheck();
	//	RECHARGERW_HELPER->CheckDeviceExchange();

	//	// 硬件参数更新
	//	RECHARGERW_HELPER->InitData();
	//	RECHARGERW_HELPER->SynchronismParam();

	//	// 判断卡槽4是否有SAM卡，否则抛异常
	//	//SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

	//	//BYTE ISAMid[6];
	//	//memset(ISAMid,0,sizeof(ISAMid));
	//	//if(memcmp(samID.rw_acc_isam_id,ISAMid,sizeof(ISAMid)) == 0){
	//	//	theTVM_STATUS_MGR.SetSam1Status(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
	//	//	throw CRWHardwareException(RW_TYPE::TVM_RECHARGE_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
	//	//}
	//	//// 向上位报状态
	//	//theTVM_STATUS_MGR.SetSam1Status(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
	//}
	//catch (CSysException& e){
	//	DoException(e,ROW_CHARGE_RW);
	//	return false;
	//}
	//catch(...)	{
	//	CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INI_RECHARGE_RW_ERROR));
	//	return false;
	//	DoException(e,ROW_CHARGE_RW);
	//}
	return true;
}




//////////////////////////////////////////////////////////////////////////
/*
@brief      票卡读写器

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeCardRW()
{
try {
	CARDRW_HELPER->Close();
	CARDRW_HELPER->InitCheck();
	CARDRW_HELPER->CheckDeviceExchange();

	// 硬件参数更新
	CARDRW_HELPER->InitData();
	//CARDRW_HELPER->SynchronismParam();

	// 判断卡槽4是否有SAM卡，否则抛异常
	SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

	BYTE ISAMid[6];
	memset(ISAMid,0,sizeof(ISAMid));
	if(memcmp(samID.th_acc_psam_id,ISAMid,sizeof(ISAMid)) == 0){
		theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
		//throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCPSAMERR,_T(__FILE__),__LINE__);
	}
	// 上报SAM卡状态
	theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_NORMAL);
}
catch (CSysException& e){
	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_ERROR);
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
/*
@brief      票卡模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeCardModule()
{
	//try {
	//	// 断开连接
	//	CARD_HELPER->Disconnect();
	//	// 初期化
	//	CARD_HELPER->Initialize();
	//	//		CARD_HELPER->CheckDeviceExchange();

	//	//if(theFunction_INFO.IsCardNormalBoxRFIDWriteable() == 1) {
	//	//	CTicketBoxExchangeSvc pSvc;
	//	//	pSvc.CheckTicketBoxIsInvalidExchanged();	
	//	//}
	//	// RFID禁用
	//	//else{
	//	//	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxAInfo;	
	//	//	BYTE byBoxIDA[4] = {0x02,0x94,0x00,0x01};
	//	//	ticketBoxAInfo.ticketBoxID.Deserialize(byBoxIDA);
	//	//	ticketBoxAInfo.ulCurCount = 500;
	//	//	ticketBoxAInfo.ulInitCount =  500;
	//	//	theCOUNT_INFO.SetTicketboxAInfo(ticketBoxAInfo);

	//	//	CCTLOGCountInfo::TICKETBOX_INFO ticketBoxBInfo;	
	//	//	BYTE byBoxIDB[4] = {0x02,0x94,0x00,0x02};
	//	//	ticketBoxBInfo.ticketBoxID.Deserialize(byBoxIDB);
	//	//	ticketBoxBInfo.ulCurCount = 500;
	//	//	ticketBoxBInfo.ulInitCount =  500;
	//	//	theCOUNT_INFO.SetTicketboxBInfo(ticketBoxBInfo);

	//	//	SC_TICKET_DATA ticketData;
	//	//	ticketData.wMagazineANumber = 500;
	//	//	ticketData.wMagazineBNumber = 500;
	//	//	ticketData.wCollectionBoxNumber = 0;
	//	//	m_pTHHelper->TicketDataWriteCMD(ticketData);
	//	//}
	//	// 初始化票卡模块数量
	//	//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxAInfo;		// token箱A
	//	//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxBInfo;		// token箱B
	//	//CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token废票箱
	//	//CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card箱A
	//	//CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card废票箱
	//	//tokenBoxAInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
	//	//tokenBoxBInfo		= theTOKEN_COUNT.GetTokenboxAInfo();
	//	//tokenWasteBoxInfo	= theTOKEN_COUNT.GetTokenboxWastedInfo();
	//	//cardBoxAInfo		= theCARD_COUNT.GetCardboxAInfo();
	//	//cardWasteBoxInfo	= theCARD_COUNT.GetCardboxWastedInfo();


	//	//SC_TICKET_DATA ticketData;
	//	//ticketData.wMagazineANumber = cardBoxAInfo.ulCurCount;
	//	//ticketData.wMagazineBNumber = 0;
	//	//ticketData.wCollectionBoxNumber = cardWasteBoxInfo.ulCurCount;
	//	//CARD_HELPER->TicketDataWriteCMD(ticketData);
	//	// 票箱更换检查（需要打印帐票，放在打印机初期化之后处理）
	//	//CTicketBoxHelper ticketBoxHelper(*this);
	//	//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
	//	theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_NORMAL);
	//	return true;
	//}
	//catch (CSysException& e) {
	//	DoException(e,ROW_CARD_MODULE);
	//	return false;
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("出票机初始化错误"));
	//	DoException(e,ROW_CARD_MODULE);
	//	return false;
	//}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票卡模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeTokenModule()
{
	try {
		// 断开连接
		TOKEN_HELPER->Disconnect();
		TOKEN_HELPER->Initialize();

		theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_NORMAL);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_TOKEN_MODULE);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("出票机初始化错误"));
		DoException(e,ROW_TOKEN_MODULE);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeCoinModule()
{
	try {
		// 断开连接
		COIN_HELPER->CH_Close();
		COIN_HELPER->CH_InitModule();
		// 监测硬币回收箱是否在位？
		//CH_COMMON_RSP rsp;
		//COIN_HELPER->CH_GetStatus(&rsp);
		//if(!rsp.sensor_info.CH_BOX1_SET_STATUS && !rsp.sensor_info.CH_BOX2_SET_STATUS){
		//	//throw CCHHardwareException(CCHHardwareException::ERR_CH_COLLECTION_BOX_NOT_INSTALL,_T(__FILE__),__LINE__);
		//}
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
/*
@brief      纸币模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeBanknoteModule()
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
@brief      纸币找零模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeBanknoteChangeModule()
{
	bool bRet = true , lRet = true;
	//try {
	//	//// 1.返初始化
	//	//lRet = BH_CHANGE_HELPER->UnInitialize();
	//	//// 2.重新初始化
	//	//if(BHTRS_RES_OK == lRet)
	//	//	lRet = BH_CHANGE_HELPER->initialize();
	//}
	//catch (CSysException& e) {
	//	bRet = false;
	//	DoException(e,ROW_BANKNOTE_CHANGE_MODULE);		
	//}
	//catch (...) {
	//	bRet = false;
	//	CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _opl(TXT_INIT_BANKNOTE_CHANGE_MODULE));
	//	DoException(e,ROW_BANKNOTE_CHANGE_MODULE);		
	//}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     打印机

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializePrinter()
{
	try {
		PRINTER_HELPER->PrinterClose();
		PRINTER_HELPER->PrinterConnect();
		PRINTER_HELPER->CheckStatus();
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
/*
@brief      IO模块

@param      无

@retval     true:成功，false失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAllDeviceStatusSvc::InitializeIO()
{
	try {
		// 停止IO监控
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_IO_MONITOR);

		IO_HELPER->CloseIO();
		// 初期化
		IO_HELPER->OpenIO();
		IO_HELPER->InitializeIO();
		// 检测一下IO状态
		IO_RSP_STATUS_INFO ioStatus;
		IO_HELPER->GetIOStatus(ioStatus);

		// 开启IO监控
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_IO_MODULE);
		// 开启IO监控
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);
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
/*
@brief      异常显示

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllDeviceStatusSvc::DoException(CSysException& e,int index)
{
	CString guideMsg = theEXCEPTION_MGR.GetDescription(e);
	CString dealMsg = theEXCEPTION_MGR.GetPrompt(e);
	m_InitialInfo[index].errorInfo = guideMsg;
	m_InitialInfo[index].dealInfo = dealMsg;
	theEXCEPTION_MGR.ProcessException(e);
	SetInitRessult(index, INIT_RESULT::FAIL);
	//Sleep(2000);
}
