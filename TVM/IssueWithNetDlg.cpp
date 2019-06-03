#include "stdafx.h"
#include "IssueWithNetDlg.h"
#include "BitmapManager.h"
#include "ParseStationMap.h"

// ����ڴ�й©
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

	ON_BTN_OK_CLICK(OnOK)										// ���ȡ����ť

	ON_MESSAGE(WM_NOTIFY_ISSUING, OnNotifyIssuing)	
	ON_MESSAGE(WM_START_ISSUING, OnStartIssue)	
	ON_MESSAGE(WM_NOTIFY_ERWEIMA_ORDER_RES, OnNotifyErweimaResult)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

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

	// �����·ѡ��ť
	m_BTN_Left_1_Group_Info.rows = 9;
	m_BTN_Left_1_Group_Info.cols = 1;

	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// �Ҳ�Ʊ�۰�ť
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
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:�����ɹ�    -1:����ʧ��

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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

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
		// ������ά�붩������
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
@brief      ������ʾ֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithNetDlg::preShow(){
	__super::preShow();

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_cashInfo->titleInfo.titleName = _tl(TXT_SCAN_TOW_DIMENSIONAL_PAYMENT);

	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET){
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

		int adjustNum = 0;
		if(model.productInfo.nProductPiece>7){
			adjustNum = 7;
			m_Left_1_Group->ClickButton(7);//��һҳ��ť
		}
		vector<int> vecCountList;
		vecCountList = pSvc->GetTicketCountTable();
		for (int i=0;i<vecCountList.size();i++){
			if(vecCountList[i]==model.productInfo.nProductPiece){
				m_Left_1_Group->ClickButton(i-adjustNum);
			}
		}
		// �ûҰ�ť��
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		// ���ð�ťΪĬ����ʽ
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
		ShowCompoment(BTN_OK);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_FINISH){
		// �ûҰ�ť��
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
		// �ûҰ�ť��
		m_Left_1_Group->EnableButtonGroup(FALSE);
		m_Right_1_Group->EnableButtonGroup(FALSE);
		m_Left_1_Group->UpdateWindow();
		m_Right_1_Group->UpdateWindow();
		m_btnOK->SetText(_tl(STR_BACK));
		m_btnOK->SetSelect(false);
		m_btnOK->SetEnabled(true);
		ShowCompoment(BTN_OK);
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
void CIssueWithNetDlg::showComplete(){
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
void CIssueWithNetDlg::UpdateUI()
{
	InitializeBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		���Label����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueWithNetDlg::ClearLabelData()
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
void CIssueWithNetDlg::InitializeBaseInfo()
{	
	// ��ʼ��Labelֵ
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	// ************* ��Ʊ��Ϣ ***************
	int index = 0;
	// ����Ʊ
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

	// ************* --��Ϣ ***************

	//************* ��ʾ��Ϣ ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//************* ��Ϣ ***************
	index = 0;
	m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
	index++;

	//m_cashInfoArea->SetTitleText((pSvc->m_ePayType==CIssueSvc::PAY_WITH_WEIXIN?_T("��ɨ��΢�Ŷ�ά��"):_T("��ɨ��֧������ά��")));
	m_baseInfoArea->Invalidate(FALSE);
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
HBRUSH	CIssueWithNetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
 @brief      �յ���̨���۳�Ʊ��֪ͨ

 @param      (i)UINT nIDEvent  

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 LRESULT CIssueWithNetDlg::OnNotifyIssuing(WPARAM wParam,LPARAM lParam)
 {
	 LOG("OnNotifyIssuing::�յ���Ʊ������֪ͨ");
	 pSvc->EndRecvTimer();
	 // ���۳�Ʊ������
	 m_strNotifyInfo = _tl(GUIDE_ISSUING);
	 HideCompoment(BTN_OK);
	 m_btnOK->SetEnabled(false);
	 pSvc->NotifyDataModelChanged();

	 PostMessage(WM_START_ISSUING,0,0);
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
 LRESULT CIssueWithNetDlg::OnStartIssue(WPARAM wParam,LPARAM lParam)
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
 LRESULT CIssueWithNetDlg::OnOK(WPARAM wParam, LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET){
		 LOG("��ά�빺Ʊ�����н��棬�˿͵��ȡ����ť");
		 if (model.operationInfo.finishType != CPassengerForeSvc::QR_ORDER_GENERATE_FAILED){
			 pSvc->EndQueryOrderTimer();
			 pSvc->CancelOrder();
		 }
		 pSvc->OnCancel();
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_FINISH){
		// ������ͼ����
		pSvc->FlowToDefaultDlg();
	}
	else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_NET_ERROR){
		// ������ͼ����
		pSvc->FlowToDefaultDlg();
	}

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
 LRESULT CIssueWithNetDlg::OnNotifyErweimaResult(WPARAM wParam,LPARAM lParam)
 {
	CIssueSvc::Model& model = pSvc->GetDataModel();
	 RETURN_CODE retCode = (RETURN_CODE)wParam;
	 LOG("OnNotifyErweimaResult::�յ���ά�붩��������:%d", retCode);
	 ShowCompoment(BTN_OK);
	 if(retCode==RSP_CODE_OK){
		 m_strNotifyInfo = _tl(TXT_SCAN_60S);
		 // ������ѯ������ʱ��
		 pSvc->InsertQueryOrderTimer();
	 }
	 else{
		m_strNotifyInfo = _tl(TXT_CONNECT_SERVER_FAILURE);
		model.operationInfo.finishType = CPassengerForeSvc::QR_ORDER_GENERATE_FAILED;
	 }
	 pSvc->NotifyDataModelChanged();

	 return TRUE;
 }