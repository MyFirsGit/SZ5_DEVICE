#include "stdafx.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "ChargeReceptionDlg.h"
#include "ChargeSvc.h"
#include "ParseStationMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CChargeReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CChargeReceptionDlg,CReceptionDlg)
	ON_WM_ACTIVATE()
    ON_WM_CREATE()

	ON_WM_CTLCOLOR()
	ON_BTN_OK_CLICK(OnOK)										// 点击取消按钮
	ON_BTN_BACK_CLICK(OnOK)
	ON_BTN_OkAndCancel1_CLICK(OnOkAndCancel1)					// 点击确定按钮(确定与取消按钮中的确定按钮）
	ON_BTN_OkAndCancel2_CLICK(OnOkAndCancel2)					// 点击取消按钮(确定与取消按钮中的取消按钮）
	ON_MESSAGE(WM_NOTIFY_ACCEPTED, OnNotifyAccepted)			// 收到纸币通知
	ON_MESSAGE(WM_NOTIFY_ISSUING, OnNotifyCharge)				// 收到充值通知	
	ON_MESSAGE(WM_UPDATE_RECEPITON_GUIDE, OnUpdateGuide)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CChargeReceptionDlg::CChargeReceptionDlg(CService* pService)
    :CReceptionDlg(CChargeReceptionDlg::IDD, pService)
{
	pSvc = (CChargeSvc*)pService;
	
	m_pTipsDlg = new CStatic;
	m_pTitleDlg = new CStatic;

	m_dwMaxTicketAmount = theFunction_INFO.GetTicketMaxAmount();

	m_cashInfo->stytle = CHARGE_INFO_STYLE;
	m_cashInfo->point = CPoint(left_number_size.cx+kongbai_x, 0);
	m_cashInfo->titleInfo.titleName = _T("充值操作");
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_cashInfo->detailInfo.row = 3;
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
CChargeReceptionDlg::~CChargeReceptionDlg()
{
	if (m_guideInfoArea!=NULL){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化基本信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::InitializeBaseInfo()
{	
	CChargeSvc::DataModel& model = pSvc->GetDataModel();
 
	ClearLabelData();

	int index = 0;
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	CAccTicketParam::_tickets_Type ticket;
	theACC_PRODUCT.GetTicketInfo(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType),ticket);
	int maxRemainValue = ticket.maxChargeSize/*theACC_PRICE.GetProductMaxRemainValue(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType))*/;
	model.paymentInfo.lPriceAmount = maxRemainValue - model.productInfo.lBalanceAmount;

	// ************* 充值说明 ***************
	CString strBanknoteTips = BanknoteShowword();	
	CString strCoinTips = ComFormatAmount(maxRemainValue) + _tl(MONEY_UNIT);

	// ************* 储值卡信息 ***************
	index = 0;
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_BALANCE), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = _T("0元");//ComFormatAmount(model.productInfo.lBalanceAmount) + _tl(MONEY_UNIT);
	index++;

	 //如果是业务进行中界面，且检测到卡被拿走，则去除界面卡信息，置会确定按钮
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		if(model.paymentInfo.lPaidAmount>0) {
			m_btnOKAndCancel1->SetEnabled(true);
		}
		if(model.paymentInfo.lChangeAmount>0) {
			m_btnOKAndCancel1->SetEnabled(false);
			//m_strNotifyInfo = _tl(TXT_INPUT_MOENY_OVERLIMIT);
		}

		if (pSvc->m_bReaderBad){// 循环读卡过程中，tup故障
			LOG("循环读卡中TPU故障，弹框提示！");
			CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(RWTEST_RW_FAULT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			pSvc->OnCancel();
			return;
		}

		//if (m_strNotifyInfo!=TXT_COLOR_RED + _tl(TXT_BANKNOTE_OVERLIMIT_CARD_MAX_REVALUE)){
		//	if(model.mediumCardInfo.cardLogicalNumber==_T("")){
		//		index = 0;
		//		m_TicketInfoGroup[index++].nameRight = _T("---");
		//		index++;
		//		m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_CARD_TYPE), MAX_HALF_CASE_LEFT_LEN);
		//		m_TicketInfoGroup[index++].nameRight = _T("---");
		//		index++;
		//		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_BALANCE), MAX_HALF_CASE_LEFT_LEN);
		//		m_TicketInfoGroup[index++].nameRight = _T("---");
		//		index++;
		//		m_btnOKAndCancel1->EnableWindow(FALSE);
		//		m_strNotifyInfo = TXT_COLOR_RED + CString(_tl(GUIDE_CHARGE_CARD_IN_PLACE));
		//	}
		//	else{
		//		m_strNotifyInfo = _tl(TXT_HOLD_YOUR_CARD);
		//	}

		//	if(pSvc->m_strReadCardRsp != _T("")){
		//		m_strNotifyInfo = TXT_COLOR_RED + pSvc->m_strReadCardRsp;
		//	}

		//	if(pSvc->m_strReadCardRsp == _T("充值中途更换票卡")){
		//		m_strNotifyInfo = TXT_COLOR_RED + _T("不是同一张卡，请勿充值中途更换票卡！");
		//		m_btnOKAndCancel1->SetEnabled(false);
		//	}
		//}

	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		m_strNotifyInfo = _tl(TXT_GUIDE_PRINT_THE_BILL);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
		//m_strNotifyInfo = _T("交易失败！请收取错误凭证后，联系操作员。");	
		if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + _tl(TXT_CONTACT_OFFICER);
		}
		if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
			CString strTmp = _tl(TXT_GUIDE_GET_MONEY_AND_CONTECT);
			//strTmp.Format(_T("退款金额：%d，请收好您的现金。"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_CANCELLED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_CHANGE_EXCEEDS_THE_MAX_COUNT) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_SORRY_NOT_ENOUGH_CHANGE) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
			CString strTmp;
			strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_CANCELLED_TIME_OUT) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
			CString strTmp = _tl(TXT_CONTACT_OFFICER);
			//strTmp.Format(_T("退款金额：%d，请收好您的现金。"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
			CString strTmp = _tl(TXT_CONTACT_OFFICER);
			//strTmp.Format(_T("退款金额：%d，请收好您的现金。"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
			CString strTmp = _tl(TXT_CONTACT_OFFICER);
			//strTmp.Format(_T("退款金额：%d，请收好您的现金。"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
	}
	m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_CHARGE_AMOUNT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = _T("0元");// ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);
	index++;

	//************* 信息 ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	m_cashInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化提示区域信息

