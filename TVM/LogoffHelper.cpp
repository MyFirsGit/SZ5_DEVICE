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
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogoffHelper::CLogoffHelper(CService& service)
    :CServiceHelper(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogoffHelper::~CLogoffHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ǳ�

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLogoffHelper::Logoff()
{
	SERVICE_STATE oldState = theSERVICE_MGR.GetState();
    try {

		CMessageBox::End();
		// �ȴ��С������С���������Ϣ����ת��������
		if (oldState == READY|| oldState== RECEPT || oldState == FINISH|| oldState == REST) {
				theSERVICE_MGR.SetState(ISSUEING);
		}
		else {
			theSERVICE_MGR.SetForeServiceBusy(true);
		}
		// ��ʾ���ڵǳ�
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGOUTING));
		
		// ���������Banknote��أ�������Banknote��ؼ�ʱ��
		if(theTVM_INFO.IsBHMonitorOpened())	{
			if(!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
				theSERVICE_MGR.GetService<CBanknoteCollectionModuleMonitorSvc>(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC)->Resume();
			}
		}
		// ���������Coin��أ�������Coin��ؼ�ʱ��
		if(theTVM_INFO.IsCHMonitorOpened()){
			if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Resume();
			}
		}
		// д��־
		STAFF_AUTHORITY staffAuthority = theAPP_SESSION.GetUserInfo().curAuthority;
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_OFF, theAPP_SESSION.GetUserInfo().sUserID, staffAuthority);

		// ����λ���͵ǳ�����
		if (!theAPP_SESSION.IsSuperUser()) {      // ��ͨվԱ
			BYTE logoffResult;
			theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestLoginLogoff(_ttoi(theAPP_SESSION.GetUserInfo().sUserID),theAPP_SESSION.GetUserInfo().sPwd,LOGIN_OFF,logoffResult);
		}
		
		// ��ͨվԱ�ǳ�ʱ
		if (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY) {
			//theSCAudit_MGR.SetCurrentLoginAuditTo(ComGetCurTime());
			//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(theSCAudit_MGR.GetCurrentLoginPurchase(),OPERATER_LOGOUT);
			//theSCAudit_MGR.SetCurrentLoginPurchaseSended();
			//theSCAudit_MGR.ClearCurrentLogin();
		}	
        // ת����ֹ��
        theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
        
		//theTVM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		theTVM_STATUS_MGR.SetLoginStatus(LOGOUT);
		theSERVICE_MGR.StartService(LOGIN_SVC);
		// ���ȫ��վԱ��Ϣ
		USER_INFO userInfo;
		theAPP_SESSION.SetUserInfo(userInfo);

		// �ر��Զ��ǳ���ʱ��
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF);

		// �����ǳ�δ���ű�����ʱ��,���û�й���
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


