#include "stdafx.h"
#include "TVM.h"
#include "LoginDlg.h"
#include "LoginSvc.h"
#include "svcdef.h"
#include "InnerException.h"
#include "exceptionmgr.h"
#include "MessageIDDefinition.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"
#include "LogoffHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 编辑框坐标组合
static CRect EDIT_PASSWORD_RECT[] = {
	BASE_INFO_RECT2(6, 250+160, DEFAULT_TXT_DISPLAY_LEN),
	BASE_INFO_RECT2(7, 250+160, DEFAULT_TXT_DISPLAY_LEN)
};

static const int IDX_ACCOUNT_EDIT = 0;		// 账号编辑框
static const int IDX_PASSWORD_EDIT = 1;		// 密码编辑框

IMPLEMENT_DYNAMIC(CLoginDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CLoginDlg, COperationDlg)
	ON_WM_CREATE()
    ON_WM_ACTIVATE()
	ON_MESSAGE(WM_EDIT_CHANGE, OnEditChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
 @brief      构造函数

 @param      (i)CService* pService  Dialog对应的Service

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLoginDlg::CLoginDlg(CService* pService)
	: COperationDlg(CLoginDlg::IDD, pService)
{
    try {
        CLoginSvc* loginSvc = (CLoginSvc*)GetService();
        m_baseInfo->detailInfo.row = CLoginSvc::LOGIN_INFO_ROWS;
        m_baseInfo->detailInfo.labelGroup = m_labelGroup;
        m_baseInfo->detailInfo.editGroup.cnt = 2;
        m_baseInfo->detailInfo.editGroup.editRect = EDIT_PASSWORD_RECT;
		m_baseInfo->stytle = OPERATOR_STATIC_STYLE;
		m_baseInfo->type = 2;

		theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CLoginDlg::OnExceptionItemNotify);

		theAPP_SESSION.SetMaxInputLength(0);
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
CLoginDlg::~CLoginDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      画面创建函数

 @param      (i)LPCREATESTRUCT lpCreateStruct  

 @retval     int  0:创建成功  -1:创建失败

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        // 创建基本信息区域、确定按钮和数字键盘
        if (__super::OnCreate(lpCreateStruct, BASE_AREA | GUIDE_AREA) != 0) {
            return -1;
        }
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);

		m_baseInfoArea->SetLableWidth(0, 250);
		m_baseInfoArea->SetLableAlign(0, DT_RIGHT | DT_VCENTER);
		GetEdit(IDX_ACCOUNT_EDIT)->SetLimitText(6);
		GetEdit(IDX_PASSWORD_EDIT)->SetPasswordChar('*');
		GetEdit(IDX_PASSWORD_EDIT)->SetLimitText(6);
		theAPP_SESSION.SetMaxInputLength(0);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

void CLoginDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	m_labelGroup[4].nameLeft = _opl(LOGIN_OPERATOR_ID);
	m_labelGroup[5].nameLeft = _opl(LOGIN_PASSWORE);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);

	//theAPP_SESSION.SetKeyboard(KEY_ENTER | KEY_ESC);
	theAPP_SESSION.SetMaxInputLength(0);

	SetEditFocus(IDX_ACCOUNT_EDIT);
	CString strOperatorID = _T("");
	if(theAPP_SESSION.GetCardOpenMainDoorOperatorID()!=0){
		strOperatorID.Format(_T("%d"), theAPP_SESSION.GetCardOpenMainDoorOperatorID());
		SetEditFocus(IDX_PASSWORD_EDIT);
	}
	GetEdit(IDX_ACCOUNT_EDIT)->SetWindowText(strOperatorID);
	GetEdit(IDX_PASSWORD_EDIT)->SetWindowText(_T(""));
	// 提示文言
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
	//SetEditFocus(IDX_ACCOUNT_EDIT);//lws 设置输入焦点在Account  theAPP_SESSION.SetKeyboard(KEY_ENTER | KEY_ESC);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      检查账号密码长度

@param      无

