#include "stdafx.h"
#include "SignCardApplyCheckOfficeDlg.h"
#include "SignCardApplySvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include <stdlib.h>

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardApplyCheckOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardApplyCheckOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSevBtnClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// 编辑框位置
static CRect editCheck[] = 
{
	BASE_INFO_RECT(2,20),
};
static const char CHECK_EDIT_COUNT			= 1;			 // 编辑框的个数

static const CString STR_CHECK_REASON		= _T("CHECK_REASON");
static const CString CERTIFI_TYPE		    = _T("CERTIFICATE_TYPE");
static const CString STR_COUNT				= _T("COUNT");

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyCheckOfficeDlg::CSignCardApplyCheckOfficeDlg(CService* pService) :
COperationDlg(CSignCardApplyCheckOfficeDlg::IDD, pService)
{
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->titleInfo.titleName = TXT_SIGNCARD_APPLY_SVC_CN;								// 标题
	m_baseInfo->detailInfo.row = USER_INFO_CHECK_SIZE;											// 行数
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();			         	// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = CHECK_EDIT_COUNT;									// 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editCheck;								    	// 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyCheckOfficeDlg::~CSignCardApplyCheckOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int \n
0:成功  -1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CSignCardApplyCheckOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域，返回键，确认键
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |  BASE_AREA | BTN_OK | BTN_RESET)){
		return -1;
	}
	GetEdit(0)->SetScope(CGEdit::NUMANDCHAR);
    SetEditFocus(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		对话框初始化

@param      无

@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CSignCardApplyCheckOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// 显示BOM文言
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));

	// 初始化按键
	InitialButtons();

	//// 启用右侧按钮
	m_Left_2_Group->EnableWindow(TRUE);
	m_Left_2_Group->Invalidate();

	return TRUE;  
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		初始化按键

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplyCheckOfficeDlg::InitialButtons()
{
	// 先读出个数
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);
	int checkReasonCount = getini.GetDataInt(STR_CHECK_REASON,STR_COUNT);
	int certificateCount = getini.GetDataInt(CERTIFI_TYPE,STR_COUNT);

	// 创建数组
	pCheckReasonBtnInfo = new BTN_INFO[checkReasonCount];
	pCertificatetypeBtnInfo = new BTN_INFO[certificateCount];
	// 初始化证件类型按钮
	CString strcertifiNo[] = {_T("1"),_T("2"),_T("3"),_T("10")};
	for (int j = 0;j < certificateCount;j++){
		CString certificateType = getini.GetDataString(CERTIFI_TYPE,strcertifiNo[j]);
		certificateType.Replace(_T("\\n"),_T("\n"));
		pCertificatetypeBtnInfo[j].btnID = j+1;
		pCertificatetypeBtnInfo[j].btnMsg = strcertifiNo[j];
		pCertificatetypeBtnInfo[j].text = certificateType;
		m_vec_Left_2_BTN_INFO.push_back(pCertificatetypeBtnInfo[j]);
	}
	// 初始化业务类型按钮
	CString checkNo;
	for (int j = 0;j < checkReasonCount;j++){
		checkNo.Format(_T("%d"),j+1);
		CString checkReasonText = getini.GetDataString(STR_CHECK_REASON,checkNo);
		checkReasonText.Replace(_T("\\n"),_T("\n"));
		pCheckReasonBtnInfo[j].btnID = j+1;
		pCheckReasonBtnInfo[j].btnMsg = checkNo;
		pCheckReasonBtnInfo[j].text = checkReasonText;
		m_vec_Right_1_BTN_INFO.push_back(pCheckReasonBtnInfo[j]);
	}
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->ClickFirstAvailButton();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickButton(3);

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyCheckOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	theSERVICE_MGR.SetState(RECEPT);
	if (!ValidateInput()){
		return FALSE;
	}

	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_CHECKING));

	if (GetService<CSignCardApplySvc>()->OK() == 0)	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_CHECK_SUCCEED));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_FAIL));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyCheckOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	// 清空编辑框的信息
	SetEditText(0,_T(""));
	SetEditFocus(0);
	// 初始化成员变量
	m_Left_2_Group->ClickFirstAvailButton();
	return __super::XButtonClick(wParam,lParam);
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      编辑框信息改变时的处理

@param      无

@retval     bool true:正确; false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyCheckOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     检查输入合法性检查 

@param      无

@retval     bool TRUE:正确 FALSE:错误

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSignCardApplyCheckOfficeDlg::ValidateInput()
{
	// 输入合法性检查
	if(GetEditText(0).GetLength() == 0) {
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_INPUT_ERROR));
		SetEditFocus(0);
		return FALSE;
	}
	// 检查输入的证件ID是否有效
	const vector<BTN_INFO>& selectedBtnInfo=m_Left_2_Group->GetSelectedBtnInfo();
	if(_ttoi(selectedBtnInfo[0].btnMsg) == CERTIFICATE_ID_CARD_MAINLAND){
		if (!CheckIDCard(GetEditText(0)))
		{
			SetEditFocus(0);
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      处理证件类型按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyCheckOfficeDlg::OnCertificateTypeClick(WPARAM wParam,LPARAM lParam)
{
	// 获取按钮信息
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE certifiType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	// 设置信息
	GetService<CSignCardApplySvc>()->SetCertifiType(certifiType,btnInfo->text);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_baseInfoArea->Invalidate();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      处理服务类型按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyCheckOfficeDlg::OnSevBtnClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	// 获取按钮信息
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sevType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	// 设置信息
	pCSignCardApplySvcc->SetSevType(sevType,btnInfo->btnID);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplyCheckOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();
	try{	
		// 查询失败
		if (pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_ERROR) {
			//隐藏所有组件	
			HideCompoment( BASE_AREA | BTN_OK | BTN_RESET|KEYBOARD_AREA  );		
		}
		// 查询结束
		else if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_CHECK_OVER)
		{
			HideCompoment(BTN_RESET | BTN_OK | EDIT_BOX |KEYBOARD_AREA  );
			m_baseInfoArea->Invalidate();
		}	
		// 查询
		else if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_CHECK)
		{	
			m_vec_Right_1_BTN_INFO[2].isSelected = true;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;	
			m_Left_2_Group->ClickFirstAvailButton();
			m_baseInfoArea->Invalidate();	
		}	
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pCSignCardApplySvcc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