@param      无

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::InitializeGuideArea()
{
	int nIndex = 0;
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT)
	{
		m_guideInfo->detailList[nIndex].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
		m_guideInfo->detailList[nIndex++].bitInfo = _T("可接收纸币：50元、100元^本次最大可充值金额500元");
		m_guideInfo->tipsCount = nIndex;
		if (pSvc->GetDataModel().paymentInfo.lPaidAmount > 0)
		{
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("请确认充值金额，点击[充值]按钮或继续充值"));
		}
		else
		{
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("请插入纸币"));

		}

	}
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		m_guideInfoArea->renderGuideInfo();
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
//////////////////////////////////////////////////////////////////////////
int CChargeReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, NUMBER_AREA | CASH_AREA | GUIDE_AREA) == -1) {
		return -1;
	}
	//m_Left_1_Group->ShowButtons(false);
	//m_pTipsDlg->Create(_T(""), WS_CHILD, CRect(1140+50,850+30,1350+50,890+30), this, ID_TIPS_DLG);
	//m_pTitleDlg->Create(_T(""), WS_CHILD, CRect(520,40,1210,100), this, ID_TITLE_DLG);

	return 0;
}

void CChargeReceptionDlg::preShow()
{
	__super::preShow();

	//m_baseInfo->titleInfo.titleName = _tl(TXT_RECHARGE_DESCRIPTION);
	if (pSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT) {
		InitDataModel();
		HideCompoment(BTN_OK);
		//HideCompoment(BTN_PRINT);
	}
	else if(pSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		// 是否显示打印按钮
		if (theAFC_TVM_OPERATION.SupportPrintSwitch() && theTVM_STATUS_MGR.GetPrintMode() == 0){
			//ShowCompoment(BTN_PRINT);
			m_btnOKAndCancel1->SetEnabled(true);
		}
		else{
			m_btnOKAndCancel1->SetEnabled(false);
		}
	}
	else if(pSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){

	}
}