@retval     bool  true:正确;  false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool  CLoginDlg::CheckLength()
{
	if (GetEditText(IDX_ACCOUNT_EDIT).GetLength() < STAFFID_LEN)
	{
		SetEditFocus(IDX_ACCOUNT_EDIT);
		return false;
	}
	if (GetEditText(IDX_PASSWORD_EDIT).GetLength() < STAFFID_LEN)
	{
		SetEditFocus(IDX_PASSWORD_EDIT);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      登陆操作

@param      无

@retval     bool  true:正确;  false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::DoLogin(DWORD authority)
{
    if (!CheckLength()) return;

	int id = _ttoi(GetEditText(IDX_ACCOUNT_EDIT));
	CString pwd = GetEditText(IDX_PASSWORD_EDIT);

	// 检查密码是否正确，正确则进行登陆。
	theSERVICE_MGR.SetForeServiceBusy(TRUE);
	CLoginSvc* pService = GetService<CLoginSvc>();
	long ret = pService->CheckPassword(id, pwd);
	if (ret == CLoginSvc::LOGIN_SUCCESS){
		pService->SetUserAuthority((STAFF_AUTHORITY)authority);
		pService->Login(id, pwd);
	}
	else{
		SetEditFocus(IDX_ACCOUNT_EDIT);
		SetEditText(IDX_ACCOUNT_EDIT, _T(""));
		SetEditText(IDX_PASSWORD_EDIT, _T(""));
	}
	theSERVICE_MGR.SetForeServiceBusy(FALSE);  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      编辑框消息响应

@param      (i)WPARAM wParam,LPARAM lParam

@retval     LRESULT  TRUE:正确;  FALSE:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CLoginDlg::OnEditChange(WPARAM wParam,LPARAM lParam)

{
	CGEdit* currentEdit = reinterpret_cast<CGEdit*>(lParam);

	// 操作员ID输入框
	if(currentEdit == GetEdit(IDX_ACCOUNT_EDIT)){		
		int len = GetEditText(IDX_ACCOUNT_EDIT).GetLength();
		if (len != STAFFID_LEN)	{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
			m_account_flag = len == 5 ? true : false;
		}
		else{
			// 账号长度第一次为6时，将m_account_flag设为false
			// 账号长度一直为6时，将输入内容设置到密码栏
			if (!m_account_flag && GetEditText(IDX_PASSWORD_EDIT).GetLength() == 0){
				SetEditText(IDX_PASSWORD_EDIT, CString((TCHAR)wParam));
			}
			m_account_flag = false;
			SetEditFocus(IDX_PASSWORD_EDIT);
		}
	}
	// 密码输入框
	else {
		int len = GetEditText(IDX_PASSWORD_EDIT).GetLength();
		if (len != STAFFPWD_LEN){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
			if (wParam == VK_BACK && len == 0){
				SetEditFocus(IDX_ACCOUNT_EDIT);
			}
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      异常队列发生变化时相应函数

@param      (i)CSysException& e 异常对象
@param      (i)bool isInsert      

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::OnExceptionItemNotify(CSysException& e,bool isInsert)
{
	CService* pService = theSERVICE_MGR.GetCurService();
	if (pService->GetServiceID() == LOGIN_SVC){
		if (theEXCEPTION_MGR.HasException()){
			//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_HAS_EXCEPTION));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
		}
	}
}

//void CLoginDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//	int id = _ttoi(GetEditText(IDX_ACCOUNT_EDIT));
//	STAFF_AUTHORITY authority = GetService<CLoginSvc>()->GetOperatorAllAccess(id);
//	if(nChar == VK_RETURN)
//	{
//		if(authority == MAINTAIN_AUTHORITY || authority == ALL_AUTHORITY)
//		{
//			DoLogin(authority);
//		}
//	}
//	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}

//BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
//{
//	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
//	{
//		int id = _ttoi(GetEditText(IDX_ACCOUNT_EDIT));
//		STAFF_AUTHORITY authority = GetService<CLoginSvc>()->GetOperatorAllAccess(id);
//		if(authority == MAINTAIN_AUTHORITY || authority == ALL_AUTHORITY)
//		{
//			DoLogin(authority);
//		}
//	}
//	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
//	{
//		theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
//	}
//	return __super::PreTranslateMessage(pMsg);
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置焦点

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::SetEditFocusOnSvc(int edIndex){
	if(IDX_ACCOUNT_EDIT == edIndex)
		SetEditFocus(IDX_ACCOUNT_EDIT);
	else if(IDX_PASSWORD_EDIT == edIndex)
		SetEditFocus(IDX_PASSWORD_EDIT);
}