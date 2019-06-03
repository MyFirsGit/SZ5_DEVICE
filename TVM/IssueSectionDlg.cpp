#include "stdafx.h"
#include "IssueSectionDlg.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "CTIssuePermit.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CIssueSectionDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueSectionDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSelectSubProduct) // 子产品
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnBeginSectionClick)// 起始车站
	ON_GROUP_BUTTON_CLICK(RIGHT_3_GROUP_ID,OnEndSectionClick)  // 目的车站
	ON_BTN_BACK_CLICK(OnCancel)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

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
@brief      析构函数

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
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:创建成功    -1:创建失败

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
	// 隐藏按钮
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
void CIssueSectionDlg::preShow(){
	__super::preShow();
	m_btnOK->SetSelect(false);

	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	// 1.设置状态
	theSERVICE_MGR.SetState(RECEPT);
	// 2.隐藏功能按钮和语言选择按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	// 3.隐藏按钮
	//HideCompoment(BTN_OK);
	HideCompoment(BTN_OK);
	HideCompoment(BTN_PRINT);
	
	// 3.显示子产品
	InitializeComponents(SUB_PRODUCT_BUTTON);

	CIssueSvc::Model model = pSvc->GetDataModel();

	// 初始化提示操作信息
	m_guideInfo->tipsCount = 2;
	CString strTemp;
	if(model.productInfo.productType == SVT){
		strTemp = _tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_AMOUNT);//!!!请选择金额
	}
	else if(model.productInfo.productType == PST){
		strTemp =_tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_PERIOD_OD);//!!!请选择日期，起始区段，目的区段。
	}
	else if(model.productInfo.productType == MST){
		strTemp = _tl(TXT_MSG_GUIDE_SHOW_PLEASE_SELECT_RIDES_OD);//!!!请选择次数，起始区段，目的区段。
	}
	m_guideInfo->detailList[0].bitInfo =  strTemp;//Need change zhengxianle
	m_guideInfo->detailList[0].strfileName = TVM_GIF_OPERATION_GUIDE;

	m_guideInfo->detailList[1].bitInfo =  _tl(TXT_GUIDE_PLEASE_CLICK_CONFIRM);
	m_guideInfo->detailList[1].strfileName = TVM_TIP_CONFIRM_NORMAL;	
	ShowGuideMsg();

	// 4.默认选中第按钮:子产品
	if(!theAPP_SESSION.IsBlindManOperation()){// 盲人购票需要自己选择,默认不点击第一个按钮
		m_Right_1_Group->ClickFirstAvailButton();
	}
	else
		// 5.更新界面
		//pSvc->NotifyDataModelChanged();	
		UpdateUI();
	// 6.显示取消按钮,返回默认业务
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示后

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::showComplete(){
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
void CIssueSectionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      向导区提示信息

@param      无

@retval     无

@exception  无
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
@brief      服务数据模型发生变化时的更新界面   

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
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

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
		// 转到选择张数和支付方式的界面
		pSvc->DoneToNextDialog();
	}
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
LRESULT CIssueSectionDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	HideCompoment(BTN_OK);
	// 返回默认界面
	pSvc->FlowToDefaultDlg();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		子产品选择按钮响应消息  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueSectionDlg::OnSelectSubProduct(WPARAM wParam,LPARAM lParam){
	// 1.设置子产品
	theSERVICE_MGR.SetState(RECEPT);
	CIssueSvc::Model& model = pSvc->GetDataModel();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetSubProductType(_ttoi(btnInfo->btnMsg));
	// 2.非SVT需要重新设置站点区间
	if(model.productInfo.productType != SVT){
		long component = 0;
		component |= pSvc->NeedBeginSectionByPassenger() ? BEGIN_SECTION_BUTTON : 0;
		component |= pSvc->NeedEndSectionByPassenger() ? END_SECTION_BUTTON : 0;
		InitializeComponents(component);
		// 如果不需要乘客选择站点，直接计算票价
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
@brief      起始站点按钮点击响应事件

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
		// 计算票价
		pSvc->CalculatePrice();
	}
	else
	{
		pSvc->SetBeginStation(dwBegin);	
	}
	// 盲人购票显示按钮选中状态
	if(theAPP_SESSION.IsBlindManOperation()){
		m_Right_2_Group->SetSelection(btnInfo->btnMsg);
	}
	pSvc->NotifyDataModelChanged();
	// 显示确认按钮
	if(model.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED){
		ShowCompoment(BTN_OK);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      选择目的车站按钮点击响应事件

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
		// 计算票价
		pSvc->CalculatePrice();	

	}
	else{
		pSvc->SetEndStation(dwEnd);
	}
	// 盲人购票显示按钮选中状态
	if(theAPP_SESSION.IsBlindManOperation()){
		m_Right_3_Group->SetSelection(btnInfo->btnMsg);
	}
	pSvc->NotifyDataModelChanged();
	// 显示确认按钮
	if(model.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED){
		ShowCompoment(BTN_OK);
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化基本信息

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

	// 1.产品类型
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
	//// 2.子产品类型,非SVT
	//if(model.productInfo.productType != SVT){
	//	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_SUBTYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	//	m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language);
	//}
	// 3.有效期
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d")); 
	// 4.乘客类型
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PERSONAL_PASSENGER_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	// 王峰 重启无该参数 如何显示待确认20170227
	//m_TicketInfoGroup[index++].nameRight = theACC_BUSINESS.GetPassengerTypeName(PASSENGER_ADULT,language);
	// 5.起始车站、目的车站\区段
	if(model.productInfo.productType != SVT){
		// Begin
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) :CString(_T("丹鹤站")); /*theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);*///lichao:暂时修改，写死车站
		// End
		m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);
	}
	// 6.产品单价
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_SALE_PRICE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount)+ _tl(MONEY_UNIT); 

	// 7.押金
	m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_DEPOSIT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.mediumInfo.lCardDeposit)+ _tl(MONEY_UNIT); 

	// 8.税费
	m_TicketInfoGroup[index].nameLeft  = add_hsp(_tl(TXT_TAX_OF_ISSUE) + _tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT);

	// 清空其他行数据
	/*for(int i = index; i < MAX_TICKET_INFO_ROWS; ++i)
	{
		m_TicketInfoGroup[i].nameLeft =  _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}*/
	m_baseInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化功能区按钮

