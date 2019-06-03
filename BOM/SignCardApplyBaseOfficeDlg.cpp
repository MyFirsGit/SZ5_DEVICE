#include "stdafx.h"
#include "SignCardApplyBaseOfficeDlg.h"
#include "SignCardApplySvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include <stdlib.h>
#include "Util.h"
#include <afxwin.h>
#include "GEdit.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardApplyBaseOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardApplyBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeClick)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnSexClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSevBtnClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// 编辑框位置
static CRect editApply[] = 
{
	BASE_INFO_RECT(1,20),
	BASE_INFO_RECT(4,20),
	//BASE_INFO_RECT(5,3),
	BASE_INFO_RECT_SP(5,4,0),//年
	BASE_INFO_RECT_SP(5,2,8),//月
	BASE_INFO_RECT_SP(5,2,14),//日
	BASE_INFO_RECT(6,20),
	BASE_INFO_RECT(7,30),
	BASE_INFO_RECT(8,20),
	BASE_INFO_RECT(9,20),

};

static const char EDIT_COUNT				= 9;			 // 编辑框的个数
static const CString STR_CHECK_REASON		= _T("CHECK_REASON");
static const CString CERTIFI_TYPE		    = _T("CERTIFICATE_TYPE");
static const CString Sex_type		        = _T("SEX_TYPE");
static const CString STR_COUNT				= _T("COUNT");

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyBaseOfficeDlg::CSignCardApplyBaseOfficeDlg(CService* pService) :
COperationDlg(CSignCardApplyBaseOfficeDlg::IDD, pService)
{
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->titleInfo.titleName = TXT_SIGNCARD_APPLY_SVC_CN;								// 标题
	m_baseInfo->detailInfo.row = USER_INFO_SIZE;												// 行数
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();			         	// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = EDIT_COUNT;											// 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editApply;								    	// 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyBaseOfficeDlg::~CSignCardApplyBaseOfficeDlg()
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
int CSignCardApplyBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域，返回键，确认键
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |  BASE_AREA | BTN_OK | BTN_RESET)){
		return -1;
	}
    SetEditFocus(0);
	GetEdit(2)->SetLimitText(4);
	GetEdit(0)->SetLimitText(20);
	GetEdit(1)->SetScope(CGEdit::NUMANDCHAR);
	GetEdit(0)->SetScope(CGEdit::NAME);
	GetEdit(6)->SetScope(CGEdit::ALL_CHAR);
	GetEdit(7)->SetScope(CGEdit::ALL_CHAR);
	GetEdit(8)->SetScope(CGEdit::ALL_CHAR);
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
BOOL CSignCardApplyBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// 显示BOM文言
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));

	// 初始化原因按键
	InitialButtons();

	// 启用右侧按钮
	m_Left_1_Group->EnableWindow(TRUE);
	m_Left_1_Group->Invalidate();
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
void CSignCardApplyBaseOfficeDlg::InitialButtons()
{
	// 先读出个数
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	int checkReasonCount = getini.GetDataInt(STR_CHECK_REASON,STR_COUNT);
	int certificateCount = getini.GetDataInt(CERTIFI_TYPE,STR_COUNT);
	int sexCount = getini.GetDataInt(Sex_type,STR_COUNT);

	// 创建数组
	pCheckReasonBtnInfo = new BTN_INFO[checkReasonCount];
	pCertificatetypeBtnInfo = new BTN_INFO[certificateCount];
	pSexBtnInfo = new BTN_INFO[sexCount];

	// 初始化性别选项按钮
	CString strSexNo;
	for (int i = 0;i < sexCount;i++){
		strSexNo.Format(_T("%d"),i+1);
		CString sexTypeText = getini.GetDataString(Sex_type,strSexNo);
		pSexBtnInfo[i].btnID = i+1;
		pSexBtnInfo[i].btnMsg = strSexNo;
		pSexBtnInfo[i].text = sexTypeText;
		m_vec_Left_1_BTN_INFO.push_back(pSexBtnInfo[i]);
	}
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
		if(checkNo == "2") //持卡人\n信息修改
		{
			pCheckReasonBtnInfo[j].isEnable = (!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID));
		}
		m_vec_Right_1_BTN_INFO.push_back(pCheckReasonBtnInfo[j]);
	}
	m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	m_Left_1_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Left_2_Group->ClickFirstAvailButton();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Right_1_Group->ClickFirstAvailButton();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置性别按钮

