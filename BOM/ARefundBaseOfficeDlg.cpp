#include "stdafx.h"
#include "ARefundBaseOfficeDlg.h"
#include "RefundApplySvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CRect editRefund[] = {
	BASE_INFO_RECT(9,6),
	BASE_INFO_RECT(10,6),
};

IMPLEMENT_DYNAMIC(CARefundBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CARefundBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRefundReasonClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnSetRefundDespoitClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CARefundBaseOfficeDlg::CARefundBaseOfficeDlg(CService* pService)
: COperationDlg(CARefundBaseOfficeDlg::IDD, pService)
{
	InitializeRefundInfo();

	// 退款信息显示区域
	m_baseInfo->titleInfo.titleName = TITLE_REFUND_INFO;								// 标题
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_10;								// 行数
	m_baseInfo->detailInfo.labelGroup = m_RefundInfo;				// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 2;											// 编辑框数目
	m_baseInfo->detailInfo.editGroup.editRect = editRefund;							    // 编辑框坐标 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CARefundBaseOfficeDlg::~CARefundBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CARefundBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA)){
			return -1;
		}
		// 操作员ID
		GetEdit(0)->SetLimitText(6);
		// 操作员密码，密码输入框中的内容显示为 '*'
		GetEdit(1)->SetPasswordChar('*');
		GetEdit(1)->SetLimitText(6);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		对话框初始化

@param      none

@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CARefundBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();
	CString strRefundMsg[] = {TXT_REFUND_REASON_PASSAGER,TXT_REFUND_REASON_SUBWAY};
	int iRefundNo[] = {REFUND_REASON_PASSAGER,REFUND_REASON_SUBWAY};
	for (int i = 0;i < 2;i++){
		BTN_INFO pRefundBtnInfo;
		pRefundBtnInfo.btnID = iRefundNo[i];
		pRefundBtnInfo.text = strRefundMsg[i];
		m_vec_Right_1_BTN_INFO.push_back(pRefundBtnInfo);
	}
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

	CString strRefundDeposit[] = {_T("不退押金"),_T("退押金")};
	for (int i = 0;i < 2;i++){
		BTN_INFO pRefundDepositBtnInfo;
		pRefundDepositBtnInfo.btnID = i;
		pRefundDepositBtnInfo.text = strRefundDeposit[i];
		m_vec_Right_2_BTN_INFO.push_back(pRefundDepositBtnInfo);
	}
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
	m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      退款产品选择按钮、返回、确定按钮消息处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n     
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CARefundBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try {
		// 退款确认
		CString msg(""),sPrice("");
		sPrice = ComFormatAmount(pRefundApplySvc->GetDataModel().refundBalance + pRefundApplySvc->GetDataModel().refundDeposit);
		msg = _T("退款金额：") + sPrice + _T("元，是否确认？");
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		// 取消
		if(confirmResult == IDCANCEL){
			return FALSE;
		}
		// 确认
		else if (confirmResult == IDOK){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING));
			theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUNDING_PASSENGER), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUNDING_PASSENGER));
			theSERVICE_MGR.SetState(ISSUEING);
			// 退款处理
			long result = pRefundApplySvc->Refund();
			if (result != 0){
				// 显示TPU错误信息
				theAPP_SESSION.ShowOfficeGuide(pRefundApplySvc->GetTpuErrorInfo(result));
				theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_ERROR_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_ERROR_PASSENGER));
				HideCompoment(BTN_OK | KEYBOARD_AREA);
				m_Right_2_Group->ShowButtons(false);
				m_Right_2_Group->ShowButtons(false);
				theSERVICE_MGR.SetState(FINISH);
			}
			else{
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_FINISH_APPLY_DLG);
			}
		}
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK | KEYBOARD_AREA);
		m_Right_2_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK | KEYBOARD_AREA);
		m_Right_2_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return __super::XButtonClick(wParam,lParam);
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
void CARefundBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{	
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_FINISH)
		{
			// 隐藏组件	
			HideCompoment(BTN_OK | KEYBOARD_AREA);
			m_Right_1_Group->ShowButtons(false);
			m_Right_2_Group->ShowButtons(false);
			theSERVICE_MGR.SetState(FINISH);
		}
		
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      退款原因按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CARefundBaseOfficeDlg::OnRefundReasonClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
		dataModel.refundReason = (RefundReasonType)btnInfo->btnID;
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      退款原因按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CARefundBaseOfficeDlg::OnSetRefundDespoitClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		pRefundApplySvc->GetDataModel().refundDeposit = btnInfo->btnID == 0 ? 0 : pRefundApplySvc->GetDataModel().ticketDeposit ;
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
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
LRESULT  CARefundBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	if(GetEditText(0).GetLength() != STAFFID_LEN && GetEditText(1).GetLength() != DEFAULT_PASSWORD_LEN)
	{
		m_btnOK->SetEnabled(false);
		return FALSE;
	}
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		int nOperatorID = _ttoi(GetEditText(0));
		if(nOperatorID==_ttoi(theAPP_SESSION.GetUserInfo().sUserID))
		{
			m_btnOK->SetEnabled(false);
		}
		else
		{
			CString sPwd = GetEditText(1);
			if(theBOM_INFO.IsFounderStaffID(nOperatorID)){
				if(theBOM_INFO.GetFounderStaffPassword() == sPwd){
					m_btnOK->SetEnabled(true);
				}
				else{
					m_btnOK->SetEnabled(false);
				}
			}
			else if(pRefundApplySvc->CheckOperatorIdAndPwd(nOperatorID,sPwd) && theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,SYS_FUNCTION_REFUND_BY_METRO)){
				m_btnOK->SetEnabled(true);
			}
			else{
				m_btnOK->SetEnabled(false);
			}
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化站员退款画面基本信息

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CARefundBaseOfficeDlg::InitializeRefundInfo()
{
	int i = 0;
	// 退款原因
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_REFUND_REASON) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 车票编号
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 车票种类
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 收据编号
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("收据编号")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 审核结果
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("审核结果")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 退款押金
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("退款押金")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 退款余额
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("退款余额")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 退款总额
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("退款总额")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 认证操作员ID
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("认证操作员ID")),MAX_SBC_CASE_LEN);
	i++;
	// 认证操作员密码
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("认证操作员密码")),MAX_SBC_CASE_LEN);
	i++;
}

