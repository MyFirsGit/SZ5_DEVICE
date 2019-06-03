#include "stdafx.h"
#include "IssueSectionDlg.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "CTIssuePermit.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CIssueSectionDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueSectionDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSelectSubProduct) // �Ӳ�Ʒ
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnBeginSectionClick)// ��ʼ��վ
	ON_GROUP_BUTTON_CLICK(RIGHT_3_GROUP_ID,OnEndSectionClick)  // Ŀ�ĳ�վ
	ON_BTN_BACK_CLICK(OnCancel)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueSectionDlg::CIssueSectionDlg(CService* pService)
: CReceptionDlg(CIssueSectionDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->detailInfo.row = MAX_TICKET_INFO_ROWS;
	m_baseInfo->stytle = PASSAGE_NARROW_STYLE;
	m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;

	m_BTN_GROUP_INFO[0].rows = 3;
	m_BTN_GROUP_INFO[0].cols = 3;
	m_BTN_GROUP_INFO[1].rows = 4;
	m_BTN_GROUP_INFO[1].cols = 3;
	m_BTN_GROUP_INFO[2].rows = 4;
	m_BTN_GROUP_INFO[2].cols = 3;

	m_BTN_GROUP_INFO[0].title = _T("");
	m_BTN_GROUP_INFO[1].title = _T("");
	m_BTN_GROUP_INFO[2].title = _T("");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueSectionDlg::~CIssueSectionDlg()
{
	if(NULL != m_guideInfoArea){
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
int CIssueSectionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, BASE_AREA|BTN_OK) == -1) {
		return -1;
	}
	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea->createWnd(this);
	// ���ذ�ť
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
void CIssueSectionDlg::preShow(){
	__super::preShow();
	m_btnOK->SetSelect(false);

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	// 1.����״̬
	theSERVICE_MGR.SetState(RECEPT);
	// 2.���ع��ܰ�ť������ѡ��ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	// 3.���ذ�ť
	//HideCompoment(BTN_OK);
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	
	// 3.��ʾ�Ӳ�Ʒ
	InitializeComponents(SUB_PRODUCT_BUTTON);

	CIssueSvc::Model model = pSvc->GetDataModel();

	// ��ʼ����ʾ������Ϣ
	m_guideInfo->tipsCount = 2;
	CString strTemp;
	if(model.productInfo.productType == SVT){
		strTemp = _tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_AMOUNT);//!!!��ѡ����
	}
	else if(model.productInfo.productType == PST){
		strTemp =_tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_PERIOD_OD);//!!!��ѡ�����ڣ���ʼ���Σ�Ŀ�����Ρ�
	}
	else if(model.productInfo.productType == MST){
		strTemp = _tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_RIDES_OD);//!!!��ѡ���������ʼ���Σ�Ŀ�����Ρ�
	}
	m_guideInfo->detailList[0].bitInfo =  strTemp;//Need change zhengxianle
	m_guideInfo->detailList[0].strfileName = TVM_GIF_OPERATION_GUIDE;

	m_guideInfo->detailList[1].bitInfo =  _tl(TXT_GUIDE_PLEASE_CLICK_CONFIRM);
	m_guideInfo->detailList[1].strfileName = TVM_TIP_CONFIRM_NORMAL;	
	ShowGuideMsg();

	// 4.Ĭ��ѡ�еڰ�ť:�Ӳ�Ʒ
	if(!theAPP_SESSION.IsBlindManOperation()){// ä�˹�Ʊ��Ҫ�Լ�ѡ��,Ĭ�ϲ������һ����ť
		m_Right_1_Group->ClickFirstAvailButton();
	}
	else
		// 5.���½���
		//pSvc->NotifyDataModelChanged();	
		UpdateUI();
	// 6.��ʾȡ����ť,����Ĭ��ҵ��
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::showComplete(){
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
void CIssueSectionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ��Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::ShowGuideMsg(){

	if(NULL != m_guideInfoArea ){
		if(theFunction_INFO.IsNeedPassengerClickOkButton())
			m_guideInfoArea->setGuideInfo(m_guideInfo);
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::UpdateUI()
{
	InitializeBaseInfo();
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
LRESULT CIssueSectionDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	HideCompoment(BTN_OK);
	theSERVICE_MGR.SetState(RECEPT);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if (model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED
		&& model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED || model.productInfo.productType == SVT)
	{
		// ת��ѡ��������֧����ʽ�Ľ���
		pSvc->DoneToNextDialog();
	}
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
LRESULT CIssueSectionDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	HideCompoment(BTN_OK);
	// ����Ĭ�Ͻ���
	pSvc->FlowToDefaultDlg();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ӳ�Ʒѡ��ť��Ӧ��Ϣ  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueSectionDlg::OnSelectSubProduct(WPARAM wParam,LPARAM lParam){
	// 1.�����Ӳ�Ʒ
	theSERVICE_MGR.SetState(RECEPT);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetSubProductType(_ttoi(btnInfo->btnMsg));
	// 2.��SVT��Ҫ��������վ������
	if(model.productInfo.productType != SVT){
		long component = 0;
		component |= pSvc->NeedBeginSectionByPassenger() ? BEGIN_SECTION_BUTTON : 0;
		component |= pSvc->NeedEndSectionByPassenger() ? END_SECTION_BUTTON : 0;
		InitializeComponents(component);
		// �������Ҫ�˿�ѡ��վ�㣬ֱ�Ӽ���Ʊ��
		if (component == 0 || model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED && model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED)
		{
			pSvc->CalculatePrice();
		}
	}	
	else{
		ShowCompoment(BTN_OK);
	}
	pSvc->NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼվ�㰴ť�����Ӧ�¼�

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     LRESULT

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueSectionDlg::OnBeginSectionClick(WPARAM wParam, LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CIssueSvc::Model& model = pSvc->GetDataModel();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD dwBegin = _ttol(btnInfo->btnMsg);
	if (dwBegin != model.productInfo.beginLocation && model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED || 
		model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED && model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED)
	{
		pSvc->SetBeginStation(dwBegin);	
		// ����Ʊ��
		pSvc->CalculatePrice();
	}
	else
	{
		pSvc->SetBeginStation(dwBegin);	
	}
	// ä�˹�Ʊ��ʾ��ťѡ��״̬
	if(theAPP_SESSION.IsBlindManOperation()){
		m_Right_2_Group->SetSelection(btnInfo->btnMsg);
	}
	pSvc->NotifyDataModelChanged();
	// ��ʾȷ�ϰ�ť
	if(model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED){
		ShowCompoment(BTN_OK);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѡ��Ŀ�ĳ�վ��ť�����Ӧ�¼�

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueSectionDlg::OnEndSectionClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CIssueSvc::Model& model = pSvc->GetDataModel();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD dwEnd = _ttol(btnInfo->btnMsg);
	if (dwEnd != model.productInfo.endLocation && model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED || 
		model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED && model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED)
	{
		pSvc->SetEndStation(dwEnd);
		// ����Ʊ��
		pSvc->CalculatePrice();	

	}
	else{
		pSvc->SetEndStation(dwEnd);
	}
	// ä�˹�Ʊ��ʾ��ťѡ��״̬
	if(theAPP_SESSION.IsBlindManOperation()){
		m_Right_3_Group->SetSelection(btnInfo->btnMsg);
	}
	pSvc->NotifyDataModelChanged();
	// ��ʾȷ�ϰ�ť
	if(model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED){
		ShowCompoment(BTN_OK);
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��������Ϣ

@param      None

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::InitializeBaseInfo()
{	
	//m_baseInfoArea->SetStyle();
	//m_cashInfoArea->SetStyle();
	ClearLabelData();
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	int index = 0;

	// 1.��Ʒ����
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	//// 2.�Ӳ�Ʒ����,��SVT
	//if(model.productInfo.productType != SVT){
	//	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_SUBTYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
	//}
	// 3.��Ч��
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d")); 
	// 4.�˿�����
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PERSONAL_PASSENGER_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	// ���� �����޸ò��� �����ʾ��ȷ��20170227
	//m_TicketInfoGroup[index++].nameRight = theACC_BUSINESS.GetPassengerTypeName(PASSENGER_ADULT,language);
	// 5.��ʼ��վ��Ŀ�ĳ�վ\����
	if(model.productInfo.productType != SVT){
		// Begin
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) :CString(_T("����վ")); /*theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*///lichao:��ʱ�޸ģ�д����վ
		// End
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);
	}
	// 6.��Ʒ����
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_SALE_PRICE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount)+ _tl(MONEY_UNIT); 

	// 7.Ѻ��
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_DEPOSIT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardDeposit)+ _tl(MONEY_UNIT); 

	// 8.˰��
	m_TicketInfoGroup[index].nameLeft  = add_hsp(_tl(TXT_TAX_OF_ISSUE) + _tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT);

	// �������������
	/*for(int i = index; i < MAX_TICKET_INFO_ROWS; ++i)
	{
		m_TicketInfoGroup[i].nameLeft =  _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}*/
	m_baseInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����������ť

@param      (i)long lComponents  �Զ��������Ϣ

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::InitializeComponents(long lComponents)
{
	CIssueSvc::Model& model = pSvc->GetDataModel();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	typedef CAccLocationParam::LOCATION LOCATION;
	vector<LOCATION> vecStations;
	theACC_LOCATION.GetAreaInfo(vecStations);
	// 1.�Ӳ�Ʒ��ť
	if (lComponents & SUB_PRODUCT_BUTTON)
	{
		m_vec_Right_1_BTN_INFO.clear();
		CString title;
		// MST or PST
		if(model.productInfo.productType != SVT){
			ACC_PRODUCT_t product;
			/*theACC_PRODUCT.GetProduct(model.productInfo.productType, product);
			vector<ACC_SUBPRODUCT_t>::iterator iter = product.vecSubProduct.begin();
			for (; iter != product.vecSubProduct.end(); ++iter)	{
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), (*iter).productTypeVariants);
			btnInfo.text = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, (*iter).productTypeVariants, language);
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			}*/
			//CString title;
			if (model.productInfo.productType == PST){
				title = _tl(TXT_GUIDE_PLEASE_SELECT_PERIOD) + pSvc->GetCurrentUnitOfProduct();
			}
			else if(model.productInfo.productType == MST){
				title = _tl(TXT_GUIDE_PLEASE_SELECT_RIDES) + pSvc->GetCurrentUnitOfProduct();
			}
			//m_Right_2_Group->ShowButtons(true);//��ʾ��ʼ���ΰ�ť��
			//m_Right_3_Group->ShowButtons(true);//��ʾĿ�����ΰ�ť��
		}
		// SVT
		else{		
			vector<int> vecFareList;
//			theACC_PRODUCT.GetProductPrices(model.productInfo.productType, vecFareList);
			for (vector<int>::iterator iter = vecFareList.begin(); iter != vecFareList.end(); ++iter)
			{
				BTN_INFO btnInfo;
				btnInfo.btnMsg.Format(_T("%d"), *iter);										// ��ť֪ͨ��Ϣ��Ʊ��
				btnInfo.text.Format(_T("%s"), ComFormatAmount(*iter));						// ��ť��ʾ�ı���Ʊ��,����ʾ��λ
				m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			}

			title = _tl(TXT_GUIDE_PLEASE_SELECT_AMOUNT) + pSvc->GetCurrentUnitOfProduct();
			
			//m_Right_2_Group->ShowButtons(false);//���ذ�ť
			//m_Right_3_Group->ShowButtons(false);//���ذ�ť
		}
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}
	else{
		// ä�˹�Ʊ������Ҫ���һ�µ�ǰѡ��ť
		if (theAPP_SESSION.IsBlindManOperation()){
			if(model.productInfo.productType != SVT){
				CString strBtnMsg;
				strBtnMsg.Format(_T("%d"),model.productInfo.productSubType);
				m_Right_1_Group->SetSelection(strBtnMsg);
			}
		}
	}
	// 2.��ʼ���ΰ�ť,��SVT
	if(lComponents & BEGIN_SECTION_BUTTON )
	{
		m_vec_Right_2_BTN_INFO.clear();
		bool bIsNeedClickDefaultBtn = false;
		for (vector<LOCATION>::iterator iter = vecStations.begin(); iter != vecStations.end(); ++iter)
		{
			//BTN_INFO btnInfo;
			//btnInfo.btnMsg.Format(_T("%d"), (*iter).locationNumber);
			//btnInfo.text = theACC_LOCATION.GetLocationName((*iter).locationNumber, language);
			//m_vec_Right_2_BTN_INFO.push_back(btnInfo);
			//// ����Ƿ���Ҫ���Ĭ�ϰ�ť
			//if (model.productInfo.beginLocation == (*iter).locationNumber){
			//	bIsNeedClickDefaultBtn = true;
			//}
		}
		m_Right_2_Group->ShowButtons(true);
		// ѡ����ʼ����
		m_Right_2_Group->SetButtonTitle(_tl(TXT_GUIDE_PLEASE_SELECT_ORIGIN));
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		if (bIsNeedClickDefaultBtn){
			CString strBtnMsg;
			strBtnMsg.Format(_T("%d"),model.productInfo.beginLocation);
			m_Right_2_Group->ClickButton(strBtnMsg);
		}
		else
		{
			if (!theAPP_SESSION.IsBlindManOperation()){
				m_Right_2_Group->ClickFirstAvailButton();
			}			
		}
	}
	else
	{
		m_vec_Right_2_BTN_INFO.clear();
		m_Right_2_Group->SetButtonTitle(_T(""));
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_2_Group->ShowButtons(false);
	}

	// 2.Ŀ�����ΰ�ť,��SVT
	if(lComponents & BEGIN_SECTION_BUTTON)
	{
		m_vec_Right_3_BTN_INFO.clear();
		bool bIsNeedClickDefaultBtn = false;
		for (vector<LOCATION>::iterator iter = vecStations.begin(); iter != vecStations.end(); ++iter)
		{
			/*BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), (*iter).locationNumber);
			btnInfo.text = theACC_LOCATION.GetLocationName((*iter).locationNumber, language);
			m_vec_Right_3_BTN_INFO.push_back(btnInfo);
			if (model.productInfo.endLocation == (*iter).locationNumber){
			bIsNeedClickDefaultBtn = true;
			}*/
		}
		m_Right_3_Group->ShowButtons(true);
		// ѡ��Ŀ������
		m_Right_3_Group->SetButtonTitle(_tl(TXT_GUIDE_PLEASE_SELECT_DESINATION));
		m_Right_3_Group->SetButtonInfos(m_vec_Right_3_BTN_INFO);
		if (bIsNeedClickDefaultBtn){
			CString strBtnMsg;
			strBtnMsg.Format(_T("%d"),model.productInfo.endLocation);
			m_Right_3_Group->ClickButton(strBtnMsg);
		}
		else{
			if(!theAPP_SESSION.IsBlindManOperation()){
				m_Right_3_Group->ClickFirstAvailButton();
			}
		}
	}
	else
	{		
		m_vec_Right_3_BTN_INFO.clear();
		m_Right_3_Group->SetButtonTitle(_T(""));
		m_Right_3_Group->SetButtonInfos(m_vec_Right_3_BTN_INFO);	
		m_Right_3_Group->ShowButtons(false);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		���Label����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::ClearLabelData(){
	// ��ʼ����Ʒ��Ϣ
	for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
		m_TicketInfoGroup[i].nameLeft  = _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}
}