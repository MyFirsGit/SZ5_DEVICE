#include "StdAfx.h"
#include "MainSvc.h"
#include "AppSession.h"
#include "MessageBox.h"
#include "LogoffHelper.h"
#include "InnerException.h"
#include "ServiceMgr.h"
#include "ExceptionMgr.h"
#include "bomdef.h"
#include "UpgradeHelper.h"
#include "BOM.h"
#include "BomStatusMgr.h"
#include "SNTPHelper.h"
#include "SCAuditManager.h"
#include "InitSvc.h"
#include "CommonDefine.h"
#include "EventDataSvc.h"
#include "audithelper.h"
#include "guideinfo.h"
#include "setting.h"
#include "TxnAuditManager.h"
#include "LogHelper.h"
#include "shieldkeyhelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TXT_WARNING = _T("����");
static const TCHAR* const TXT_BUSIDAY_CHANGE_LOGOUT = _T("��Ӫ�����л����밴��ȷ�ϡ���ť�ǳ�");
const static const int INTERVAL_SWITH_BUSINESS_DAY=1;
static const TCHAR* const TXT_FORCE_LOGOUT_SERVER_REQUEST = _T("������Ҫ��ǿ�Ƶǳ�");
static const TCHAR* const TXT_FORCE_LOGOUT_DEVICE_CONTROL = _T("�豸����ģʽ�ı�");
static const TCHAR* const TXT_FORCE_LOGOUT_EMERGENCY_MODE = _T("����ģʽ");
static const TCHAR* const TXT_FORCE_LOGOUT_NEWPARAMETER = _T("�²������������");

