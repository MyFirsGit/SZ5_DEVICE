#include "stdafx.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "CTIssuePermit.h"
#include "IssueACCIniDlg.h"

// ����ڴ�й©
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
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPieceClick)   // ����ѡ��
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnPayTypeClick) // ֧����ʽ
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_PRINT_CLICK(OnPrint)
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCIniDlg::CIssueACCIniDlg(CService* pService)
: CReceptionDlg(CIssueACCIniDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
	
	// ����Ӧ���������ļ�����
	m_bShowOkButtonAndPayMented = theFunction_INFO.IsShowPaymentMethod();
	m_baseInfo->point = CPoint(5,0);
	m_baseInfo->stytle = PASSAGE_WIDE_STYLE;
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
	m_baseInfo->detailInfo.row = MAX_TICKET_INFO_ROWS;

	
	// �����Ҳ๦������ť�鰴ť����������m_Right_1_Group�Ȼ��ʼ��ʧ��
	//m_BTN_GROUP_INFO[0].rows = m_bShowOkButtonAndPayMented ? 3 : 12;
	//m_BTN_GROUP_INFO[0].cols = m_bShowOkButtonAndPayMented ? 3 : 1;
	// �����Ҳ๦������ť����⣬����m_Right_1_Group�Ȼ�û�б���߶�
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
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:�����ɹ�    -1:����ʧ��

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

	// ����ʱ���ذ�ť��
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::preShow()
{
	__super::preShow();

	// ���ð�ť��Ϊѡ��״̬
	m_btnOK->SetSelect(false);
	m_btnPrint->SetSelect(false);

	// 1.����״̬
	theSERVICE_MGR.SetState(RECEPT);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
	// 2.��ʼ�����ع�������ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);

	// 3.����OK��Print��ť
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	// 4.��ʼ����ť(ֻѡ������)
	if(m_bShowOkButtonAndPayMented){
		InitializeFunctionBtns(/*PASSAGETYPE|*/PAYTYPE|PIECE_BUTTON);
	}
	else{
		InitializeFunctionBtns(/*PASSAGETYPE|PAYTYPE|*/PIECE_BUTTON);
	}
	

	CIssueSvc::Model& model = pSvc->GetDataModel();
	// 5.��ʼ������
	if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_INIT)
	{
		m_bConfirmPayment = false;
		// �Ƿ���ʾ����ʾͶ����������ͼƬ����ֹ�ػ����ˢ�¡�
		m_bIsShowCoinMaxCount = false;
		m_bIsShowBanknoteMaxCount = false;
		m_bIsShowFinshedInsertMoney = false;

		/*-----------------------֣��ֱ��Ĭ��ֱ�ӿ�ʼ��Ǯ----------------------------*/
		if(m_bShowOkButtonAndPayMented){
			// ������ť
			m_Right_1_Group->ClickFirstAvailButton();
			if(!theAPP_SESSION.IsBlindManOperation() || model.productInfo.productType == SJT){
				// ֧����ʽ,����Ʊ��ѡ��֧����ʽ��
				m_Right_2_Group->ClickFirstAvailButton();
			}
		
			// ��ʾ������Ϣ
			m_guideInfo->tipsCount = 2;
			m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_PLEASE_SELECT_QUANTITY_PAYMENT);
			m_guideInfo->detailList[0].strfileName = TVM_GIF_OPERATION_GUIDE;

			m_guideInfo->detailList[1].bitInfo =  _tl(TXT_GUIDE_PLEASE_CLICK_CONFIRM);
			m_guideInfo->detailList[1].strfileName = TVM_TIP_CONFIRM_NORMAL;
		}
		else{
			// ��ʾ������Ϣ
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_PLEASE_SELECT_QUANTITY);// ��ѡ������
			m_guideInfo->detailList[0].strfileName = TVM_TIP_CONFIRM_BIG;
			
			m_Right_1_Group->ClickFirstAvailButton();
		}

		// ��������ѡ��ť
		if (m_Right_1_Group!=NULL && !m_Right_1_Group->IsWindowEnabled()){
			m_Right_1_Group->EnableWindow(TRUE);
			m_Right_1_Group->Invalidate();
			m_Right_1_Group->UpdateWindow();
		}

		// Ĭ��ѡ��һ��
		//m_Right_1_Group->ClickFirstAvailButton();

		// ��ʾ��Ǯ����
		
		model.mediumInfo.passengerType = PASSENGER_ADULT;

		// ��ʾȡ����ť
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		
		InitializeBaseInfo();
	}
	// 6.��ɽ���
	else if (pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_FINISH){
		// �������״̬
		// �ж�ֽ��Ѻ���Ƿ����
		if(model.lDeviceStatus == CPassengerForeSvc::DEVICE_ALL_OK/*model.operationInfo.banknoteAcceptState != CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_INSERTED*/){
			theSERVICE_MGR.SetState(FINISH);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		}
		// �Ƿ���ʾ��ӡ��ť(����ӡ)
		/*if (theAFC_TVM_OPERATION.SupportPrintSwitch()&& theTVM_STATUS_MGR.GetPrintMode() == 0){
		ShowCompoment(BTN_PRINT);			
		}*/	
		// �����ä�˹�Ʊ����ʾ���׳ɹ�,�Ƿ��ӡ?
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(AUDIO_BUSINESS_SUCCESS);
		}
		m_Right_1_Group->ShowButtons(FALSE);
		m_bShowOkButtonAndPayMented ? m_Right_2_Group->ShowButtons(FALSE) : NULL;

		// Token
		if(model.mediumInfo.cardPHType == CARD_TYPE_ACC_UL){
			// ������ʾ����ʾ��Ϣ
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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

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
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

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
		// �ֽ�֧������Ϊ������
		if (model.paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH && theSERVICE_MGR.GetState() == COUNTING){
			
			// �����Ӳ���쳣
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN){
				if(theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)){
					theTVM_STATUS_MGR.SetPaymentMode((theTVM_STATUS_MGR.GetPaymentMode() ^ DEVICE_CONTRL_CODE_PAYMENT_COIN | DEVICE_CONTRL_CODE_PAYMENT));
					theTVM_STATUS_MGR.SetChangeMode((theTVM_STATUS_MGR.GetChangeMode() ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE | DEVICE_CONTRL_CODE_NO_CHANGE));
					// ˢ�½���
					ShowGuideMsgOfPayInfo();
				}
			}

			// ���ֽ�Ҵﵽ�����������ʾֽ�Ҵﵽ�������
			if (pSvc->IsBanknoteMaxCount() && model.operationInfo.isBanknotesMaxCount && !m_bIsShowBanknoteMaxCount){
				ShowBanknoteMaxCount();
				m_bIsShowBanknoteMaxCount = true;
			}
			// ���Ӳ�Ҵﵽ�����������ʾӲ�Ҵﵽ���������
			if(pSvc->IsCoinMaxCount() && model.operationInfo.isCoinMaxCount && !m_bIsShowCoinMaxCount){
				ShowCoinMaxCount();
				m_bIsShowCoinMaxCount = true;
			}
		}

		// ����Ѿ�֧��Ǯ����ѡ��������ť����
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
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCIniDlg::OnOK(WPARAM wParam, LPARAM lParam)
{
	SYNC(ACCINIDLG_ONBUTTON_CLICK,_T("ACCINIDLG_ONBUTTON_CLICK"));
	// ��������
	if (theSERVICE_MGR.GetState() == RECEPT){
		HideCompoment(BTN_OK);
		/*m_Right_1_Group->ShowButtons(false);
		m_Right_1_Group->UpdateWindow();
		if(m_bShowOkButtonAndPayMented){
			m_Right_2_Group->ShowButtons(false);
			m_Right_2_Group->UpdateWindow();
		}*/
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);		
		ShowGuideMsgOfPayInfo();// ��ʾ֧��֧��Ӳ�ҡ�ֽ����ʾ��ʾ		
		pSvc->StartReceiveMoney();// ��ʼ����Ǯ�����п�
	}
	else if (theSERVICE_MGR.GetState() == COUNTING)	{
		InitializeBaseInfo();	// ǿ����ʾ����״̬
		if(NULL != m_baseInfoArea){
			m_baseInfoArea->UpdateWindow();
		}
		
		CIssueSvc::Model& model = pSvc->GetDataModel();
		HideCompoment(BTN_OK); // ����ȡ����ť��
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
@brief      ����ȡ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CIssueACCIniDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	// �����ť��ֱ������
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	// ���ҵ��û�н���Ӳ��������ֱ�ӷ���Ĭ�Ͻ���
	if (!model.operationInfo.isBeginBuyProduct){
		pSvc->FlowToDefaultDlg();
		return TRUE;
	}
	
	// �����ɣ�ֱ�ӷ���Ĭ��ҵ��
	if(theSERVICE_MGR.GetState() == FINISH){
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(0);
		}
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		pMainSvc->StartDefaultService();
		return TRUE;
	}
	// ������Ʊ��ɡ�
	//model.operationInfo.isSuccessIssueCard = true;

	if(theAPP_SESSION.IsBlindManOperation())
		pSvc->PlayAudioInDlg(AUDIO_BUSINESS_CANCEL);
	else	
		pSvc->OnCancel();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ�ۻ��Ӳ�Ʒ��ť��Ϣ��Ӧ��δʹ�ã�

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
@brief      ������ť��Ϣ��Ӧ  

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
@brief      ���ʽ�����Ӧ�¼�

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

	// ä�˹�Ʊ��ʾ��ťѡ��״̬
	if(theAPP_SESSION.IsBlindManOperation()){		
		if(payment == PAYMENT_METHOD_BANK_CARD /*|| payment == PAYMENT_METHOD_EFT*/){// ռʱ��֧�ִ�֧����ʽ
			pSvc->PlayAudioInDlg(AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT);
			return FALSE;
		}
		m_Right_2_Group->SetSelection(btnInfo->btnMsg);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ӡ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CIssueACCIniDlg::OnPrint(WPARAM wParam,LPARAM lParam)
{
	CIssueSvc::Model model = pSvc->GetDataModel();
	try{
		// ä�˳�ֵȷ�ϰ�ť���ʱ����ֹͣ��������
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(0);
		}
		// �������ڲ�����...
		theSERVICE_MGR.SetState(ISSUEING);
		HideCompoment(BTN_PRINT);
		// ���ڴ�ӡ��...
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_YOUR_CREDENTIALS_PRINTING);
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTING;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		pSvc->PrinterPrintIssueRecipt();

		// ��ӡ��ɣ����պ�ƾ�ݡ�
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_PRINT_IS_COMPLEATED)/*T("��ӡ��ɣ���ȡ������ƾ����")*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTED;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		// ���ô�ӡ״̬Ϊ�ɹ�
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS;
		// ä�˹�Ʊ��ӡ��
		if(theAPP_SESSION.IsBlindManOperation()){
			pSvc->PlayAudioInDlg(AUDIO_BUSINESS_PRINT_SUCCESS);// ��ӡ�ɹ�
		}
		// �������״̬������Ĭ��ҵ���ʱ����ʼ 
		theSERVICE_MGR.SetState(FINISH);
		return 0;
	}
	catch(CSysException& e){
		// ���ô�ӡʧ��״̬
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��������Ϣ

@param      None

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::InitializeBaseInfo()
{	
	// ��ʼ��Labelֵ
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	int index = 0;

	// BugHere 1967
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_CASH_INFO);

	// 1.��Ʒ����
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName((model.productInfo.productType<<8 | model.productInfo.productSubType), language);
	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	//m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
	index++;

	// 2.�Ӳ�Ʒ����:PST,MST
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
	// 3.��Ʒ��Ч��
	m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d"));
	index++;

	// 4.�˿�����
	//m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PERSONAL_PASSENGER_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = language == LANGUAGE_ID_CHINESE ? _T("����") : _T("Adult");//theACC_BUSINESS.GetPassengerTypeName(model.mediumInfo.passengerType, language);
	
	// 5.��ʼĿ�ĳ�վ/����
	if(model.productInfo.productType == SJT ){
		// ����Ʊ
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
		// �ƴ�Ʊ������Ʊ
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_SECTION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
/*		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*/	
		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) :CString(_T("����վ")); /*theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*///lichao:��ʱ�޸ģ�д����վ
		index++;		

		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_SECTION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
		index++;

	}

	// 6.��Ʒ����
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_ISSUE_AMOUNT)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount) + _tl(MONEY_UNIT);
	index++;
	
	// 7.Ѻ�𣨿��˿���or �ɱ��ѣ������˿���
	if(model.mediumInfo.cardPHType == CARD_TYPE_ACC_M1)
	{		
		// �ǵ���Ʊ
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_DEPOSIT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardDeposit) + _tl(MONEY_UNIT);
		//m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_CARD_FEE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;//�ɱ���
		//m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardFee) + _tl(MONEY_UNIT);	
		index++;

	}

	// 9.˰��
	//m_TicketInfoGroup[index].nameLeft  = add_hsp(_tl(TXT_TAX_OF_ISSUE) + _tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	//m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT);

	// 8.��������
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_COUNT)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN) ;
	m_TicketInfoGroup[index++].nameRight =  ComFormatCount(model.productInfo.nProductPiece) ;//+ _tl(TICKET_UNIT);GUI Ҫ��Ҫ��λ��zhengxianle
	index++;
	index+=2;
	if(!m_bShowOkButtonAndPayMented){
		// 9.Ӧ�����
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
		index++;

		// 10.ʵ�ʸ�����
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_INCOME) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);
		index++;

		// 11.������
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CHANGE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lChangeAmount >=0 ? model.paymentInfo.lChangeAmount : 0) + _tl(MONEY_UNIT);
		index++;

	}

	m_baseInfoArea->Invalidate(FALSE);


	if(m_bShowOkButtonAndPayMented){
		// ֧����ʽ
		m_cashInfo->detailInfo.labelGroup[0].nameLeft = add_hsp(_tl(TXT_PAYMENT_METHOD) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[0].nameRight = _tl(TXT_PAYMENT_CASH);/*theACC_BUSINESS.GetPayTypeName(model.paymentInfo.paymentMethod, language)*/;
		// Ӧ�����
		m_cashInfo->detailInfo.labelGroup[1].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[1].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
		// ʵ�����
		m_cashInfo->detailInfo.labelGroup[2].nameLeft = add_hsp(_tl(TXT_INCOME) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);	
		// ������
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
	
	// ���ͶǮ����ʾ��ʾ��Ϣ��
	if(model.operationInfo.isFinishedReceveMoney &&(pSvc->GetCurDialogGroupID() != CIssueSvc::DIALOG_GROUP_FINISH) && !m_bIsShowFinshedInsertMoney)
	{
		if(theFunction_INFO.IsNeedPassengerClickOkButton()){
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_CONFIRM_AFTER_PAY/*TXT_GUIDE_PLEASE_CLICK_CONFIRM*/);
			m_guideInfo->detailList[0].strfileName = TVM_TIP_CONFIRM_BIG;

			InitializeGuideArea();
			m_guideInfoArea->renderGuideInfo();
		}
		
		// �����ä�˹�Ʊ����ʾ���׳ɹ�,�Ƿ��ӡ?
		if(theAPP_SESSION.IsBlindManOperation()){
			//CMessagePictureBox::Show(_T("��ʾ"),_T("��ȷ��֧����Ϣ!"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			// ֧�����ȷ��
			pSvc->PlayAudioInDlg(AUDIO_CONFORM_PAYINFO);
		}
		// ִֻ��һ��
		m_bIsShowFinshedInsertMoney = true;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����������ť

@param      (i)long lComponents  �Զ��������Ϣ

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::InitializeFunctionBtns(long lComponents)
{
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	
	// �˿����Ͱ�ť
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
			title = _T("��ѡ��˿�����");
		}
		else
		{
			title = _T("Please select passenger type");
		}
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}*/

	// 1.������ť
	if(lComponents&PIECE_BUTTON)
	{
		m_vec_Right_1_BTN_INFO.clear();
		// �����Ʊ����
		int nMaxCount = 0;
		CAfcTVMOperationParam::MAGAZINE_PARAM cardBoxParam;
		theAFC_TVM_OPERATION.GetMagazineParam(cardBoxParam);
	
		CTCardCountInfo::CARD_BOX_INFO card_Box_A = theCARD_COUNT.GetCardboxAInfo();
		CTCardCountInfo::CARD_BOX_INFO card_Box_B = theCARD_COUNT.GetCardboxBInfo();

		int nMaxCardASaleCount = 0,nMaxCardBSaleCount = 0;// ʵ�ʿ������Ʊ��

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
		uiInfo.m_Font=theGUI_INFO.GetBtnFont();	//Ӣ������ᷢ���仯
	}
		
	// 2.֧����ʽ��ť
	if(lComponents & PAYTYPE)
	{
		m_vec_Right_2_BTN_INFO.clear();
		vector<WORD>& vecPayment = pSvc->GetIssuePayment();
		for (vector<WORD>::iterator iter = vecPayment.begin(); iter != vecPayment.end(); ++iter)
		{
			// ���Ϊä�˹�Ʊ��ȥ��EP֧��������еĻ���
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
		uiInfo.m_Font=theGUI_INFO.GetBtnFont();	//Ӣ������ᷢ���仯
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����ʾ������Ϣ

@param      (i)long lComponents  �Զ��������Ϣ

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
@brief		��ʾ֧����Ϣ    

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowGuideMsgOfPayInfo(){
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH)
	{
		// ����֧����ʾ��Ϣ
		int index = 0;
		// ֽ��
		if (!pSvc->BanknoteShowword().IsEmpty())
		{
			// �жϵ�ǰģʽ�Ƿ�֧��ֽ��Ͷ�룿
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
				m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_BANKTOTES);
				m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
			}
			else{
				m_guideInfo->detailList[index].bitInfo =  pSvc->BanknoteShowword();
				m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
			}
		}
		// ��ʾ��֧�ִ˱���
		else{
			m_guideInfo->detailList[index].bitInfo =  _tl(GUIDE_CURRENT_NOT_RECIVE_BANKTOTES);
			m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
		}
		// Ӳ��
		if (!pSvc->CoinShowword().IsEmpty())
		{
			// ��֧��Ӳ��Ͷ�룿
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
		// �����ä�˹�Ʊ
		if(theAPP_SESSION.IsBlindManOperation()){
			LOG("��ʾͶ���ֽ�.");
			//CMessagePictureBox::Show(_T("��ʾ"),_T("��Ͷ���ֽ�Ӳ��֧��5MOP��2MOP��1MOP��50AVOS��ֽ��֧��10MOP��20MOP��50MOP��100MOP��500MOP��1000MOP��"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			// ��Ͷ���ֽ�Ӳ��֧��5MOP��2MOP��1MOP��50AVOS��ֽ��֧��10MOP��20MOP��50MOP��100MOP��500MOP��1000MOP��
			pSvc->PlayAudioInDlg(AUDIO_INSERT_CASH);
			LOG("��ʾ��ɡ�");
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
		m_guideInfo->detailList[0].bitInfo =  _T("��ʹ�����п�");
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_BANKCARD;
		// �����ä�˹�Ʊ
		if(theAPP_SESSION.IsBlindManOperation()){
			// ��������п�
			pSvc->PlayAudioInDlg(AUDIO_INSERT_BANKCARD);
		}
	}
	// ���ع��ܰ�ť
	//m_Right_1_Group->ShowButtons(false);
	//m_bShowOkButtonAndPayMented ? m_Right_2_Group->ShowButtons(false) : NULL;
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��֤�û������Ƿ���ȷ 

@param      None

@retval     bool  true:������Ϣ�Ϸ�  false:�������ݲ��Ϸ�

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueACCIniDlg::ValidateInput()
{
	//CIssueSvc::Model &model = pSvc->GetDataModel();

	//// ������Ϣ����Ϣ���
	//// ��վ��Ϣ��Ч�Լ�� ��Ԥ����
	//if(theISSUE_PERMIT.IsPermitIssueStation(model.BegainStation))
	//{
	//	CMessageBox::Show(theAPP_SESSION.GetServiceName(ISSUE_SVC),_T("��վ���Ʒ���"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
	//	return FALSE;
	//}

	//if( theISSUE_PERMIT.IsPermitIssueStation(pSvc->GetDataModel().EndStation))
	//{
	//	CMessageBox::Show(theAPP_SESSION.GetServiceName(ISSUE_SVC),_T("���Ʒ��۸ó�վ��Ʊ"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
	//	return FALSE;
	//}

	//	//��Ϣ��Ч�Լ��
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
@brief		���Label����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ClearLabelData(){
	// ��ʼ����Ʒ��Ϣ
	for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
		m_TicketInfoGroup[i].nameLeft  = _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}
	// ��ʼ��������Ϣ
	for(int n = 0;n < 4;n++){
		m_cashInfo->detailInfo.labelGroup[n].nameLeft = _T("");
		m_cashInfo->detailInfo.labelGroup[n].nameRight = _T("");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʾֽ������������ʾ��Ϣ  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowBanknoteMaxCount(){
	// ������ʾ����ʾ��Ϣ
	LOG("��ʾֽ�������ﵽ���ֵ��");
	int index = 0;
	if (!pSvc->BanknoteShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = _tl(GUIDE_CURRENT_RECIVE_MAX_BANKNOTES); // ��ʾ��ʾ����
		m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
	}
	if (!pSvc->CoinShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = pSvc->CoinShowword();				 // ��ʾ֧�ֵ�Ӳ�ҵı���
		m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_COIN;
	}
	m_guideInfo->tipsCount = index;	
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʾӲ�ҳ�����Ϣ  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCIniDlg::ShowCoinMaxCount(){
	// ������ʾ����ʾ��Ϣ
	int index = 0;
	if (!pSvc->BanknoteShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo = pSvc->BanknoteShowword();             // ��ʾ֧�ֵ�ֽ�ұ���
		m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
	}
	if (!pSvc->CoinShowword().IsEmpty()){
		m_guideInfo->detailList[index].bitInfo =_tl(GUIDE_CURRENT_REVICE_MAX_COINS); // ��ʾ��ʾ����				 // ��ʾ֧�ֵ�Ӳ�ҵı���
		m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
	}
	m_guideInfo->tipsCount = index;	
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}
