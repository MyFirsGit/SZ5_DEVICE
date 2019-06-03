#include "stdafx.h"
#include "TVM.h"
#include "WaitingOfficeDlg.h"
#include "tvmforeservice.h"
#include "ChargeSvc.h"
#include "AnalyzeSvc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWaitingOfficeDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CWaitingOfficeDlg, CReceptionDlg)
    ON_WM_ACTIVATE()
    ON_WM_CREATE()
	ON_BTN_BACK_CLICK(OnOK)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*   pService  当前的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::CWaitingOfficeDlg(CService* pService, CString strServiceName)
	: CReceptionDlg(CWaitingOfficeDlg::IDD, pService)
{
	pSvc = (CTVMForeService*)pService;
    m_strServiceName = strServiceName;
	m_cashInfo->stytle = ISSUE_INFO_STYLE;
	m_cashInfo->point = CPoint(left_number_size.cx+kongbai_x, 0);
	m_cashInfo->titleInfo.titleName = _T("充值操作");
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_cashInfo->detailInfo.row = 1;//MAX_TICKET_INFO_ROWS;

	m_cashInfo->size = CSize(1665,843);
	m_cashInfo->type = 1;
	m_bkgImage.Load(_T(".\\res\\TVM_center_charge_bkg.png"));
	m_pImageBack_center = &m_bkgImage;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::~CWaitingOfficeDlg()
{
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
int CWaitingOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(-1 ==__super::OnCreate(lpCreateStruct,CASH_AREA | GUIDE_AREA | NUMBER_AREA))
	{
		return -1;
	}	

	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
	return 0;
}

void CWaitingOfficeDlg::preShow()
{
	__super::preShow();
	m_cashInfoArea->Invalidate(FALSE);

}

void CWaitingOfficeDlg::showComplete()
{
	__super::showComplete();
	this->RedrawWindow();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWaitingOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	try {
		//theSERVICE_MGR.SetState(ISSUEING);

		// 发出读卡命令
		SERVICE_ID serviceIDCurrent = (SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID();
		//if(m_strServiceName == _T("TRUE")){
		//	RECHARGERW_HELPER->ReadCard(serviceIDCurrent,true);
		//}
		//else{
		//RECHARGERW_HELPER->ReadCard(serviceIDCurrent);
		//}
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
		WORD device_service_mode = theTVM_STATUS_MGR.GetWorkMode();
		if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) ==  DEVICE_CONTRL_CODE_BUSINESS_CARD){
			m_cashInfo->szContent = _T("请插卡…");

		}
		else
		{
			m_cashInfo->szContent = _T("本机只能充值^充值请插入储值卡");
		}
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T(" "));

		// 超时时间
		int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();

		//// 打开充值卡槽,并检查卡插入状态
		//try{
		//	// 点亮储值卡指示灯
		//	IO_HELPER->OpenBKCardIndicator();
		//	// 打开电磁铁
		//	//IO_HELPER->OpenCardLocker();// 设备故障暂时屏蔽zhengxianle
		//	// 检查卡插入状态
		//	IO_HELPER->CheckCardStandby(theSERVICE_MGR.GetCurService(),iReadTimeLongth);
		//}
		//catch(CSysException& e){
		//	theSERVICE_MGR.SetState(RECEPT);
		//	theEXCEPTION_MGR.ProcessException(e);
		//	// 提示电磁铁口打开异常，然后取消读卡，自动返回默认业务
		//	CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_ERROR_OPEN_CARD_LOCK_MSG),TVM_TIP_SHOW_WARINGS,CMessagePictureBox::BTN_CANCEL,DT_LEFT,iReadTimeLongth);
		//	NOTIFY_PROCESSING_WINDOW();
		//	IO_HELPER->AbortCheckCardStandby();
		//	return;
		//}
		//catch(...){
		//	theSERVICE_MGR.SetState(RECEPT);
		//	theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//	// 提示电磁铁口打开异常，然后取消读卡，自动返回默认业务
		//	CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_ERROR_OPEN_CARD_LOCK_MSG),TVM_TIP_SHOW_WARINGS,CMessagePictureBox::BTN_CANCEL,DT_LEFT,iReadTimeLongth);
		//	NOTIFY_PROCESSING_WINDOW();
		//	IO_HELPER->AbortCheckCardStandby();
		//	return;
		//}
		
		//// 显示等待读卡消息框
		//CString strTips;
		//if(serviceIDCurrent==CHARGE_SVC){
		//	strTips = _tl(GUIDE_IN_PUT_CARD_FIRST);//请将储值卡放入储值卡卡槽
		//}
		//else {
		//	strTips = _tl(GUIDE_IN_PUT_CARD_FIRST_EX);//请将车票放入储值卡卡槽
		//}
		//INT_PTR ret = CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),strTips,TVM_TIP_SWIPE_CARD,CMessagePictureBox::BTN_CANCEL,DT_LEFT,iReadTimeLongth);
		//IO_HELPER->CloseBKCardIndicator();
		//if (IDCANCEL == ret || IDABORT == ret){
		//	NOTIFY_PROCESSING_WINDOW();
		//	IO_HELPER->AbortCheckCardStandby();
		//	RECHARGERW_HELPER->StopReadCard();
		//}
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetState(RECEPT);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theSERVICE_MGR.SetState(RECEPT);
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
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
void CWaitingOfficeDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	if (_T("") != m_strServiceName && m_strServiceName != _T("TRUE")) {
		DrawRoundRect(pDC, SUBMENU_BTN4_RECT, LINE_BORDER, BLACKGRAY, BLACKGRAY);
		DrawText(pDC, SUBMENU_BTN4_POINT, m_strServiceName, _T(""));
	}
}
LRESULT CWaitingOfficeDlg::OnOK(WPARAM wParam, LPARAM lParam)
{
	IO_HELPER->ReturnRechargeCard();

	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();

	return TRUE;
}
void CWaitingOfficeDlg::UpdateUI()
{
	m_cashInfoArea->Invalidate(FALSE);

	int nIndex = 0;
	m_guideInfo->detailList[nIndex].strfileName = TVM_TIP_INSERT_CARD;
	m_guideInfo->detailList[nIndex++].bitInfo = _T("");
	m_guideInfo->tipsCount = nIndex;
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		m_guideInfoArea->renderGuideInfo();
	}

}

