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

// �༭���������
static CRect EDIT_PASSWORD_RECT[] = {
	BASE_INFO_RECT2(6, 250+160, DEFAULT_TXT_DISPLAY_LEN),
	BASE_INFO_RECT2(7, 250+160, DEFAULT_TXT_DISPLAY_LEN)
};

static const int IDX_ACCOUNT_EDIT = 0;		// �˺ű༭��
static const int IDX_PASSWORD_EDIT = 1;		// ����༭��

IMPLEMENT_DYNAMIC(CLoginDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CLoginDlg, COperationDlg)
	ON_WM_CREATE()
    ON_WM_ACTIVATE()
	ON_MESSAGE(WM_EDIT_CHANGE, OnEditChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��

 @param      (i)CService* pService  Dialog��Ӧ��Service

 @retval     ��

 @exception  ��
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
 @brief      ��������

 @param      ��

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLoginDlg::~CLoginDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���洴������

 @param      (i)LPCREATESTRUCT lpCreateStruct  

 @retval     int  0:�����ɹ�  -1:����ʧ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        // ����������Ϣ����ȷ����ť�����ּ���
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
	// ��ʾ����
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
	//SetEditFocus(IDX_ACCOUNT_EDIT);//lws �������뽹����Account  theAPP_SESSION.SetKeyboard(KEY_ENTER | KEY_ESC);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����˺����볤��

@param      ��

@retval     bool  true:��ȷ;  false:����ȷ

@exception  ��
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
@brief      ��½����

@param      ��

@retval     bool  true:��ȷ;  false:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::DoLogin(DWORD authority)
{
    if (!CheckLength()) return;

	int id = _ttoi(GetEditText(IDX_ACCOUNT_EDIT));
	CString pwd = GetEditText(IDX_PASSWORD_EDIT);

	// ��������Ƿ���ȷ����ȷ����е�½��
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
@brief      �༭����Ϣ��Ӧ

@param      (i)WPARAM wParam,LPARAM lParam

@retval     LRESULT  TRUE:��ȷ;  FALSE:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CLoginDlg::OnEditChange(WPARAM wParam,LPARAM lParam)

{
	CGEdit* currentEdit = reinterpret_cast<CGEdit*>(lParam);

	// ����ԱID�����
	if(currentEdit == GetEdit(IDX_ACCOUNT_EDIT)){		
		int len = GetEditText(IDX_ACCOUNT_EDIT).GetLength();
		if (len != STAFFID_LEN)	{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_LOGIN_INPUT_ID_AND_PWD));
			m_account_flag = len == 5 ? true : false;
		}
		else{
			// �˺ų��ȵ�һ��Ϊ6ʱ����m_account_flag��Ϊfalse
			// �˺ų���һֱΪ6ʱ���������������õ�������
			if (!m_account_flag && GetEditText(IDX_PASSWORD_EDIT).GetLength() == 0){
				SetEditText(IDX_PASSWORD_EDIT, CString((TCHAR)wParam));
			}
			m_account_flag = false;
			SetEditFocus(IDX_PASSWORD_EDIT);
		}
	}
	// ���������
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
@brief      �쳣���з����仯ʱ��Ӧ����

@param      (i)CSysException& e �쳣����
@param      (i)bool isInsert      

@retval     ��

@exception  ��
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
@brief      ���ý���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::SetEditFocusOnSvc(int edIndex){
	if(IDX_ACCOUNT_EDIT == edIndex)
		SetEditFocus(IDX_ACCOUNT_EDIT);
	else if(IDX_PASSWORD_EDIT == edIndex)
		SetEditFocus(IDX_PASSWORD_EDIT);
}