@param      (i)long lComponents  自定义组件信息

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
	// 1.子产品按钮
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
			//m_Right_2_Group->ShowButtons(true);//显示起始区段按钮组
			//m_Right_3_Group->ShowButtons(true);//显示目的区段按钮组
		}
		// SVT
		else{		
			vector<int> vecFareList;
//			theACC_PRODUCT.GetProductPrices(model.productInfo.productType, vecFareList);
			for (vector<int>::iterator iter = vecFareList.begin(); iter != vecFareList.end(); ++iter)
			{
				BTN_INFO btnInfo;
				btnInfo.btnMsg.Format(_T("%d"), *iter);										// 按钮通知消息：票价
				btnInfo.text.Format(_T("%s"), ComFormatAmount(*iter));						// 按钮显示文本：票价,不显示单位
				m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			}

			title = _tl(TXT_GUIDE_PLEASE_SELECT_AMOUNT) + pSvc->GetCurrentUnitOfProduct();
			
			//m_Right_2_Group->ShowButtons(false);//隐藏按钮
			//m_Right_3_Group->ShowButtons(false);//隐藏按钮
		}
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}
	else{
		// 盲人购票界面需要点击一下当前选择按钮
		if (theAPP_SESSION.IsBlindManOperation()){
			if(model.productInfo.productType != SVT){
				CString strBtnMsg;
				strBtnMsg.Format(_T("%d"),model.productInfo.productSubType);
				m_Right_1_Group->SetSelection(strBtnMsg);
			}
		}
	}
	// 2.起始区段按钮,非SVT
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
			//// 检查是否需要点击默认按钮
			//if (model.productInfo.beginLocation == (*iter).locationNumber){
			//	bIsNeedClickDefaultBtn = true;
			//}
		}
		m_Right_2_Group->ShowButtons(true);
		// 选择起始区段
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

	// 2.目的区段按钮,非SVT
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
		// 选择目的区段
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
@brief		清除Label数据  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSectionDlg::ClearLabelData(){
	// 初始化产品信息
	for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
		m_TicketInfoGroup[i].nameLeft  = _T("");
		m_TicketInfoGroup[i].nameRight = _T("");
	}
}