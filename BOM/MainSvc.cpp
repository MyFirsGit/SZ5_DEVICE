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

static const TCHAR* const TXT_WARNING = _T("警告");
static const TCHAR* const TXT_BUSIDAY_CHANGE_LOGOUT = _T("运营日期切换，请按“确认”按钮登出");
const static const int INTERVAL_SWITH_BUSINESS_DAY=1;
static const TCHAR* const TXT_FORCE_LOGOUT_SERVER_REQUEST = _T("服务器要求强制登出");
static const TCHAR* const TXT_FORCE_LOGOUT_DEVICE_CONTROL = _T("设备运行模式改变");
static const TCHAR* const TXT_FORCE_LOGOUT_EMERGENCY_MODE = _T("紧急模式");
static const TCHAR* const TXT_FORCE_LOGOUT_NEWPARAMETER = _T("新参数或程序下载");

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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainSvc::CMainSvc(void)
    :CBOMForeService(MAIN_SVC, ROOT_SVC)
{

	currentBusiDate = ComGetBusiDate();

	 // 设置操作员画面
    m_pDialogFlow->AddDialog(CMenuDlg::IDD, new CMenuDlg(this));
    // 设置画面分组
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _T(""));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CMenuDlg::IDD);
    // 设置初始画面组
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	m_pDialogFlow->ShowFirstDialogGroup();

	theMENU_MGR.Initialize(/*GetMenuDlg()*/);
	CXButton::Clicking.AddHandler(this,&CMainSvc::OnButtonClicking);
	CGEdit::EditTextChanged.AddHandler(this,&CMainSvc::OnEditChanged);
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
@brief      模式变化响应函数

@param      (i)UINT mode    模式种类（SP_MODE_SYSTEM，，）

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnModeChange(UINT mode)
{
    
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
@brief      业务结束

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::CloseOperation(void)
{
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theBOM_STATUS_MGR.SetRunStatus(CLOSEING);
	PostMessage(SM_MAIN_CLOSE_OPERATION,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重启BOM

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::RestartBOM()
{
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
	theBOM_STATUS_MGR.SetRunStatus(START_RUNING);
	//OnShutDown(NULL,NULL);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      关机

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::Shutdown(void)
{
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theBOM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
	//OnShutDown(NULL,NULL);
}

void CMainSvc::ExitBOM()
{
	// 设置审计结束时间
	theSERVICE_MGR.SetForeServiceBusy(true);
	CLogoffHelper logoffHelper(*this);
	logoffHelper.Logoff();
	theAPP_SESSION.SetCloseStartFlag(CS_EXITBOM);
	theBOM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
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
		forceLogoutMessage = _T("未知原因");
	}
	CMessageBox::Show(_T("强制登出"),forceLogoutMessage	,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	CLogoffHelper logoffHelper(*this);
	logoffHelper.Logoff();
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
	theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,lParam == SC_CONNECTION_ON ? _T("连接"):_T("脱机"));
	theAPP_SESSION.GetChildView()->m_pStatusDlg->SetNetWorkStatus(lParam == SC_CONNECTION_ON ? _T("连接"):_T("脱机"));
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
@brief      计时器响应

@param      UINT nEventID 计时器ID


@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMainSvc::OnTimer(UINT nEventID)
{
	if(nEventID == TIMER_NTP)
	{
		try
		{
			if(theBOM_STATUS_MGR.GetRunStatus() != SHUTDOWNING)//没在关机中
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
	if(nEventID == TIMER_AUTOMATIC_LOGOFF)// 自动登出定时器
	{
		  // 停止定时器
		  StopTimer(TIMER_AUTOMATIC_LOGOFF);
		  // 登出
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
		
		// 运营日切换，则提示并登出（等待和结束状态）
		if (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY
			&& (serviceState !=INIT && serviceState!=OUT_OF)) {
				// 提示登出
				CMessageBox::Show(TXT_WARNING, TXT_BUSIDAY_CHANGE_LOGOUT, CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
				// 登出
				CLogoffHelper logoffHelper(*(theSERVICE_MGR.GetCurService()));
				logoffHelper.Logoff();
		}
		// SC审计数据
		theSCAudit_MGR.SetCurrentDayAuditTo(ComGetCurTime());
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(theSCAudit_MGR.GetCurrentDayPurchase(),END_OF_DAY);
		theSCAudit_MGR.SetCurrentDayPurchaseSended();
		//打印
		//CAuditHelper auditHelper(*this);
		//auditHelper.PrintAuditReport(CURRENT_DATE,theSCAudit_MGR.GetCurrentDayPurchase());
		theSCAudit_MGR.ClearCurrentBusinessDay();
		theSCAudit_MGR.SetCurrentBusinessDay(ComGetBusiDate());
		// 交易审核数据
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertTransactionAudit(theTxnAudit_MGR.GetCurrentTxnAudit());
		theTxnAudit_MGR.SetCurrentTxnAuditSended();
		theTxnAudit_MGR.ClearCurrentBusinessDay();
		theTxnAudit_MGR.SetCurrentBusinessDay(ComGetBusiDate());
		try{
			CLogHelper::UploadLogFile(currentBusiDate);//将运营结束切换前的日志发送给服务器。
		}
		catch(CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
		}
		currentBusiDate = ComGetBusiDate();
		// 设置营业开始时间
		CAfcDeviceTimeParam::OPRATION_TIME operationTime;
		theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
		ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);
		//清空密码锁定
		thePWD_ERR.Internalize(theBOM_INFO.GetDataPath(),theBOM_INFO.GetBackupPath());
		StartTimer(TIMER_BUSIDAY_SWITH,INTERVAL_SWITH_BUSINESS_DAY);
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
		
		// 显示向导提示
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
				// 设置审计结束时间
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
			if (theAPP_SESSION.GetCloseStartFlag() == CS_CLOSE) {   // 业务结束
				// 打印销售统计帐票	
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
			// 显示向导提示
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
		//关闭出票机门禁
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
			// 断开出票机
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
			// 断开外部读写器
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
			// 断开内部读写器
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
			// 断开打印机
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
			CLogHelper::UploadLogFile(ComGetCurDate());//把当天的日志发给服务器
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
		// 关闭框架窗口

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
		if(!exitMainRecWndSuccess || !exitMainWndSuccess)//结束窗体未成功，强制退出
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
				// 启动Login画面
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



