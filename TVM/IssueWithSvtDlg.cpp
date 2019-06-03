#include "stdafx.h"
#include "IssueWithSvtDlg.h"
#include "BitmapManager.h"
#include "ParseStationMap.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CIssueWithSvtDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueWithSvtDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()

	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnTicketNumClick)	// 购票数量按钮

	ON_BTN_OK_CLICK(OnOK)										// 点击取消按钮
	ON_BTN_OkAndCancel1_CLICK(OnOkAndCancel1)					// 点击确定按钮(确定与取消按钮中的确定按钮）
	ON_BTN_OkAndCancel2_CLICK(OnOkAndCancel2)					// 点击取消按钮(确定与取消按钮中的取消按钮）

	ON_MESSAGE(WM_NOTIFY_START_CIRCLE_READ, OnNotifyStartReadCard)	
	ON_MESSAGE(WM_START_ISSUING, OnStartIssue)		
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueWithSvtDlg::CIssueWithSvtDlg(CService* pService)
: CReceptionDlg(CIssueWithSvtDlg::IDD, pService)
{
	m_pTipsDlg = new CStatic;
	m_pTitleDlg = new CStatic;

	pSvc = (CIssueSvc*)pService;

	// 左侧线路选择按钮
	m_BTN_Left_1_Group_Info.rows = 12;
	m_BTN_Left_1_Group_Info.cols = 1;

	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// 右侧票价按钮
	m_BTN_Right_1_Group_Info.rows = 2;
	m_BTN_Right_1_Group_Info.cols = 4;

	m_BTN_Right_2_Group_Info.rows = 2;
	m_BTN_Right_2_Group_Info.cols = 4;

	m_BTN_Right_1_Group_Info.title = _T("");
	m_BTN_Right_2_Group_Info.title = _T("");


	m_baseInfo->stytle = ISSUE_INFO_STYLE;
	m_baseInfo->point = CPoint(line_map_posX, BASEINFO_AREA_POINT.y + 110);
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->detailInfo.labelGroup = m_TipsInfoGroup;
	m_baseInfo->detailInfo.row = 14;

	m_cashInfo->stytle = ISSUE_INFO_STYLE;
	m_cashInfo->point = CPoint(line_map_posX, 565/*CASHINFO_AREA_POINT.y*/);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SVT_INFORMATION);
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_cashInfo->detailInfo.row = 4;

	m_dwMaxTicketAmount = theFunction_INFO.GetTicketMaxAmount();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueWithSvtDlg::~CIssueWithSvtDlg()
{

}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:创建成功    -1:创建失败

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
int CIssueWithSvtDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, LINE_AREA | PRICES_AREA | NUMBER_AREA | TOP_STATION_AREA | FUNC_AREA /*|BASE_AREA*/ | CASH_AREA | BTN_OK | BTN_OK_AND_CANCEL1 | BTN_OK_AND_CANCEL2) == -1) {
		return -1;
	}

	m_pTipsDlg->Create(_T(""), WS_CHILD, CRect(1140+50,850+30,1350+50,890+30), this, ID_TIPS_DLG);
	m_pTitleDlg->Create(_T(""), WS_CHILD, CRect(690,40,1000,100), this, ID_TITLE_DLG);

	m_strNotifyInfo = _T("");
	m_strTipInfo = _T("");

	// 显示信息界面


	return 0;
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
void CIssueWithSvtDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		theSERVICE_MGR.SetState(RECEPT);
		m_strNotifyInfo = _tl(TXT_PLEASE_INPUT_CARD);
		IO_HELPER->OpenBKCardIndicator();
		// 发送开始读卡消息
		PostMessage(WM_NOTIFY_START_CIRCLE_READ, 0, 0);
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_FINISH){
		theSERVICE_MGR.SetState(FINISH);
		m_strNotifyInfo = _tl(GUIDE_ISSUE_FINISH_C);
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_ERROR){
		theSERVICE_MGR.SetState(FINISH);
		CString strTmp = _tl(GUIDE_ISSUE_RESULT_FAILTURE);
		m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示之前

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::preShow(){
	__super::preShow();

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SVT_INFORMATION);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		// 设置按钮组
		long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
		InitializeFunctionBtns(component);
		// 设置按钮组选中
		vector<int> vecFareList;
		vecFareList = pSvc->GetTicketAmountTable();
		for (int i=0;i<vecFareList.size();i++){
			if(vecFareList[i]==model.productInfo.lProductSaleAmount){
				m_Right_1_Group->ClickButton(i);
			}
		}
		vector<int> vecCountList;
		vecCountList = pSvc->GetTicketCountTable();
		for (int i=0;i<vecCountList.size();i++){
			if(vecCountList[i]==model.productInfo.nProductPiece){
				m_Left_1_Group->ClickButton(i);
			}
		}
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// 其他控件
		m_pTitleDlg->SetWindowText(_tl(TXT_SVT_ISSUE));
		m_pTitleDlg->ShowWindow(SW_SHOW);
		m_btnOKAndCancel1->SetSelect(false);
		m_btnOKAndCancel2->SetSelect(false);
		ShowCompoment(BTN_OK_AND_CANCEL1);
		ShowCompoment(BTN_OK_AND_CANCEL2);
		HideCompoment(BTN_OK);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_FINISH){
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// 设置按钮为默认样式
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->SetEnabled(true);
		m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
		m_btnOKAndCancel1->SetText(_tl(STR_OK));
		m_btnOKAndCancel1->SetSelect(false);
		m_btnOKAndCancel1->MoveWindow(&REC_BTN_OkAndCancel1_RECT, FALSE);
		m_btnOKAndCancel2->SetText(_tl(STR_CANCEL));
		m_btnOKAndCancel2->SetSelect(false);
		m_btnOKAndCancel2->MoveWindow(&REC_BTN_OkAndCancel2_RECT, FALSE);
		ShowCompoment(BTN_OK);
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_ERROR){
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// 设置按钮为默认样式
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetEnabled(true);
		m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
		m_btnOKAndCancel1->SetText(_tl(STR_OK));
		m_btnOKAndCancel1->MoveWindow(&REC_BTN_OkAndCancel1_RECT, FALSE);
		m_btnOKAndCancel2->SetText(_tl(STR_CANCEL));
		m_btnOKAndCancel2->MoveWindow(&REC_BTN_OkAndCancel2_RECT, FALSE);
		ShowCompoment(BTN_OK);
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
	}
	// 显示信息区域
	InitializeBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示后

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::showComplete(){
	__super::showComplete();
	this->RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务数据模型发生变化时的更新界面   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::UpdateUI()
{
	InitializeBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      车票数量选择按钮

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueWithSvtDlg::OnTicketNumClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetTicketPiece(_ttoi(btnInfo->btnMsg));
	pSvc->NotifyDataModelChanged();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		清空Label数据  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::ClearLabelData()
{
	// 初始化收银信息
	for(int n = 0;n < MAX_TICKET_INFO_ROWS;n++){
		m_TipsInfoGroup[n].nameLeft  = _T("");
		m_TipsInfoGroup[n].nameRight = _T("");
	}
	// 初始化产品信息
	for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
		m_TicketInfoGroup[i].nameLeft  = _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}

	for(int n = 0;n < 1;n++){
		m_NotifyInfoGroup[n].nameLeft  = _T("");
		m_NotifyInfoGroup[n].nameRight = _T("");
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化基本信息

@param      None

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::InitializeBaseInfo()
{	
	// 初始化Label值
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	int index = 0;
	// ************* 售票信息 ***************
	index = 0;
	// 产品类型
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	index++;
	// 产品有效期
	m_TipsInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d"));
	index++;
	// 起始目的车站/区段
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);
	index++;		
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
	index++;
	// 产品单价
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_ISSUE_AMOUNT)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
	m_TipsInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount) + _tl(MONEY_UNIT);
	index++;
	// 购买张数
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_COUNT)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN) ;
	m_TipsInfoGroup[index++].nameRight =  ComFormatCount(model.productInfo.nProductPiece) ;//+ _tl(TICKET_UNIT);GUI 要求不要单位；zhengxianle
	index++;
	// 应付金额
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
	index++;

	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		// ************* 储值卡信息 ***************
		if (pSvc->m_bReaderBad){// 循环读卡过程中，tup故障
			LOG("循环读卡中TPU故障，弹框提示！");
			CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(RWTEST_RW_FAULT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			pSvc->OnCancel();
			return;
		}

		if(pSvc->m_strReadCardRsp == _T("")){
			m_strNotifyInfo = _tl(TXT_PLEASE_INPUT_CARD);
		}
		else{
			m_strNotifyInfo = pSvc->m_strReadCardRsp;
		}
	}
	

	index = 0;
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_ID), MAX_HALF_CASE_LEFT_LEN);
	if(model.mediumCardInfo.cardLogicalNumber==_T("")){
		m_TicketInfoGroup[index++].nameRight = _T("---");
		index++;
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_BALANCE), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = _T("---");
		m_btnOKAndCancel1->EnableWindow(FALSE);
	}
	else{
		m_TicketInfoGroup[index++].nameRight = model.mediumCardInfo.cardLogicalNumber;
		index++;
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_BALANCE), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productCardInfo.lBalanceAmount) + _tl(MONEY_UNIT);
		if(model.productCardInfo.lBalanceAmount >= model.productInfo.lProductSaleAmount*model.productInfo.nProductPiece){
			m_btnOKAndCancel1->EnableWindow(TRUE);
		}
		else{
			m_btnOKAndCancel1->EnableWindow(FALSE);
		}
	}
	index++;

	//************* 信息 ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//m_baseInfoArea->Invalidate(FALSE);
	m_cashInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      控件颜色和字体控制

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
HBRUSH	CIssueWithSvtDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if( ID_TIPS_DLG ==  pWnd->GetDlgCtrlID()){//判断发出消息的空间是否是该静态文本框
		CFont font;
		font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, _T("黑体"));

		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(255,0,0));//设置文本颜色为红色
		//pDC->SetBkMode(OPAQUE);//设置文本背景模式为透明
		//pDC->SetBkColor(RGB(0,0,255));//设置文本背景为蓝色
		//hbr=CreateSolidBrush(RGB(0,255,0));//控件的背景色为绿色
	}
	else if( ID_TITLE_DLG ==  pWnd->GetDlgCtrlID()){//判断发出消息的空间是否是该静态文本框
		CFont font;
		font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE+16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, _T("黑体"));

		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色为红色
	}

	return hbr;//返回用于重绘控件背景的画刷
}
 
  //////////////////////////////////////////////////////////////////////////
 /**
 @brief      收到开始接收现金通知

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnNotifyStartReadCard(WPARAM wParam,LPARAM lParam)
 {
	 // 启动操作空闲倒计时定时器
	 pSvc->InsertRecvStartTimer();
	 // 循环寻卡
	 //pSvc->StartCircleReadCard();

	 return 0;
 }


  //////////////////////////////////////////////////////////////////////////
 /**
 @brief      开始发售车票

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnStartIssue(WPARAM wParam,LPARAM lParam)
 {
	 LOG("OnStartIssue::发售车票并找零");
	 // 发售车票并找零
	 pSvc->StartIssueAndChange(); 

	 return 0;
 }
  


 //////////////////////////////////////////////////////////////////////////
 /**
 @brief      处理确认按钮消息

 @param      (i)WPARAM wParam  消息信息，表示按钮的ID号
 @param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

 @retval     LRESULT  TRUE:处理成功  FALSE:处理失败

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	 if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		 // 如果此时，OK按钮不显示，不可能进来
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_FINISH){
		 // 返回总图界面
		 pSvc->FlowToDefaultDlg();
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_ERROR){
		 // 返回总图界面
		 pSvc->FlowToDefaultDlg();
	 }

	 return TRUE;
 }

 //////////////////////////////////////////////////////////////////////////
 /**
 @brief      处理点击确定按钮(确定与取消按钮中的确定按钮）

 @param      (i)WPARAM wParam  消息信息，表示按钮的ID号
 @param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

 @retval     LRESULT  TRUE:处理成功  FALSE:处理失败

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnOkAndCancel1(WPARAM wParam, LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel();
	m_Left_1_Group->EnableButtonGroup(FALSE);
	m_btnOKAndCancel1->EnableWindow(FALSE);

	HideCompoment(BTN_OK_AND_CANCEL1);
	HideCompoment(BTN_OK_AND_CANCEL2);
	theSERVICE_MGR.SetForeServiceBusy(true);
	pSvc->EndRecvTimer();
	m_pTipsDlg->ShowWindow(SW_HIDE);

	// 点击确认，则从储值票中扣款
	LOG("储值票购票，卡中扣款");
	// 停止循环寻卡
	pSvc->StopCircleReadCard();

	// 开始卡内扣款
	long resultCode = -1;
	//resultCode = RECHARGERW_HELPER->TryReadCard(ANALYZE_SVC,m_readResponse,false);
	resultCode = pSvc->FinishEPPayment(&(pSvc->m_readResponse));
	if(resultCode==0){
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
		m_strNotifyInfo = _tl(TXT_ISSUING_NOT_MOVE_CARD);
		pSvc->StartIssueAndChange();
	}
	else{
		pSvc->FlowToErrorDlg();
	}
	theSERVICE_MGR.SetForeServiceBusy(false);

	return TRUE;
 }

  //////////////////////////////////////////////////////////////////////////
 /**
 @brief      处理点击取消按钮(确定与取消按钮中的确定按钮）

 @param      (i)WPARAM wParam  消息信息，表示按钮的ID号
 @param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

 @retval     LRESULT  TRUE:处理成功  FALSE:处理失败

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnOkAndCancel2(WPARAM wParam, LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel(); 
	HideCompoment(BTN_OK_AND_CANCEL1);
	HideCompoment(BTN_OK_AND_CANCEL2);
	// 如果此时，未投入现金，则返回总图界面；如果已经投入纸币，则退出现金
	LOG("储值票购票过程中界面，乘客点击取消按钮");
	// 停止循环寻卡
	pSvc->StopCircleReadCard();
	pSvc->OnCancel();

	return TRUE;
 }