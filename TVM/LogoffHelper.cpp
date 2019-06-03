#include "StdAfx.h"
#include "LogoffHelper.h"
#include "InnerException.h"
#include "ServiceDefinition.h"
#include "AppSession.h"
#include "SYSInfo.h"
#include "ExceptionMgr.h"
#include "ServiceMgr.h"
#include "messagebox.h"
#include "tvmStatusMgr.h"
//#include "SCAuditManager.h" mzy
#include "EventDataSvc.h"
#include "ServerClientSvc.h"
#include "clogloginandlogoff.h"
#include "SvcDef.h"
#include "guideinfo.h"
#include "tvmsetting.h"
#include "CoinModuleMonitorSvc.h"
#include "BanknoteCollectionModuleMonitorSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief Logoff Helper
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLogoffHelper::CLogoffHelper(CService& service)
    :CServiceHelper(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLogoffHelper::~CLogoffHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      登出

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLogoffHelper::Logoff()
{
	SERVICE_STATE oldState = theSERVICE_MGR.GetState();
    try {

		CMessageBox::End();
		// 等待中、操作中、结束，休息中则转入运行中
		if (oldState == READY|| oldState== RECEPT || oldState == FINISH|| oldState == REST) {
				theSERVICE_MGR.SetState(ISSUEING);
		}
		else {
			theSERVICE_MGR.SetForeServiceBusy(true);
		}
		// 显示正在登出
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGOUTING));
		
		// 如果允许开启Banknote监控，则启动Banknote监控计时器
		if(theTVM_INFO.IsBHMonitorOpened())	{
			if(!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
				theSERVICE_MGR.GetService<CBanknoteCollectionModuleMonitorSvc>(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC)->Resume();
			}
		}
		// 如果允许开启Coin监控，则启动Coin监控计时器
		if(theTVM_INFO.IsCHMonitorOpened()){
			if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Resume();
			}
		}
		// 写日志
		STAFF_AUTHORITY staffAuthority = theAPP_SESSION.GetUserInfo().curAuthority;
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_OFF, theAPP_SESSION.GetUserInfo().sUserID, staffAuthority);

		// 向上位发送登出电文
		if (!theAPP_SESSION.IsSuperUser()) {      // 普通站员
			BYTE logoffResult;
			theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestLoginLogoff(_ttoi(theAPP_SESSION.GetUserInfo().sUserID),theAPP_SESSION.GetUserInfo().sPwd,LOGIN_OFF,logoffResult);
		}
		
		// 普通站员登出时
		if (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY) {
			//theSCAudit_MGR.SetCurrentLoginAuditTo(ComGetCurTime());
			//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(theSCAudit_MGR.GetCurrentLoginPurchase(),OPERATER_LOGOUT);
			//theSCAudit_MGR.SetCurrentLoginPurchaseSended();
			//theSCAudit_MGR.ClearCurrentLogin();
		}	
        // 转入中止中
        theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
        
		//theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		theTVM_STATUS_MGR.SetLoginStatus(LOGOUT);
		theSERVICE_MGR.StartService(LOGIN_SVC);
		// 清除全局站员信息
		USER_INFO userInfo;
		theAPP_SESSION.SetUserInfo(userInfo);

		// 关闭自动登出计时器
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF);

		// 启动登出未关门报警计时器,如果没有关门
		theSERVICE_MGR.GetService(SERVICE_ID::MAIN_SVC)->PostMessage(SM_MAIN_DOOR_OPEN_WITH_OUT_LOGIN,NULL,NULL);
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}


