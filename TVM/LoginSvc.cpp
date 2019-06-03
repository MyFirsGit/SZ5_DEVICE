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
#include "tvmsetting.h"
#include "LanguageInfo.h"
#include "LogoffHelper.h"
#include "BankNoteCollectionModuleMonitorSvc.h"
#include "CoinModuleMonitorSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AllowMaxLoginTimes (theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM ? theAFC_TVM_OPERATION.GetPwdInputMaxTimes() : theAFC_TVM_OPERATION.GetPwdInputMaxTimes())

BEGIN_MESSAGE_MAP(CLoginSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::CLoginSvc(void)
	:CTVMForeService(LOGIN_SVC,MAIN_SVC)
{
	try {
		// 设置操作员画面
		m_pDialogFlow->AddDialog(CLoginDlg::IDD, new CLoginDlg(this));

		// 设置乘客画面

		// 设置画面分组
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CLoginDlg::IDD);

		// 设置初始画面组
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

		//// 初期化LoginDlg显示内容
		//InitLoginAreaInfo();

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
		//theMENU_MGR.SetMenuKind(CMenuMgr::MENU_LOGIN);
		// 设置登录操作员信息（＝全局操作员信息）
		m_userInfo = theAPP_SESSION.GetUserInfo();
		m_userInfo.allAuthority = m_userInfo.curAuthority;

		// 密码连续输错次数清零
		m_UserLoginCount.first  = 0;
		m_UserLoginCount.second = 0;
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
//PLABEL_GROUP CLoginSvc::GetLoginAreaInfo() 
//{
//	return m_loginInfoGroup;
//}


//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化登录的显示内容

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void CLoginSvc::InitLoginAreaInfo()
//{
//    try {
//        m_loginInfoGroup[0].nameLeft = _tl(LOGIN_OPERATOR_ID);
//        m_loginInfoGroup[1].nameLeft = _tl(LOGIN_PASSWORE);
//    }
//    catch(CSysException&) {
//        throw;
//    }
//    catch (...) {
//		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
//    }
//}

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
	if(IsSuperOperatorLogin(nOperatorID)){
		return ALL_AUTHORITY;
	}
	BYTE staffAuthority = STAFF_AUTHORITY::NONE_AUTHORITY;
	SYS_FUNCTION_CODE maintainFunctions [] = {
		SYS_TVM_FUNCTION_CASH_SALE_INFO, // 交易一览、销售统计、设备状态
		SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE	,// 纸币箱管理、硬币箱管理
		SYS_TVM_FUNCTION_CARD_BOX_MANAGE,// 票箱管理
		SYS_TVM_FUNCTION_BALANCE_MANAGE,// 结算处理、一键收机、凭证补打
		SYS_TVM_FUNCTION_MODE_SET	,// 模式设定
		SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST,// 系统设定、硬件自检
		SYS_TVM_FUNCTION_DATA_MANAGE	,// 数据管理
	};
	for (int i = 0; i< sizeof(maintainFunctions)/sizeof(SYS_FUNCTION_CODE); ++i){
		if(theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID, maintainFunctions[i])){
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
	return theFunction_INFO.IsFounderStaffID(nOperatorID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      确认处理

@param      (i)CString pwd 账号
@param      (i)CString pwd 密码

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::Login(int id, CString pwd)
{
	try {
		// 显示正在登录中
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_LOGINNING));

		// 发送登录登出电文,如果非超级用户
		if(theAPP_SESSION.IsSCConnected() && !IsSuperOperatorLogin(id)){
			//BYTE serverLoginResult;
			//long commandExecuteResult = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestLoginLogoff(id,pwd,LOGIN_ON,serverLoginResult);
			//if(commandExecuteResult!=SP_SUCCESS){
			//	LANGUAGE_MSG msg = GetGuidCode(serverLoginResult);
			//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(msg));
			//	SaveOperationLog(false,true,id);
			//	return;
			//}
			//// 成功反回，根据反馈数据判断返回类型
			//else{
			//	if(LOGIN_SUCCESS != serverLoginResult){
			//		LANGUAGE_MSG msg = GetGuidCode(serverLoginResult);
			//		theAPP_SESSION.ShowOfficeGuide(_opl(msg));
			//		SaveOperationLog(false,true,id);
			//		if(LOGIN_AND_PWD_WILL_OVERDUE == serverLoginResult || LOGIN_AND_MODIFIED == serverLoginResult){
			//			// 允许登录，但是需要提示信息
			//		}
			//		else return;
			//	}
			//}
			SaveOperationLog(true,true,id);
		}

		// 设置当前用户信息
		m_userInfo.sUserID.Format(_T("%.6d"),id);
		m_userInfo.sPwd = pwd;
		_DATE_TIME loginTime = ComGetCurTime();
		m_userInfo.loginTime = loginTime;
		thePWD_ERR.ClearStuffInfo();
		theAPP_SESSION.SetUserInfo(m_userInfo);

		// 写日志
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_IN, theAPP_SESSION.GetUserInfo().sUserID, m_userInfo.curAuthority);

		bool superUser = theFunction_INFO.IsFounderStaffID(id);
		// 如果有硬箱管理权限
		if(superUser||theAFC_ACCESS_LEVEL.IsStaffHasRight(id, SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE)){
			// 停止硬币箱监控
			if(theTVM_INFO.IsCHMonitorOpened()){
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Pause();
			}
		}

		// 如果有纸币箱管理权限
		if(superUser||theAFC_ACCESS_LEVEL.IsStaffHasRight(id, SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE)){
			// 停止纸币箱监控
			if(theTVM_INFO.IsCHMonitorOpened()){
				theSERVICE_MGR.GetService<CBanknoteCollectionModuleMonitorSvc>(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC)->Pause();
			}
		}
		// 登录设备为维护业务
		theTVM_STATUS_MGR.SetServiceStatus(MANT_SERVICE);
		theTVM_STATUS_MGR.SetLoginStatus(LOGIN);

		// 授权访问正常
		theTVM_STATUS_MGR.SetAuthorizedAccessStatus(AUTHORIZED_STATUS_NORMAL);

		// 显示主菜单
		theMENU_MGR.SetUserPermission(id);
		theSERVICE_MGR.StartService(MAINTENANCE_SVC);
		theSERVICE_MGR.SetState(MAINTENANCE);
		m_UserLoginCount.first = 0;

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
@brief      登陆密码检查

@param      (i)int id 操作员ID
@param      (i)CString pwd 密码

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::CheckPassword(int id, CString pwd)
{
	long ret = LOGIN_SUCCESS ;
	if (IsSuperOperatorLogin(id)){
		if (!theFunction_INFO.IsUseFounderStaffID()){
			ret = LOGIN_CANNOT_USE_SUPEROPERATOR;
		}
		if(pwd != theFunction_INFO.GetFounderStaffPassword()){
			ret = LOGIN_USER_PWD_ERROR;
		}
	}
	else{
		ret = LocalValidate(id, pwd);
		if(ret != LOGIN_SUCCESS && ret == LOGIN_OVER_MAX_COUNT)	{
			m_UserLoginCount.second = 0;
			LockedOperatorID(id,/*m_userInfo.curAuthority == COMMON_AUTHORITY ? 0x03 : */0x06);
		}
	}

	LANGUAGE_MSG msg = GetGuidCode(ret);
	theAPP_SESSION.ShowOfficeGuide(_opl(msg));
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      本地验证

@param      (i)int id 操作员ID
@param      (i)CString pwd 密码

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LocalValidate(int id, CString pwd)
{
	// 查找当前id信息
	typedef CAfcStaffPwdParam::STAFF STAFF_t;
	vector<STAFF_t> vecStaff = theAFC_STAFF.GetStaffParam();
	STAFF_t currentStaff;
	bool hasFound = false;
	for (int i = 0; i < vecStaff.size(); ++i)
	{
		if (vecStaff.at(i).staffID == id)
		{
			currentStaff = vecStaff.at(i);
			hasFound = true;
			break;
		}
	}
	if (!hasFound)
	{
		return LOGIN_USER_PWD_ERROR;
	}
	if (thePWD_ERR.IsStuffIDExist(id))
	{
		return LOGIN_LOCKED;
	}

	// 超过登陆最大次数
	if (m_UserLoginCount.first != id)
	{
		m_UserLoginCount.first = id;
		m_UserLoginCount.second = 0;
	}
	if (m_UserLoginCount.second >= AllowMaxLoginTimes)
	{
		return LOGIN_LOCKED;
	}
	if (pwd != currentStaff.staffPWD)
	{
		++m_UserLoginCount.second;
		if(m_UserLoginCount.second >= AllowMaxLoginTimes)
		{
			return LOGIN_OVER_MAX_COUNT;
		}
		return LOGIN_USER_PWD_ERROR;
	}

	// 登陆超时
	_DATE currentDate = ComGetCurDate();
	CString strDate = currentDate.ToString(_T("%04d:%02d:%02d"));
	CString strCard = currentStaff.pwdEndDate.ToString(_T("%04d:%02d:%02d"));
	if(CmpTwoDate(currentDate, currentStaff.pwdEndDate) >= 0)
	{
		return LOGIN_ID_EXPIRED;
	}

	return LOGIN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      锁定账户

@param      (i)int id 操作员ID
@param      (i)int nPosInfo

@retval     long

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LockedOperatorID(int id, int nPosInfo)
{
	thePWD_ERR.AddPWDErrorStuffID(id);
	// 设置为锁定状态
	theTVM_STATUS_MGR.SetLoginStatus(LOCKED);
	return theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendAccountLock(id,nPosInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存操作业务日志

@param      (i)bool success
@param      (i)bool isLogin
@param      (i)int nOperationID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::SaveOperationLog(bool success, bool isLogin, int id)
{
	OPERATOR_LOG log;
	log.operCode = (isLogin) ? OPERATION_LOGIN : OPERATION_LOGOUT; 
	log.operLevel = OPERATION_ONE_LEVEL; 
	log.result = (success) ? 0x01 : 0x00;
	memset(log.operTip, 0, sizeof(log.operTip));
	char* pDetail = (char*)(log.operDetail);
	if (isLogin)
		strcpy(pDetail, "Login");
	else
		strcpy(pDetail, "logoff");
	log.csUserID.Format(_T("%d"), id);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(log);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取登陆失败文言代码

@param      (i)int loginResult 登陆返回代码

@retval     LANGUAGE_MSG

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_MSG CLoginSvc::GetGuidCode(int loginResult)
{
	LANGUAGE_MSG msg;
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

//登陆方式改为输完密码之后 Enter键登陆
LRESULT CLoginSvc::OnKeyboardEnter(WPARAM wp , LPARAM lp)
{
	CLoginDlg *pDlg = dynamic_cast<CLoginDlg*>(m_pDialogFlow->GetDialog(CLoginDlg::IDD));
	// 判断是否失去焦点
	if(pDlg->GetEditText(0).IsEmpty() || pDlg->GetEditText(1).IsEmpty()){
		pDlg->GetEditText(0).IsEmpty() ? pDlg->SetEditFocusOnSvc(0) : pDlg->SetEditFocusOnSvc(1);
		theAPP_SESSION.ShowOfficeGuide(GetGuidCode(LOGIN_SETITEM_EMPTY));
		return FALSE;
	}
	// 判断操作员是否有登录权限
	int id = _ttoi(pDlg->GetEditText(0));
	STAFF_AUTHORITY authority = GetOperatorAllAccess(id);
	// 有权限
	if(authority == MAINTAIN_AUTHORITY || authority == ALL_AUTHORITY){
		// 检查密码是否正确，正确则进行登陆。
		CString pwd = pDlg->GetEditText(1);
		theSERVICE_MGR.SetForeServiceBusy(TRUE);
		long ret = CheckPassword(id, pwd);
		if (ret == LOGIN_SUCCESS){	
			SetUserAuthority((STAFF_AUTHORITY)authority);
			Login(id, pwd);
		}
		theSERVICE_MGR.SetForeServiceBusy(FALSE);  
	}
	// 无权限
	else{
		LANGUAGE_MSG msg = GetGuidCode(LOGIN_NOT_ACCESS);
		theAPP_SESSION.ShowOfficeGuide(_opl(msg));
	}
	return 1;
}

//ESC 按键响应
//LRESULT CLoginSvc::OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/)
//{
//	theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
//	return 1;
//}
