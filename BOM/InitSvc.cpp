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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CInitSvc::CInitSvc(void)
    :CBOMForeService(INIT_SVC,MAIN_SVC)
{
    // �����ʼ��״̬
    theSERVICE_MGR.SetState(INIT);

	//theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CInitSvc::OnExceptionNotify);

    // ���ڻ���ʾ�ı�
    m_InitialInfo[ROW_DATA_CHECK].title = "����һ���Լ��:";
    m_InitialInfo[ROW_PARAMETER].title  = "��Ⲣ��ʼ������:";
    m_InitialInfo[ROW_RW].title         = "��Ⲣ��ʼ���ⲿ��д��:";
	m_InitialInfo[ROW_TW].title         = "��Ⲣ��ʼ���ڲ���д��:";
	m_InitialInfo[ROW_TH].title         = "��Ⲣ��ʼ����Ʊ��:";
    m_InitialInfo[ROW_PRINT].title      = "��Ⲣ��ʼ����ӡ��:";

    for (int i = 0; i < MAX_INITIAL_INFO_ROW; i++) {
        m_InitialInfo[i].result = UNDOWN;
    }

    // ���ò���Ա����
    m_pDialogFlow->AddDialog(CInitDlg::IDD, new CInitDlg(this));

    // ���ó˿ͻ���

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
		
		bool bInitOK = Initialize();

		 // ��ͣ�����������ϵ���ʾ���
		 Sleep(2000);

		 // �����ȴ����
		 theCURSOR_MGR.EndWaitCursor();

		 // ������ֹ��״̬
		 theSERVICE_MGR.SetState(OUT_OF);
		 if (bInitOK && (!theEXCEPTION_MGR.HasException())) { 
			 // ����Login����
			 theSERVICE_MGR.StartService(LOGIN_SVC);
		 }
		 // ���ڻ�ʧ��
		 else {          
			 // ��ʾ���˵�
				theMENU_MGR.SetMenuKind(CMenuMgr::MENU_INIT_ABNORMAL);
		 }
		 theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
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

	if(!InitializeController()){
		return false;
	}
	
	// ����һ���Լ��
	if (!CheckConsistent()) {
		theAPP_SESSION.SetInitCheckError(true);
		theAPP_SESSION.SetInitDetailErrorFlag(INI_ERROR);
		ret = false;
	}
	else {
		SetInitRessult(ROW_DATA_CHECK, INIT_RESULT::SUCCESS);   // ���ó�ʼ�����������һ���Լ�飩
	}

	//// ��ʼ������
	//if (!InitializeParameter()) {		
	//	ret = false;
	//}
	//else {
	//	SetInitRessult(ROW_PARAMETER, INIT_RESULT::SUCCESS);   // ���ó�ʼ�����������һ���Լ�飩
	//}

	////��ʼ���ⲿ��д��
	//if(!InitializeRW()) {
	//	SetInitRessult(ROW_RW, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
	//	ret = false;
	//}
	//else {
	//	// ��ȷ��
	//	CheckSAMidChange(RW_YPT_ISAM);
	//	CheckSAMidChange(RW_YPT_PSAM);
	//	CheckSAMidChange(RW_YKT_ISAM);
	//	CheckSAMidChange(RW_YKT_PSAM);
	//	SetInitRessult(ROW_RW, INIT_RESULT::SUCCESS);
	//}

	////��ʼ���ڲ���д��
	//if (!InitializeTW()) {
	//	SetInitRessult(ROW_TW, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_RW_ERROR);
	//	ret = false;
	//}
	//else {
	//	// ��ȷ��
	//	CheckSAMidChange(TH_YPT_ISAM);
	//	CheckSAMidChange(TH_YPT_PSAM);
	//	CheckSAMidChange(TH_YKT_ISAM);
	//	CheckSAMidChange(TH_YKT_PSAM);
	//	SetInitRessult(ROW_TW, INIT_RESULT::SUCCESS);
	//}

	//// ��ʼ����Ʊ��
	//// ��ʼ����Ʊ���Ž�
	//if(!InitializeLPT()){
	//	SetInitRessult(ROW_TH, INIT_RESULT::FAIL);
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_TH_ERROR);
	//	ret = false;
	//}
	//// ����ģʽ
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
	//// ����ģʽ
	//else{
	//	SetInitRessult(ROW_TH, INIT_RESULT::UNDOWN);
	//}

	//// ��ʼ����ӡ��
	//if (!InitializePrinter()) {
	//	theAPP_SESSION.SetInitCheckError(true);
	//	theAPP_SESSION.SetInitDetailErrorFlag(INIT_PRINT_ERROR);
	//	ret = false;
	//}
	//else {
	//	SetInitRessult(ROW_PRINT, INIT_RESULT::SUCCESS);
	//}
	//// ��ʼ��UPS
	//if(!InitializeUPS()){
	//	theAPP_SESSION.SetInitCheckError(true);
	//	//theAPP_SESSION.SetInitDetailErrorFlag(INIT_UPS_ERROR);
	//	ret = false;
	//}

	// �����ڸ��³�����ػ����³���
	bool needUpgrade = theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade();
	if(needUpgrade)	{
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);	
		return false;
	}

	// �����豸���
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

	// ��ʼ����λ
	if (InitializeCPS() != true){
		ret = false;
	}

    return ret;
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

	// ��ʼ����̨service
	theSERVICE_MGR.StartService(SC_SVC,false);
	theSERVICE_MGR.StartService(BUSINESS_INTERVAL_SVC,false);
	theSERVICE_MGR.StartService(EVENT_MSG_SVC,false);
	theSERVICE_MGR.StartService(TRANSMISSION_INTERVAL_SVC,false);

    return true;
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
	try {
		// ���ڻ�INI�ļ�
		int initResult = 0;
		initResult = theBOM_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _T("����ϵά����Ա���°�װ����");
			CString stm = _T("ϵͳ�ļ�����");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		initResult = theSETTING.Initialize();
		if (initResult != 0) {
			CString msg = _T("����ϵά����Ա���°�װ����");
			CString stm = _T("ϵͳ�ļ�����");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		initResult = theMAINTENANCE_INFO.Initialize();
		if (initResult != 0) {
			CString msg = _T("����ϵά����Ա���°�װ����");
			CString stm = _T("ϵͳ�ļ�����");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}
		//initResult = theTEMP_BACKUP_INFO.Initialize();
		//if (initResult != 0) {
		//	CString msg = "����ϵά����Ա���°�װ����";
		//	CString stm = "ϵͳ�ļ�����";
		//	m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
		//	m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
		//	return false;
		//}

		// ��鴴��������Ŀ¼
		if (!CheckCreateHomeFolder()) {
			CString msg = _T("����ϵά����Ա���Ӳ��");
			CString stm = _T(",����Ŀ¼����");
			m_InitialInfo[ROW_DATA_CHECK].errorInfo = stm;
			m_InitialInfo[ROW_DATA_CHECK].dealInfo = msg;
			return false;
		}

		// ��鴴������Ŀ¼
		if (!CheckCreateBackupFolder()) {
			CString msg = _T("����ϵά����Ա���CF��");
			CString stm = _T("����Ŀ¼����");
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
@brief       ���ڻ�ϵͳ����

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeParameter()
{
	bool bRes = true;
	try
	{
		CString szDataPath = theBOM_INFO.GetDataPath();
		CString szBackePath = theBOM_INFO.GetBackupPath();
		// ��ʼ����־
		try{
			// ������־Ŀ¼
			CString tracePath = theBOM_INFO.GetLogPath();
			CLog::Initialize(tracePath);
			//MIMTrs_Initialize(tracePath);       // Th Trace (��������)
			// �Ƿ��¼Debug��Ϣ
			theDEBUG_LOG->SetWriteDebug(theBOM_INFO.IsWriteDebug());

			// ɾ��BOM��־ 
			DeleteLog(theBOM_INFO.GetStoreDay());

			// ɾ����ֵ�����ļ� 
			DeleteChargeReceiptFile(7);

			//ɾ��TPU��־		
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��ʼ����־�ļ������쳣"));
			DoException(e,ROW_PARAMETER);
		}
		
		// ��ʼ���쳣
		int ret = theGlobalParameter.GetExceptionDetail();
		if(ret != 0){
			bRes = false;
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��ʼ���쳣�ļ������쳣"));
			DoException(e,ROW_PARAMETER);
		}
		// ��ʼ��MASTER�ļ�	
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ʼ��MASTER�ļ��쳣"));
			DoException(e,ROW_PARAMETER);

		}
	
		// ��ʼ����������
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��ʼ�����������ļ��쳣"));
			DoException(e,ROW_PARAMETER);
		}

		//��ʼ�������ļ�
		try
		{
		
			thePARAM_HELPER.InitAllParam();

			// ����Ӫҵ��ʼʱ��
			CAfcDeviceTimeParam::OPRATION_TIME operationTime;
			theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
			ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);

			// �жϵ�ǰ��վ�Ƿ����			
			// ȡ�õ�ǰ��վ����
			WORD stationCode = 0;
			stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
			

			CAccLineInfoParam::STATION_INFO stationgInfo;
			if(!theACC_LINE.GetStationInfobyCode(stationCode,stationgInfo)) {
				// �����������쳣�������ж�
				bRes = false;
				theAPP_SESSION.SetInitCheckError(true);
				theAPP_SESSION.SetInitDetailErrorFlag(PARAMETER_ERROR);
				throw  CParamException(CParamException::STATION_NOT_IN_VFARE, _T(__FILE__), __LINE__, _T("���������쳣:�]����Ӧ�ĳ�վ����ת��"));
			}
			else {
				// ���õ�ǰ��վ����
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ʼ��Param�ļ��쳣"));
			DoException(e,ROW_PARAMETER);

		}



		//��ʼ���Ĵ�������
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ʼ��AR�ļ��쳣"));
			DoException(e,ROW_PARAMETER);

		}
		//��ʼ������ļ�
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ʼ��SC����ļ��쳣"));
			DoException(e,ROW_PARAMETER);

		}

		//��ʼ����������ļ�
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("��ʼ����������ļ��쳣"));
			DoException(e,ROW_PARAMETER);

		}
		theMENU_MGR.InitializeIssueMenu();

		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("ok"));
		// ͨ��NTPͬ������ʱ��
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
			CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("ʱ��ͬ������"));
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������ʼ������"));
		DoException(e,ROW_PARAMETER);
	}
	return bRes;
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���д��(RW)

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("�ⲿ��д����ʼ������"));
		return false;
		DoException(e,ROW_RW);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���д��(TW)

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("�ڲ���д����ʼ������"));
		return false;
		DoException(e,ROW_TW);
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��ӡ����ʼ������"));
		DoException(e,ROW_PRINT);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���Ʊ��(TH)

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeTH()
{
	try {
		// ���ڻ�
		m_pTHHelper->Initialize();

		//m_pTHHelper->CheckDeviceExchange();
		//
		//if(theSETTING.GetRFIDWriteStatus() == 1){
		//	CTicketBoxExchangeSvc pSvc;
		//	pSvc.CheckTicketBoxIsInvalidExchanged();	
		//}
		//// RFID����
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

		// Ʊ�������飨��Ҫ��ӡ��Ʊ�����ڴ�ӡ�����ڻ�֮����
		//CTicketBoxHelper ticketBoxHelper(*this);
		//ticketBoxHelper.CheckTicketBox(CTicketBoxHelper::POWER_ON);
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_TH);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��Ʊ����ʼ������"));
		DoException(e,ROW_TH);
		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���Ʊ���Ž�

@param       ��

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CInitSvc::InitializeLPT()
{
	try {
		// ���ڻ�
		m_LPTHelper->LPTOpenLPT();
		return true;
	}
	catch (CSysException& e) {
		DoException(e,ROW_TH);
		return false;
	}
	catch (...) {
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��Ʊ����ʼ������"));
		DoException(e,ROW_TH);
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("UPS��ʼ������"));
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}

	return false;
}

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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__,_T("��ʼ����λʧ��"))); ;
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
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath())) {    // not exist
		return false;
	}
	// ��鲢������������Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����master�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����ACC�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����ECT�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ECT_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_ECT_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����AFC�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_AFC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + PARAM_AFC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢���������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + BOM_PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	
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
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath())) {    // not exist
		return false;
	}
	// ��鲢������������Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + DEVICE_MESSAGE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����master�ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + DEVICE_MASTER_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����ACC�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ACC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ACC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����ECT�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ECT_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_ECT_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢����AFC�����ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_AFC_CURRENT_DIR)) {    // if not exist, create it
		return false;
	}	
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + PARAM_AFC_FUTURE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢���������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + BOM_PURCHASE_DIR)) {    // if not exist, create it
		return false;
	}
	// ��鲢��������ļ�Ŀ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetBackupPath() + AR_MANAGE_FOLDER)) {    // if not exist, create it
		return false;
	}

	// ��鲢����traceĿ¼
	if (! ComCheckAndCreateDirectory(theBOM_INFO.GetLogPath())) {    // if not exist, create it
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ�BRContext����

@param      ��

@retval     ��

@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
// void CInitSvc::InitialBRContext()
// {
// }

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
		CString sDir = theBOM_INFO.GetLogPath();

		// �����ļ�
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".log"));

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
				dateStr = (sFileName.Right(12)).Left(8);

				// �������С��ָ��������ɾ��
				if(dateStr < dayLimitStr) {
					// ɾ�����ļ�
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
@brief      ɾ��ָ������ǰ�ĳ�ֵ�����ļ�

@param      (i)int days    ָ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::DeleteChargeReceiptFile(int days)
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
		CString sDir = theBOM_INFO.GetDataPath() + _T("PrintHistory\\ChargePrint");

		// �����ļ�
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".txt"));

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

				// �������С��ָ��������ɾ��
				if(dateStr < dayLimitStr) {
					// ɾ�����ļ�
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
		CString sDir = theBOM_INFO.GetLogPath();

		// �����ļ�
		CFileFind finder;
		CString sFileName = _T("");
		BOOL bContinue = finder.FindFile(sDir +_T("*")+_T(".log"));

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
				dateStr = (sFileName.Right(12)).Left(8);
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
// 	CString stm = "";
// 	//stm.Format("%s%08X%s%s", "(", e.GetActualMTC(), "),", e.GetDescription());
// 	//stm = "," + e.GetDescription() + "," + e.GetPrompt();
// 	stm.Format("%s%08X%s%s%s%s", "(", e.GetActualMTC(), "),", e.GetDescription(),",",e.GetPrompt());
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
	Sleep(2000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���SamId�仯

@param      (i)bool, true:�ж�TH��ISAM�仯;  false:�ж�RW��ISAM�仯

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInitSvc::CheckSAMidChange(SAM_POS_ samPos)
{
	// SAMid�����ı�
	DWORD samIdNo = 0;
	// ISAM��������ı�
	SAM_ID_INFO samId;
	samId = theAPP_SESSION.GetSAMInfo();
	BYTE nosam[6] = {0};
	CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
	theDeviceInfo.GetDeviceInfo(deviceInfo);
	//theDEBUG_LOG.WriteData(_T(__FILE__),__FUNCTION__,__LINE__,"");
	switch (samPos) {
		case RW_YPT_ISAM:
			//�ⲿһƱͨISAM
			if (memcmp(samId.rw_acc_isam_id,nosam,6) != 0 && memcmp(samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.rw_acc_isam_id,deviceInfo.ACC_RW1_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW1_ISAM(samId.rw_acc_isam_id);
			}	
			break;
		case RW_YPT_PSAM:
			//�ⲿһƱͨPSAM
			if (memcmp(samId.rw_acc_psam_id,nosam,6) != 0 && memcmp(samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.rw_acc_psam_id,deviceInfo.ACC_RW1_PSAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW1_PSAM(samId.rw_acc_psam_id);
			}	
			break;
		case TH_YPT_ISAM:
			// �ڲ�һƱͨISAM
			if (memcmp(samId.th_acc_isam_id,nosam,6) != 0 && memcmp(samId.th_acc_isam_id,deviceInfo.ACC_RW2_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_ISAM_RW,samId.th_acc_isam_id,deviceInfo.ACC_RW2_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW2_ISAM(samId.th_acc_isam_id);
			}	
			break;
		case TH_YPT_PSAM:
			// �ڲ�һƱͨPSAM
			if (memcmp(samId.th_acc_psam_id,nosam,6) != 0 && memcmp(samId.th_acc_psam_id,deviceInfo.ACC_RW2_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ACC_PSAM_RW,samId.th_acc_psam_id,deviceInfo.ACC_RW2_PSAM);
				// ����master�ļ�
				theDeviceInfo.SetACC_RW2_PSAM(samId.th_acc_psam_id);
			}	
			break;
		case RW_YKT_ISAM:
			// �ⲿһ��ͨISAM
			if (memcmp(samId.rw_ect_isam_id,nosam,6) != 0 && memcmp(samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.rw_ect_isam_id,deviceInfo.ECT_RW1_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW1_ISAM(samId.rw_ect_isam_id);
			}
			break;
		case RW_YKT_PSAM:
			// �ⲿһ��ͨPSAM
			if (memcmp(samId.rw_ect_psam_id,nosam,6) != 0 && memcmp(samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.rw_ect_psam_id,deviceInfo.ECT_RW1_PSAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW1_PSAM(samId.rw_ect_psam_id);
			}
			break;
		case TH_YKT_ISAM:
			// �ڲ�һ��ͨISAM
			if (memcmp(samId.th_ect_isam_id,nosam,6) != 0 && memcmp(samId.th_ect_isam_id,deviceInfo.ECT_RW2_ISAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_ISAM_RW,samId.th_ect_isam_id,deviceInfo.ECT_RW2_ISAM);
				// ����master�ļ�
				theDeviceInfo.SetETC_RW2_ISAM(samId.th_ect_isam_id);
			}
			break;
		case TH_YKT_PSAM:
			// �ڲ�һ��ͨPSAM
			if (memcmp(samId.th_ect_psam_id,nosam,6) != 0 && memcmp(samId.th_ect_psam_id,deviceInfo.ECT_RW2_PSAM,6) != 0) {
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(ECT_PSAM_RW,samId.th_ect_psam_id,deviceInfo.ECT_RW2_PSAM);
				// ����master�ļ�
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
		// ����Login����
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
}