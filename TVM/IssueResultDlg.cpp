#include "stdafx.h"
#include "IssueResultDlg.h"
#include "IssueSvc.h"
#include "tickethelper.h"
#include "GuideInfo.h"


IMPLEMENT_DYNAMIC(CIssueResultDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueResultDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::CIssueResultDlg(CService* pService) : CReceptionDlg(CIssueResultDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::~CIssueResultDlg()
{
	if (m_guideInfoArea!=NULL){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化提示区域信息

@param      (i)long lComponents  自定义组件信息

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::ShowGuideArea()
{
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		m_guideInfoArea->Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	CIssueSvc::Model& model = pSvc->GetDataModel();
	DWORD textSize = theGUI_INFO.GetRecTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	CString textFontName = theGUI_INFO.GetRecTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());
	// 异常画面
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		CRect m_error_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_error_msg_rc,_T("交易失败"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_error_dealInfo_rc = CRect(50,400,450,500);
		DrawText(pDC, m_error_dealInfo_rc,_T("请收取错误凭证后，联系操作员"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
	// 取消画面
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		CRect m_cancle_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_cancle_msg_rc,_T("交易取消"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_msg_rc = CRect(50,400,250,500);
		DrawText(pDC, m_refund_msg_rc,_T("退款金额："), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_amount_rc = CRect(280,400,450,500);
		DrawText(pDC, m_refund_amount_rc,ComFormatAmount(0)+_tl(MONEY_UNIT), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_dealInfo_rc = CRect(50,600,450,500);
		DrawText(pDC, m_refund_dealInfo_rc,_T("请收好您的现金"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
	// 找零超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		CRect m_cancle_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_cancle_msg_rc,_T("金额超限"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_msg_rc = CRect(50,400,250,500);
		DrawText(pDC, m_refund_msg_rc,_T("退款金额："), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_amount_rc = CRect(280,400,450,500);
		DrawText(pDC, m_refund_amount_rc,ComFormatAmount(0)+_tl(MONEY_UNIT), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_dealInfo_rc = CRect(50,600,450,500);
		DrawText(pDC, m_refund_dealInfo_rc,_T("请收好您的现金"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面创建函数
//
//@param      (i)LPCREATESTRUCT lpCreateStruct
//
//@retval     int    \n
//			0:创建成功    -1:创建失败
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
int CIssueResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea ->createWnd(this);

	theSERVICE_MGR.SetState(RECEPT);
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      服务数据模型发生变化时的更新界面   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::UpdateUI()
{
	CIssueSvc::Model& model = pSvc->GetDataModel();

	// 异常画面
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}
	// 取消画面
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}

	// 找零超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}

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
void CIssueResultDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_btnOK->SetSelect(false);

	/*theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);*/
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理取消按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueResultDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	pSvc->OnCancel();
	return 0;
}