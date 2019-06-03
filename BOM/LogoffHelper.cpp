#include "StdAfx.h"
#include "LogoffHelper.h"
#include "InnerException.h"
#include "ServiceDefinition.h"
#include "AppSession.h"
#include "bominfo.h"
#include "ExceptionMgr.h"
#include "ServiceMgr.h"
#include "messagebox.h"
#include "BomStatusMgr.h"
#include "SCAuditManager.h"
#include "EventDataSvc.h"
#include "ServerClientSvc.h"
#include "clogloginandlogoff.h"
#include "SvcDef.h"
#include "guideinfo.h"
#include "setting.h"

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
	m_AuditHelper = new CAuditHelper(m_service);
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
	delete m_AuditHelper;
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
     try {
		CMessageBox::End();

		// 等待中、操作中、结束，则转入运行中
		if (theSERVICE_MGR.GetState() == READY
			|| theSERVICE_MGR.GetState() == RECEPT
			|| theSERVICE_MGR.GetState() == FINISH) {
				theSERVICE_MGR.SetState(ISSUEING);
		}
		else {
			theSERVICE_MGR.SetForeServiceBusy(true);
		}
		// 显示正在登出
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_OTHERS_LOGOUT));
  
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
			try
			{
				theSCAudit_MGR.SetCurrentLoginAuditTo(ComGetCurTime());

				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(theSCAudit_MGR.GetCurrentLoginPurchase(),OPERATER_LOGOUT);
				theSCAudit_MGR.SetCurrentLoginPurchaseSended();

				// 打印销售统计
				////	// 员工如果没有拥有销售统计权限不打印				
				////	//if(theAFC_ACCESS_LEVEL.IsStaffHasRight(_ttoi(theAPP_SESSION.GetUserInfo().sUserID),SYS_FUNCTION_SALE_STAT)){
				////		//m_AuditHelper->PrintAuditReport(STAFF_ONCE,theSCAudit_MGR.GetCurrentLoginPurchase());
				////	//}

				////清空登录员工信息。
				//theSETTING.SetPrintStaffID("");
				////清空登录时间。
				//theSETTING.SetPrintLoginTime(_DATE_TIME());
				theSCAudit_MGR.ClearCurrentLogin();
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
        // 转入中止中
        theSERVICE_MGR.SetState(OUT_OF);
        // 登出前不是休息中，则启动登录画面
		theBOM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		theBOM_STATUS_MGR.SetLoginStatus(LOGOUT);
		theSERVICE_MGR.StartService(LOGIN_SVC);
		// 清除全局站员信息（新建1个USER_INFO对象）
		USER_INFO userInfo;
		theAPP_SESSION.SetUserInfo(userInfo);
     }
    catch (CSysException& e) {
        theEXCEPTION_MGR.AddException(e);
		theSERVICE_MGR.StartService(LOGIN_SVC);
    }
    catch (...) {
        theEXCEPTION_MGR.AddException(CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theSERVICE_MGR.StartService(LOGIN_SVC);
    }
}