void CChargeReceptionDlg::showComplete()
{
	__super::showComplete();

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
void CChargeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CChargeSvc::DataModel& model = pSvc->GetDataModel();

	// 显示信息界面
	ShowCompoment(CASH_AREA);
	m_bShowBaseArea = true;	
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
	//m_pTitleDlg->SetWindowText(_tl(TXT_GUIDE_PUT_IN_NOTES_NOT_CHANGE));
	//m_pTitleDlg->ShowWindow(SW_SHOW);

	// 置灰按钮组
	m_Left_1_Group->EnableWindow(FALSE);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		theSERVICE_MGR.SetState(RECEPT);
		m_strNotifyInfo = _tl(TXT_HOLD_YOUR_CARD);
		pSvc->InsertRecvStartTimer();
		pSvc->StartReceiveMoney();// 开始接收钱或银行卡
		// 循环寻卡
		//pSvc->StartCircleReadCard();//只要检测不到卡，就返回到上一界面。

	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		LOG("in finish dialog group");
		theSERVICE_MGR.SetState(FINISH);
		pSvc->StopCircleReadCard();

		//// 显示返回按钮
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

		pSvc->NotifyDataModelChanged();
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
		LOG("in error dialog group");
		theSERVICE_MGR.SetState(FINISH);
		pSvc->StopCircleReadCard();	
		// 显示返回按钮
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

		pSvc->NotifyDataModelChanged();
	}

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      允许纸币显示文言

@param      无

@retval     CString

@exception  None
*/
//////////////////////////////////////////////////////////////////////////

