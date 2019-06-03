#include "stdafx.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "CTIssuePermit.h"
#include "IssueACCIniDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CIssueACCIniDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueACCIniDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPassagerTypeClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPieceClick)   // 张数选择
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnPayTypeClick) // 支付方式
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_PRINT_CLICK(OnPrint)
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCIniDlg::CIssueACCIniDlg(CService* pService)
: CReceptionDlg(CIssueACCIniDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
	
	// 这里应该由配置文件配置
	m_bShowOkButtonAndPayMented = theFunction_INFO.IsShowPaymentMethod();
	m_baseInfo->point = CPoint(5,0);
	m_baseInfo->stytle = PASSAGE_WIDE_STYLE;
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_baseInfo->detailInfo.row = MAX_TICKET_INFO_ROWS;

	
	// 设置右侧功能区按钮组按钮个数，否则m_Right_1_Group等会初始化失败
	//m_BTN_GROUP_INFO[0].rows = m_bShowOkButtonAndPayMented ? 3 : 12;
	//m_BTN_GROUP_INFO[0].cols = m_bShowOkButtonAndPayMented ? 3 : 1;
	// 设置右侧功能区按钮组标题，否则m_Right_1_Group等会没有标题高度
	//m_BTN_GROUP_INFO[0].title = _T("");

	m_BTN_Right_1_Group_Info.rows = 8;
	m_BTN_Right_1_Group_Info.cols = 2;
	m_BTN_Right_1_Group_Info.title = _T("");

	/*if(m_bShowOkButtonAndPayMented){
	m_BTN_GROUP_INFO[1].rows = 3;
	m_BTN_GROUP_INFO[1].cols = 3;
	m_BTN_GROUP_INFO[1].title = _T("");
	}*/
	
	//m_BTN_GROUP_INFO[2].rows = 3;
	//m_BTN_GROUP_INFO[2].cols = 3;
	//m_BTN_GROUP_INFO[2].title = _T("");

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCIniDlg::~CIssueACCIniDlg()
{
	if (m_guideInfoArea!=NULL)
	{
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:创建成功    -1:创建失败

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
int CIssueACCIniDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(m_bShowOkButtonAndPayMented){
		if (__super::OnCreate(lpCreateStruct, NUMBER_AREA|BASE_AREA|CASH_AREA|BTN_PRINT|BTN_OK) == -1) {
			return -1;
		}
	}
	else{
		if (__super::OnCreate(lpCreateStruct,NUMBER_AREA | BASE_AREA|/*CASH_AREA|*/BTN_PRINT|BTN_OK) == -1) {
			return -1;
		}
	}
	

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea->createWnd(this);

	// 创建时隐藏按钮。
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示之前

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::preShow()
{
	__super::preShow();

	// 设置按钮不为选中状态
	m_btnOK->SetSelect(false);
	m_btnPrint->SetSelect(false);

	// 1.设置状态
	theSERVICE_MGR.SetState(RECEPT);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
	// 2.初始化隐藏功能区按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);

	// 3.隐藏OK和Print按钮
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	// 4.初始化按钮(只选择张数)
	if(m_bShowOkButtonAndPayMented){
		InitializeFunctionBtns(/*PASSAGETYPE|*/PAYTYPE|PIECE_BUTTON);
	}
	else{
		InitializeFunctionBtns(/*PASSAGETYPE|PAYTYPE|*/PIECE_BUTTON);
	}
	

	CIssueSvc::Model& model = pSvc->GetDataModel();
	// 5.初始化界面
	if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_INIT)
	{
		m_bConfirmPayment = false;
		// 是否显示过提示投入数量超限图片？防止重绘界面刷新。
		m_bIsShowCoinMaxCount = false;
		m_bIsShowBanknoteMaxCount = false;
		m_bIsShowFinshedInsertMoney = false;

		/*-----------------------郑州直接默认直接开始收钱----------------------------*/
		if(m_bShowOkButtonAndPayMented){
			// 张数按钮
			m_Right_1_Group->ClickFirstAvailButton();
			if(!theAPP_SESSION.IsBlindManOperation() || model.productInfo.productType == SJT){
				// 支付方式,单程票不选择支付方式。
				m_Right_2_Group->ClickFirstAvailButton();
			}
		
			// 提示操作信息
			m_guideInfo->tipsCount = 2;
			m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_PLEASE_SELECT_QUANTITY_PAYMENT);
			m_guideInfo->detailList[0].strfileName = TVM_GIF_OPERATION_GUIDE;

			m_guideInfo->detailList[1].bitInfo =  _tl(TXT_GUIDE_PLEASE_CLICK_CONFIRM);
			m_guideInfo->detailList[1].strfileName = TVM_TIP_CONFIRM_NORMAL;
		}
		else{
			// 提示操作信息
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_PLEASE_SELECT_QUANTITY);// 请选择张数
			m_guideInfo->detailList[0].strfileName = TVM_TIP_CONFIRM_BIG;
			
			m_Right_1_Group->ClickFirstAvailButton();
		}

		// 启用张数选择按钮
		if (m_Right_1_Group!=NULL && !m_Right_1_Group->IsWindowEnabled()){
			m_Right_1_Group->EnableWindow(TRUE);
			m_Right_1_Group->Invalidate();
			m_Right_1_Group->UpdateWindow();
		}

		// 默认选择一张
		//m_Right_1_Group->ClickFirstAvailButton();

		// 显示收钱界面
		
		model.mediumInfo.passengerType = PASSENGER_ADULT;

		// 显示取消按钮
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		
		InitializeBaseInfo();
	}
	// 6.完成界面
	else if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_FINISH){
		// 设置完成状态
		// 判断纸币押钞是否完成
		if(model.lDeviceStatus == CPassengerForeSvc::DEVICE_ALL_OK/*model.operationInfo.banknoteAcceptState != CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_INSERTED*/){
			theSERVICE_MGR.SetState(FINISH);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		}
		// 是否显示打印按钮(不打印)
		/*if (theAFC_TVM_OPERATION.SupportPrintSwitch()&& theTVM_STATUS_MGR.GetPrintMode() == 0){
		ShowCompoment(BTN_PRINT);			
		}*/	
		// 如果是盲人购票，提示交易成功,是否打印?
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(AUDIO_BUSINESS_SUCCESS);
		}
		m_Right_1_Group->ShowButtons(FALSE);
		m_bShowOkButtonAndPayMented ? m_Right_2_Group->ShowButtons(FALSE) : NULL;

		// Token
		if(model.mediumInfo.cardPHType == CARD_TYPE_ACC_UL){
			// 设置提示区显示信息
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_PLEASE_COLLECT_TOKEN_CHANGE);
			m_guideInfo->detailList[0].strfileName = TVM_TIP_GET_TOKEN_CHANGE;
		}
		else{
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_PLEASE_COLLECT_CARD_CHANGE);
			m_guideInfo->detailList[0].strfileName = TVM_TIP_GET_TOKEN_CHANGE;
		}

		InitializeGuideArea();
	}

	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->RedrawWindow();
	
	//InitializeGuideArea();
}

