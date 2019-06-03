#include "stdafx.h"
#include "RefundApplySvc.h"
#include "RefundApplyOfficeDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CRect editApply[] = {
	BASE_INFO_RECT(4,10),
	BASE_INFO_RECT(6,20),
	BASE_INFO_RECT(9,20),
	BASE_INFO_RECT(10,10),
};

const int IDX_EDIT_TICKET_NUMBER = 0;
const int IDX_EDIT_USER_NAME = 1;
const int IDX_EDIT_CERTIFICATE_NUMBER = 2;
const int IDX_EDIT_SUSPEND_NUMBER = 3;

static const char* const TXT_BTN_GOOD_CARD_APPLY	= "好卡申退";
static const char* const TXT_BTN_BAD_CARD_APPLY		= "坏卡申退";
static const char* const TXT_BTN_SUSPEND_CARD_APPLY	= "挂失申退";
static const char* const WM_BTN_GOOD_CARD_APPLY		= "goodcardapply";
static const char* const WM_BTN_BAD_CARD_APPLY		= "badcardapply";
static const char* const WM_BTN_SUSPEND_CARD_APPLY	= "suspendcardapply";



IMPLEMENT_DYNAMIC(CRefundApplyOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefundApplyOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_MESSAGE(WM_XBUTTON,XButtonClick)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnSexTypeBtnClick)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRefundReasonBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnApplyReasonBtnClick)
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
CRefundApplyOfficeDlg::CRefundApplyOfficeDlg(CService* pService) 
	: COperationDlg(CRefundApplyOfficeDlg::IDD,pService)
{
	InitializeApplyInfo();
	// 申退信息显示区域
	m_baseInfo->titleInfo.titleName = TITLE_REFUND_INFO;							// 标题
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_10;							// 行数
	m_baseInfo->detailInfo.labelGroup = m_ApplyInfo;			// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 4;										// 编辑框数目
	m_baseInfo->detailInfo.editGroup.editRect = editApply;							// 编辑框坐标


	UIINFO badCardApplyButtonUI;
	badCardApplyButtonUI.m_pParentWnd = this;
	badCardApplyButtonUI.m_Location.y = BTN_OK_RECT.top;
	badCardApplyButtonUI.m_Location.x = 110;
	badCardApplyButtonUI.m_Size = CSize(80,50);

	BTN_INFO badCardApplyButtonInfo;
	badCardApplyButtonInfo.btnMsg = WM_BTN_BAD_CARD_APPLY;
	badCardApplyButtonInfo.isEnable = true;
	badCardApplyButtonInfo.btnID = 0;
	badCardApplyButtonInfo.text = TXT_BTN_BAD_CARD_APPLY;

	m_btnBadCardApply = new CXButton(badCardApplyButtonUI,badCardApplyButtonInfo);


	UIINFO suspendCardApplyButtonUI;
	suspendCardApplyButtonUI.m_pParentWnd = this;
	suspendCardApplyButtonUI.m_Location.y = BTN_OK_RECT.top;
	suspendCardApplyButtonUI.m_Location.x = 210;
	suspendCardApplyButtonUI.m_Size = CSize(80,50);

	BTN_INFO suspendCardApplyButtonInfo;
	suspendCardApplyButtonInfo.btnMsg = WM_BTN_SUSPEND_CARD_APPLY;
	suspendCardApplyButtonInfo.isEnable = true;
	suspendCardApplyButtonInfo.btnID = 0;
	suspendCardApplyButtonInfo.text = TXT_BTN_SUSPEND_CARD_APPLY;

	m_btnSuspendCardApply = new CXButton(suspendCardApplyButtonUI,suspendCardApplyButtonInfo);
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化站员申请画面基本信息

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyOfficeDlg::InitializeApplyInfo()
{
	int i = 0;
	// 申请类型
	m_ApplyInfo[i].nameLeft = add_wsp(CString(_T("申请类型")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 退款原因
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_REFUND_REASON) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 申请原因
	m_ApplyInfo[i].nameLeft = add_wsp(CString(_T("申请原因")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 车票编号
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 车票种类
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 姓名
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_USER_NAME) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 性别
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_USER_SEX) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 证件类型
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 证件编号
	m_ApplyInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 挂失单据编号
	m_ApplyInfo[i].nameLeft = add_wsp(CString(_T("挂失单据编号")) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundApplyOfficeDlg::~CRefundApplyOfficeDlg()
{

	delete m_btnBadCardApply;
	m_btnBadCardApply = NULL;

	delete m_btnSuspendCardApply;
	m_btnSuspendCardApply = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
int CRefundApplyOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA)){
			return -1;
		}
		//m_btnGoodCardApply->Create();
		m_btnBadCardApply->Create();
		m_btnSuspendCardApply->Create();
		// 设置编辑框属性
		GetEdit(IDX_EDIT_TICKET_NUMBER)->SetLimitText(9);
		GetEdit(IDX_EDIT_TICKET_NUMBER)->SetScope(CGEdit::DIGIT_CHAR);
		GetEdit(IDX_EDIT_USER_NAME)->SetLimitText(20);
		GetEdit(IDX_EDIT_USER_NAME)->SetScope(CGEdit::NAME);
		GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->SetLimitText(20);
		GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->SetScope(CGEdit::NUMANDCHAR);
		// 挂失单据号
		GetEdit(IDX_EDIT_SUSPEND_NUMBER)->SetLimitText(10);
		GetEdit(IDX_EDIT_SUSPEND_NUMBER)->SetScope(CGEdit::DIGIT_CHAR);
		GetEdit(IDX_EDIT_SUSPEND_NUMBER)->EnableWindow(FALSE);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理ok按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundApplyOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try {
		// 合法性检查
		if (!CheckInputStatus()){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_INPUT_ERROR_ECT));
			return FALSE;
		}
		// 退款申请信息确认
		CString msg(_T("请检查退款申请信息，是否确认？"));
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(confirmResult == IDCANCEL){
			return FALSE;
		}
		else if (confirmResult == IDOK){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING_APPLY));
			theSERVICE_MGR.SetState(ISSUEING);
			CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
			dataModel.strTicketNum = GetEditText(IDX_EDIT_TICKET_NUMBER);
			dataModel.strUserName = GetEditText(IDX_EDIT_USER_NAME);
			dataModel.strCertificateNumber = GetEditText(IDX_EDIT_CERTIFICATE_NUMBER);
			dataModel.strSuspendBillNum = GetEditText(IDX_EDIT_SUSPEND_NUMBER);
			// 退款申请处理
			long result = pRefundApplySvc->RefundApply();
			if (result != SP_SUCCESS){
				// 显示错误信息
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_APPLY_ERROR));
				HideGUIModule(BTN_OK | EDIT_BOX | KEYBOARD_AREA);
				theSERVICE_MGR.SetState(FINISH);
			}
			else{

				if(dataModel.applyType == APPLY_TYPE_GOOD_CARD)
				{
					// 查询申请状态
					BYTE status = pRefundApplySvc->QueryRefundApplyResult();
					// 待审核转到结束界面
					if(status == 0 || status == 0xFF){
						pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_APPLY_FINISH_DLG);
						pRefundApplySvc->PrinterPrint(true);

					}
					// 审核通过转到退款界面
					else if(status == 1){
						pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_BASE_APPLY_DLG);
					}
				}
				else
				{
					pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_APPLY_FINISH_DLG);
					pRefundApplySvc->PrinterPrint(true);
				}

			}
		}
	}
	catch (CSysException e) {
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | EDIT_BOX | KEYBOARD_AREA);
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
void CRefundApplyOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_APPLY)
	{
		// 初始化性别按钮
		CString strSexMsg[] = {MALE,FEMALE};
		int strSexNo[] = {SEX_MALE,SEX_FEMALE};
		for (int i = 0;i < 2;i++){
			BTN_INFO pSexBtnInfo;
			pSexBtnInfo.btnID = strSexNo[i];
			pSexBtnInfo.text = strSexMsg[i];
			m_vec_Left_1_BTN_INFO.push_back(pSexBtnInfo);
		}
		m_Left_1_Group->ShowButtons(false);
		m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
		m_Left_1_Group->SetSelectType(CButtonGroup::SIGLE);
		m_Left_1_Group->ClickFirstAvailButton();

		// 初始化证件类型按钮
		CString strCertificateMsg[] = {TXT_CERTIFICATE_ID_CARD_MAINLAND,TXT_CERTIFICATE_STUDENT,TXT_CERTIFICATE_MILITARY,TXT_CERTIFICATE_FOREIGNER};
		int strCertificateNo[] = {CERTIFICATE_ID_CARD_MAINLAND,CERTIFICATE_STUDENT_CARD,CERTIFICATE_MILITARY_OFFICER,CERTIFICATE_POST_CARD};
		for (int i = 0;i < 4;i++){
			BTN_INFO pCertificateBtnInfo;
			pCertificateBtnInfo.btnID = strCertificateNo[i];
			pCertificateBtnInfo.text = strCertificateMsg[i];
			if(pCertificateBtnInfo.text.GetLength()>16)
			{
				pCertificateBtnInfo.text.Insert(12,'\n');
			}
			m_vec_Left_2_BTN_INFO.push_back(pCertificateBtnInfo);
		}
		m_Left_2_Group->ShowButtons(false);
		m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
		m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
		m_Left_2_Group->ClickFirstAvailButton();

		// 初始化退款原因按钮
		CString strRefundMsg[] = {TXT_REFUND_REASON_PASSAGER,TXT_REFUND_REASON_SUBWAY};
		int strRefundNo[] = {REFUND_REASON_PASSAGER,REFUND_REASON_SUBWAY};
		for (int i = 0;i < 2;i++){
			BTN_INFO pRefundBtnInfo;
			pRefundBtnInfo.btnID = strRefundNo[i];
			pRefundBtnInfo.text = strRefundMsg[i];
			m_vec_Right_1_BTN_INFO.push_back(pRefundBtnInfo);
		}
		//m_Right_1_Group->ShowButtons(false);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
		m_Right_1_Group->ClickFirstAvailButton();

		// 初始化申请原因按钮
		CString strApplyMsg[] = {TXT_APPLY_REASON_BAD_CARD,TXT_APPLY_REASON_LARGE_CARD,TXT_APPLY_REASON_OTHER};
		int strApplyNo[] = {APPLY_REASON_BAD_CARD,APPLY_REASON_LARGE_MONEY,APPLY_REASON_OTHER};
		for (int i = 0;i < 3;i++){
			BTN_INFO pApplyBtnInfo;
			pApplyBtnInfo.btnID = strApplyNo[i];
			pApplyBtnInfo.text = strApplyMsg[i];
			m_vec_Right_2_BTN_INFO.push_back(pApplyBtnInfo);
		}
		//m_Right_2_Group->ShowButtons(false);
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
		m_Right_2_Group->ClickFirstAvailButton();
	}
}