@param      (i)WPARAM wParam  消息信息

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickSexButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Left_1_Group->ClickButton(index-1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       设置证件类型按钮

@param      (i)WPARAM wParam  消息信息

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickCredentialsTypeButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Left_2_Group->ClickButton(index-1);
	if (index==10){
		m_Left_2_Group->ClickButton(4);
	}
	else{
		m_Left_2_Group->ClickButton(index-1);
	}
	SetEditFocus(2);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       设置服务类型按钮

@param      (i)WPARAM wParam  消息信息

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickSevTypeButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Right_1_Group->ClickButton(index);	
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
LRESULT  CSignCardApplyBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	theSERVICE_MGR.SetState(RECEPT);
	if(!ValidateInput()){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_INPUT_ERROR));
		return FALSE;
	}
	GetService<CSignCardApplySvc>()->OK();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      编辑框信息改变时的处理

@param      无

@retval     bool true:正确; false:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	theSERVICE_MGR.SetState(RECEPT);
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
bool CSignCardApplyBaseOfficeDlg::ValidateInput()
{
	// 检查输入信息是否完整
	for(int i=0;i<2;i++){
		if(GetEditText(i) == "") {
			SetEditFocus(i);
			return FALSE;
		}
	}
	// 检查输入的证件ID是否有效
	const vector<BTN_INFO>& selectedBtnInfo=m_Left_2_Group->GetSelectedBtnInfo();
	if(_ttoi(selectedBtnInfo[0].btnMsg) == CERTIFICATE_ID_CARD_MAINLAND){
		if (!CheckIDCard(GetEditText(1)))
		{
			SetEditFocus(1);
			return false;
		}
	}
	// 检查输入的证件有效期是否有效
	if (GetEditText(3) != ""&&GetEditText(4) != ""&&GetEditText(2) != ""){
		_DATE data;
		data.wYear = _ttoi(GetEditText(2));
		data.biMonth = _ttoi(GetEditText(3));
		data.biDay = _ttoi(GetEditText(4));
		if(ComCheckDateValid(data)!=0){
			SetEditFocus(2);
			return FALSE;
		}	
		if (ComGetCurDate()>data)
		{
			SetEditFocus(2);
			return FALSE;
		}
	}
	else if(!(GetEditText(3) == ""&&GetEditText(4) == ""&&GetEditText(2) == "")){
		return false;
	}
	// EMAIL有效性检查	
	if (GetEditText(6) != ""&&!checkEmailValidate(GetEditText(6)))
	{
		SetEditFocus(6);
		return FALSE;
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
LRESULT  CSignCardApplyBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	Reset();
	return __super::XButtonClick(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重置

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::Reset()
{
	// 清空编辑框的信息
	SetEditText(0,_T(""));
	SetEditText(1,_T(""));
	SetEditText(2,_T(""));
	SetEditText(3,_T(""));
	SetEditText(4,_T(""));
	SetEditText(5,_T(""));
	SetEditText(6,_T(""));
	SetEditText(7,_T(""));
	SetEditText(8,_T(""));
	SetEditFocus(0);
	// 初始化成员变量
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->ClickFirstAvailButton();
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
LRESULT CSignCardApplyBaseOfficeDlg::OnCertificateTypeClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE certifiType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	GetService<CSignCardApplySvc>()->SetCertifiType(certifiType,btnInfo->text);
	m_baseInfoArea->Invalidate();
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      处理性别按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息 

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyBaseOfficeDlg::OnSexClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sex =  _ttoi(btnInfo->btnMsg);
	GetService<CSignCardApplySvc>()->SetSex(sex);
	m_baseInfoArea->Invalidate();
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
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
LRESULT CSignCardApplyBaseOfficeDlg::OnSevBtnClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sevType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	pCSignCardApplySvcc->SetSevType(sevType,btnInfo->btnID);
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
void CSignCardApplyBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	
	__super::OnActivate(nState,pWndOther,bMinimized);
	Reset();
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();
	try{	
		// 申请、变更基本画面
		if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_BASE&&pCSignCardApplySvcc->GetSevType()==1){
			m_Left_1_Group->ClickFirstAvailButton();
			m_Left_2_Group->ClickFirstAvailButton();	
			m_vec_Right_1_BTN_INFO[0].isSelected = true;
			m_vec_Right_1_BTN_INFO[1].isSelected = false;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;			
			m_baseInfoArea->Invalidate();
		}
		else if (pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_BASE&&pCSignCardApplySvcc->GetSevType()==2)
		{
			CCPUCard cpuCard = pCSignCardApplySvcc->GetCardInfo();
			m_vec_Right_1_BTN_INFO[1].isSelected = true;
			m_vec_Right_1_BTN_INFO[0].isSelected = false;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;	
			SetEditText(0,cpuCard.personalData.Name);
			SetEditText(1,cpuCard.personalData.CredentialsNumber);
			SetEditFocus(2);
			m_baseInfoArea->Invalidate();
		}					
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pCSignCardApplySvcc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}	
}
