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
	ON_BTN_OK_CLICK(OnOK)										// ���ȡ����ť
	ON_BTN_BACK_CLICK(OnOK)
	ON_BTN_OkAndCancel1_CLICK(OnOkAndCancel1)					// ���ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��
	ON_BTN_OkAndCancel2_CLICK(OnOkAndCancel2)					// ���ȡ����ť(ȷ����ȡ����ť�е�ȡ����ť��
	ON_MESSAGE(WM_NOTIFY_ACCEPTED, OnNotifyAccepted)			// �յ�ֽ��֪ͨ
	ON_MESSAGE(WM_NOTIFY_ISSUING, OnNotifyCharge)				// �յ���ֵ֪ͨ	
	ON_MESSAGE(WM_UPDATE_RECEPITON_GUIDE, OnUpdateGuide)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
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
	m_cashInfo->titleInfo.titleName = _T("��ֵ����");
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_cashInfo->detailInfo.row = 3;
	m_cashInfo->size = CSize(1665,843);
	m_cashInfo->type = 1;
	m_bkgImage.Load(_T(".\\res\\TVM_center_charge_bkg.png"));
	m_pImageBack_center = &m_bkgImage;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ʼ��������Ϣ

@param      ��

@retval     ��

@exception  ��
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

	// ************* ��ֵ˵�� ***************
	CString strBanknoteTips = BanknoteShowword();	
	CString strCoinTips = ComFormatAmount(maxRemainValue) + _tl(MONEY_UNIT);

	// ************* ��ֵ����Ϣ ***************
	index = 0;
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CARD_BALANCE), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = _T("0Ԫ");//ComFormatAmount(model.productInfo.lBalanceAmount) + _tl(MONEY_UNIT);
	index++;

	 //�����ҵ������н��棬�Ҽ�⵽�������ߣ���ȥ�����濨��Ϣ���û�ȷ����ť
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		if(model.paymentInfo.lPaidAmount>0) {
			m_btnOKAndCancel1->SetEnabled(true);
		}
		if(model.paymentInfo.lChangeAmount>0) {
			m_btnOKAndCancel1->SetEnabled(false);
			//m_strNotifyInfo = _tl(TXT_INPUT_MOENY_OVERLIMIT);
		}

		if (pSvc->m_bReaderBad){// ѭ�����������У�tup����
			LOG("ѭ��������TPU���ϣ�������ʾ��");
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

		//	if(pSvc->m_strReadCardRsp == _T("��ֵ��;����Ʊ��")){
		//		m_strNotifyInfo = TXT_COLOR_RED + _T("����ͬһ�ſ��������ֵ��;����Ʊ����");
		//		m_btnOKAndCancel1->SetEnabled(false);
		//	}
		//}

	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		m_strNotifyInfo = _tl(TXT_GUIDE_PRINT_THE_BILL);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
		//m_strNotifyInfo = _T("����ʧ�ܣ�����ȡ����ƾ֤����ϵ����Ա��");	
		if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + _tl(TXT_CONTACT_OFFICER);
		}
		if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
			CString strTmp = _tl(TXT_GUIDE_GET_MONEY_AND_CONTECT);
			//strTmp.Format(_T("�˿��%d�����պ������ֽ�"), model.paymentInfo.lPaidAmount);
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
			//strTmp.Format(_T("�˿��%d�����պ������ֽ�"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
			CString strTmp = _tl(TXT_CONTACT_OFFICER);
			//strTmp.Format(_T("�˿��%d�����պ������ֽ�"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
		else if (model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
			CString strTmp = _tl(TXT_CONTACT_OFFICER);
			//strTmp.Format(_T("�˿��%d�����պ������ֽ�"), model.paymentInfo.lPaidAmount);
			m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
		}
	}
	m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_CHARGE_AMOUNT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = _T("0Ԫ");// ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);
	index++;

	//************* ��Ϣ ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	m_cashInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����ʾ������Ϣ

@param      ��

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
		m_guideInfo->detailList[nIndex++].bitInfo = _T("�ɽ���ֽ�ң�50Ԫ��100Ԫ^�������ɳ�ֵ���500Ԫ");
		m_guideInfo->tipsCount = nIndex;
		if (pSvc->GetDataModel().paymentInfo.lPaidAmount > 0)
		{
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("��ȷ�ϳ�ֵ�����[��ֵ]��ť�������ֵ"));
		}
		else
		{
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("�����ֽ��"));

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
             0:�����ɹ�    -1:����ʧ��

