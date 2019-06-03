#include "stdafx.h"
#include "BOM.h"
#include "LoginDlg.h"
#include "LoginSvc.h"
#include "svcdef.h"
#include "InnerException.h"
#include "exceptionmgr.h"
#include "MessageIDDefinition.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 编辑框坐标组合
//static CRect EDIT_PASSWORD_RECT[] = {BASE_INFO_RECT_SP(1,DEFAULT_TXT_DISPLAY_LEN,0),BASE_INFO_RECT(2, DEFAULT_TXT_DISPLAY_LEN)};
static CRect EDIT_PASSWORD_RECT[] = {BASE_INFO_RECT_SP(1,DEFAULT_TXT_DISPLAY_LEN,0),BASE_INFO_RECT(2, DEFAULT_TXT_DISPLAY_LEN)};

static const char* const TXT_BTN_COMMON     = "普通业务";
static const char* const TXT_BTN_MAINTAIN   = "维护业务";
static const char* const WM_BTN_COMMON    	= "common";			 // 普通业务
static const char* const WM_BTN_MAINTAIN	= "maintain";		 // 维护业务

IMPLEMENT_DYNAMIC(CLoginDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CLoginDlg, COperationDlg)
	ON_WM_CREATE()
    ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XBUTTON, XButtonClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
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
        m_btnCommon = NULL;	                       // 普通业务按钮
        m_btnMaintain = NULL;                      // 维护业务按钮
		
        CLoginSvc* loginSvc = (CLoginSvc*)GetService();
        m_baseInfo->titleInfo.titleName = "操作员登录";
        m_baseInfo->detailInfo.row = CLoginSvc::LOGIN_INFO_ROWS;
        m_baseInfo->detailInfo.labelGroup = loginSvc->GetLoginAreaInfo();
        m_baseInfo->detailInfo.editGroup.cnt = 2;
        m_baseInfo->detailInfo.editGroup.editRect = EDIT_PASSWORD_RECT;

		UIINFO normalButtonUI;
		normalButtonUI.m_pParentWnd = this;
		normalButtonUI.m_Location.y = BTN_OK_RECT.top;
		normalButtonUI.m_Location.x = (OPERATION_FRMAE_RECT.right - OPERATION_FRMAE_RECT.left - DEFAULT_SIZE_BUTTON.cx*2)/3;
		normalButtonUI.m_Size = CSize(80*CXSCREENRATIO,50*CYSCREENRATIO);

		BTN_INFO normalButtonInfo;
		normalButtonInfo.btnMsg = WM_BTN_COMMON;
		normalButtonInfo.isEnable = false;
		normalButtonInfo.btnID = 100;
		normalButtonInfo.text = "普通业务";

		m_btnCommon = new CXButton(normalButtonUI,normalButtonInfo);

		UIINFO maintainButtonUI;
		maintainButtonUI.m_pParentWnd = this;
		maintainButtonUI.m_Location.y = BTN_OK_RECT.top;
		maintainButtonUI.m_Location.x = normalButtonUI.m_Location.x + DEFAULT_SIZE_BUTTON.cx + normalButtonUI.m_Location.x;
		maintainButtonUI.m_Size = CSize(80*CXSCREENRATIO,50*CYSCREENRATIO);

		BTN_INFO maintainButtonInfo;
		maintainButtonInfo.btnMsg = WM_BTN_MAINTAIN;
		maintainButtonInfo.isEnable = false;
		maintainButtonInfo.btnID = 101;
		maintainButtonInfo.text = "维护业务";

		m_btnMaintain = new CXButton(maintainButtonUI,maintainButtonInfo);
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
    delete m_btnCommon;
    m_btnCommon = NULL;	                       // 普通业务按钮

    delete m_btnMaintain;
    m_btnMaintain = NULL;                      // 维护业务按钮

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      画面创建函数

 @param      (i)LPCREATESTRUCT lpCreateStruct  

 @retval     int    \n
             0:创建成功    -1:创建失败

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        // 创建基本信息区域、确定按钮和数字键盘
        if (__super::OnCreate(lpCreateStruct, BASE_AREA|GUIDE_AREA|KEYBOARD_AREA) != 0) {
            return -1;
        }
		m_btnCommon->Create();
		m_btnCommon->SetEnabled(false);
		m_btnMaintain->Create();
		m_btnMaintain->SetEnabled(false);
		
		GetEdit(0)->SetLimitText(6);

		// 密码输入框中的内容显示为 '*'
		GetEdit(1)->SetPasswordChar('*');
		GetEdit(1)->SetLimitText(6);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文指针

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief      处理按钮消息

 @param      (i)WPARAM wParam  消息信息，表示按钮的ID号
 @param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

 @retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CLoginDlg::XButtonClick(WPARAM wParam, LPARAM lParam)
{
    try {
        
		if(!CheckInput())
		{
			return FALSE;
		}
		if(theAPP_SESSION.IsInitErrorOver())
		{
			return FALSE;
		}
		int nOperatorID = _ttoi(GetEditText(0));
		CString sPwd = GetEditText(1);
		CString sMsg = (LPCTSTR)lParam;
		CLoginSvc* pService = GetService<CLoginSvc>();
		pService->SetUserAuthority(sMsg == WM_BTN_COMMON?COMMON_AUTHORITY:MAINTAIN_AUTHORITY);
		theSERVICE_MGR.SetForeServiceBusy(TRUE);
		pService->OnOK(nOperatorID,sPwd);
		theSERVICE_MGR.SetForeServiceBusy(FALSE);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查输入操作员ID是否正确

@param      无

@retval     bool true:正确; false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool  CLoginDlg::CheckInput()
{
    if(GetEditText(0).GetLength() < STAFFID_LEN){
        SetEditFocus(0);  
        return false;
    }
    if(GetEditText(1).GetLength() == 0){
        SetEditFocus(1);  
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查输入操作员ID是否正确

@param      无

@retval     bool true:正确; false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CLoginDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CGEdit *currentEdit = reinterpret_cast<CGEdit*>(lParam);
	if(currentEdit != GetEdit(0))
	{
		return FALSE;
	}
	if( GetEditText(0).GetLength()!=STAFFID_LEN)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_INPUT_ID_AND_PWD));
		m_btnCommon->SetEnabled(false);
		m_btnMaintain->SetEnabled(false);
		return FALSE;
	}
	int nOperatorID = _ttoi(GetEditText(0));
	STAFF_AUTHORITY staffAuthority = GetService<CLoginSvc>()->GetOperatorAllAccess(nOperatorID);
	m_btnCommon->SetEnabled(staffAuthority&COMMON_AUTHORITY);
	m_btnMaintain->SetEnabled(staffAuthority&MAINTAIN_AUTHORITY);
	if(staffAuthority!=NONE_AUTHORITY)
	{
		SetEditFocus(1);
	}else
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_UNAUTHORIZED));
		SetEditFocus(0);  
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLoginDlg::OnServiceBusy(bool busy)
{
	__super::OnServiceBusy(busy);
	if(busy)
	{
		bool commonEnable = m_btnCommon->IsEnabled();
		m_btnCommon->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,commonEnable);
		m_btnCommon->SetEnabled(false);
		bool maintainEnable = m_btnMaintain->IsEnabled();
		m_btnMaintain->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,maintainEnable);
		m_btnMaintain->SetEnabled(false);
	}
	else
	{
		bool commonEnable = true;
		m_btnCommon->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,commonEnable);
		m_btnCommon->SetEnabled(commonEnable);
		bool maintainEnable = true;
		m_btnMaintain->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,maintainEnable);
		m_btnMaintain->SetEnabled(maintainEnable);
	}
}