#include "stdafx.h"
#include "IssueWithSvtDlg.h"
#include "BitmapManager.h"
#include "ParseStationMap.h"

// ����ڴ�й©
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

	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnTicketNumClick)	// ��Ʊ������ť

	ON_BTN_OK_CLICK(OnOK)										// ���ȡ����ť
	ON_BTN_OkAndCancel1_CLICK(OnOkAndCancel1)					// ���ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��
	ON_BTN_OkAndCancel2_CLICK(OnOkAndCancel2)					// ���ȡ����ť(ȷ����ȡ����ť�е�ȡ����ť��

	ON_MESSAGE(WM_NOTIFY_START_CIRCLE_READ, OnNotifyStartReadCard)	
	ON_MESSAGE(WM_START_ISSUING, OnStartIssue)		
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

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

	// �����·ѡ��ť
	m_BTN_Left_1_Group_Info.rows = 12;
	m_BTN_Left_1_Group_Info.cols = 1;

	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// �Ҳ�Ʊ�۰�ť
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
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:�����ɹ�    -1:����ʧ��

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

	// ��ʾ��Ϣ����


	return 0;
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
void CIssueWithSvtDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		theSERVICE_MGR.SetState(RECEPT);
		m_strNotifyInfo = _tl(TXT_PLEASE_INPUT_CARD);
		IO_HELPER->OpenBKCardIndicator();
		// ���Ϳ�ʼ������Ϣ
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
@brief      ������ʾ֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::preShow(){
	__super::preShow();

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SVT_INFORMATION);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		// ���ð�ť��
		long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
		InitializeFunctionBtns(component);
		// ���ð�ť��ѡ��
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
		// �ûҰ�ť��
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// �����ؼ�
		m_pTitleDlg->SetWindowText(_tl(TXT_SVT_ISSUE));
		m_pTitleDlg->ShowWindow(SW_SHOW);
		m_btnOKAndCancel1->SetSelect(false);
		m_btnOKAndCancel2->SetSelect(false);
		ShowCompoment(BTN_OK_AND_CANCEL1);
		ShowCompoment(BTN_OK_AND_CANCEL2);
		HideCompoment(BTN_OK);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_FINISH){
		// �ûҰ�ť��
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// ���ð�ťΪĬ����ʽ
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
		// �ûҰ�ť��
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// ���ð�ťΪĬ����ʽ
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
	// ��ʾ��Ϣ����
	InitializeBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::showComplete(){
	__super::showComplete();
	this->RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

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
@brief      ��Ʊ����ѡ��ť

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  ��
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
@brief		���Label����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::ClearLabelData()
{
	// ��ʼ��������Ϣ
	for(int n = 0;n < MAX_TICKET_INFO_ROWS;n++){
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
/*
@brief      ��ʼ��������Ϣ

@param      None

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithSvtDlg::InitializeBaseInfo()
{	
	// ��ʼ��Labelֵ
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	int index = 0;
	// ************* ��Ʊ��Ϣ ***************
	index = 0;
	// ��Ʒ����
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	index++;
	// ��Ʒ��Ч��
	m_TipsInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d"));
	index++;
	// ��ʼĿ�ĳ�վ/����
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);
	index++;		
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
	index++;
	// ��Ʒ����
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_ISSUE_AMOUNT)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
	m_TipsInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount) + _tl(MONEY_UNIT);
	index++;
	// ��������
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_COUNT)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN) ;
	m_TipsInfoGroup[index++].nameRight =  ComFormatCount(model.productInfo.nProductPiece) ;//+ _tl(TICKET_UNIT);GUI Ҫ��Ҫ��λ��zhengxianle
	index++;
	// Ӧ�����
	m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TipsInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
	index++;

	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		// ************* ��ֵ����Ϣ ***************
		if (pSvc->m_bReaderBad){// ѭ�����������У�tup����
			LOG("ѭ��������TPU���ϣ�������ʾ��");
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

	//************* ��Ϣ ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//m_baseInfoArea->Invalidate(FALSE);
	m_cashInfoArea->Invalidate(FALSE);
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
HBRUSH	CIssueWithSvtDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
 @brief      �յ���ʼ�����ֽ�֪ͨ

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnNotifyStartReadCard(WPARAM wParam,LPARAM lParam)
 {
	 // �����������е���ʱ��ʱ��
	 pSvc->InsertRecvStartTimer();
	 // ѭ��Ѱ��
	 //pSvc->StartCircleReadCard();

	 return 0;
 }


  //////////////////////////////////////////////////////////////////////////
 /**
 @brief      ��ʼ���۳�Ʊ

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnStartIssue(WPARAM wParam,LPARAM lParam)
 {
	 LOG("OnStartIssue::���۳�Ʊ������");
	 // ���۳�Ʊ������
	 pSvc->StartIssueAndChange(); 

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
 LRESULT CIssueWithSvtDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	 if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT){
		 // �����ʱ��OK��ť����ʾ�������ܽ���
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_FINISH){
		 // ������ͼ����
		 pSvc->FlowToDefaultDlg();
	 }
	 else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_SVT_ERROR){
		 // ������ͼ����
		 pSvc->FlowToDefaultDlg();
	 }

	 return TRUE;
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

	// ���ȷ�ϣ���Ӵ�ֵƱ�пۿ�
	LOG("��ֵƱ��Ʊ�����пۿ�");
	// ֹͣѭ��Ѱ��
	pSvc->StopCircleReadCard();

	// ��ʼ���ڿۿ�
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
 @brief      ������ȡ����ť(ȷ����ȡ����ť�е�ȷ����ť��

 @param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 @param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 @retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

 @exception  CSysException
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithSvtDlg::OnOkAndCancel2(WPARAM wParam, LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel(); 
	HideCompoment(BTN_OK_AND_CANCEL1);
	HideCompoment(BTN_OK_AND_CANCEL2);
	// �����ʱ��δͶ���ֽ��򷵻���ͼ���棻����Ѿ�Ͷ��ֽ�ң����˳��ֽ�
	LOG("��ֵƱ��Ʊ�����н��棬�˿͵��ȡ����ť");
	// ֹͣѭ��Ѱ��
	pSvc->StopCircleReadCard();
	pSvc->OnCancel();

	return TRUE;
 }