@exception  ��
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
		// �Ƿ���ʾ��ӡ��ť
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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CChargeSvc::DataModel& model = pSvc->GetDataModel();

	// ��ʾ��Ϣ����
	ShowCompoment(CASH_AREA);
	m_bShowBaseArea = true;	
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
	//m_pTitleDlg->SetWindowText(_tl(TXT_GUIDE_PUT_IN_NOTES_NOT_CHANGE));
	//m_pTitleDlg->ShowWindow(SW_SHOW);

	// �ûҰ�ť��
	m_Left_1_Group->EnableWindow(FALSE);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		theSERVICE_MGR.SetState(RECEPT);
		m_strNotifyInfo = _tl(TXT_HOLD_YOUR_CARD);
		pSvc->InsertRecvStartTimer();
		pSvc->StartReceiveMoney();// ��ʼ����Ǯ�����п�
		// ѭ��Ѱ��
		//pSvc->StartCircleReadCard();//ֻҪ��ⲻ�������ͷ��ص���һ���档

	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		LOG("in finish dialog group");
		theSERVICE_MGR.SetState(FINISH);
		pSvc->StopCircleReadCard();

		//// ��ʾ���ذ�ť
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

		pSvc->NotifyDataModelChanged();
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
		LOG("in error dialog group");
		theSERVICE_MGR.SetState(FINISH);
		pSvc->StopCircleReadCard();	
		// ��ʾ���ذ�ť
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

		pSvc->NotifyDataModelChanged();
	}

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ֽ����ʾ����

@param      ��

@retval     CString

@exception  None
*/
//////////////////////////////////////////////////////////////////////////

