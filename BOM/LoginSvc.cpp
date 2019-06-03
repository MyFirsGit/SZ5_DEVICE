#include "StdAfx.h"
#include "LoginSvc.h"
#include "OutOfReceptionDlg.h"
#include "MainSvc.h"
#include "MainFrm.h"
#include "InnerException.h"
#include "servicemgr.h"
#include "CursorMgr.h"
#include "EventDataSvc.h"
#include "Validation.h"
#include "clogloginandlogoff.h"
#include "guideinfo.h"
#include "setting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int AllowMaxLoginTimes=3;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::CLoginSvc(void)
    :CBOMForeService(LOGIN_SVC,MAIN_SVC)
{
    try {
        // ���ò���Ա����
        m_pDialogFlow->AddDialog(CLoginDlg::IDD, new CLoginDlg(this));
		
        // ���ó˿ͻ���

        // ���û������
        m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_INPUT_ID_AND_PWD));
        m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CLoginDlg::IDD);

        // ���ó�ʼ������
        m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

        // ���õ�¼����Ա��Ϣ����ȫ�ֲ���Ա��Ϣ��
        m_userInfo = theAPP_SESSION.GetUserInfo();
        m_userInfo.allAuthority = m_userInfo.curAuthority;

        // ������������������
        m_passwordErrCount = 0;

        // ���ڻ�LoginDlg��ʾ����
        InitLoginAreaInfo();
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}	

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::~CLoginSvc(void)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::OnStart()
{
    try {
        __super::OnStart();
		theMENU_MGR.SetMenuKind(CMenuMgr::MENU_LOGIN);
    }
    catch (CSysException & e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Serviceֹͣǰ����

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CLoginSvc::OnStop()
{
    try {
        return __super::OnStop();
    }
    catch (CSysException& e) {
       theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��¼����ʾ����

@param      ��

@retval     PLABEL_GROUP  �ı����ݵ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CLoginSvc::GetLoginAreaInfo() 
{
	return m_loginInfoGroup;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ���¼����ʾ����

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::InitLoginAreaInfo()
{
    try {
        m_loginInfoGroup[0].nameLeft = "����������ԱID��";
        m_loginInfoGroup[1].nameLeft = "�������ܡ�  �룺";
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�¼����ԱȨ�ޣ������»��棩

@param      (i)STAFF_AUTHORITY authority

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::SetUserAuthority(STAFF_AUTHORITY authority)
{
    m_userInfo.curAuthority = authority;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ����Ա��ͨҵ���ά��ҵ�����Ȩ��Ϣ

@param      int nOperatorID ����ԱID

@retval     STAFF_AUTHORITY ��Ȩ��Ϣ

@exception  
*/
//////////////////////////////////////////////////////////////////////////
STAFF_AUTHORITY CLoginSvc::GetOperatorAllAccess(int nOperatorID)
{
	if(IsSuperOperatorLogin(nOperatorID))
	{
		return ALL_AUTHORITY;
	}
	BYTE staffAuthority = STAFF_AUTHORITY::NONE_AUTHORITY;
	SYS_FUNCTION_CODE commonFunctions [] = {	
		SYS_FUNCTION_ISSUE,//��Ʊ
		SYS_FUNCTION_ADJUST,//��Ʊ
		SYS_FUNCTION_CHARGE,//��ֵ
		SYS_FUNCTION_REFUND,//�˿�
		SYS_FUNCTION_EXCHANGE,//����
		SYS_FUNCTION_REFRESH,//����
		SYS_FUNCTION_ANALYZE,//��ѯ	
		SYS_FUNCTION_OTHER
	};
	for (int i = 0;i< sizeof(commonFunctions)/sizeof(SYS_FUNCTION_CODE); i++)
	{
		if(theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,commonFunctions[i]))
		{
			staffAuthority |= (BYTE)STAFF_AUTHORITY::COMMON_AUTHORITY;
			break;
		}
	}
	SYS_FUNCTION_CODE maintainFunctions [] ={
		SYS_FUNCTION_SYSTEM_SET,//ϵͳ����	
		SYS_FUNCTION_DATA_MANAGE	,//���ݹ���	
		SYS_FUNCTION_HARDWARE_TEST,//Ӳ������	
	};
	for (int i = 0;i< sizeof(maintainFunctions)/sizeof(SYS_FUNCTION_CODE); i++)
	{
		if(theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,maintainFunctions[i]))
		{
			staffAuthority |= (BYTE)STAFF_AUTHORITY::MAINTAIN_AUTHORITY;
			break;
		}
	}
	return (STAFF_AUTHORITY)staffAuthority;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ж��û��Ƿ��ǳ����û�

@param      ����ԱID

@retval     bool 

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CLoginSvc::IsSuperOperatorLogin(int nOperatorID)
{
	return theBOM_INFO.IsFounderStaffID(nOperatorID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�ϴ���

@param      (i)CString sPassWord                  // �û����������

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::OnOK(int nOperatorID,CString password)
{
    try {
		// ��ֻ��Ӳ���쳣��ɵ�½
		// ��ͨҵ��
		if(m_userInfo.curAuthority == COMMON_AUTHORITY && (!IsSuperOperatorLogin(nOperatorID))){
			bool hasException = theEXCEPTION_MGR.HasException();
			vector<long> canLoginModules;
			canLoginModules.push_back(CTHHardwareException::MODULE_ID);
			canLoginModules.push_back(CTHException::MODULE_ID);
			canLoginModules.push_back(CTWHardwareException::MODULE_ID);
			canLoginModules.push_back(CTWException::MODULE_ID);
			canLoginModules.push_back(CRWHardwareException::MODULE_ID);
			canLoginModules.push_back(CRWException::MODULE_ID);
			canLoginModules.push_back(CPrinterException::MODULE_ID);
			canLoginModules.push_back(CUPSException::MODULE_ID);
			
			//���ڲ������¼���쳣
			if(hasException && !theEXCEPTION_MGR.OnlyHasException(canLoginModules))
			{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_HAS_EXCEPTION));
				return;		
			}
		}
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_LOGINNING));
		long loginResult = Login(nOperatorID,password);
		if(loginResult == LOGIN_OVER_MAX_COUNT)
		{
			LockedOperatorID(nOperatorID,m_userInfo.curAuthority == COMMON_AUTHORITY ? 0x03 : 0x06);
		}
		long allowLoginResults[] = {LOGIN_SUCCESS,LOGIN_AND_PWD_WILL_OVERDUE,LOGIN_AND_MODIFIED,LOGIN_REQUEST_SERVER_FAILED};
		if(!CValidation::IsInRange(loginResult,allowLoginResults))
		{
			OPERATION_MSG msg = GetGuidCode(loginResult);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(msg));
			SaveOperationLog(false,true,nOperatorID);
			return;
		}
		m_userInfo.sUserID.Format(_T("%.6d"),nOperatorID);
		m_userInfo.sPwd = password;
		_DATE_TIME loginTime = ComGetCurTime();
		m_userInfo.loginTime = loginTime;
		theAPP_SESSION.SetUserInfo(m_userInfo);
		//����Ա����������Ӫ��������
		_DATE_TIME latestLoginTime = theSETTING.GetPrintLoginTime();
		_DATE latestLDate;
		latestLDate.wYear = latestLoginTime.wYear;
		latestLDate.biMonth = latestLoginTime.biMonth;
		latestLDate.biDay = latestLoginTime.biDay;
		if(ComGetBusiDate()>latestLDate){
			theCOUNT_INFO.SetLoginCount(0);
		}
		theCOUNT_INFO.AccumulateLoginCount();
		if(theBOM_INFO.IsRWUsed() && !theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID) && ! theEXCEPTION_MGR.HasException(CRWException::MODULE_ID))	{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_PARAM_SYNING));
			m_pRWHelper->InitData();
			m_pRWHelper->SynchronismParam();
		}
		if(theBOM_INFO.IsRWUsed(false) && !theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTWException::MODULE_ID))
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_PARAM_SYNING));
			m_pTWHelper->InitData();
			m_pTWHelper->SynchronismParam();
		}
		theBOM_STATUS_MGR.SetServiceStatus(m_userInfo.curAuthority == COMMON_AUTHORITY ? IN_SERVICE :OUTOF_SERVICE);
		theBOM_STATUS_MGR.SetLoginStatus(LOGIN);
		
		if(loginResult == LOGIN_AND_MODIFIED)
		{
			CMessageBox::Show(_T("��¼"),_T("�����¼����ǿ���޸����룡"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		if(loginResult == LOGIN_AND_PWD_WILL_OVERDUE)
		{
			CMessageBox::Show(_T("��¼"),_T("�����¼�����뼴�����ڣ�"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		
		// д��־
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_IN, theAPP_SESSION.GetUserInfo().sUserID, m_userInfo.curAuthority);
		
		if(m_userInfo.curAuthority == COMMON_AUTHORITY)
		{
			//����Ա���
			theSCAudit_MGR.SetCurrentLogin(m_userInfo.sUserID,loginTime);
			//theSETTING.SetPrintLoginTime(loginTime);
			//theSETTING.SetPrintStaffID(m_userInfo.sUserID);
		}
        // ��ʾ���˵�
		theMENU_MGR.SetUserPermission(nOperatorID);
		theMENU_MGR.SetMenuKind(m_userInfo.curAuthority == COMMON_AUTHORITY ? CMenuMgr::MENU_COMMON:CMenuMgr::MENU_MAINTAIN);
		theSERVICE_MGR.SetState(m_userInfo.curAuthority == COMMON_AUTHORITY ? READY:MAINTENANCE);
        // �����ȴ����
        theCURSOR_MGR.EndWaitCursor();
    }
    catch (CSysException& e) {
		m_userInfo.Initialize();
		theAPP_SESSION.SetUserInfo(m_userInfo);
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��½

@param      int nOperatorID ����ԱID
@param      CString sPwd ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::Login(int nOperatorID,CString sPwd)
{
	if(IsSuperOperatorLogin(nOperatorID)){
		if(!theBOM_INFO.IsUseFounderStaffID()){
			return LOGIN_CANNOT_USE_SUPEROPERATOR;
		}
		if(sPwd!=theBOM_INFO.GetFounderStaffPassword())
		{
			return LOGIN_USER_PWD_ERROR;
		}
		return LOGIN_SUCCESS;
	}
	long  localValidateResult = LocalValidate(nOperatorID,sPwd);
	if(localValidateResult!=LOGIN_SUCCESS)
	{
		return localValidateResult;
	}
	if(!theAPP_SESSION.IsSCConnected())
	{
		return localValidateResult;
	}
	BYTE serverLoginResult;
	long commandExecuteResult = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestLoginLogoff(nOperatorID,sPwd,LOGIN_ON,serverLoginResult);
	if(commandExecuteResult!=SP_SUCCESS)
	{
		return LOGIN_REQUEST_SERVER_FAILED;
	}
	return serverLoginResult;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������֤

@param      int nOperatorID ����ԱID
@param      CString sPwd ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LocalValidate(int nOperatorID,CString sPwd)
{
	typedef CAfcStaffPwdParam::STAFF STAFF_t;
	vector<STAFF_t> vecStaff=theAFC_STAFF.GetStaffParam();
	STAFF_t currentStaff;
	bool hasFound=false;
	for(int i=0;i<vecStaff.size();i++)
	{
		if(vecStaff.at(i).staffID ==nOperatorID)
		{
			currentStaff = vecStaff.at(i);
			hasFound = true;
			break;
		}
	}
	if(!hasFound)
	{
		return LOGIN_USER_PWD_ERROR;
	}
	if(thePWD_ERR.IsStuffIDExist(nOperatorID))
	{
		return LOGIN_LOCKED;
	}

	if(m_UserLoginCount.first!=nOperatorID)
	{
		m_UserLoginCount.first = nOperatorID;
		m_UserLoginCount.second = 0;
	}
	if(m_UserLoginCount.second >= AllowMaxLoginTimes)
	{
		return LOGIN_LOCKED;
	}
	if(sPwd != currentStaff.staffPWD)//���벻��ȷ
	{
		m_UserLoginCount.second +=1;
		if(m_UserLoginCount.second>=AllowMaxLoginTimes)
		{
			return LOGIN_OVER_MAX_COUNT;
		}
		return LOGIN_USER_PWD_ERROR;
	}
	_DATE currentDate = ComGetCurDate();
	if(CmpTwoDate(currentDate,currentStaff.pwdEndDate)>=0)
	{
		return LOGIN_ID_OVERDUE;
	}
	return LOGIN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����˻�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LockedOperatorID(int nOperatorID,int nPosInfo)
{
	thePWD_ERR.AddPWDErrorStuffID(nOperatorID);
	return theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendAccountLock(nOperatorID,nPosInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������ҵ����־

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::SaveOperationLog(bool success,bool isLogin,int nOperationID)
{
	OPERATOR_LOG operatorLog;
	operatorLog.operCode = (isLogin) ? OPERATION_LOGIN : OPERATION_LOGOUT; 
	operatorLog.operLevel = OPERATION_ONE_LEVEL; 
	operatorLog.result = (success) ? 0x01:0x00;
	memset(operatorLog.operTip, 0, sizeof(operatorLog.operTip));
	char *pDetail = (char*)(operatorLog.operDetail);
	if (isLogin)
		strcpy(pDetail, "��¼");
	else
		strcpy(pDetail, "�ǳ�");
	operatorLog.csUserID.Format(_T("%d"),nOperationID);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��½ʧ�����Դ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
OPERATION_MSG CLoginSvc::GetGuidCode(int  loginResult)
{
	OPERATION_MSG msg;
	switch (loginResult)
	{
	case LOGIN_SUCCESS :		//�����¼��
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGIN_USER_PWD_ERROR : //�û��������ڻ��������
		msg = GUIDE_LOGIN_ID_PASSWORD_NOT_EXITS;
		break;
	case LOGIN_REFUSE:  //�ܾ���¼���ظ���¼��֤����............
		msg = GUIDE_LOGIN_REPEAT_LOGIN;
		break;
	case LOGIN_AND_PWD_WILL_OVERDUE	:  //�����¼�����뼴��ʧЧ��
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGIN_AND_MODIFIED :	//�����¼��ǿ���޸����룻
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGOUT_FAILURE:	//0x06���ܾ��ǳ���
		msg = GUIDE_LOGOUT_REFUSE;
		break;
	case LOGIN_ID_OVERDUE :	//0x07���ܾ���¼���˻���������
		msg = GUIDE_LOGIN_ID_LOCKED;
		break;
	case LOGIN_PWD_OVERDUE :	//0x08���ܾ���¼��������ֹ��
		msg = GUIDE_LOGIN_PASSWORD_STOPED;
		break;
	case LOGIN_ID_EXPIRED:
		msg = GUIDE_LOGIN_ID_STOPED;
		break;
	case LOGIN_SETITEM_EMPTY:		//�û��������벻��Ϊ�ա�.......
		msg = GUIDE_LOGIN_ID_PASSWORD_EMPTY;
		break;
	case LOGIN_NOT_ACCESS: //û�з���Ȩ�ޣ�
		msg = GUIDE_LOGIN_UNAUTHORIZED;
		break;
	case LOGIN_OVER_MAX_COUNT: //��������½������
		msg = GUIDE_LOGIN_PWD_FAIL_MAX;
		break;
	case LOGIN_LOCKED:	//��ID�����������ܽ��е�½��
		msg = GUIDE_LOGIN_ID_LOCKED;
		break;
	case LOGIN_CANNOT_USE_SUPEROPERATOR: //�����û�Ȩ��û�зſ���
		msg = GUIDE_LOGIN_CANNOT_USE_SUPEROPERATOR;
		break;
	case LOGIN_FAILURE:		//δ֪����
		msg = GUIDE_LOGIN_UNKNOW_RESPONSE;
		break;
	case LOGIN_HAS_EXCEPTION:          // �쳣�У���ֹ��½
		msg = GUIDE_LOGIN_HAS_EXCEPTION;
		break;
	case LOGIN_REQUEST_SERVER_FAILED: //�������ʧ��
		msg = GUIDE_LOGIN_REQUEST_SERVER_FAILED;
		break;
	default:
		msg = GUIDE_LOGIN_UNKNOW_RESPONSE;
		break;
	}
	
	return msg;
}