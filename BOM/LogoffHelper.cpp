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
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogoffHelper::CLogoffHelper(CService& service)
    :CServiceHelper(service)
{
	m_AuditHelper = new CAuditHelper(m_service);
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
	delete m_AuditHelper;
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
     try {
		CMessageBox::End();

		// �ȴ��С������С���������ת��������
		if (theSERVICE_MGR.GetState() == READY
			|| theSERVICE_MGR.GetState() == RECEPT
			|| theSERVICE_MGR.GetState() == FINISH) {
				theSERVICE_MGR.SetState(ISSUEING);
		}
		else {
			theSERVICE_MGR.SetForeServiceBusy(true);
		}
		// ��ʾ���ڵǳ�
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_OTHERS_LOGOUT));
  
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
			try
			{
				theSCAudit_MGR.SetCurrentLoginAuditTo(ComGetCurTime());

				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(theSCAudit_MGR.GetCurrentLoginPurchase(),OPERATER_LOGOUT);
				theSCAudit_MGR.SetCurrentLoginPurchaseSended();

				// ��ӡ����ͳ��
				////	// Ա�����û��ӵ������ͳ��Ȩ�޲���ӡ				
				////	//if(theAFC_ACCESS_LEVEL.IsStaffHasRight(_ttoi(theAPP_SESSION.GetUserInfo().sUserID),SYS_FUNCTION_SALE_STAT)){
				////		//m_AuditHelper->PrintAuditReport(STAFF_ONCE,theSCAudit_MGR.GetCurrentLoginPurchase());
				////	//}

				////��յ�¼Ա����Ϣ��
				//theSETTING.SetPrintStaffID("");
				////��յ�¼ʱ�䡣
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
        // ת����ֹ��
        theSERVICE_MGR.SetState(OUT_OF);
        // �ǳ�ǰ������Ϣ�У���������¼����
		theBOM_STATUS_MGR.SetServiceStatus(OUTOF_SERVICE);
		theBOM_STATUS_MGR.SetLoginStatus(LOGOUT);
		theSERVICE_MGR.StartService(LOGIN_SVC);
		// ���ȫ��վԱ��Ϣ���½�1��USER_INFO����
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