CString CChargeReceptionDlg::BanknoteShowword() {
	vector<WORD> vecBanknotes;
	theAFC_TVM_OPERATION.GetAcceptBanknotesFaceOfRecharge(vecBanknotes);
	// �����ǰû�пɽ��յ����
	if (vecBanknotes.size() == 0){
		return _T("");
	}
	// ��ֵ���յ�ֽ����� BIN	��λ��Ԫ
	CString banknote_showword = _T("")/*_tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES)+_T("\n")*/;
	int nCount = 0;
	for (vector<WORD>::iterator it = vecBanknotes.begin();it != vecBanknotes.end(); it++){
		nCount++;
		CString strTemp;
		strTemp.Format(_T("%d%s%s"),*it,_tl(MONEY_UNIT),_T(","));//10MOP,		
		banknote_showword += strTemp;
		//// ÿ3����һ��
		//if (nCount == 3){
		//	banknote_showword += _T("\n");
		//}
	}
	banknote_showword.TrimRight(_T(","));
	return banknote_showword;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ��������ʾ����

@param      ��

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
	//ȥ��ĩβ�Ķ��š�
	coin_showword.TrimRight(_T(","));
	return coin_showword;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
@brief      ��ʼ������ģ��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::InitDataModel(){
	// ���ع�������ť��������ʾ��ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	// ��ʼ��״̬λ
	m_bIsShowFinshedInsertMoney = false;
	m_bIsShowCoinMaxCount		= false;
	m_bIsShowBanknoteMaxCount	= false;
	// ��ʼ����ť״̬
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);

	//long component =  PRICE_BUTTON | NUMBER_BUTTON ;
	//InitializeFunctionBtns(component);
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		���Label����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeReceptionDlg::ClearLabelData()
{
	// ��ʼ��������Ϣ
	for(int n = 0;n < 4;n++){
		m_TipsInfoGroup[n].nameLeft  = _T("");
		m_TipsInfoGroup[n].nameRight = _T("");
	}
	// ��ʼ����Ʒ��Ϣ
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
 @brief      ������ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��

 @param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 @param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 @retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

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
		 // ���ȷ�ϣ���Ӵ�ֵƱ�пۿ�
		 LOG("���г�ֵ����ֵ���Ϊ%d", model.paymentInfo.lPaidAmount);
		 ////theSERVICE_MGR.SetForeServiceBusy(true);
		 // ֹͣѭ��Ѱ��
		 pSvc->StopCircleReadCard();
		 // ֹֽͣ�ҽ���,ѹ��Ʊ
		 pSvc->StopReceiveMoney();	
	 }
	 else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){	
		try{
			m_btnOKAndCancel1->SetEnabled(false);
			pSvc->PrinterPrintReceipt();
			model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS;
			m_strNotifyInfo = _T("Ʊ�ݴ�ӡ���");
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
 @brief      ������ȡ����ť(ȷ����ȡ����ť�е�ȷ����ť��

 @param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 @param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 @retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnOkAndCancel2(WPARAM wParam, LPARAM lParam)
 {
	 HideCompoment(BTN_OK_AND_CANCEL1);
	 HideCompoment(BTN_OK_AND_CANCEL2);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
		 // �����ʱ��δͶ���ֽ��򷵻���ͼ���棻����Ѿ�Ͷ��ֽ�ң����˳��ֽ�
		 LOG("��ֵ�����н��棬�˿͵��ȡ����ť");
		 // ֹͣѭ��Ѱ��
		 pSvc->StopCircleReadCard();
		 pSvc->OnCancel();
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH){
		 // ������ͼ����
		 pSvc->FlowToDefaultDlg();
	 }
	 //else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_ERROR){
	 // // ������ͼ����
	 // pSvc->FlowToDefaultDlg();
	 //}

	 return TRUE;
 }

 
   //////////////////////////////////////////////////////////////////////////
 /**
 @brief      �յ���̨���۳�Ʊ��֪ͨ

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnNotifyCharge(WPARAM wParam,LPARAM lParam)
 {
	 CChargeSvc::DataModel &model = pSvc->GetDataModel();
	 LOG("OnNotifyIssuing::�յ���ֵ֪ͨ����ֵ���Ϊ%d", model.paymentInfo.lPaidAmount);
	 // ���۳�Ʊ������
	 //m_strNotifyInfo = _tl(GUIDE_ISSUING);
	 // ��ֵд��
	 theSERVICE_MGR.SetState(ISSUEING);

	 long resultCode = -1;
	 while(pSvc->m_bCircleReading){//�����ǰ����ѭ�������ӿ�δ���أ���ȴ�������д��
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
 @brief      �ؼ���ɫ���������

 @param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 @param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 @retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 HBRUSH	CChargeReceptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
 {
	 HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	 if( ID_TIPS_DLG ==  pWnd->GetDlgCtrlID()){//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
		 CFont font;
		 font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			 CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			 DEFAULT_PITCH|FF_SWISS, _T("����"));

		 pDC->SelectObject(&font);
		 pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫΪ��ɫ
		 //pDC->SetBkMode(OPAQUE);//�����ı�����ģʽΪ͸��
		 //pDC->SetBkColor(RGB(0,0,255));//�����ı�����Ϊ��ɫ
		 //hbr=CreateSolidBrush(RGB(0,255,0));//�ؼ��ı���ɫΪ��ɫ
	 }
	 else if( ID_TITLE_DLG ==  pWnd->GetDlgCtrlID()){//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
		 CFont font;
		 font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE+16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			 CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			 DEFAULT_PITCH|FF_SWISS, _T("����"));

		 pDC->SelectObject(&font);
		 pDC->SetTextColor(RGB(0,0,0));//�����ı���ɫΪ��ɫ
	 }

	 return hbr;//���������ػ�ؼ������Ļ�ˢ
 }


   
 //////////////////////////////////////////////////////////////////////////
 /**
 @brief      �յ���̨���ܵ��ֽ��֪ͨ

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
 @brief      �յ���̨���ܵ��ֽ��֪ͨ

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
 @brief      ����ȷ�ϰ�ť��Ϣ

 @param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 @param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 @retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CChargeReceptionDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	//CChargeSvc::DataModel& model = pSvc->GetDataModel();
	 BANKNOTE_HELPER->StopValidation();
	//// ������ͼ����
	 pSvc->FlowToDefaultDlg();
	//if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_INIT){
	//	//// �����ʱ��δͶ���ֽ��򷵻���ͼ���棻����Ѿ�Ͷ��ֽ�ң����˳��ֽ�
	//	//LOG("��ֵ�����н��棬�˿͵��ȡ����ť");
	//	//// ֹͣѭ��Ѱ��
	//	//pSvc->StopCircleReadCard();
	//	//pSvc->OnCancel();
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();

	//}

	return TRUE;
 }