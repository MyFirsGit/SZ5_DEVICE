#include "stdafx.h"
#include "RefundQueryOfficeDlg.h"
#include "GuideInfo.h"
#include "RefundApplySvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//退款查询信息显示编辑框坐标组合
static CRect editQuery[] = {
	BASE_INFO_RECT(1,10),
	BASE_INFO_RECT(3,20),
};

IMPLEMENT_DYNAMIC(CRefundQueryOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefundQueryOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeBtnClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundQueryOfficeDlg::CRefundQueryOfficeDlg(CService* pService) 
	: COperationDlg(CRefundQueryOfficeDlg::IDD,pService)
{
	InitQueryInfo();
	// 申退查询信息显示区域
	m_baseInfo->titleInfo.titleName = TITLE_REFUND_INFO;                    // 标题
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_3;					// 行数
	m_baseInfo->detailInfo.labelGroup = m_QueryInfo;    // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 2;                               // 编辑框数目
	m_baseInfo->detailInfo.editGroup.editRect = editQuery;                  // 编辑框坐标 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundQueryOfficeDlg::~CRefundQueryOfficeDlg()
{
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
int CRefundQueryOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA)){
			return -1;
		}
		// 设置编辑框属性
		GetEdit(0)->SetLimitText(9);
		GetEdit(0)->SetScope(CGEdit::DIGIT_CHAR);
		GetEdit(1)->SetLimitText(20);
		GetEdit(1)->SetScope(CGEdit::NUMANDCHAR);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

void CRefundQueryOfficeDlg::InitQueryInfo()
{
	// 车票编号
	m_QueryInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 证件类型
	m_QueryInfo[1].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 证件编号
	m_QueryInfo[2].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
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
LRESULT CRefundQueryOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try {
		// 合法性检查
		if (!CheckInputStatus()){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_INPUT_ERROR_ECT));
			return FALSE;
		}
		//申退查询信息确认
		CString msg(_T("请检查申退查询信息，是否确认？"));
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(confirmResult == IDCANCEL){
			return FALSE;
		}
		else if (confirmResult == IDOK){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING_QUERY));
			theSERVICE_MGR.SetState(ISSUEING);
			CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
			dataModel.strTicketNum = GetEditText(0);
			dataModel.strCertificateNumber = GetEditText(1);
			// 查询申请状态
			BYTE status = pRefundApplySvc->QueryRefundApplyResult();
			if(status == 0xFF){
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
				// 显示错误信息
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_QUERY_ERROR));
			}
			// 待审核转到结束界面
			else if(status == 0)
			{
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
			}
			// 审核通过转到退款界面
			else if(status == 1){
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_BASE_APPLY_DLG);
			}
		}
	}
	catch (CSysException& e){
		pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		CInnerException e = CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
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
void CRefundQueryOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_QUERY)
		{
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
			m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
			m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
			m_Left_2_Group->ClickFirstAvailButton();
		}
		else if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_QUERY_FINISH)
		{
			HideCompoment(BTN_OK|BASE_AREA|KEYBOARD_AREA);
			m_Left_2_Group->ShowButtons(false);
		}
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
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
LRESULT CRefundQueryOfficeDlg::OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam)
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

void CRefundQueryOfficeDlg::UpdateUI()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
		switch(dataModel.certificateType)
		{
		case CERTIFICATE_TYPE::CERTIFICATE_ID_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_ID_CARD_MAINLAND;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_STUDENT_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_STUDENT;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_MILITARY_OFFICER:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_MILITARY;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_POST_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_FOREIGNER;
			break;
		default:
			m_QueryInfo[1].nameRight = TXT_UNKNOW_STATUS;
			break;
		}
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检查输入信息

@param      无

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundQueryOfficeDlg::CheckInputStatus()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();

	if(GetEditText(0).IsEmpty() && GetEditText(1).IsEmpty())
	{
		SetEditFocus(0);
		return false;
	}
	if(!GetEditText(1).IsEmpty())
	{
		if (!CheckIDCard(GetEditText(1)))
		{
			SetEditFocus(1);
			return false;
		}
	}
	return true;
}
