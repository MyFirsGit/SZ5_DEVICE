#include "stdafx.h"
#include "PassengerResultDlg.h"
#include "ChargeSvc.h"
#include "tickethelper.h"
#include "GuideInfo.h"


IMPLEMENT_DYNAMIC(CPassengerResultDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CPassengerResultDlg, CReceptionDlg)
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
CPassengerResultDlg::CPassengerResultDlg(CService* pService) : CReceptionDlg(CPassengerResultDlg::IDD, pService)
{
	pSvc = dynamic_cast<CPassengerForeSvc*>(pService);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPassengerResultDlg::~CPassengerResultDlg()
{
	if (m_guideInfoArea!=NULL){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化提示区域信息

@param      (i)long lComponents  自定义组件信息

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerResultDlg::ShowGuideArea()
{
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		//注意作用域，否则只有部分界面刷新。
		//m_guideInfoArea->Invalidate(FALSE);	
		m_guideInfoArea->renderGuideInfo();
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
void CPassengerResultDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();

	DWORD textSize = theGUI_INFO.GetRecTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	CString textFontName = theGUI_INFO.GetRecTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());
	// 详细信息
	CString strText;
	CString strMoneyReturn = _tl(TXT_ALL_THE_INSERTED_CASH);
	strMoneyReturn.Replace(_T("999"),ComFormatAmount(model.paymentInfo.lPaidAmount)); // 金额替换
	strMoneyReturn.Replace(_T("MOP"),_tl(MONEY_UNIT));
	strText.Format(_T("%s %s %s %s"),_tl(TXT_ALL_THE_INSERTED_CASH),ComFormatAmount(model.paymentInfo.lPaidAmount),_tl(MONEY_UNIT),_tl(TXT_WILL_BE_RETURNED));

	bool bPrintFalied = model.operationInfo.failurePrintState == CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
	// 1.异常画面
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		CRect m_error_msg_rc = CRect(50,280,650,380);
		DrawText(pDC, m_error_msg_rc,_tl(TXT_TRANSACTION_FAILED), textFontName,textSize + 30,FW_BOLD, BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		CRect m_error_dealInfo_rc = CRect(50,400,650,500);
		DrawText(pDC, m_error_dealInfo_rc,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER), textFontName,textSize,FW_BOLD, BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 2.取消画面
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		// 2.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_CANCELLED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER);
		// 2.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 2.3提示信息
		CString strCol = _tl(TXT_COLLECT_YOUR_CASH);
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,strCol/*_tl(TXT_COLLECT_YOUR_CASH)*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 3.投现金金额超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		// 3.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 3.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 3.3提示信息
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 4.投现金数量超限画面(澳门不分数量和金额超限，提示和上面一样)
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
		// 4.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 4.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 4.3提示信息
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 5.找零数量超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		// 5.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_CHANGE_EXCEEDS_THE_MAX_COUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 5.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 5.3提示信息
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 6.找零不足
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		// 6.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_SORRY_NOT_ENOUGH_CHANGE),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.3提示信息
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 7.充值失败退钱画面
	if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
		//暂无
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 9.2凭条
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(TXT_GUIDE_GET_MONEY_AND_CONTECT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 8.投钱超时退钱画面
	if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
		// 6.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_CANCELLED_TIME_OUT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.2详细信息
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.3提示信息
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 9.票卡不足，无法售票
	if (model.operationInfo.finishType ==  CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
		// 9.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER);
		// 9.2凭条
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER/*TXT_GUIDE_GET_MONEY_AND_CONTECT*/),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 10.交易失败。
	if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
		// 10.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 10.2凭条
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER | DT_WORDBREAK);
	}
	// 11.找零失败
	if(model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
		// 11.1原因
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 11.2凭条
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER | DT_WORDBREAK);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
			0:创建成功    -1:创建失败

@exception  无
*/
////////////////////////////////////////////////////////////////////////
int CPassengerResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea->createWnd(this);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      服务数据模型发生变化时的更新界面   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerResultDlg::UpdateUI()
{
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();

	// 异常画面
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		m_guideInfo->tipsCount = 2;
		// 异常显示
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}
	// 取消画面
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 投现金金额超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 投现金数量超限画面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 找零数量超限画面或者找零不足界面
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT ||
		model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 充值失败退钱画面
	if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL||
		model.operationInfo.finishType == CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 投钱超时退钱画面
	if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 交易失败
	if(model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// 找零失败
	if(model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
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
void CPassengerResultDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	m_btnOK->SetSelect(false);
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();
	if(model.operationInfo.banknoteAcceptState != CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_INSERTED){
		// 乘客非正常业务结束界面，应设置状态为完成状态
		theSERVICE_MGR.SetState(FINISH);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	}	
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
LRESULT CPassengerResultDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	// 返回按钮，启动默认Service
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();
	return TRUE;
}
