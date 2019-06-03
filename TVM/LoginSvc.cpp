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
@brief      ���캯��

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CLoginSvc::CLoginSvc(void)
	:CTVMForeService(LOGIN_SVC,MAIN_SVC)
{
	try {
		// ���ò���Ա����
		m_pDialogFlow->AddDialog(CLoginDlg::IDD, new CLoginDlg(this));

		// ���ó˿ͻ���

		// ���û������
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CLoginDlg::IDD);

		// ���ó�ʼ������
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

		//// ���ڻ�LoginDlg��ʾ����
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
		//theMENU_MGR.SetMenuKind(CMenuMgr::MENU_LOGIN);
		// ���õ�¼����Ա��Ϣ����ȫ�ֲ���Ա��Ϣ��
		m_userInfo = theAPP_SESSION.GetUserInfo();
		m_userInfo.allAuthority = m_userInfo.curAuthority;

		// ������������������
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
//PLABEL_GROUP CLoginSvc::GetLoginAreaInfo() 
//{
//	return m_loginInfoGroup;
//}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ���¼����ʾ����

@param      ��

@retval     ��

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
	if(IsSuperOperatorLogin(nOperatorID)){
		return ALL_AUTHORITY;
	}
	BYTE staffAuthority = STAFF_AUTHORITY::NONE_AUTHORITY;
	SYS_FUNCTION_CODE maintainFunctions [] = {
		SYS_TVM_FUNCTION_CASH_SALE_INFO, // ����һ��������ͳ�ơ��豸״̬
		SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE	,// ֽ�������Ӳ�������
		SYS_TVM_FUNCTION_CARD_BOX_MANAGE,// Ʊ�����
		SYS_TVM_FUNCTION_BALANCE_MANAGE,// ���㴦��һ���ջ���ƾ֤����
		SYS_TVM_FUNCTION_MODE_SET	,// ģʽ�趨
		SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST,// ϵͳ�趨��Ӳ���Լ�
		SYS_TVM_FUNCTION_DATA_MANAGE	,// ���ݹ���
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
@brief      �ж��û��Ƿ��ǳ����û�

@param      ����ԱID

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
@brief      ȷ�ϴ���

@param      (i)CString pwd �˺�
@param      (i)CString pwd ����

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLoginSvc::Login(int id, CString pwd)
{
	try {
		// ��ʾ���ڵ�¼��
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_LOGINNING));

		// ���͵�¼�ǳ�����,����ǳ����û�
		if(theAPP_SESSION.IsSCConnected() && !IsSuperOperatorLogin(id)){
			//BYTE serverLoginResult;
			//long commandExecuteResult = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestLoginLogoff(id,pwd,LOGIN_ON,serverLoginResult);
			//if(commandExecuteResult!=SP_SUCCESS){
			//	LANGUAGE_MSG msg = GetGuidCode(serverLoginResult);
			//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(msg));
			//	SaveOperationLog(false,true,id);
			//	return;
			//}
			//// �ɹ����أ����ݷ��������жϷ�������
			//else{
			//	if(LOGIN_SUCCESS != serverLoginResult){
			//		LANGUAGE_MSG msg = GetGuidCode(serverLoginResult);
			//		theAPP_SESSION.ShowOfficeGuide(_opl(msg));
			//		SaveOperationLog(false,true,id);
			//		if(LOGIN_AND_PWD_WILL_OVERDUE == serverLoginResult || LOGIN_AND_MODIFIED == serverLoginResult){
			//			// �����¼��������Ҫ��ʾ��Ϣ
			//		}
			//		else return;
			//	}
			//}
			SaveOperationLog(true,true,id);
		}

		// ���õ�ǰ�û���Ϣ
		m_userInfo.sUserID.Format(_T("%.6d"),id);
		m_userInfo.sPwd = pwd;
		_DATE_TIME loginTime = ComGetCurTime();
		m_userInfo.loginTime = loginTime;
		thePWD_ERR.ClearStuffInfo();
		theAPP_SESSION.SetUserInfo(m_userInfo);

		// д��־
		theLOGIN_LOGOFF.WriteData(CLOGLoginAndLogoff::LOG_IN, theAPP_SESSION.GetUserInfo().sUserID, m_userInfo.curAuthority);

		bool superUser = theFunction_INFO.IsFounderStaffID(id);
		// �����Ӳ�����Ȩ��
		if(superUser||theAFC_ACCESS_LEVEL.IsStaffHasRight(id, SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE)){
			// ֹͣӲ������
			if(theTVM_INFO.IsCHMonitorOpened()){
				theSERVICE_MGR.GetService<CCoinModuleMonitorSvc>(MONITOR_COIN_MODULE_SVC)->Pause();
			}
		}

		// �����ֽ�������Ȩ��
		if(superUser||theAFC_ACCESS_LEVEL.IsStaffHasRight(id, SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE)){
			// ֹֽͣ������
			if(theTVM_INFO.IsCHMonitorOpened()){
				theSERVICE_MGR.GetService<CBanknoteCollectionModuleMonitorSvc>(MONITOR_BANKNOTE_ACCEPT_MODULE_SVC)->Pause();
			}
		}
		// ��¼�豸Ϊά��ҵ��
		theTVM_STATUS_MGR.SetServiceStatus(MANT_SERVICE);
		theTVM_STATUS_MGR.SetLoginStatus(LOGIN);

		// ��Ȩ��������
		theTVM_STATUS_MGR.SetAuthorizedAccessStatus(AUTHORIZED_STATUS_NORMAL);

		// ��ʾ���˵�
		theMENU_MGR.SetUserPermission(id);
		theSERVICE_MGR.StartService(MAINTENANCE_SVC);
		theSERVICE_MGR.SetState(MAINTENANCE);
		m_UserLoginCount.first = 0;

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
@brief      ��½������

@param      (i)int id ����ԱID
@param      (i)CString pwd ����

@retval     ��

@exception  ��
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
@brief      ������֤

@param      (i)int id ����ԱID
@param      (i)CString pwd ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LocalValidate(int id, CString pwd)
{
	// ���ҵ�ǰid��Ϣ
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

	// ������½������
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

	// ��½��ʱ
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
@brief      �����˻�

@param      (i)int id ����ԱID
@param      (i)int nPosInfo

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CLoginSvc::LockedOperatorID(int id, int nPosInfo)
{
	thePWD_ERR.AddPWDErrorStuffID(id);
	// ����Ϊ����״̬
	theTVM_STATUS_MGR.SetLoginStatus(LOCKED);
	return theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendAccountLock(id,nPosInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������ҵ����־

@param      (i)bool success
@param      (i)bool isLogin
@param      (i)int nOperationID

@retval     ��

@exception  ��
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
@brief      ��ȡ��½ʧ�����Դ���

@param      (i)int loginResult ��½���ش���

@retval     LANGUAGE_MSG

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_MSG CLoginSvc::GetGuidCode(int loginResult)
{
	LANGUAGE_MSG msg;
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

//��½��ʽ��Ϊ��������֮�� Enter����½
LRESULT CLoginSvc::OnKeyboardEnter(WPARAM wp , LPARAM lp)
{
	CLoginDlg *pDlg = dynamic_cast<CLoginDlg*>(m_pDialogFlow->GetDialog(CLoginDlg::IDD));
	// �ж��Ƿ�ʧȥ����
	if(pDlg->GetEditText(0).IsEmpty() || pDlg->GetEditText(1).IsEmpty()){
		pDlg->GetEditText(0).IsEmpty() ? pDlg->SetEditFocusOnSvc(0) : pDlg->SetEditFocusOnSvc(1);
		theAPP_SESSION.ShowOfficeGuide(GetGuidCode(LOGIN_SETITEM_EMPTY));
		return FALSE;
	}
	// �жϲ���Ա�Ƿ��е�¼Ȩ��
	int id = _ttoi(pDlg->GetEditText(0));
	STAFF_AUTHORITY authority = GetOperatorAllAccess(id);
	// ��Ȩ��
	if(authority == MAINTAIN_AUTHORITY || authority == ALL_AUTHORITY){
		// ��������Ƿ���ȷ����ȷ����е�½��
		CString pwd = pDlg->GetEditText(1);
		theSERVICE_MGR.SetForeServiceBusy(TRUE);
		long ret = CheckPassword(id, pwd);
		if (ret == LOGIN_SUCCESS){	
			SetUserAuthority((STAFF_AUTHORITY)authority);
			Login(id, pwd);
		}
		theSERVICE_MGR.SetForeServiceBusy(FALSE);  
	}
	// ��Ȩ��
	else{
		LANGUAGE_MSG msg = GetGuidCode(LOGIN_NOT_ACCESS);
		theAPP_SESSION.ShowOfficeGuide(_opl(msg));
	}
	return 1;
}

//ESC ������Ӧ
//LRESULT CLoginSvc::OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/)
//{
//	theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
//	return 1;
//}
