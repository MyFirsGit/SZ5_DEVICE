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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CInitSvc::CInitSvc(void)
	:CTVMForeService(INIT_SVC,MAIN_SVC)
{
	// �����ʼ��״̬
	theSERVICE_MGR.SetState(INIT);
	//theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CInitSvc::OnExceptionNotify);

	// ��ʼ�����ݱ���
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
	m_InitialInfo[ROW_DISPLAY].title				= _T("״̬��ʾ��");
	// �Ƿ��ʼ�����Գ�ʼ��������������
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
	// ���ó�ʼ�����
	for (int i = 0; i < ROW_MAX; ++i) {
		m_InitialInfo[i].result = UNKNOWN;
	}

	// ���ò���Ա����
	m_pDialogFlow->AddDialog(CInitDlg::IDD, new CInitDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CInitDlg::IDD);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CInitSvc::~CInitSvc(void)
{
	//theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CInitSvc::OnExceptionNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::OnStart()
{
	// ��ʾ��ʼ������
	__super::OnStart();

	// ǿ��ˢ����Ļ
	m_pDialogFlow->GetDialog(CInitDlg::IDD)->UpdateWindow();		

	// ��ʾ�ȴ����
	theCURSOR_MGR.BeginWaitCursor();

	// ������־Ŀ¼
	CString tracePath = theTVM_INFO.GetLogPath();
	CLog::Initialize(tracePath);
	LOG("----------------------- TVM Start run-------------------");
	bool bInitOK = Initialize();

	// ��ͣ�����������ϵ���ʾ���
	Sleep(2000);

	// �����ȴ����
	theCURSOR_MGR.EndWaitCursor();

	/*theSERVICE_MGR.SetState(MAINTENANCE);
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
	theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);*/
	//if (bInitOK && (!theEXCEPTION_MGR.HasException())) { 
	//	// ����Login����
	//	theSERVICE_MGR.StartService(LOGIN_SVC);
	//}
	//// ���ڻ�ʧ��
	//else {          
	//	// ��ʾ���˵�
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);
	//	//theMENU_MGR.SetMenuKind(CMenuMgr::MENU_INIT_ABNORMAL);
	//}

	// ������ֹ��״̬
	theSERVICE_MGR.SetState(OUT_OF);
	// ����Ĭ�Ϸ���
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();	
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
}

//////////////////////////////////////////////////////////////////////////
/**	mei_xiaowei
@brief       �������ּ���

@param     BOOL TRUE ����
FALSE ����

@retval      ��

@exception   ��
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
@brief      ���ڻ�

@param      ��

@retval     bool, true:OK; false:FAIL

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::Initialize()
{
	bool ret = true;
	int DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// ��ȡ�豸����

	// ��ʼ��������
	if(!InitializeController()){
		return false;
	}

	// ������̨service
	StartBackService();

	// ����Ŀ¼֮���ʼ��
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
	// ���̼��
	LOG("[Initialize] CheckDiskFreeSpace");
	if(!CheckDiskFreeSpace()){
		ret = false;
	}
	else{
		SetInitRessult(ROW_DISK_CHECK, INIT_RESULT::SUCCESS);   // ���ó�ʼ����������̼�⣩
	}
	// ���ݳ�ʼ��
	LOG("[Initialize] InitializeParameter");
	if (!InitializeParameter()) {
		ret = false;
	}
	else {
		bIsInitParameterOk = true;
		SetInitRessult(ROW_DATA_INIT, INIT_RESULT::SUCCESS);   // ���ó�ʼ�����������һ���Լ�飩
	}
	// ��ʾ���ݳ�ʼ�����
	if(bIsInitParameterOk){
		SetInitRessult(ROW_DATA_INIT,INIT_RESULT::SUCCESS);		
	}


	//MessageBoxA(NULL,NULL,NULL,1);
	// IOģ��
	
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
	

	//��ֵ��д��
	LOG("[Initialize] InitializeChargeRW");
	if(!InitializeChargeRW()) {
		SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
		ret = false;
	}
	else {
		//// ��ȷ��
		//CheckSAMidChange(RECHARGE_RW_YPT_ISAM);
		//CheckSAMidChange(RECHARGE_RW_YPT_PSAM);
		//CheckSAMidChange(RECHARGE_RW_YKT_ISAM);
		//CheckSAMidChange(RECHARGE_RW_YKT_PSAM);
		SetInitRessult(ROW_CHARGE_RW, INIT_RESULT::SUCCESS);
	}

	// Card��д��
	LOG("[Initialize] InitializeCardRW");
	if(!InitializeCardRW()) {
		SetInitRessult(ROW_CARD_RW, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
		ret = false;
	}
	else {
		// ��ȷ��
		CheckSAMidChange(CARD_RW_YPT_ISAM);
		CheckSAMidChange(CARD_RW_YPT_PSAM);
		CheckSAMidChange(CARD_RW_YKT_ISAM);
		CheckSAMidChange(CARD_RW_YKT_PSAM);
		SetInitRessult(ROW_CARD_RW, INIT_RESULT::SUCCESS);
	}

	// Cardģ��
	if(!InitializeCardModule()) {
		SetInitRessult(ROW_CARD_MODULE, INIT_RESULT::FAIL);
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_CARD_MODULE, INIT_RESULT::SUCCESS);
	}
	
	// Ӳ��ģ��
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
	// ֽ��ģ��
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
	
	// ƾ֤��ӡ��
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
	
	//��ʼ��״̬��ʾ��
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
	//// ��ʼ����ϣ����ó������б�ʶΪfalse.
	//LOG("[Initialize] Initialize finish");
	//theAPP_SESSION.SetIsFirstRun(false);
	m_pDialogFlow->GetDialog(CInitDlg::IDD)->UpdateWindow();

	// �����豸���
	StartDeviceMonitor();

	// �����ڸ��³�����ػ����³���
	bool needUpgrade = theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade();
	if(needUpgrade)	{
		LOG("[Initialize] TPU need update, restart!");
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);	
		return false;
	}

	// ��ʼ����λ
	LOG("[Initialize] link SC!");
	if (InitializeCPS() != true){
		ret = false;
	}

	// ������ά�붩�����ش���ʱ��
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_WEB_IDLE_PROCESS,NULL,NULL);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RECHARGE_SERVER_TIMER,NULL,NULL);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_PARAMETER_UPDATE_TIMER,NULL,NULL);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����豸����߳�

@param      (i)int index
@param      (i)INIT_RESULT result

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::StartDeviceMonitor()
{
	// ���ó�ֵ��д���豸���
	if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
		theSERVICE_MGR.StartService(MONITOR_RECHARGE_RW_SVC,false);
		if(!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)){
			theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();
		}
	}
	// ���ö�д���豸���
	if(theTVM_INFO.IsRWMonitorOpened(TVM_CARD_RW)){
		theSERVICE_MGR.StartService(MONITOR_CARD_RW__SVC,false);
		if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID)){
			theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_CARD_RW__SVC)->Resume();
		}
	}

	// ����������̶�д���豸���
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
	// ���������Banknote��أ�������Banknote��ؼ�ʱ��
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
		// ���������Coin��أ�������Coin��ؼ�ʱ��
		if(theTVM_INFO.IsCHMonitorOpened())
		{
			theSERVICE_MGR.StartService(MONITOR_COIN_MODULE_SVC,false);
			if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID))
			{
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Resume();
			}
		}
	}

	// ���������IO��أ�������IO��ؼ�ʱ��
	if(theTVM_INFO.IsIOMonitorOpened()){
		if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			theSERVICE_MGR.StartService(MONITOR_IO_SVC,false);
			theSERVICE_MGR.GetService<CIOMonitorSvc>(MONITOR_IO_SVC)->Resume();
			theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);

		}
	}
	// �����������ӡ����أ�������RP��ؼ�ʱ��
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
@brief      ���ó��ڻ����

@param      (i)int index
@param      (i)INIT_RESULT result

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::SetInitRessult(int index, INIT_RESULT result)
{
	m_InitialInfo[index].result = result;

	// ˢ��Dialog
	CInitDlg* pInitialDlg = (CInitDlg*)(m_pDialogFlow->GetDialog(CInitDlg::IDD));
	pInitialDlg->ShowInitRessult(index);
	//Sleep(100);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�Controller

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeController()
{

	// ����FTP������
	g_pFTPControl = new CFTPControl();
	ASSERT(g_pFTPControl != NULL);
	g_pFTPControl->Initialize();

	// �������ݿ������
	g_pDataControl = new CDataControl();
	ASSERT(g_pDataControl != NULL);
	g_pDataControl->Initialize();

	// ����SCͨ�ſ�����
	g_pSCControl = new CSCControl();
	ASSERT(g_pSCControl != NULL);
	g_pSCControl->Initialize();

	// ������ʽ��Ʊ��������
	g_pMIMControl = new CMIMControl();
	ASSERT(g_pMIMControl != NULL);
	g_pMIMControl->Initialize();
	//	g_pMemControl = new CMemController(100);
	//	ASSERT(g_pMemControl != NULL);
	//	g_pMIMControl->SetMemController(g_pMemControl);

	// ������д��������
	g_pTVMRWControl = new CRWControl();
	g_pTVMRWControl->Initialize();

	//// �����ۿ���д��������
	//g_pCardRWControl = new CRWControl();
	//g_pCardRWControl->Initialize();

	//// ����TOKEN��д��������
	//g_pTokenRWControl = new CRWControl();
	//g_pTokenRWControl->Initialize();

	//// ������ֵ��д��������
	//g_pChargeRWControl = new CRWControl();
	//g_pChargeRWControl->Initialize();

	// ������ӡ��������
	//g_pPrinterControl = new CTVMPrinterControl();
	//g_pPrinterControl->Initialize();

	// ����Ӳ��ģ�������
	g_pCHControl = new CCHControl();
	g_pCHControl->Initialize();

	// ����ֽ��ģ�������
	g_pBHControl = new CBRHControl();
	g_pBHControl->Initialize();

	// ����ֽ������ģ�������
	g_pBHChangeControl = new CBHChangeControl();
	g_pBHChangeControl->Initialize();
	// ����Token��Ʊ��������
	//g_pTokenControl = new CTokenControl();
	//g_pTokenControl->Initialize();

	//// ����IO������
	//g_pIOControl = new CIOControl();
	//g_pIOControl->Initialize();

	// ����������̿�����
	//g_pPinpadControl = new CTVMPinPadControl();
	//g_pPinpadControl->Initialize();

	// �������п�����ģ�������
	//g_pBankCardControl = new CTVMBankCardControl();
	//g_pBankCardControl->Initialize();

	// ����״̬��ʾ��ģ�������
	g_pStatusDisplayControl = new CStatusDisplayControl();
	g_pStatusDisplayControl->Initialize();

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ����̨service

@param    none

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::StartBackService()
{
	// ������̨service
	theSERVICE_MGR.StartService(SC_SVC,false);
	theSERVICE_MGR.StartService(BUSINESS_INTERVAL_SVC,false);
	theSERVICE_MGR.StartService(EVENT_MSG_SVC,false);
	theSERVICE_MGR.StartService(TRANSMISSION_INTERVAL_SVC,false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����һ���Լ��

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckConsistent()
{
	bool bRet = false;
	try {
		// ���ڻ�ϵͳ�����ļ�
		if(InitializeSystemConfig()){
			// ��ʼ��ϵͳĿ¼
			if(InitializeSystemFolder()){
				// ��ʼ��ϵͳ����
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
@brief      �������̿ռ�

@param      ��

@retval     ��

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckDiskFreeSpace()
{
	try{
		// �����̿ռ�
		ULARGE_INTEGER freeBytes,totalBytes,totalFreeBytes;

		// ������Ŀ¼
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

		// ��������Ŀ¼
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

		// ��־�ļ�Ŀ¼
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
@brief       ���ڻ�ϵͳ����

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeParameter()
{
	bool bRes = true;
	try{

		DWORD dw1=GetTickCount();
		// ��ʼ��ϵͳ��־
		if(!InitializeSystemLog()){
			LOG("[Initialize] InitializeSystemLog failed!");
			bRes = false;	
		}

		// ��ʼ���쳣����
		if(!InitializeSystemException()){
			LOG("[Initialize] InitializeSystemException failed!");
			bRes = false;	
		}

		// ��ʼ��MASTER
		if(!InitializeSystemMaster()){
			LOG("[Initialize] InitializeSystemMaster failed!");
			bRes = false;	
		}

		// ��ʼ����������
		if(!InitializeTxnData()){
			LOG("[Initialize] InitializeTxnData failed!");
			bRes = false;	
		}

		// ��ʼ�������ļ�
		if(!InitializeParamData()){
			LOG("[Initialize] InitializeParamData failed!");
			bRes = false;	
		}

		// ��ʼ�����ݿ�
		if(!InitializeDatabase()){
			LOG("[Initialize] InitializeDatabase failed!");
			bRes = false;	
		}
		// ��ʼ���Ĵ�������
		if(!InitializeRegistAudit()){
			LOG("[Initialize] InitializeRegistAudit failed!");
			bRes = false;	
		}
		// ��ʼ��ϵͳʱ��
		if(!InitializeSystemTime()){
			LOG("[Initialize] InitializeSystemTime failed!");
			bRes = false;	 
		}
		// ��ʼ���������
		if(!InitializeSCAudit()){
			LOG("[Initialize] InitializeSCAudit failed!");
			bRes = false;	
		}

		// ͬ�����ס���־���������ݱ�������
		theTVM_INFO.SetStoreDay(theAFC_TVM_OPERATION.GetMaxLogSaveDays());

		theFunction_INFO.SetLimitedDaysOfSendData(theAFC_TVM_OPERATION.GetSendedMsgSaveDays());

		theTVM_SETTING.SetMaxOffLineDay(theAFC_TVM_OPERATION.GetOfflineWorkDays());
		// �ж��Ƿ񳬹������������
		try{
			_DATE dtDay	= theTVM_SETTING.GetLastOnlineDate();
			CTime lastTime(dtDay.wYear,dtDay.biMonth,dtDay.biDay,0,0,0,-1);
			WORD  wDay	= theTVM_SETTING.GetMaxOffLineDay();//theAFC_TVM_OPERATION.GetMaxOffLineTime();

			CTimeSpan timeSpan = CTime::GetCurrentTime() - lastTime;
			// ���������������
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
@brief       ��ʼ��״̬����

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
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
@brief       ���ڻ�Card��д��

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCardRW()
{
	try {
		CARDRW_HELPER->InitCheck();
		CARDRW_HELPER->CheckDeviceExchange();

		// Ӳ����������
		CARDRW_HELPER->InitData();
		//CARDRW_HELPER->SynchronismParam();

		//// ͬ����ǰվ�㵽ȫ�����Ʊ��
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

		//// �жϿ���4�Ƿ���SAM�����������쳣
		//SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

		//BYTE PSAMid[6];
		//memset(PSAMid,0,sizeof(PSAMid));
		//if(memcmp(samID.th_acc_psam_id,PSAMid,sizeof(PSAMid)) == 0){
		//	theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
		//	//throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
		//}
		//// �ϱ�SAM��״̬
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
@brief       ���ڻ�Token��д��

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CInitSvc::InitializeTokenRW()
//{
//	try {
//		TOKENRW_HELPER->InitCheck();
//		TOKENRW_HELPER->CheckDeviceExchange();
//
//		// Ӳ����������
//		TOKENRW_HELPER->InitData();
//		TOKENRW_HELPER->SynchronismParam();
//
//		// �жϿ���4�Ƿ���SAM�����������쳣
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
@brief       ���ڻ���ֵ��д��

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeChargeRW()
{
	try {
		RECHARGERW_HELPER->InitCheck();
		RECHARGERW_HELPER->CheckDeviceExchange();

		// Ӳ����������
		RECHARGERW_HELPER->InitData();
		//RECHARGERW_HELPER->SynchronismParam();

		//// �жϿ���4�Ƿ���SAM�����������쳣
		//SAM_ID_INFO samID= theAPP_SESSION.GetSAMInfo();

		//BYTE ISAMid[6];
		//memset(ISAMid,0,sizeof(ISAMid));
		//if(memcmp(samID.rw_acc_isam_id,ISAMid,sizeof(ISAMid)) == 0){
		//	theTVM_STATUS_MGR.SetSam1Status(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
		//	throw CRWHardwareException(RW_TYPE::TVM_RECHARGE_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
		//}
		//// ����λ��״̬
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
@brief       ��ֵ�ն���Ȩ��һ��ͨ����֤

@param       ��

@retval      INIT_RESULT    ��֤���

@exception   ��
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
@brief       ACC��Ȩ��һƱͨ����֤

@param       ��

@retval      INIT_RESULT    ��֤���

@exception   ��
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
@brief       ���ڻ���ӡ��(Printer)

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
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
@brief       ���ڻ�IOģ��

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeIO()
{
	try {
		// ���ڻ�
		IO_HELPER->OpenIO();
		IO_HELPER->InitializeIO();
		// ���һ��IO״̬
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
@brief       ���ڻ�UPS

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
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
@brief       ���ڻ���λ(CPS)

@param       ��

@retval      INIT_RESULT    ��֤���

@exception   ��
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
@brief      ��鴴��������Ŀ¼

@param      ��

@retval     ��

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckCreateHomeFolder()
{
	// ��鲢����������Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath())) {    // not exist
		return false;
	}
	// ��鲢������������Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}

	// ��鲢����master�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢���������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢���������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}	
	// ��鲢����ͼƬ�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PHOTO_MAIN_DIR)) {
		return false;
	}
	// ��鲢������ʱͼƬ�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetDataPath() + PHOTO_TEMP_DIR)) {
		return false;
	}
	// ɾ����ʱͼƬ
	DeletePicture();
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��鴴������Ŀ¼

@param      ��

@retval     ��

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::CheckCreateBackupFolder()
{
	// ��鲢����������Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath())) {    // not exist
		return false;
	}
	// ��鲢������������Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����master�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����ACC�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢���������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢��������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetBackupPath() + AR_MANAGE_FOLDER)) {    // if not exist, create it
		return false;
	}
	// ��鲢����traceĿ¼
	if (! ComCheckAndCreateDirectory(theTVM_INFO.GetLogPath())) {    // if not exist, create it
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ��ָ������ǰ��FTP�ϴ�����־

@param      (i)int days    ָ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeleteLog(int days)
{
	try {
		// �õ�ָ��������ǰ�����ڽṹ���Լ��ַ���
		CString dayLimitStr;
		vector<int> logdate;
		GetLimitLogDate(days,logdate);
		if (logdate.size() == 0) {
			return;
		}
		dayLimitStr.Format(_T("%d"),logdate[0]);

		// ȡ���ļ�Ŀ¼
		CString sDir = theTVM_INFO.GetLogPath();		
		sDir += _T("LogZipBackup\\History");
		// �����ļ�
		vector<CString> fileNames;
		SearchFile(sDir + _T("*.zip"),fileNames);
		vector<CString>::iterator iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// ȡ���ļ��������ڱȽ�����
			CString dateStr = _T("");
			dateStr = ((*iter).Right(18)).Left(8);

			// �������С��ָ��������ɾ��
			if(dateStr < dayLimitStr) {
				// ɾ�����ļ�
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
@brief      ɾ����Ƭ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeletePicture()
{
	try {
		// ȡ���ļ�Ŀ¼
		CString sDir = theTVM_INFO.GetDataPath() + PHOTO_MAIN_DIR;		

		// �����ļ�
		vector<CString> fileNames;
		SearchFile(sDir + _T("\\*.*"),fileNames);
		vector<CString>::iterator iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// ɾ�����ļ�
			DeleteFile(*iter);
			iter ++;
		}
		sDir = theTVM_INFO.GetDataPath() + PHOTO_TEMP_DIR;	
		// �����ļ�
		fileNames.clear();
		SearchFile(sDir + _T("\\*.*"),fileNames);
		iter = fileNames.begin();
		while (iter != fileNames.end()) {
			// ɾ�����ļ�
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
@brief       ��ȡָ��������log��־�ļ�����

@param       (i)int days			  ָ������
@param       (o)vector<int>& logdate  �����б�

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::GetLimitLogDate(int days,vector<int>& logdate)
{
	try {
		// ȡ���ļ�Ŀ¼
		CString sDir = theTVM_INFO.GetLogPath();

		sDir += _T("LogZipBackup\\History");
		// �����ļ�
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".zip"));

		// ���û���ҵ��ļ����͹ر��ļ�
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
				// ȡ���ļ��������ڱȽ�����
				CString dateStr;
				sFileName = finder.GetFileName();
				dateStr = (sFileName.Right(18)).Left(8);
				logdate.push_back(_ttoi(dateStr));
			}
		}

		finder.Close();

		// ����־�ļ�����������(��С����)
		sort(logdate.begin(),logdate.end(),less<int>());

		// ɾ�������ظ�������
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
@brief      ɾ��ָ������ǰ����־�ļ�

@param      (i)int days    ָ������

@retval     ��

@exception  ��
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
@brief       ��ȡָ��������log��־�ļ�����

@param       (i)int days			  ָ������
@param       (o)vector<int>& logdate  �����б�

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::GetLimitTpuLogDate(int days,vector<CString>& logdate)
{
	try {
		// ȡ���ļ�Ŀ¼
		CString appPath;
		GetAppPath(appPath);
		CString sDir = appPath + _T("Log\\");

		// �����ļ�
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*.*"));

		// ���û���ҵ��ļ����͹ر��ļ�
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
				// ȡ���ļ��������ڱȽ�����
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
@brief      ��ȡ�쳣��ʾ��Ϣ

@param      (i)CSysException e    �쳣

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// CString CInitSvc::GetInitErrorInfo(CSysException e)
// {
// 	// ��ʾ�쳣��Ϣ
// 	CString stm = _T("");
// 	//stm.Format(_T("%s%08X%s%s"), _T("("), e.GetActualMTC(), _T("),"), e.GetDescription());
// 	//stm = _T(",") + e.GetDescription() + _T(",") + e.GetPrompt();
// 	stm.Format(_T("%s%08X%s%s%s%s"), _T("("), e.GetActualMTC(), _T("),"), e.GetDescription(),_T(","),e.GetPrompt());
// 	return stm;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ׷��δ�ͼ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// void CInitSvc::AddUnsendAudit()
// {
// 	
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief     �����쳣

@param      ��

@retval     ��

@exception  ��
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
@brief      ���SamId�仯

@param      (i)bool, true:�ж�TH��ISAM�仯;  false:�ж�RW��ISAM�仯

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::CheckSAMidChange(SAM_POSITION samPos)
{
	// SAMid�����ı�
	DWORD samIdNo = 0;
	// ISAM��������ı�
	SAM_ID_INFO samId;
	samId = theAPP_SESSION.GetSAMInfo();
	BYTE nosam[6] = {0};
	CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
	theDeviceInfo.GetDeviceInfo(deviceInfo);
	//theDEBUG_LOG.WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T(""));
	switch (samPos) {
	case RECHARGE_RW_YPT_ISAM:
		//�ⲿһƱͨISAM
		if (memcmp(samId.rw_acc_isam_id,nosam,6) != 0 && memcmp(samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM,6) != 0) {
			// �����豸������������
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM);
			// ����master�ļ�
			theDeviceInfo.SetACC_RW1_ISAM(samId.rw_acc_isam_id);
		}	
		break;
	case RECHARGE_RW_YPT_PSAM:
		//�ⲿһƱͨPSAM
		if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM,6) != 0) {
			// �����豸������������
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM);
			// ����master�ļ�
			theDeviceInfo.SetACC_RW1_PSAM(samId.rw_acc_psam_id);
		}	
		break;
	case RECHARGE_RW_YKT_ISAM:
		{// ECT ISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW1_ISAM(samId.rw_ect_isam_id);
			}
		}
		break;
	case RECHARGE_RW_YKT_PSAM:
		{// ECT PSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM);
				// ����master�ļ�
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
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW2_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW2_ISAM(samId.rw_acc_isam_id);
			}	
		}
		break;
	case TOKEN_RW_YPT_PSAM:
		{//ACC PSAM
			if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW2_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW2_PSAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW2_PSAM(samId.rw_acc_psam_id);
			}	
		}
		break;
	case TOKEN_RW_YKT_ISAM:
		{//ETC ISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW2_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW2_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW2_ISAM(samId.rw_ect_isam_id);
			}
		}
		break;
	case TOKEN_RW_YKT_PSAM:
		{//ETC PSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW2_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendSamComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW2_PSAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW2_PSAM(samId.rw_ect_psam_id);
			}
		}
		break;
		//case TH_YPT_ISAM:
		//	// �ڲ�һƱͨISAM
		//	if (memcmp(samId.th_acc_isam_id,nosam,6) != 0 && memcmp(samId.th_acc_isam_id,deviceInfo.ACC_TH_ISAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.th_acc_isam_id,deviceInfo.ACC_TH_ISAM);
		//		// ����master�ļ�
		//		theDeviceInfo.SetACC_TH_ISAM(samId.th_acc_isam_id);
		//	}	
		//	break;
		//case TH_YPT_PSAM:
		//	// �ڲ�һƱͨPSAM
		//	if (memcmp(samId.th_acc_psam_id,nosam,6) != 0 && memcmp(samId.th_acc_psam_id,deviceInfo.ACC_TH_PSAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.th_acc_psam_id,deviceInfo.ACC_TH_PSAM);
		//		// ����master�ļ�
		//		theDeviceInfo.SetACC_TH_PSAM(samId.th_acc_psam_id);
		//	}	
		//	break;
		//case RW_YKT_ISAM:
		//	// �ⲿһ��ͨISAM
		//	if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW_ISAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW_ISAM);
		//		// ����master�ļ�
		//		theDeviceInfo.SetECT_RW_ISAM(samId.rw_ect_isam_id);
		//	}
		//	break;
		//case RW_YKT_PSAM:
		//	// �ⲿһ��ͨPSAM
		//	if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW_PSAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW_PSAM);
		//		// ����master�ļ�
		//		theDeviceInfo.SetECT_RW_PSAM(samId.rw_ect_psam_id);
		//	}
		//	break;
		//case TH_YKT_ISAM:
		//	// �ڲ�һ��ͨISAM
		//	if (memcmp(samId.th_ect_isam_id,nosam,6) != 0 && memcmp(samId.th_ect_isam_id,deviceInfo.ECT_TH_ISAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.th_ect_isam_id,deviceInfo.ECT_TH_ISAM);
		//		// ����master�ļ�
		//		theDeviceInfo.SetECT_TH_ISAM(samId.th_ect_isam_id);
		//	}
		//	break;
		//case TH_YKT_PSAM:
		//	// �ڲ�һ��ͨPSAM
		//	if (memcmp(samId.th_ect_psam_id,nosam,6) != 0 && memcmp(samId.th_ect_psam_id,deviceInfo.ECT_TH_PSAM,6) != 0) {
		//		// �����豸������������
		//		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.th_ect_psam_id,deviceInfo.ECT_TH_PSAM);
		//		// ����master�ļ�
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
		// ����Login����
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ��Smart-Cardģ��

@param       ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeCardModule()
{
	try {
		// ���ڻ�
		CARD_HELPER->Initialize();
		//		CARD_HELPER->CheckDeviceExchange();

		//if(theFunction_INFO.IsCardNormalBoxRFIDWriteable() == 1) {
		//	CTicketBoxExchangeSvc pSvc;
		//	pSvc.CheckTicketBoxIsInvalidExchanged();	
		//}
		// RFID����
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
		// ��ʼ��Ʊ��ģ������
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxAInfo;		// token��A
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxBInfo;		// token��B
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token��Ʊ��
		//CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card��A
		//CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card��Ʊ��
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
		// Ʊ�������飨��Ҫ��ӡ��Ʊ�����ڴ�ӡ�����ڻ�֮����
		//CTicketBoxHelper ticketBoxHelper(*this);
		//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_CARD_MODULE);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��Ʊ����ʼ������"));
		DoException(e,ROW_CARD_MODULE);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ��Tokenģ��

@param       ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTokenModule()
{
	//	try {
	//		// ���ڻ�
	//		TOKEN_HELPER->Initialize();
	//		//TOKEN_HELPER->CheckTicketBoxExist();
	//
	//// 		if(theFunction_INFO.IsCardNormalBoxRFIDWriteable() == 1) {
	//// 			//CTicketBoxExchangeSvc pSvc;
	//// 			//pSvc.CheckTicketBoxIsInvalidExchanged();	
	//// 		}
	//		 //RFID����
	//// 		else{
	//// 			// ��ʼ��Ʊ��ģ������
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxAInfo;		// token��A
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxBInfo;		// token��B
	//// 			CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token��Ʊ��
	//// 			CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card��A
	//// 			CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card��Ʊ��
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
	//		// Ʊ�������飨��Ҫ��ӡ��Ʊ�����ڴ�ӡ�����ڻ�֮����
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
@brief       ��ʼ��ֽ��ģ��

@param       ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeBanknoteModule()
{
	bool bRet = true;
	try {
		// ���ڻ�
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
@brief      ��ʼ��ֽ������ģ��

@param      ��

@retval     bool true:�ɹ��� false��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeBanknoteChangeModule(){
	bool bRet = true;
	// ֽ������ģ��RFID
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
@brief       ��ʼ��Ӳ��ģ��

@param       ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
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
@brief       ��ʼ��ϵͳ��־

@param       ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemLog()
{
	bool bRet =true;
	// ��ʼ����־
	try{
		// ������־Ŀ¼
		CString tracePath = theTVM_INFO.GetLogPath();
		//CLog::Initialize(tracePath);20180309,lichao:��־�ĳ�ʼ���ŵ����г�ʼ��֮ǰ

		// ��ʼ��Ӳ��ģ��ͨ����־
		USES_CONVERSION;
		//MIMTrs_Initialize(T2A(tracePath));        // Th Trace (��������)
		//BHTrs_Initialize(T2A(tracePath));			// ֽ�ҽ���ģ����־
		BHChangeTrs_Initialize(T2A(tracePath));		// ֽ������ģ����־
		//BHChangeRfidTrs_Initialize(T2A(tracePath));
		CHTrs_Initialize(T2A(tracePath));			// Ӳ��ģ����־
		//IOTrs_Initialize(T2A(tracePath));			// IOģ����־
		//CTSPTrs_Initialize(T2A(tracePath));			// ������ʾ����־

		// �Ƿ��¼Debug��Ϣ
		theDEBUG_LOG->SetWriteDebug(theTVM_INFO.IsWriteDebug());

		// ɾ��TVM��־ 
		//DeleteLog(theTVM_INFO.GetStoreDay());
		// ɾ��TPU��־		
		//DeleteTpuLog(theTVM_INFO.GetStoreDay());
		// ɾ����ӡ��־
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
@brief       ��ʼ��ϵͳ�쳣

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemException()
{
	bool bRet = true;
	// ��ʼ���쳣����
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
@brief       ��ʼ��ϵͳ����

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemConfig() 
{
	bool bRet = true;
	try {
		// ���ڻ���INI�ļ�
		int initResult = 0;
		initResult = theTVM_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _opl(TXT_INI_REINSTALL_PROGRAM);
			CString stm = _opl(TXT_INI_SYSTEM_FILES_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}
		// ��ʼ��ϵͳ����INI�ļ�
		initResult = theTVM_SETTING.Initialize();
		if (initResult != 0) {
			CString msg = _opl(TXT_INI_REINSTALL_PROGRAM);
			CString stm = _opl(TXT_INI_SYSTEM_FILES_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}

		// ��ʼ��ά������INI�ļ�
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
@brief       ��ʼ������ϵͳ����(���̹��ܼ��������������������ʾ�������ʽ���������վ��)

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeOSSetting() 
{
	bool bRet = true;
	try {
		// ���ܼ�����
		CShieldKeyHelper::SetFunctionKeysEnable(theTVM_INFO.IsFunctionKeyEnabled());
		// �������������
		CShieldKeyHelper::SetTaskManagerEnable(theTVM_INFO.IsTaskManagerEnabled());
		// ��������
		CShieldKeyHelper::SetDesktopEnable(theTVM_INFO.IsDesktopEnabled());
		// ��ջ���վ
		SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|SHERB_NOSOUND);
		// �����ػ�����
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
@brief       ��ʼ��ϵͳĿ¼�ṹ

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemFolder()
{
	bool bRet = true;
	try {
		// ��鱾��TPUǿ�Ƹ���Ŀ¼
		TCHAR szAppPath[_MAX_PATH];
		if (ComGetAppPath(szAppPath, _MAX_PATH)>0) {
			CString fileName = CString(szAppPath) + _T("PG\\TPU-APP\\CURRENT");
			ComCheckAndCreateDirectory(fileName);
		}

		// ��鴴��������Ŀ¼
		if (!CheckCreateHomeFolder()) {
			CString msg = _opl(TXT_INI_CHECK_HARD_DISK);
			CString stm = _opl(TXT_INI_CREATE_ERROR);
			m_InitialInfo[ROW_DISK_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DISK_CHECK].dealInfo = msg;
			bRet = false;
		}

		// ��鴴������Ŀ¼
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
@brief       ��ʼ��ϵͳMaster

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemMaster()
{
	bool bRet = true;
	// ��ʼ��MASTER�ļ�	
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
@brief       ��ʼ����������

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTxnData()
{
	bool bRet = true;
	// ��ʼ����������
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
@brief       ��ʼ����������

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeParamData()
{
	bool bRet = true;
	//��ʼ�������ļ�
	try{
		thePARAM_HELPER.InitParam();
		thePARAM_HELPER.InitAllParam();
		// ȡ�õ�ǰ��վ����
		WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();			
		// �жϵ�ǰ��վ�Ƿ����			
		//if(!theACC_LOCATION.IsStationCodeExist(stationCode)) {
		//	// �����������쳣�������ж�
		//	theAPP_SESSION.SetInitCheckError(true);
		//	theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
		//	throw  CParamException(CParamException::STATION_NOT_IN_VFARE, _T(__FILE__), __LINE__,_opl(TXT_INI_FUNCTION_EXCEPTION));
		//}
		//else 
		{
			//LocationNumber_t locationNum = theACC_LOCATION.GetStationLocationNumber(stationCode);
			// ���õ�ǰ��վ����
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
@brief       ��ʼ���Ĵ�������

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeRegistAudit()
{
	bool bRet = true;
	//��ʼ���Ĵ�������
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
@brief       ��ʼ��SC�������

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSCAudit()
{
	bool bRet = true;
	//��ʼ��SC�������
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

		// ����û�з��͵������־
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

		// ������Ӫ��
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
@brief       ��ʼ��ϵͳʱ��

@param     ��

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeSystemTime()
{
	bool bRet = true;
	// ͨ��NTPͬ������ʱ��
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
	// ����Ӫҵ���л�ʱ��
	CAfcDeviceTimeParam::OPRATION_TIME operationTime;
	theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
	ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);

	// ����NTP��ʱ��ʱ��
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_NTP_TIMER,NULL,NULL);
	// ������Ӫ���л���ʱ��
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER,NULL,NULL);
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ�����ݿ�

@param     none

@retval      bool  true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeDatabase()
{
	// ��ʼ�����ݿ�
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
@brief      ɾ����ӡ��ʷ��¼

@param      (i)int days;��������

@retval     ��

@exception  ��
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
@brief      ȡ��ָ��Ŀ¼��ָ������Ӧ��ɾ������֮����ļ�

@param      (i)int days;					ָ������
(i)CString strPath;				ָ��·��
(o)vector<CString>& vecNeedDel;	ָ������·���¿�ɾ�����ļ�
(i)bool bIsDir;					ָ��·���Ƿ���Ŀ¼

@retval     ��

@exception  ��
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