void CIssueACCIniDlg::showComplete()
{
	//AfxMessageBox(L"CIssueACCIniDlg::showComplete");
	__super::showComplete();
	this->RedrawWindow();	
	m_guideInfoArea->renderGuideInfo();
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
void CIssueACCIniDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务数据模型发生变化时的更新界面   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::UpdateUI()
{
	InitializeBaseInfo();
	if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_INIT)
	{
		CIssueSvc::Model& model = pSvc->GetDataModel();
		// 现金支付，且为计数中
		if (model.paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH && theSERVICE_MGR.GetState() == COUNTING){
			
			// 如果有硬件异常
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN){
				if(theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)){
					theTVM_STATUS_MGR.SetPaymentMode((theTVM_STATUS_MGR.GetPaymentMode() ^ DEVICE_CONTRL_CODE_PAYMENT_COIN | DEVICE_CONTRL_CODE_PAYMENT));
					theTVM_STATUS_MGR.SetChangeMode((theTVM_STATUS_MGR.GetChangeMode() ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE));
					// 刷新界面
					ShowGuideMsgOfPayInfo();
				}
			}

			// 如果纸币达到最大数量，显示纸币达到最大数量
			if (pSvc->IsBanknoteMaxCount() && model.operationInfo.isBanknotesMaxCount && !m_bIsShowBanknoteMaxCount){
				ShowBanknoteMaxCount();
				m_bIsShowBanknoteMaxCount = true;
			}
			// 如果硬币达到最大数量，显示硬币达到最大数量。
			if(pSvc->IsCoinMaxCount() && model.operationInfo.isCoinMaxCount && !m_bIsShowCoinMaxCount){
				ShowCoinMaxCount();
				m_bIsShowCoinMaxCount = true;
			}
		}

		// 如果已经支付钱，将选择张数按钮禁用
		if(model.paymentInfo.lPaidAmount > 0 ){
			if (m_Right_1_Group!=NULL && m_Right_1_Group->IsWindowEnabled()){
				m_Right_1_Group->EnableWindow(FALSE);
				//m_Right_1_Group->Invalidate();
				//m_Right_1_Group->UpdateWindow();
			}
		}
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->RedrawWindow();

		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

	}
	else if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_FINISH)
	{
		//m_Right_1_Group->ShowButtons(false);
		//m_Right_2_Group->ShowButtons(false);
		//m_Right_3_Group->ShowButtons(false);
		//HideCompoment(BTN_OK);
		//ShowCompoment(BTN_PRINT);
	}
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
LRESULT CIssueACCIniDlg::OnOK(WPARAM wParam, LPARAM lParam)
{
	SYNC(ACCINIDLG_ONBUTTON_CLICK,_T("ACCINIDLG_ONBUTTON_CLICK"));
	// 正常流程
	if (theSERVICE_MGR.GetState() == RECEPT){
		HideCompoment(BTN_OK);
		/*m_Right_1_Group->ShowButtons(false);
		m_Right_1_Group->UpdateWindow();
		if(m_bShowOkButtonAndPayMented){
			m_Right_2_Group->ShowButtons(false);
			m_Right_2_Group->UpdateWindow();
		}*/
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);		
		ShowGuideMsgOfPayInfo();// 显示支持支持硬币、纸币显示提示		
		pSvc->StartReceiveMoney();// 开始接收钱或银行卡
	}
	else if (theSERVICE_MGR.GetState() == COUNTING)	{
		InitializeBaseInfo();	// 强制显示最新状态
		if(NULL != m_baseInfoArea){
			m_baseInfoArea->UpdateWindow();
		}
		
		CIssueSvc::Model& model = pSvc->GetDataModel();
		HideCompoment(BTN_OK); // 隐藏取消按钮。
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo = _tl(GUIDE_ISSUING);
		m_guideInfo->detailList[0].strfileName = TVM_TIP_ISSUE_TICKETS;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		pSvc->StartIssueAndChange(); 
	}
	m_btnOK->SetSelect(false);
	return TRUE;
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
LRESULT  CIssueACCIniDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	// 点击按钮后直接隐藏
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	// 如果业务没有进行硬件动作，直接返回默认界面
	if (!model.operationInfo.isBeginBuyProduct){
		pSvc->FlowToDefaultDlg();
		return TRUE;
	}
	
	// 如果完成，直接返回默认业务
	if(theSERVICE_MGR.GetState() == FINISH){
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(0);
		}
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		pMainSvc->StartDefaultService();
		return TRUE;
	}
	// 设置售票完成。
	//model.operationInfo.isSuccessIssueCard = true;

	if(theAPP_SESSION.IsBlindManOperation())
		pSvc->PlayAudioInDlg(AUDIO_BUSINESS_CANCEL);
	else	
		pSvc->OnCancel();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票价或子产品按钮消息响应（未使用）

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCIniDlg::OnPassagerTypeClick(WPARAM wParam, LPARAM lParam)
{	
	CIssueSvc::Model& model = pSvc->GetDataModel();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	if (model.mediumInfo.passengerType != _ttoi(btnInfo->btnMsg))
	{
		pSvc->SetPassengerType(_ttoi(btnInfo->btnMsg));
	}
	pSvc->NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      张数按钮消息响应  

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCIniDlg::OnPieceClick(WPARAM wParam, LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetTicketPiece(_ttoi(btnInfo->btnMsg));
	pSvc->NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      付款方式点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCIniDlg::OnPayTypeClick(WPARAM wParam,LPARAM lParam)
{
	SYNC(ACCINIDLG_ONBUTTON_CLICK,_T("ACCINIDLG_ONBUTTON_CLICK"));
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	PaymentMethod_t payment = (PaymentMethod_t)_ttoi(btnInfo->btnMsg);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (model.paymentInfo.paymentMethod != payment)
	{
		pSvc->SetPayment(payment);
		//InitializeGuideArea();
	}
	if(payment == PaymentMethod_t::PAYMENT_METHOD_BANK_CARD /*|| payment == PaymentMethod_t::PAYMENT_METHOD_CREDIT_CARD*/)
	{
		HideCompoment(BTN_OK);
	}
	else
		ShowCompoment(BTN_OK);
	pSvc->NotifyDataModelChanged();

	// 盲人购票显示按钮选中状态
	if(theAPP_SESSION.IsBlindManOperation()){		
		if(payment == PAYMENT_METHOD_BANK_CARD /*|| payment == PAYMENT_METHOD_EFT*/){// 占时不支持此支付方式
			pSvc->PlayAudioInDlg(AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT);
			return FALSE;
		}
		m_Right_2_Group->SetSelection(btnInfo->btnMsg);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理打印按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CIssueACCIniDlg::OnPrint(WPARAM wParam,LPARAM lParam)
{
	CIssueSvc::Model model = pSvc->GetDataModel();
	try{
		// 盲人充值确认按钮点击时立即停止语音播报
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(0);
		}
		// 设置正在操作中...
		theSERVICE_MGR.SetState(ISSUEING);
		HideCompoment(BTN_PRINT);
		// 正在打印中...
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_YOUR_CREDENTIALS_PRINTING);
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTING;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		pSvc->PrinterPrintIssueRecipt();

		// 打印完成，请收好凭据。
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_PRINT_IS_COMPLEATED)/*T("打印完成，请取走您的凭条！")*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTED;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		// 设置打印状态为成功
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS;
		// 盲人购票打印了
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(AUDIO_BUSINESS_PRINT_SUCCESS);// 打印成功
		}
		// 设置完成状态，返回默认业务计时器开始 
		theSERVICE_MGR.SetState(FINISH);
		return 0;
	}
	catch(CSysException& e){
		// 设置打印失败状态
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

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
void CIssueACCIniDlg::InitializeBaseInfo()
{	
	// 初始化Label值
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	int index = 0;

	// BugHere 1967
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_CASH_INFO);

	// 1.产品类型
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName((model.productInfo.productType<<8 | model.productInfo.productSubType), language);
	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	//m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
	index++;

	// 2.子产品类型:PST,MST
	if (model.productInfo.productType == PST)
	{
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PURCHASE_TIMES) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
		index++;
	}else if ( model.productInfo.productType == MST)
	{
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PURCHASE_RIDES) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
		index++;
	}
	// 3.产品有效期
	m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d"));
	index++;

	// 4.乘客类型
	//m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PERSONAL_PASSENGER_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = language == LANGUAGE_ID_CHINESE ? _T("成人") : _T("Adult");//theACC_BUSINESS.GetPassengerTypeName(model.mediumInfo.passengerType, language);
	
	// 5.起始目的车站/区段
	if(model.productInfo.productType == SJT ){
		// 单程票
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
/*		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);	*/	
		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);
		index++;		
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
		index++;

	}
	else if(model.productInfo.productType == PST || model.productInfo.productType == MST)
	{
		// 计次票、定期票
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_SECTION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
/*		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*/	
		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) :CString(_T("丹鹤站")); /*theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*///lichao:暂时修改，写死车站
		index++;		

		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_SECTION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
		index++;

	}

	// 6.产品单价
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_ISSUE_AMOUNT)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount) + _tl(MONEY_UNIT);
	index++;
	
	// 7.押金（可退卡）or 成本费（不可退卡）
	if(model.mediumInfo.cardPHType == CARD_TYPE_ACC_M1)
	{		
		// 非单程票
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_DEPOSIT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardDeposit) + _tl(MONEY_UNIT);
		//m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_CARD_FEE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;//成本费
		//m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardFee) + _tl(MONEY_UNIT);	
		index++;

	}

	// 9.税费
	//m_TicketInfoGroup[index].nameLeft  = add_hsp(_tl(TXT_TAX_OF_ISSUE) + _tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT);

	// 8.购买张数
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_COUNT)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN) ;
	m_TicketInfoGroup[index++].nameRight =  ComFormatCount(model.productInfo.nProductPiece) ;//+ _tl(TICKET_UNIT);GUI 要求不要单位；zhengxianle
	index++;
	index+=2;
	if(!m_bShowOkButtonAndPayMented){
		// 9.应付金额
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
		index++;

		// 10.实际付款金额
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_INCOME) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);
		index++;

		// 11.找零金额
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CHANGE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lChangeAmount >=0 ? model.paymentInfo.lChangeAmount : 0) + _tl(MONEY_UNIT);
		index++;

	}

	m_baseInfoArea->Invalidate(FALSE);


	if(m_bShowOkButtonAndPayMented){
		// 支付方式
		m_cashInfo->detailInfo.labelGroup[0].nameLeft = add_hsp(_tl(TXT_PAYMENT_METHOD) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[0].nameRight = _tl(TXT_PAYMENT_CASH);/*theACC_BUSINESS.GetPayTypeName(model.paymentInfo.paymentMethod, language)*/;
		// 应付金额
		m_cashInfo->detailInfo.labelGroup[1].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[1].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
		// 实付金额
		m_cashInfo->detailInfo.labelGroup[2].nameLeft = add_hsp(_tl(TXT_INCOME) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);	
		// 找零金额
		if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			m_cashInfo->detailInfo.labelGroup[3].nameLeft = add_hsp(_tl(TXT_CHANGE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_cashInfo->detailInfo.labelGroup[3].nameRight = ComFormatAmount(model.paymentInfo.lChangeAmount >=0 ? model.paymentInfo.lChangeAmount : 0) + _tl(MONEY_UNIT);
		}
		else{
			m_cashInfo->detailInfo.labelGroup[3].nameLeft = _T("");
			m_cashInfo->detailInfo.labelGroup[3].nameRight = _T("");
		}

		m_cashInfoArea->Invalidate(FALSE);
	}
	
	// 完成投钱，显示提示信息。
	if(model.operationInfo.isFinishedReceveMoney &&(pSvc->GetCurDialogGroupID() != CIssueSvc::DIALOG_GROUP_FINISH) && !m_bIsShowFinshedInsertMoney)
	{
		if(theFunction_INFO.IsNeedPassengerClickOkButton()){
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_CONFIRM_AFTER_PAY/*TXT_GUIDE_PLEASE_CLICK_CONFIRM*/);
			m_guideInfo->detailList[0].strfileName = TVM_TIP_CONFIRM_BIG;

			InitializeGuideArea();
			m_guideInfoArea->renderGuideInfo();
		}
		
		// 如果是盲人购票，提示交易成功,是否打印?
		if(theAPP_SESSION.IsBlindManOperation()){
			//CMessagePictureBox::Show(_T("提示"),_T("请确认支付信息!"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			// 支付完成确认
			pSvc->PlayAudioInDlg(AUDIO_CONFORM_PAYINFO);
		}
		// 只执行一次
		m_bIsShowFinshedInsertMoney = true;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化功能区按钮

@param      (i)long lComponents  自定义组件信息

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::InitializeFunctionBtns(long lComponents)
{
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	
	// 乘客类型按钮
	/*if(lComponents & PASSAGETYPE)
	{	
		m_vec_Right_1_BTN_INFO.clear();
		map<WORD,CString> availablePassageTypeList;
		theACC_BUSINESS.GetPassengerTypeParam(language,availablePassageTypeList);
		map<WORD,CString>::iterator it = availablePassageTypeList.begin();
		for (it = availablePassageTypeList.begin(); it != availablePassageTypeList.end(); ++it)
		{
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"),it->first);
			btnInfo.text = it->second;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}

		CString title;
		if (language == LANGUAGE_ID_CHINESE)
		{
			title = _T("请选择乘客类型");
		}
		else
		{
			title = _T("Please select passenger type");
		}
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}*/

	// 1.张数按钮
	if(lComponents&PIECE_BUTTON)
	{
		m_vec_Right_1_BTN_INFO.clear();
		// 最大买票张数
		int nMaxCount = 0;
		CAfcTVMOperationParam::MAGAZINE_PARAM cardBoxParam;
		theAFC_TVM_OPERATION.GetMagazineParam(cardBoxParam);
	
		CTCardCountInfo::CARD_BOX_INFO card_Box_A = theCARD_COUNT.GetCardboxAInfo();
		CTCardCountInfo::CARD_BOX_INFO card_Box_B = theCARD_COUNT.GetCardboxBInfo();

		int nMaxCardASaleCount = 0,nMaxCardBSaleCount = 0;// 实际可以买的票数

		nMaxCardASaleCount = card_Box_A.ulCurCount - cardBoxParam.TicketEmptyValue;
		nMaxCardBSaleCount = card_Box_B.ulCurCount - cardBoxParam.TicketEmptyValue;

		if(nMaxCardASaleCount > 0){
			nMaxCount += nMaxCardASaleCount;
		}

		if(nMaxCardBSaleCount > 0){
			nMaxCount += nMaxCardBSaleCount;
		}

		map<int,CString> availablePieceList;
		pSvc->GetIssuePiece(availablePieceList);
		map<int,CString>::iterator it = availablePieceList.begin();
		for (it = availablePieceList.begin(); it != availablePieceList.end(); ++it)
		{
			if (it->first > nMaxCount){
				break;
			}
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"),it->first);
			btnInfo.text = it->second + _tl(TICKET_UNIT);
			
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}
		CString title;
		m_bShowOkButtonAndPayMented ? title.Format(_T("%s(%s)"),_tl(TXT_GUIDE_PLEASE_SELECT_QUANTITY),_tl(TICKET_UNIT)) : title.Format(_T("%s"),_tl(TXT_GUIDE_SELECT_TICKET_COUNT));
		m_Right_1_Group->ShowButtons(true);
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		UIINFO uiInfo=m_Right_1_Group->getUIInfo();
		uiInfo.m_Font=theGUI_INFO.GetBtnFont();	//英文字体会发生变化
	}
		
	// 2.支付方式按钮
	if(lComponents & PAYTYPE)
	{
		m_vec_Right_2_BTN_INFO.clear();
		vector<WORD>& vecPayment = pSvc->GetIssuePayment();
		for (vector<WORD>::iterator iter = vecPayment.begin(); iter != vecPayment.end(); ++iter)
		{
			// 如果为盲人购票，去除EP支付，如果有的话。
			/*if(theAPP_SESSION.IsBlindManOperation() && (PaymentMethod_t)(*iter) == PAYMENT_METHOD_EFT){
				continue;
			}*/
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), *iter);
			btnInfo.text = _tl(TXT_PAYMENT_CASH);//theACC_BUSINESS.GetPayTypeName(*iter, language);
			m_vec_Right_2_BTN_INFO.push_back(btnInfo);
		}
		m_Right_2_Group->ShowButtons(true);
		m_Right_2_Group->SetButtonTitle(_tl(TXT_RECHARGE_SELECT_PAYMENT_METHOD));
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		UIINFO uiInfo=m_Right_2_Group->getUIInfo();
		uiInfo.m_Font=theGUI_INFO.GetBtnFont();	//英文字体会发生变化
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
void CIssueACCIniDlg::InitializeGuideArea()
{
	if(NULL!=m_guideInfoArea){
		m_guideInfoArea->setGuideInfo(m_guideInfo);		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		显示支付信息    

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowGuideMsgOfPayInfo(){
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH)
	{
		// 设置支付提示信息
		int index = 0;
		// 纸币
		if (!pSvc->BanknoteShowword().IsEmpty())
		{
			// 判断当前模式是否支持纸币投入？
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
				m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_BANKTOTES);
				m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
			}
			else{
				m_guideInfo->detailList[index].bitInfo =  pSvc->BanknoteShowword();
				m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
			}
		}
		// 显示不支持此币种
		else{
			m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_BANKTOTES);
			m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
		}
		// 硬币
		if (!pSvc->CoinShowword().IsEmpty())
		{
			// 不支持硬币投入？
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
				m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_COINS);
				m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
			}
			else{
				m_guideInfo->detailList[index].bitInfo =  pSvc->CoinShowword();
				m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_COIN;
			}
		}
		else{
			m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_COINS);
			m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
		}

		m_guideInfo->tipsCount = index;
		// 如果是盲人购票
		if(theAPP_SESSION.IsBlindManOperation()){
			LOG("提示投入现金.");
			//CMessagePictureBox::Show(_T("提示"),_T("请投入现金，硬币支持5MOP，2MOP，1MOP，50AVOS，纸币支持10MOP，20MOP，50MOP，100MOP，500MOP，1000MOP。"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			// 请投入现金，硬币支持5MOP，2MOP，1MOP，50AVOS，纸币支持10MOP，20MOP，50MOP，100MOP，500MOP，1000MOP。
			pSvc->PlayAudioInDlg(AUDIO_INSERT_CASH);
			LOG("提示完成。");
		}
	}
	/*else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_EFT)
	{
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_PSG_PAYMENT_EP);
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_EP;
		m_guideInfo->tipsCount = 1;
	}*/
	else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD /*||model.paymentInfo.paymentMethod == PAYMENT_METHOD_CREDIT_CARD*/)
	{
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _T("请使用银行卡");
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_BANKCARD;
		// 如果是盲人购票
		if(theAPP_SESSION.IsBlindManOperation()){
			// 请插入银行卡
			pSvc->PlayAudioInDlg(AUDIO_INSERT_BANKCARD);
		}
	}
	// 隐藏功能按钮
	//m_Right_1_Group->ShowButtons(false);
	//m_bShowOkButtonAndPayMented ? m_Right_2_Group->ShowButtons(false) : NULL;
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      验证用户输入是否正确 