CString CChargeReceptionDlg::BanknoteShowword() {
	vector<WORD> vecBanknotes;
	theAFC_TVM_OPERATION.GetAcceptBanknotesFaceOfRecharge(vecBanknotes);
	// 如果当前没有可接收的面额
	if (vecBanknotes.size() == 0){
		return _T("");
	}
	// 充值接收的纸币面额 BIN	单位：元
	CString banknote_showword = _T("")/*_tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES)+_T("\n")*/;
	int nCount = 0;
	for (vector<WORD>::iterator it = vecBanknotes.begin();it != vecBanknotes.end(); it++){
		nCount++;
		CString strTemp;
		strTemp.Format(_T("%d%s%s"),*it,_tl(MONEY_UNIT),_T(","));//10MOP,		
		banknote_showword += strTemp;
		//// 每3个换一行
		//if (nCount == 3){
		//	banknote_showword += _T("\n");
		//}
	}
	banknote_showword.TrimRight(_T(","));
	return banknote_showword;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      允许硬币类型显示文言

@param      无

@retval     CString

@exception  None
*/
//////////////////////////////////////////////////////////////////////////

CString CChargeReceptionDlg::CoinShowword() {

	COIN_ACCEPTABLE coinTable = pSvc->GetAcceptCoinFaceTable();
	CString coin_showword = _tl(TXT_RECHARGE_ACCEPTABLE_COINS)+_T("\n");
	CString temp;
	int nCount = 0;
	if(coinTable.CH_COIN_ALLOW_05){
		temp.Format(_T("50%s%s"),_tl(AVOS_UNIT),_T(","));
		coin_showword += temp;
		nCount++;
	}
	if (coinTable.CH_COIN_ALLOW_1){
		temp.Format(_T("1%s%s"),_tl(MONEY_UNIT),_T(","));
		coin_showword += temp;
		nCount++;
	}
	/*if(coinTable.CH_COIN_ALLOW_2){
	temp.Format(_T("2%s%s"),_tl(MONEY_UNIT),_T(","));
	coin_showword += temp;
	if(++nCount == 4){
	coin_showword += _T("\n");
	}
	}
	if(coinTable.CH_COIN_ALLOW_5){
	temp.Format(_T("5%s%s"),_tl(MONEY_UNIT),_T(","));
	coin_showword += temp;
	}*/
	//去掉末尾的逗号。
	coin_showword.TrimRight(_T(","));
	return coin_showword;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::UpdateUI()
{
	if(m_bShowBaseArea){
		InitializeBaseInfo();
		InitializeGuideArea();
		m_bShowBaseArea = false;
	}
	else{

	}
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::InitDataModel(){
	// 隐藏功能区按钮及语言显示按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	// 初始化状态位
	m_bIsShowFinshedInsertMoney = false;
	m_bIsShowCoinMaxCount		= false;
	m_bIsShowBanknoteMaxCount	= false;
	// 初始化按钮状态
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);

	//long component =  PRICE_BUTTON | NUMBER_BUTTON ;
	//InitializeFunctionBtns(component);
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		清空Label数据  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::ClearLabelData()
{
	// 初始化收银信息
	for(int n = 0;n < 4;n++){
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
 /**
 @brief      处理点击确定按钮(确定与取消按钮中的确定按钮）

 @param      (i)WPARAM wParam  消息信息，表示按钮的ID号
 @param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

 @retval     LRESULT  TRUE:处理成功  FALSE:处理失败

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnOkAndCancel1(WPARAM wParam, LPARAM lParam)
 {
	CChargeSvc::DataModel &model = pSvc->GetDataModel();
	HideCompoment(BTN_OK_AND_CANCEL1);
	HideCompoment(BTN_OK_AND_CANCEL2);

	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		 pSvc->EndRecvTimer();
		 //m_pTipsDlg->ShowWindow(SW_HIDE);
		 // 点击确认，则从储值票中扣款
		 LOG("进行充值，充值金额为%d", model.paymentInfo.lPaidAmount);
		 ////theSERVICE_MGR.SetForeServiceBusy(true);
		 // 停止循环寻卡
		 pSvc->StopCircleReadCard();
		 // 停止纸币接收,压钞票
		 pSvc->StopReceiveMoney();	
	 }
	 else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){	
		try{
			m_btnOKAndCancel1->SetEnabled(false);
			pSvc->PrinterPrintReceipt();
			model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS;
			m_strNotifyInfo = _T("票据打印完成");
			pSvc->NotifyDataModelChanged();
		 }
		 catch(CSysException& e){
			 model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
			 theEXCEPTION_MGR.ProcessException(e);
		 }
		 catch(...){

		 }
	 }
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
 LRESULT CChargeReceptionDlg::OnOkAndCancel2(WPARAM wParam, LPARAM lParam)
 {
	 HideCompoment(BTN_OK_AND_CANCEL1);
	 HideCompoment(BTN_OK_AND_CANCEL2);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		 // 如果此时，未投入现金，则返回总图界面；如果已经投入纸币，则退出现金
		 LOG("充值过程中界面，乘客点击取消按钮");
		 // 停止循环寻卡
		 pSvc->StopCircleReadCard();
		 pSvc->OnCancel();
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		 // 返回总图界面
		 pSvc->FlowToDefaultDlg();
	 }
	 //else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
	 // // 返回总图界面
	 // pSvc->FlowToDefaultDlg();
	 //}

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
 LRESULT CChargeReceptionDlg::OnNotifyCharge(WPARAM wParam,LPARAM lParam)
 {
	 CChargeSvc::DataModel &model = pSvc->GetDataModel();
	 LOG("OnNotifyIssuing::收到充值通知，充值金额为%d", model.paymentInfo.lPaidAmount);
	 // 发售车票并找零
	 //m_strNotifyInfo = _tl(GUIDE_ISSUING);
	 // 充值写卡
	 theSERVICE_MGR.SetState(ISSUEING);

	 long resultCode = -1;
	 while(pSvc->m_bCircleReading){//如果当前正好循环读卡接口未返回，则等待返回再写卡
		 Sleep(500);
	 }
	 resultCode = pSvc->DoCardCharge();
	 theSERVICE_MGR.SetState(RECEPT);
	 if(resultCode==0){
		 HideCompoment(BTN_OK_AND_CANCEL1);
		 HideCompoment(BTN_OK_AND_CANCEL2);
		 pSvc->ReturnFinishDlg();
	 }
	 else{
		 pSvc->ReturnFinishDlg();
	 }

	 return 0;
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
 HBRUSH	CChargeReceptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
 @brief      收到后台接受到现金的通知

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnNotifyAccepted(WPARAM wParam,LPARAM lParam)
 {
	LOG("OnNotifyAccepted::");
	CChargeSvc::DataModel& model = pSvc->GetDataModel();

	m_btnOKAndCancel1->SetEnabled(true);

	return 0;
 }

  //////////////////////////////////////////////////////////////////////////
 /**
 @brief      收到后台接受到现金的通知

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnUpdateGuide(WPARAM wParam,LPARAM lParam)
 {
	//CIssueSvc::Model& model = pSvc->GetDataModel();
	CString strTips = (TCHAR*)(wParam);
	pSvc->m_strReadCardRsp = strTips;
	pSvc->NotifyDataModelChanged();

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
 LRESULT CChargeReceptionDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	//CChargeSvc::DataModel& model = pSvc->GetDataModel();
	 BANKNOTE_HELPER->StopValidation();
	//// 返回总图界面
	 pSvc->FlowToDefaultDlg();
	//if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
	//	//// 如果此时，未投入现金，则返回总图界面；如果已经投入纸币，则退出现金
	//	//LOG("充值过程中界面，乘客点击取消按钮");
	//	//// 停止循环寻卡
	//	//pSvc->StopCircleReadCard();
	//	//pSvc->OnCancel();
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();

	//}

	return TRUE;
 }