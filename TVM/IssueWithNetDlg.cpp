#include "stdafx.h"
#include "IssueWithNetDlg.h"
#include "BitmapManager.h"
#include "ParseStationMap.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CIssueWithNetDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueWithNetDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()

	ON_BTN_OK_CLICK(OnOK)										// 点击取消按钮

	ON_MESSAGE(WM_NOTIFY_ISSUING, OnNotifyIssuing)	
	ON_MESSAGE(WM_START_ISSUING, OnStartIssue)	
	ON_MESSAGE(WM_NOTIFY_ERWEIMA_ORDER_RES, OnNotifyErweimaResult)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueWithNetDlg::CIssueWithNetDlg(CService* pService)
: CReceptionDlg(CIssueWithNetDlg::IDD, pService)
{
	m_pTipsDlg = new CStatic;

	pSvc = (CIssueSvc*)pService;

	// 左侧线路选择按钮
	m_BTN_Left_1_Group_Info.rows = 9;
	m_BTN_Left_1_Group_Info.cols = 1;

	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// 右侧票价按钮
	m_BTN_Right_1_Group_Info.rows = 9;
	m_BTN_Right_1_Group_Info.cols = 1;

	m_BTN_Right_2_Group_Info.rows = 2;
	m_BTN_Right_2_Group_Info.cols = 4;

	m_BTN_Right_1_Group_Info.title = _T("");


	m_baseInfo->stytle = ISSUE_INFO_STYLE;
	m_baseInfo->point = CPoint(line_map_posX, BASEINFO_AREA_POINT.y);
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->detailInfo.labelGroup = m_TipsInfoGroup;
	m_baseInfo->detailInfo.row = 8;

	m_cashInfo->stytle = IMAGE_CENTER_STYLE;
	m_cashInfo->point = CPoint(line_map_posX, CASHINFO_AREA_POINT.y + 110);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SCAN_TOW_DIMENSIONAL_PAYMENT);
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_cashInfo->detailInfo.row =  MAX_TICKET_INFO_ROWS-4;

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
CIssueWithNetDlg::~CIssueWithNetDlg()
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
int CIssueWithNetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, LINE_AREA | PRICES_AREA | NUMBER_AREA | TOP_STATION_AREA | FUNC_AREA /*|BASE_AREA*/ | CASH_AREA  | BTN_OK) == -1) {
		return -1;
	}

	m_pTipsDlg->Create(_T(""), WS_CHILD, CRect(1140+50,850+30,1350+50,890+30), this, ID_TIPS_DLG);

	m_strNotifyInfo = _T("");
	m_strTipInfo = _T("");

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
void CIssueWithNetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET){
		theSERVICE_MGR.SetState(RECEPT);
		m_strNotifyInfo = _tl(TXT_ERWEIMA_ORDER_REQUESTING);
		// 发生二维码订单请求
		pSvc->RequestOrder();
		HideCompoment(BTN_OK);
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_FINISH){
		theSERVICE_MGR.SetState(FINISH);
		m_strNotifyInfo = _tl(GUIDE_ISSUE_FINISH_C);
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_ERROR){
		theSERVICE_MGR.SetState(FINISH);
		if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
			CString strTmp = _tl(TXT_PAYMENT_SUCCESS_SYSTEM_WILL_RETUND);
			m_strNotifyInfo = TXT_COLOR_RED + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
			CString strTmp = _tl(TXT_FAILURE_TICKET_SYSTEM_WILL_RETUND);
			m_strNotifyInfo = TXT_COLOR_RED + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
			CString strTmp = _tl(TXT_PAYMENT_SUCCESS_SYSTEM_WILL_RETUND);
			m_strNotifyInfo = TXT_COLOR_RED + strTmp;
		}
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
void CIssueWithNetDlg::preShow(){
	__super::preShow();

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SCAN_TOW_DIMENSIONAL_PAYMENT);

	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET){
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

		int adjustNum = 0;
		if(model.productInfo.nProductPiece>7){
			adjustNum = 7;
			m_Left_1_Group->ClickButton(7);//下一页按钮
		}
		vector<int> vecCountList;
		vecCountList = pSvc->GetTicketCountTable();
		for (int i=0;i<vecCountList.size();i++){
			if(vecCountList[i]==model.productInfo.nProductPiece){
				m_Left_1_Group->ClickButton(i-adjustNum);
			}
		}
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// 设置按钮为默认样式
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
		ShowCompoment(BTN_OK);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_FINISH){
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->SetEnabled(true);
		ShowCompoment(BTN_OK);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_ERROR){
		// 置灰按钮组
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->SetEnabled(true);
		ShowCompoment(BTN_OK);
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
void CIssueWithNetDlg::showComplete(){
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
void CIssueWithNetDlg::UpdateUI()
{
	InitializeBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		清空Label数据  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithNetDlg::ClearLabelData()
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
void CIssueWithNetDlg::InitializeBaseInfo()
{	
	// 初始化Label值
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	// ************* 购票信息 ***************
	int index = 0;
	// 单程票
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

	// ************* --信息 ***************

	//************* 提示信息 ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//************* 信息 ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//m_cashInfoArea->SetTitleText((pSvc->m_ePayType==CIssueSvc::PAY_WITH_WEIXIN?_T("请扫描微信二维码"):_T("请扫描支付宝二维码")));
	m_baseInfoArea->Invalidate(FALSE);
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
HBRUSH	CIssueWithNetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
 @brief      收到后台发售车票的通知

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithNetDlg::OnNotifyIssuing(WPARAM wParam,LPARAM lParam)
 {
	 LOG("OnNotifyIssuing::收到车票并找零通知");
	 pSvc->EndRecvTimer();
	 // 发售车票并找零
	 m_strNotifyInfo = _tl(GUIDE_ISSUING);
	 HideCompoment(BTN_OK);
	 m_btnOK->SetEnabled(false);
	 pSvc->NotifyDataModelChanged();

	 PostMessage(WM_START_ISSUING,0,0);
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
 LRESULT CIssueWithNetDlg::OnStartIssue(WPARAM wParam,LPARAM lParam)
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
 LRESULT CIssueWithNetDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET){
		 LOG("二维码购票过程中界面，乘客点击取消按钮");
		 if (model.operationInfo.finishType != CPassengerForeSvc::QR_ORDER_GENERATE_FAILED){
			 pSvc->EndQueryOrderTimer();
			 pSvc->CancelOrder();
		 }
		 pSvc->OnCancel();
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_FINISH){
		// 返回总图界面
		pSvc->FlowToDefaultDlg();
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_ERROR){
		// 返回总图界面
		pSvc->FlowToDefaultDlg();
	}

	 return TRUE;
 }

    //////////////////////////////////////////////////////////////////////////
 /**
 @brief      收到后台发售车票的通知

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithNetDlg::OnNotifyErweimaResult(WPARAM wParam,LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel();
	 RETURN_CODE retCode = (RETURN_CODE)wParam;
	 LOG("OnNotifyErweimaResult::收到二维码订单请求结果:%d", retCode);
	 ShowCompoment(BTN_OK);
	 if(retCode==RSP_CODE_OK){
		 m_strNotifyInfo = _tl(TXT_SCAN_60S);
		 // 启动查询订单定时器
		 pSvc->InsertQueryOrderTimer();
	 }
	 else{
		m_strNotifyInfo = _tl(TXT_CONNECT_SERVER_FAILURE);
		model.operationInfo.finishType = CPassengerForeSvc::QR_ORDER_GENERATE_FAILED;
	 }
	 pSvc->NotifyDataModelChanged();

	 return TRUE;
 }