void CRefundApplyOfficeDlg::UpdateUI()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();

	// 不能退款
	if (pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_APPLY_NOT){	
		HideGUIModule(BTN_OK | BASE_AREA | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetState(FINISH);
		return;
	}
	// 退款申请结束
	if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_APPLY_FINISH){
		HideGUIModule(BTN_OK | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetState(FINISH);
		return;
	}
	CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
	switch(dataModel.applyType)
	{
	case  APPLY_TYPE_GOOD_CARD:
		m_ApplyInfo[0].nameRight = TXT_BTN_GOOD_CARD_APPLY;
		break;
	case APPLY_TYPE_BAD_CARD:
		m_ApplyInfo[0].nameRight = TXT_BTN_BAD_CARD_APPLY;
		break;
	case APPLY_TYPE_SUSPEND:
		m_ApplyInfo[0].nameRight = TXT_BTN_SUSPEND_CARD_APPLY;
		break;
	default:
		m_ApplyInfo[0].nameRight = TXT_UNKNOW_STATUS;
	}
	
	// 退款原因
	m_ApplyInfo[1].nameRight = dataModel.refundReason == REFUND_REASON_PASSAGER ? TXT_REFUND_REASON_PASSAGER : TXT_REFUND_REASON_SUBWAY;
	
	switch(dataModel.applyReason)
	{
	case APPLY_REASON_BAD_CARD:
		m_ApplyInfo[2].nameRight = TXT_APPLY_REASON_BAD_CARD;
		break;
	case APPLY_REASON_LARGE_MONEY:
		m_ApplyInfo[2].nameRight = TXT_APPLY_REASON_LARGE_CARD;
		break;
	case APPLY_REASON_OTHER:
		m_ApplyInfo[2].nameRight = TXT_APPLY_REASON_OTHER;
		break;
	default:
		m_ApplyInfo[2].nameRight = TXT_UNKNOW_STATUS;
	}

	//// 车票编号
	SetEditText(IDX_EDIT_TICKET_NUMBER,dataModel.strTicketNum);
	//i++;
	// 车票种类
	m_ApplyInfo[4].nameRight = dataModel.strTicketType_CN;
	// 姓名
	SetEditText(1,dataModel.strUserName);
	// 性别
	m_ApplyInfo[6].nameRight = dataModel.sexType == Sex_t::SEX_MALE ? MALE : FEMALE;
	// 证件类型
	switch(dataModel.certificateType)
	{
	case CERTIFICATE_TYPE::CERTIFICATE_ID_CARD:
		m_ApplyInfo[7].nameRight = TXT_CERTIFICATE_ID_CARD_MAINLAND;
		break;
	case CERTIFICATE_TYPE::CERTIFICATE_STUDENT_CARD:
		m_ApplyInfo[7].nameRight = TXT_CERTIFICATE_STUDENT;
		break;
	case CERTIFICATE_TYPE::CERTIFICATE_MILITARY_OFFICER:
		m_ApplyInfo[7].nameRight = TXT_CERTIFICATE_MILITARY;
		break;
	case CERTIFICATE_TYPE::CERTIFICATE_POST_CARD:
		m_ApplyInfo[7].nameRight = TXT_CERTIFICATE_FOREIGNER;
		break;
	default:
		m_ApplyInfo[7].nameRight = TXT_UNKNOW_STATUS;
		break;
	}
	// 证件编号
	SetEditText(IDX_EDIT_CERTIFICATE_NUMBER,dataModel.strCertificateNumber);

	// 挂失单据编号
	SetEditText(IDX_EDIT_SUSPEND_NUMBER,dataModel.strSuspendBillNum);

	switch(dataModel.applyType)
	{
	case  APPLY_TYPE_GOOD_CARD:
		if(dataModel.strUserName.IsEmpty() && (dataModel.certificateType == CERTIFICATE_UNSPECIFIED || dataModel.strCertificateNumber.IsEmpty()))
		{	
			m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_5;
			GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->ShowWindow(SW_HIDE);//证件号
			GetEdit(IDX_EDIT_USER_NAME)->ShowWindow(SW_HIDE);//姓名
		}
		else
		{
			GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->ShowWindow(SW_SHOW);//证件号
			GetEdit(IDX_EDIT_USER_NAME)->ShowWindow(SW_SHOW);//姓名
			m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_9;
		}
		break;
	case APPLY_TYPE_BAD_CARD:
		m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_9;
		GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->ShowWindow(SW_SHOW);//证件号
		GetEdit(IDX_EDIT_USER_NAME)->ShowWindow(SW_SHOW);//姓名
		break;
	case APPLY_TYPE_SUSPEND:
		m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_10;
		GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->ShowWindow(SW_SHOW);//证件号
		GetEdit(IDX_EDIT_USER_NAME)->ShowWindow(SW_SHOW);//姓名
		break;
	}
	GetEdit(IDX_EDIT_SUSPEND_NUMBER)->ShowWindow(dataModel.applyType == APPLY_TYPE_SUSPEND ? SW_SHOW : SW_HIDE);// 挂失单据号
	GetEdit(IDX_EDIT_SUSPEND_NUMBER)->SetEnabled(dataModel.applyType == APPLY_TYPE_SUSPEND);// 挂失单据号
	m_Left_1_Group->ShowButtons(dataModel.applyType != APPLY_TYPE_GOOD_CARD);
	m_Left_2_Group->ShowButtons(dataModel.applyType != APPLY_TYPE_GOOD_CARD);
	GetEdit(IDX_EDIT_TICKET_NUMBER)->SetEnabled(dataModel.applyType!=APPLY_TYPE_GOOD_CARD);
	GetEdit(IDX_EDIT_TICKET_NUMBER)->ShowWindow(dataModel.applyType==APPLY_TYPE_SUSPEND ? SW_HIDE:SW_SHOW);

	GetEdit(IDX_EDIT_USER_NAME)->SetEnabled(dataModel.applyType!=APPLY_TYPE_GOOD_CARD);
	GetEdit(IDX_EDIT_CERTIFICATE_NUMBER)->SetEnabled(dataModel.applyType!=APPLY_TYPE_GOOD_CARD);
	m_btnBadCardApply->SetVisible(dataModel.applyType!=APPLY_TYPE_GOOD_CARD); // 坏卡申请
	m_btnSuspendCardApply->SetVisible(dataModel.applyType!=APPLY_TYPE_GOOD_CARD);//挂失退卡
	m_btnBadCardApply->SetSelect(dataModel.applyType == APPLY_TYPE_BAD_CARD);
	m_btnSuspendCardApply->SetSelect(dataModel.applyType == APPLY_TYPE_SUSPEND);
	this->m_baseInfoArea->Invalidate();
	this->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**8
@brief      处理按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundApplyOfficeDlg::XButtonClick(WPARAM wParam, LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
	try {
		CString sMsg = (LPCTSTR)lParam;
		if(sMsg == WM_BTN_BAD_CARD_APPLY)
		{
			dataModel.applyType = APPLY_TYPE_BAD_CARD;
		}
		else if(sMsg == WM_BTN_SUSPEND_CARD_APPLY)
		{
			dataModel.applyType = APPLY_TYPE_SUSPEND;
		}
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      性别按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundApplyOfficeDlg::OnSexTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		dataModel.sexType = (Sex_t)btnInfo->btnID;
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
@brief      证件类型按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundApplyOfficeDlg::OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		dataModel.certificateType = (CERTIFICATE_TYPE)btnInfo->btnID;
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
LRESULT CRefundApplyOfficeDlg::OnRefundReasonBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		dataModel.refundReason = (RefundReasonType)btnInfo->btnID;
		dataModel.strCertificateNumber = "";
		dataModel.strSuspendBillNum = "";
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
@brief      申请原因按钮

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundApplyOfficeDlg::OnApplyReasonBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		dataModel.applyReason = (RefundApplyReasonType)btnInfo->btnID;
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

LRESULT CRefundApplyOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();
	CGEdit* currentEdit = (CGEdit*)lParam;
	if(currentEdit == GetEdit(IDX_EDIT_TICKET_NUMBER))
	{
		dataModel.strTicketNum = GetEditText(IDX_EDIT_TICKET_NUMBER);
	}
	else if(currentEdit == GetEdit(IDX_EDIT_USER_NAME))
	{
		dataModel.strUserName = GetEditText(IDX_EDIT_USER_NAME);
	}
	else if(currentEdit == GetEdit(IDX_EDIT_CERTIFICATE_NUMBER))
	{
		dataModel.strCertificateNumber = GetEditText(IDX_EDIT_CERTIFICATE_NUMBER);
	}
	else if(currentEdit == GetEdit(IDX_EDIT_SUSPEND_NUMBER))
	{
		dataModel.strSuspendBillNum = GetEditText(IDX_EDIT_SUSPEND_NUMBER);
	}
	return TRUE;	
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      按钮状态检查

@param      none

@retval     bool	true：成功	false：失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundApplyOfficeDlg::CheckInputStatus()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();
	
	if(dataModel.applyType == APPLY_TYPE_GOOD_CARD)
	{
		return true;
	}
	if(dataModel.applyType == APPLY_TYPE_BAD_CARD)
	{
		if(GetEditText(IDX_EDIT_TICKET_NUMBER).IsEmpty())
		{
			SetEditFocus(IDX_EDIT_TICKET_NUMBER);
			return false;
		}
		if(!GetEditText(IDX_EDIT_CERTIFICATE_NUMBER).IsEmpty())
		{
			if (dataModel.certificateType == CERTIFICATE_ID_CARD_MAINLAND && !CheckIDCard(GetEditText(IDX_EDIT_CERTIFICATE_NUMBER)))
			{
				SetEditFocus(IDX_EDIT_CERTIFICATE_NUMBER);
				return false;
			}
		}
		return true;
	}
	if(dataModel.applyType == APPLY_TYPE_SUSPEND)
	{
		if (dataModel.certificateType == CERTIFICATE_ID_CARD_MAINLAND && !CheckIDCard(GetEditText(IDX_EDIT_CERTIFICATE_NUMBER)))
		{
			SetEditFocus(IDX_EDIT_CERTIFICATE_NUMBER);
			return false;
		}
		if(GetEditText(IDX_EDIT_CERTIFICATE_NUMBER).IsEmpty())
		{
			SetEditFocus(IDX_EDIT_CERTIFICATE_NUMBER);
			return false;
		}
		if(GetEditText(IDX_EDIT_SUSPEND_NUMBER).IsEmpty()){
			SetEditFocus(IDX_EDIT_SUSPEND_NUMBER);
			return false;
		}
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏界面组件

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplyOfficeDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// 设置标签文字
	m_baseInfo->detailInfo.labelGroup[3].nameRight = GetEditText(0);
	m_baseInfo->detailInfo.labelGroup[5].nameRight = GetEditText(1);
	m_baseInfo->detailInfo.labelGroup[8].nameRight = GetEditText(2);
	m_baseInfo->detailInfo.labelGroup[9].nameRight = GetEditText(3);
	// 隐藏组件
	HideCompoment(ulCompomentFlag);
	// 隐藏自定义按钮
//	m_btnGoodCardApply->ShowWindow(SW_HIDE);
	m_btnBadCardApply->ShowWindow(SW_HIDE);
	m_btnSuspendCardApply->ShowWindow(SW_HIDE);
	m_Left_1_Group->ShowButtons(false);
	m_Left_2_Group->ShowButtons(false);
	m_Right_1_Group->ShowButtons(false);
	m_Right_2_Group->ShowButtons(false);

}