@param      None

@retval     bool  true:输入信息合法  false:输入内容不合法

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueACCIniDlg::ValidateInput()
{
	//CIssueSvc::Model &model = pSvc->GetDataModel();

	//// 基本信息区信息检查
	//// 车站信息有效性检查 （预留）
	//if(theISSUE_PERMIT.IsPermitIssueStation(model.BegainStation))
	//{
	//	CMessageBox::Show(theAPP_SESSION.GetServiceName(ISSUE_SVC),_T("本站限制发售"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
	//	return FALSE;
	//}

	//if( theISSUE_PERMIT.IsPermitIssueStation(pSvc->GetDataModel().EndStation))
	//{
	//	CMessageBox::Show(theAPP_SESSION.GetServiceName(ISSUE_SVC),_T("限制发售该车站车票"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
	//	return FALSE;
	//}

	//	//信息有效性检查
	//if(m_PieceEditIndex != -1){
	//	if( GetEditText(m_PieceEditIndex).GetLength() == 0 )	{
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_COUNT));
	//		GetEdit(m_PieceEditIndex)->SetFocus();
	//		return false;
	//	}
	//}	
	//if(m_PriceEditIndex != -1){
	//	if( GetEditText(m_PriceEditIndex).GetLength() == 0 )	{
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_PRICE_ERROR));
	//		GetEdit(m_PriceEditIndex)->SetFocus();
	//		return false;
	//	}
	//	CAccProductParam::PRODUCT_INFO product;
	//	theACC_PRODUCT.GetProduct(model.issueProductInfo.ProductType,product);
	//	if( (model.issueProductInfo.ProductyCategory == PURSE)&&(model.issueProductInfo.SaleAmount<product.minPurseReload||model.issueProductInfo.SaleAmount>product.maxPurseReload) )	{
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_PRICE_ERROR));
	//		GetEdit(m_PriceEditIndex)->SetFocus();
	//		return false;
	//	}
	//}	
	//if(m_OrigEditIndex != -1){
	//	if( model.BegainStation==0 &&model.issueProductInfo.SaleMode == BY_BEGIN_END_STATION)	{
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_SELECT_START));
	//		GetEdit(m_OrigEditIndex)->SetFocus();
	//		return false;	
	//	}
	//}	
	//if(m_DestEditIndex != -1){
	//	if(model.EndStation==0 &&model.issueProductInfo.SaleMode == BY_BEGIN_END_STATION )	{
	//		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_SELECT_END));
	//		GetEdit(m_DestEditIndex)->SetFocus();
	//		return false;	
	//	}
	//}	
	//if(theMAINTENANCE_INFO.GetDeviceType() == DEVICE_VM&&theTICKET_HELPER.GetVMIssueAmountLast()<model.issueInfo.due){
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_AMOUNT_TOO_LARGE_ERROR));
	//	return false;
	//}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		清空Label数据  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ClearLabelData(){
	// 初始化产品信息
	for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
		m_TicketInfoGroup[i].nameLeft  = _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}
	// 初始化收银信息
	for(int n = 0;n < 4;n++){
		m_cashInfo->detailInfo.labelGroup[n].nameLeft = _T("");
		m_cashInfo->detailInfo.labelGroup[n].nameRight = _T("");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		显示纸币数量超限提示信息  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowBanknoteMaxCount(){
	// 设置提示区显示信息
	LOG("显示纸币数量达到最大值。");
	int index = 0;
	if (!pSvc->BanknoteShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = _tl(GUIDE_CURRENT_RECIVE_MAX_BANKNOTES); // 显示提示文言
		m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
	}
	if (!pSvc->CoinShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = pSvc->CoinShowword();				 // 显示支持的硬币的币种
		m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_COIN;
	}
	m_guideInfo->tipsCount = index;	
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		显示硬币超限信息  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowCoinMaxCount(){
	// 设置提示区显示信息
	int index = 0;
	if (!pSvc->BanknoteShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = pSvc->BanknoteShowword();             // 显示支持的纸币币种
		m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
	}
	if (!pSvc->CoinShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo =_tl(GUIDE_CURRENT_REVICE_MAX_COINS); // 显示提示文言				 // 显示支持的硬币的币种
		m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
	}
	m_guideInfo->tipsCount = index;	
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}
