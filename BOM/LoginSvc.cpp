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
@brief      构造函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::CLoginSvc(void)
    :CBOMForeService(LOGIN_SVC,MAIN_SVC)
{
    try {
        // 设置操作员画面
        m_pDialogFlow->AddDialog(CLoginDlg::IDD, new CLoginDlg(this));
		
        // 设置乘客画面

        // 设置画面分组
        m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_INPUT_ID_AND_PWD));
        m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CLoginDlg::IDD);

        // 设置初始画面组
        m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

        // 设置登录操作员信息（＝全局操作员信息）
        m_userInfo = theAPP_SESSION.GetUserInfo();
        m_userInfo.allAuthority = m_userInfo.curAuthority;

        // 密码连续输错次数清零
        m_passwordErrCount = 0;

        // 初期化LoginDlg显示内容
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::~CLoginSvc(void)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

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
@brief      Service停止前处理

@param      无

@retval     无

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
@brief      获取登录的显示内容

@param      无

@retval     PLABEL_GROUP  文本内容的组合

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CLoginSvc::GetLoginAreaInfo() 
{
	return m_loginInfoGroup;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化登录的显示内容

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::InitLoginAreaInfo()
{
    try {
        m_loginInfoGroup[0].nameLeft = "　　　操作员ID：";
        m_loginInfoGroup[1].nameLeft = "　　　密　  码：";
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
@brief      设置登录操作员权限（并更新画面）

@param      (i)STAFF_AUTHORITY authority

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::SetUserAuthority(STAFF_AUTHORITY authority)
{
    m_userInfo.curAuthority = authority;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取操作员普通业务和维护业务的授权信息

@param      int nOperatorID 操作员ID

@retval     STAFF_AUTHORITY 授权信息

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
		SYS_FUNCTION_ISSUE,//售票
		SYS_FUNCTION_ADJUST,//补票
		SYS_FUNCTION_CHARGE,//充值
		SYS_FUNCTION_REFUND,//退款
		SYS_FUNCTION_EXCHANGE,//换卡
		SYS_FUNCTION_REFRESH,//激活
		SYS_FUNCTION_ANALYZE,//查询	
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
		SYS_FUNCTION_SYSTEM_SET,//系统设置	
		SYS_FUNCTION_DATA_MANAGE	,//数据管理	
		SYS_FUNCTION_HARDWARE_TEST,//硬件测试	
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
@brief      判断用户是否是超级用户

@param      操作员ID

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
@brief      确认处理

@param      (i)CString sPassWord                  // 用户输入的密码

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::OnOK(int nOperatorID,CString password)
{
    try {
		// 若只有硬件异常则可登陆
		// 普通业务
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
			
			//存在不允许登录的异常
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
		//操作员计数，跨运营日则清零
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
			CMessageBox::Show(_T("登录"),_T("允许登录，并强制修改密码！"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		if(loginResult == LOGIN_AND_PWD_WILL_OVERDUE)
		{
			CMessageBox::Show(_T("登录"),_T("允许登录，密码即将过期！"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		
		// 写日志
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_IN, theAPP_SESSION.GetUserInfo().sUserID, m_userInfo.curAuthority);
		
		if(m_userInfo.curAuthority == COMMON_AUTHORITY)
		{
			//操作员审计
			theSCAudit_MGR.SetCurrentLogin(m_userInfo.sUserID,loginTime);
			//theSETTING.SetPrintLoginTime(loginTime);
			//theSETTING.SetPrintStaffID(m_userInfo.sUserID);
		}
        // 显示主菜单
		theMENU_MGR.SetUserPermission(nOperatorID);
		theMENU_MGR.SetMenuKind(m_userInfo.curAuthority == COMMON_AUTHORITY ? CMenuMgr::MENU_COMMON:CMenuMgr::MENU_MAINTAIN);
		theSERVICE_MGR.SetState(m_userInfo.curAuthority == COMMON_AUTHORITY ? READY:MAINTENANCE);
        // 结束等待光标
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
@brief      登陆

@param      int nOperatorID 操作员ID
@param      CString sPwd 密码

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
@brief      本地验证

@param      int nOperatorID 操作员ID
@param      CString sPwd 密码

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
	if(sPwd != currentStaff.staffPWD)//密码不正确
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
@brief      锁定账户

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
@brief      保存操作业务日志

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
		strcpy(pDetail, "登录");
	else
		strcpy(pDetail, "登出");
	operatorLog.csUserID.Format(_T("%d"),nOperationID);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取登陆失败文言代码

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
	case LOGIN_SUCCESS :		//允许登录；
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGIN_USER_PWD_ERROR : //用户名不存在或密码错误；
		msg = GUIDE_LOGIN_ID_PASSWORD_NOT_EXITS;
		break;
	case LOGIN_REFUSE:  //拒绝登录（重复登录验证）；............
		msg = GUIDE_LOGIN_REPEAT_LOGIN;
		break;
	case LOGIN_AND_PWD_WILL_OVERDUE	:  //允许登录，密码即将失效；
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGIN_AND_MODIFIED :	//允许登录，强制修改密码；
		msg = GUIDE_LOGIN_LOGINNING;
		break;
	case LOGOUT_FAILURE:	//0x06：拒绝登出；
		msg = GUIDE_LOGOUT_REFUSE;
		break;
	case LOGIN_ID_OVERDUE :	//0x07：拒绝登录（账户锁定）；
		msg = GUIDE_LOGIN_ID_LOCKED;
		break;
	case LOGIN_PWD_OVERDUE :	//0x08：拒绝登录（密码终止）
		msg = GUIDE_LOGIN_PASSWORD_STOPED;
		break;
	case LOGIN_ID_EXPIRED:
		msg = GUIDE_LOGIN_ID_STOPED;
		break;
	case LOGIN_SETITEM_EMPTY:		//用户名或密码不能为空。.......
		msg = GUIDE_LOGIN_ID_PASSWORD_EMPTY;
		break;
	case LOGIN_NOT_ACCESS: //没有访问权限；
		msg = GUIDE_LOGIN_UNAUTHORIZED;
		break;
	case LOGIN_OVER_MAX_COUNT: //超过最大登陆次数；
		msg = GUIDE_LOGIN_PWD_FAIL_MAX;
		break;
	case LOGIN_LOCKED:	//该ID被锁定，不能进行登陆。
		msg = GUIDE_LOGIN_ID_LOCKED;
		break;
	case LOGIN_CANNOT_USE_SUPEROPERATOR: //超级用户权限没有放开。
		msg = GUIDE_LOGIN_CANNOT_USE_SUPEROPERATOR;
		break;
	case LOGIN_FAILURE:		//未知错误。
		msg = GUIDE_LOGIN_UNKNOW_RESPONSE;
		break;
	case LOGIN_HAS_EXCEPTION:          // 异常中，禁止登陆
		msg = GUIDE_LOGIN_HAS_EXCEPTION;
		break;
	case LOGIN_REQUEST_SERVER_FAILED: //请求服务失败
		msg = GUIDE_LOGIN_REQUEST_SERVER_FAILED;
		break;
	default:
		msg = GUIDE_LOGIN_UNKNOW_RESPONSE;
		break;
	}
	
	return msg;
}