void CARefundBaseOfficeDlg::UpdateUI()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{	
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_FINISH)
		{
			HideCompoment(BTN_OK|KEYBOARD_AREA);
			m_Right_1_Group->ShowButtons(false);
			m_Right_2_Group->ShowButtons(false);
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
		int i=0;
		// 退款原因
		m_RefundInfo[i].nameRight = dataModel.refundReason ==REFUND_REASON_PASSAGER ? TXT_REFUND_REASON_PASSAGER : TXT_REFUND_REASON_SUBWAY;
		i++;
		// 车票编号
		m_RefundInfo[i].nameRight = dataModel.strTicketNum;
		i++;
		// 车票种类
		m_RefundInfo[i].nameRight = dataModel.strTicketType_CN;
		i++;
		// 收据编号
		m_RefundInfo[i].nameRight = dataModel.strApplyBillNum;
		i++;
		// 审核结果
		m_RefundInfo[i].nameRight = _T("已审核");
		i++;
		// 退款押金
		m_RefundInfo[i].nameRight = ComFormatAmount(dataModel.refundDeposit) + MONEY_UNIT;
		i++;
		// 退款余额
		m_RefundInfo[i].nameRight = ComFormatAmount(dataModel.refundBalance) + MONEY_UNIT;
		i++;
		// 退款总额
		m_RefundInfo[i].nameRight = ComFormatAmount(dataModel.refundDeposit + dataModel.refundBalance) + MONEY_UNIT;
		i++;

		m_baseInfo->detailInfo.row = dataModel.refundReason ==REFUND_REASON_PASSAGER ? LABEL_GROUP_EDIT_ROWS_8 : LABEL_GROUP_EDIT_ROWS_10;
		GetEdit(0)->ShowWindow(dataModel.refundReason ==REFUND_REASON_PASSAGER ? SW_HIDE : SW_SHOW);
		GetEdit(1)->ShowWindow(dataModel.refundReason ==REFUND_REASON_PASSAGER ? SW_HIDE : SW_SHOW);
		if(dataModel.refundReason == REFUND_REASON_PASSAGER)
		{
			m_btnOK->SetEnabled(true);
		}
		else
		{
			int nOperatorID = _ttoi(GetEditText(0));
			if(nOperatorID == _ttoi(theAPP_SESSION.GetUserInfo().sUserID))
			{
				m_btnOK->SetEnabled(false);
			}
			else
			{
				CString sPwd = GetEditText(1);
				if(theBOM_INFO.IsFounderStaffID(nOperatorID)){
					if(theBOM_INFO.GetFounderStaffPassword() == sPwd){
						m_btnOK->SetEnabled(true);
					}
					else{
						m_btnOK->SetEnabled(false);
					}
				}
				else if(pRefundApplySvc->CheckOperatorIdAndPwd(nOperatorID,sPwd) && theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,SYS_FUNCTION_REFUND_BY_METRO)){
					m_btnOK->SetEnabled(true);
				}
				else{
					m_btnOK->SetEnabled(false);
				}
			}
		}
		m_baseInfoArea->Invalidate();
		this->Invalidate();

	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}