BEGIN_MESSAGE_MAP(CMainSvc,CBOMForeService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SM_NET_STATUS,OnNetStatusChange)
	ON_SERVICE_MESSAGE(SM_FORCE_LOGOUT,OnForceLogout)
	ON_SERVICE_MESSAGE(SM_MAIN_SHUT_DOWN,OnShutDown)
	ON_SERVICE_MESSAGE(SM_MAIN_CLOSE_OPERATION,OnCloseOperation)
	ON_SERVICE_MESSAGE(SM_MAIN_START_NTP_TIMER,OnStartNtpTimer)
	ON_SERVICE_MESSAGE(SM_MAIN_NEW_WORK_MODE,OnNewWorkMode)
	ON_SERVICE_MESSAGE(SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER,OnStartSwithBusinessDayTimer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainSvc::CMainSvc(void)
    :CBOMForeService(MAIN_SVC, ROOT_SVC)
{

	currentBusiDate = ComGetBusiDate();

	 // ���ò���Ա����
    m_pDialogFlow->AddDialog(CMenuDlg::IDD, new CMenuDlg(this));
    // ���û������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _T(""));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CMenuDlg::IDD);
    // ���ó�ʼ������
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	m_pDialogFlow->ShowFirstDialogGroup();

	theMENU_MGR.Initialize(/*GetMenuDlg()*/);
	CXButton::Clicking.AddHandler(this,&CMainSvc::OnButtonClicking);
	CGEdit::EditTextChanged.AddHandler(this,&CMainSvc::OnEditChanged);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainSvc::~CMainSvc(void)
{
	CXButton::Clicking.RemoveHandler(this,&CMainSvc::OnButtonClicking);
	CGEdit::EditTextChanged.RemoveHandler(this,&CMainSvc::OnEditChanged);
	theMENU_MGR.ShutDown();

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
	m_pDialogFlow->GetDialog(CMenuDlg::IDD)->Invalidate();
	m_pDialogFlow->GetDialog(CMenuDlg::IDD)->UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģʽ�仯��Ӧ����

@param      (i)UINT mode    ģʽ���ࣨSP_MODE_SYSTEM������

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnModeChange(UINT mode)
{
    
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ĭ�Ϸ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::StartDefaultService()
{
	CMenuDlg* menuDlg =  (CMenuDlg*)(m_pDialogFlow->GetDialog(CMenuDlg::IDD));
	bool clickSecondMenuSuccess = menuDlg->GetMainGroup()->ClickFirstAvailButton();
	if(clickSecondMenuSuccess==TRUE)
	{
		menuDlg->GetSecondGroup()->ClickFirstAvailButton();
	}
	if (theSERVICE_MGR.GetState() != MAINTENANCE) {
		theSERVICE_MGR.SetState(READY);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ͷ�Controller

@param       ��

@retval      ��

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
        // delete FTP control
        if (g_pFTPControl != NULL) {
            g_pFTPControl->Shutdown();
            delete g_pFTPControl;
			g_pFTPControl = NULL;
        }

        // delete SC control
        if (g_pSCControl != NULL) {
            g_pSCControl->Shutdown();
            delete g_pSCControl;
            g_pSCControl = NULL;
        }

        // delete RW control
        if (g_pRWControl != NULL) {
            g_pRWControl->Shutdown();
            delete g_pRWControl;
            g_pRWControl = NULL;
        }


		// delete MimControl
		if (g_pMIMControl != NULL) {
			g_pMIMControl->Shutdown();
			delete g_pMIMControl;
			g_pMIMControl = NULL;
		}

		// delete memory control
		//if (g_pMemControl != NULL) {
		//	delete []g_pMemControl;
		//	g_pMemControl = NULL;
		//}


        // delete printer control
        if (g_pPrinterControl != NULL) {
            g_pPrinterControl->Shutdown();
            delete g_pPrinterControl;
            g_pPrinterControl = NULL;
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
@brief      ҵ�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::CloseOperation(void)
{
	// ������ƽ���ʱ��
	theSERVICE_MGR.SetForeServiceBusy(true);
	theBOM_STATUS_MGR.SetRunStatus(CLOSEING);
	PostMessage(SM_MAIN_CLOSE_OPERATION,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����BOM

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::RestartBOM()
{
	// ������ƽ���ʱ��
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
	theBOM_STATUS_MGR.SetRunStatus(START_RUNING);
	//OnShutDown(NULL,NULL);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �ػ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::Shutdown(void)
{
	// ������ƽ���ʱ��
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theBOM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
	//OnShutDown(NULL,NULL);
}

void CMainSvc::ExitBOM()
{
	// ������ƽ���ʱ��
	theSERVICE_MGR.SetForeServiceBusy(true);
	CLogoffHelper logoffHelper(*this);
	logoffHelper.Logoff();
	theAPP_SESSION.SetCloseStartFlag(CS_EXITBOM);
	theBOM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ǿ�Ƶǳ�

@param      WPARAM wParam ǿ�Ƶǳ�ԭ��

@param      LPARAM lParam ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnForceLogout(WPARAM wParam,LPARAM lParam)
{
	if(theSERVICE_MGR.GetState() == INIT || theSERVICE_MGR.GetState() == OUT_OF)
	{
		return SP_SUCCESS;
	}
	CString forceLogoutMessage = _T("");
	switch(wParam)
	{
	case 	FORCE_LOGOUT_SERVER_REQUEST:
		forceLogoutMessage = TXT_FORCE_LOGOUT_SERVER_REQUEST;
		break;
	case FORCE_LOGOUT_DEVICE_CONTROL:
		forceLogoutMessage = TXT_FORCE_LOGOUT_DEVICE_CONTROL;
		break;
	case FORCE_LOGOUT_EMERGENCY_MODE:
		forceLogoutMessage = TXT_FORCE_LOGOUT_EMERGENCY_MODE;
		break;
	case FORCE_LOGOUT_NEWPARAMETER:
		forceLogoutMessage = TXT_FORCE_LOGOUT_NEWPARAMETER;
		break;
	default:
		forceLogoutMessage = _T("δ֪ԭ��");
	}
	CMessageBox::Show(_T("ǿ�Ƶǳ�"),forceLogoutMessage	,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	CLogoffHelper logoffHelper(*this);
	logoffHelper.Logoff();
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����λ����״̬�ı�

@param      WPARAM wParam ������

@param      LPARAM lParam ��ǰ״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnNetStatusChange(WPARAM wParam,LPARAM lParam)
{
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,lParam == SC_CONNECTION_ON ? _T("����"):_T("�ѻ�"));
	theAPP_SESSION.GetChildView()->m_pStatusDlg->SetNetWorkStatus(lParam == SC_CONNECTION_ON ? _T("����"):_T("�ѻ�"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����NTP��ʱ��

@param      WPARAM wParam ������

@param      LPARAM lParam ������

@retval     ��

@exception  ��
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
	currentBusiDate = ComGetBusiDate();
	StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
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
LRESULT CMainSvc::OnNewWorkMode(WPARAM wParam, LPARAM lParam)
{
	theMENU_MGR.SetWorkMode((WORK_MODE)wParam);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ����Ӧ

@param      UINT nEventID ��ʱ��ID


@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnTimer(UINT nEventID)
{
	if(nEventID == TIMER_NTP)
	{
		try
		{
			if(theBOM_STATUS_MGR.GetRunStatus() != SHUTDOWNING)//û�ڹػ���
			{
				theSNTP_HELPER.TimeSynchronize();
			}
		}
		catch (CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...)
		{
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
	}
	if(nEventID == TIMER_DEFAULT_SERVICE)
	{
		StopTimer(TIMER_DEFAULT_SERVICE);
		StartDefaultService();
	}
	if(nEventID == TIMER_AUTOMATIC_LOGOFF)// �Զ��ǳ���ʱ��
	{
		  // ֹͣ��ʱ��
		  StopTimer(TIMER_AUTOMATIC_LOGOFF);
		  // �ǳ�
		  CLogoffHelper logoffHelper(*(theSERVICE_MGR.GetCurService()));
		  logoffHelper.Logoff();
	}
	if(nEventID == TIMER_BUSIDAY_SWITH)
	{
		if(currentBusiDate==ComGetBusiDate())
		{
			return;
		}
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		if(serviceState == ISSUEING)
		{
			return;
		}
		StopTimer(TIMER_DEFAULT_SERVICE);
		StopTimer(TIMER_AUTOMATIC_LOGOFF);
		StopTimer(TIMER_BUSIDAY_SWITH);
		
		// ��Ӫ���л�������ʾ���ǳ����ȴ��ͽ���״̬��
		if (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY
			&& (serviceState !=INIT && serviceState!=OUT_OF)) {
				// ��ʾ�ǳ�
				CMessageBox::Show(TXT_WARNING, TXT_BUSIDAY_CHANGE_LOGOUT, CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				// �ǳ�
				CLogoffHelper logoffHelper(*(theSERVICE_MGR.GetCurService()));
				logoffHelper.Logoff();
		}
		// SC�������
		theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(theSCAudit_MGR.GetCurrentDayPurchase(),END_OF_DAY);
		theSCAudit_MGR.SetCurrentDayPurchaseSended();
		//��ӡ
		//CAuditHelper auditHelper(*this);
		//auditHelper.PrintAuditReport(CURRENT_DATE,theSCAudit_MGR.GetCurrentDayPurchase());
		theSCAudit_MGR.ClearCurrentBusinessDay();
		theSCAudit_MGR.SetCurrentBusinessDay(ComGetBusiDate());
		// �����������
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertTransactionAudit(theTxnAudit_MGR.GetCurrentTxnAudit());
		theTxnAudit_MGR.SetCurrentTxnAuditSended();
		theTxnAudit_MGR.ClearCurrentBusinessDay();
		theTxnAudit_MGR.SetCurrentBusinessDay(ComGetBusiDate());
		try{
			CLogHelper::UploadLogFile(currentBusiDate);//����Ӫ�����л�ǰ����־���͸���������
		}
		catch(CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
		}
		currentBusiDate = ComGetBusiDate();
		// ����Ӫҵ��ʼʱ��
		CAfcDeviceTimeParam::OPRATION_TIME operationTime;
		theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
		ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);
		//�����������
		thePWD_ERR.Internalize(theBOM_INFO.GetDataPath(),theBOM_INFO.GetBackupPath());
		StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ��ҵ��������������ʱ������ػ�

@param      WPARAM wParam ������

@param      LPARAM lParam ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainSvc::OnShutDown(WPARAM wParam,LPARAM lParam)
{
	try{
		CShieldKeyHelper::SetHostProgarmEnable(false);

	}
	catch(...)
	{

	}

	try {

		theSERVICE_MGR.SetForeServiceBusy(true);
		theBOM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
		StopTimer(TIMER_NTP);
		StopTimer(TIMER_DEFAULT_SERVICE);
		StopTimer(TIMER_AUTOMATIC_LOGOFF);
		StopTimer(TIMER_BUSIDAY_SWITH);
		
		// ��ʾ����ʾ
		switch (theAPP_SESSION.GetCloseStartFlag()) {
		case CS_CLOSE:
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_CLOSE));
			break;
		case CS_RESTART:
		case CS_SHUTDOWN:
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_SHUTDOWN));
			break;
		case CS_EXITBOM:
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_EXITBOM));
			//theBomAR.UpdateSerialFile();
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
	theSERVICE_MGR.StopService(MONITOR_RW_SVC);
	theSERVICE_MGR.StopService(MONITOR_TW_SVC);
	theSERVICE_MGR.StopService(MONITOR_TH_SVC);
	theSERVICE_MGR.StopService(MONITOR_UPS_SVC);
	theSERVICE_MGR.StopService(MONITOR_LPT_SVC);
	try {
		if (theAPP_SESSION.IsInitErrorOver() == 0) {
			if (theAPP_SESSION.GetCloseStartFlag() == CS_SHUTDOWN) {
				// ������ƽ���ʱ��
				theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
				if(theEXCEPTION_MGR.HasException())
				{
					theBOM_STATUS_MGR.SetServiceStatus(ERROR_SERVICE);
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
	try {
			if (theAPP_SESSION.GetCloseStartFlag() == CS_CLOSE) {   // ҵ�����
				// ��ӡ����ͳ����Ʊ	
				/*CAuditHelper* m_AuditHelper = new CAuditHelper(*this);
				m_AuditHelper->PrintAuditReport(CURRENT_DATE,theSCAudit_MGR.GetCurrentDayPurchase());*/
				theSCAudit_MGR.ClearCurrentBusinessDay();
				theTxnAudit_MGR.ClearCurrentBusinessDay();
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
		if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXITBOM)
		{
			theUPGRADE_HELPER::Upgrade();
			// ��ʾ����ʾ
			switch (theAPP_SESSION.GetCloseStartFlag()) {
				case CS_CLOSE:
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_CLOSE));
					break;
				case CS_RESTART:
				case CS_SHUTDOWN:
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_SHUTDOWN));
					break;
				case CS_EXITBOM:
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_EXITBOM));
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
		//�رճ�Ʊ���Ž�
		if (theAPP_SESSION.IsInitErrorOver() == 0) {
			m_LPTHelper->LPTCloseLPT();
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
		if (theAPP_SESSION.IsInitErrorOver() == 0 && theSETTING.GetTHMode() == TH_MODE_NORMAL) {
			// �Ͽ���Ʊ��
			m_pTHHelper->Disconnect();
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
			// �Ͽ��ⲿ��д��
			m_pRWHelper->Close();
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
			// �Ͽ��ڲ���д��
			m_pTWHelper->Close();
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
			// �Ͽ���ӡ��
			m_pPRTHelper->PrinterClose();
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
	try{
		if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXITBOM)
		{
			CLogHelper::UploadLogFile(ComGetCurDate());//�ѵ������־����������
		}
	}
	catch(CSysException& e)
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
		// �ͷ�Controller
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
		// �رտ�ܴ���

#ifndef _DEBUG
		BOOL exitMainRecWndSuccess = theApp.GetMainWndRec()->PostMessage(WM_CLOSE);
		BOOL exitMainWndSuccess = theApp.GetMainWnd()->PostMessage(WM_CLOSE);
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
		CShieldKeyHelper::SetFunctionKeysEnable(true);
		CShieldKeyHelper::SetTaskManagerEnable(true);
		if(theAPP_SESSION.GetCloseStartFlag()!=CS_EXITBOM)
		{
			bool reboot = theAPP_SESSION.GetCloseStartFlag() == CS_RESTART;
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
			ComShutDownSystem(reboot);
		}
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
		if(!exitMainRecWndSuccess || !exitMainWndSuccess)//��������δ�ɹ���ǿ���˳�
		{

			//theApp.GetMainWnd()->
			ExitProcess(0);
		}

#endif

	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
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
	theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
	theAPP_SESSION.SetCloseStartFlag(CS_CLOSE);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_CLOSE));
	theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_CLOSEOPERATION,NULL,NULL);
	OnShutDown(NULL,NULL);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����쳣

@param      WPARAM wParam ������

@param      LPARAM lParam ������

@retval     ��

@exception  ��
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
			if (!(theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CRWException::MODULE_ID))) {
				pInitSvc->SetInitRessult(CInitSvc::ROW_RW,CInitSvc::SUCCESS);
			}
			if (!(theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID))) {
				pInitSvc->SetInitRessult(CInitSvc::ROW_PRINT,CInitSvc::SUCCESS);
			}
			if (!(theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTWException::MODULE_ID))) {
				pInitSvc->SetInitRessult(CInitSvc::ROW_TW,CInitSvc::SUCCESS);
			}
			if (!(theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))) {
				pInitSvc->SetInitRessult(CInitSvc::ROW_TH,CInitSvc::SUCCESS);
			}
			if(!(theEXCEPTION_MGR.HasException(CSNTPSException::MODULE_ID)))
			{
				pInitSvc->SetInitRessult(CInitSvc::ROW_PARAMETER,CInitSvc::SUCCESS);
			}
			if(!theEXCEPTION_MGR.HasException())
			{
				// ����Login����
				theSERVICE_MGR.StartService(LOGIN_SVC);
			}
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}



