#include "stdafx.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include "functioninfo.h"
#include "ProductIssueReceptionDlg.h"
#include "ProductIssueSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CProductIssueReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CProductIssueReceptionDlg,CReceptionDlg)
	ON_WM_ACTIVATE()
    ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,On1GroupBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,On2GroupBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_3_GROUP_ID,On3GroupBtnClick)
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_PRINT_CLICK(OnPrint)
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CProductIssueReceptionDlg::CProductIssueReceptionDlg(CService* pService)
    :CReceptionDlg(CProductIssueReceptionDlg::IDD, pService)
	,m_CurDlg(0)
{
	pSvc = dynamic_cast<CProductIssueSvc*>(pService);
	language_mode = theAPP_SESSION.GetCurrentClientLanguageType();

	m_baseInfo->titleInfo.titleName = _tl(TXT_PRODUCT_ISSUE);                    // 标题
	m_baseInfo->stytle = PASSAGE_NARROW_STYLE;
	m_baseInfo->detailInfo.row = MAX_ISSUE_PRODUCT_INFO_ROWS;
	m_baseInfo->detailInfo.labelGroup = m_ProductIssueInfoGroup;	

	// 这里是绘制按钮区域子区域的大小。因为这是supergroup ReceptionDlg的变量，所以要在构造函数中定义。
	// Dlg的构造函数比Svc的构造函数先执行。因而在这里仍不知道DialogGroup，所以如其它函数的惯用方法，
	// 在这里直接InitializeFunctionBtns()并不可行。这同时意味着，在一个Dlg中，Btn_group的区域大小不能修改。
	m_BTN_GROUP_INFO[0].rows = 4;
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

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CProductIssueReceptionDlg::~CProductIssueReceptionDlg()
{
	pSvc = NULL;
	delete pSvc;

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
void CProductIssueReceptionDlg::InitializeBaseInfo()
{	
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();
	m_baseInfo->titleInfo.titleName = _tl(TXT_PRODUCT_ISSUE);
	m_cashInfo->titleInfo.titleName = _tl(TXT_CASH_INFO);

	int row = 0;
	// 产品类型
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language_mode);
	// 过期日期
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_ANALYZE_EXPIRE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d")).Left(10);
	// 钱包产品
	if (model.productInfo.productCategory == PURSE) {
		// 充值金额
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_TRANSACTION_AMOUNT)+ _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.productInfo.lBalanceAmount)
			/*theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language_mode)*/ + _tl(MONEY_UNIT);
	}
	// 计次产品
	else if (model.productInfo.productCategory == MULTIRIDE){
		// 起始区段
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_START_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language_mode);

		// 目的区段
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_END_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language_mode);

		//// 购买次数
		//m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PURCHASE_RIDES) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
		//m_ProductIssueInfoGroup[row++].nameRight = theACC_PRODUCT.(model.productInfo.productType, model.productInfo.productSubType, language_mode) + + _tl(TIMES_UNIT);

		//单价
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PRODUCT_SALE_AMOUNT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount/*lPriceAmount*//*lProductSaleAmount*/);

	}
	// 定期产品
	else if (model.productInfo.productCategory == PERIOD)	{
		// 起始区段
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_START_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language_mode);

		// 目的区段
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_END_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language_mode);

		////购买期限
		//m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PURCHASE_TIMES) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		//m_ProductIssueInfoGroup[row++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language_mode) + _tl(DAY_CNT_UNIT);
		
		//单价
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PRODUCT_SALE_AMOUNT) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);

	}
	// 稅費
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_TAX_OF_ISSUE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT);

	/*================================================================================================================
	
	// 票卡子类型
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_PRODUCT_SUBTYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = theACC_PRODUCT.GetSubProductName(model.productInfo.productType, model.productInfo.productSubType, language_mode);

	if (model.productInfo.productCategory != PURSE) {
		// 区段起点
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_START_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language_mode);

		// 区段终点
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_END_SECTION) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language_mode);
	}

	// 有效期
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d")).Left(10);

	// SVT初始金额
	if (model.productInfo.productCategory == PURSE) {
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_TICKET_BALANCE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.productInfo.lBalanceAmount) + _tl(MONEY_UNIT);
	}

	// MST初始次数
	if (model.productInfo.productCategory == MULTIRIDE) {
		m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_tl(TXT_REMAIN_RIDES) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
		m_ProductIssueInfoGroup[row++].nameRight.Format(_T("%d%s"), model.productInfo.lBalanceRides, _tl(TIMES_UNIT));
	}

	// 应收金额
	m_ProductIssueInfoGroup[row].nameLeft = add_hsp(_T("票价") + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
	m_ProductIssueInfoGroup[row++].nameRight = ComFormatAmount(model.paymentInfo.lPriceAmount) + _tl(MONEY_UNIT);
	*/

	m_ProductIssueInfoGroup[row].nameLeft = TXT_NOTHING;
	m_ProductIssueInfoGroup[row++].nameRight = TXT_NOTHING;

	m_ProductIssueInfoGroup[row].nameLeft = TXT_NOTHING;
	m_ProductIssueInfoGroup[row++].nameRight = TXT_NOTHING;

	m_ProductIssueInfoGroup[row].nameLeft = TXT_NOTHING;
	m_ProductIssueInfoGroup[row++].nameRight = TXT_NOTHING;
	
	m_baseInfoArea->Invalidate(FALSE);
	
	// 支付方式
	m_cashInfo->detailInfo.labelGroup[0].nameLeft = add_hsp(_tl(TXT_PAYMENT_METHOD)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	m_cashInfo->detailInfo.labelGroup[0].nameRight = _T("");//theACC_BUSINESS.GetPayTypeName(model.paymentInfo.paymentMethod, theAPP_SESSION.GetCurrentClientLanguageType());zhengxianle
	// 应收金额
	m_cashInfo->detailInfo.labelGroup[1].nameLeft = add_hsp(_tl(TXT_TOTAL)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	m_cashInfo->detailInfo.labelGroup[1].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount)+_tl(MONEY_UNIT);	
	// 实收
	m_cashInfo->detailInfo.labelGroup[2].nameLeft = add_hsp(_tl(TXT_INCOME)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount)+_tl(MONEY_UNIT);	
	// 如果支付方式为现金
	if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
		m_cashInfo->detailInfo.labelGroup[3].nameLeft = add_hsp(_tl(TXT_CHANGE)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);
		m_cashInfo->detailInfo.labelGroup[3].nameRight = ComFormatAmount(model.paymentInfo.lChangeAmount >=0 ? model.paymentInfo.lChangeAmount : 0)+_tl(MONEY_UNIT);
	}
	else{
		m_cashInfo->detailInfo.labelGroup[3].nameLeft = _T("");
		m_cashInfo->detailInfo.labelGroup[3].nameRight = _T("");
	}
	
	m_cashInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化功能区按钮

@param      (i)long lComponents  自定义组件信息

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueReceptionDlg::InitializeFunctionBtns(long lComponents)
{
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();
	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_INIT) {
		// 票卡类型按钮
		if(lComponents & PRODUCTTYPE)	
		{	
			m_vec_Right_1_BTN_INFO.clear();
			m_BTN_GROUP_INFO[0].title = _tl(TXT_RECHARGE_SELECT_PRODUCTTYPE);
			map<WORD,CString> availableProductNames = pSvc->GetAvailableProductNames();
			map<WORD,CString>::iterator it;
			for (it = availableProductNames.begin();it!= availableProductNames.end();it++)
			{
				BTN_INFO btnInfo;
				btnInfo.btnID = RIGHT_1_GROUP_ID;
				btnInfo.btnMsg.Format(_T("%u"), static_cast<unsigned short>(it->first));
				btnInfo.text = it->second;
				m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			}
			if(m_Right_1_Group!=NULL){
				m_Right_1_Group->ShowButtons(true);
				m_Right_1_Group->SetButtonTitle(m_BTN_GROUP_INFO[0].title);
				m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
				m_Right_1_Group->ClickFirstAvailButton();
				m_Right_1_Group->Invalidate();
			}
		}

		// 票卡子类型按钮
		if(lComponents & SUBPRODUCTTYPE)	
		{	
			m_vec_Right_2_BTN_INFO.clear();
			//m_BTN_GROUP_INFO[1].title = _T("請選擇子產品類型（MOP）")/*_tl(TXT_RECHARGE_SELECT_PRODUCTSUBTYPE)*/;
			
			// 储值票情形，直接显示价钱
			if (model.productInfo.productCategory == PURSE) {
				m_BTN_GROUP_INFO[1].title = _tl(TXT_SELECT_AMOUNT)/*T("請選擇子產品類型（MOP）")*/;
				vector<int> vecFareList;
//				theACC_PRODUCT.GetProductPrices(model.productInfo.productType, vecFareList);zhengxianle
				for(unsigned int i=0; i<vecFareList.size(); i++){
					BTN_INFO btnInfo;
					btnInfo.btnID = RIGHT_2_GROUP_ID;
					btnInfo.btnMsg.Format(_T("%d"),vecFareList[i]);
					btnInfo.text.Format(_T("%s"),ComFormatAmount(vecFareList[i])/*,_tl(MONEY_UNIT)*/);
					m_vec_Right_2_BTN_INFO.push_back(btnInfo);
				}
			}
			// 其它情形，显示子产品名称
			else {
				if (model.productInfo.productCategory == PERIOD) // 定期类产品
				{
					m_BTN_GROUP_INFO[1].title = _tl(TXT_SELECT_PERIOD);
				}
				else if (model.productInfo.productCategory == MULTIRIDE) // 计次类产品
				{
					m_BTN_GROUP_INFO[1].title = _tl(TXT_SELECT_RIDES);
				}
				// 取可售的产品类型
				map<WORD,CString> availableSubproductNames = pSvc->GetAvailableSubproductNames();
				map<WORD,CString>::iterator it;
				// 解析，将会显示在右侧第二个按钮组
				for (it = availableSubproductNames.begin();it!= availableSubproductNames.end();it++)
				{
					BTN_INFO btnInfo;
					btnInfo.btnID = RIGHT_2_GROUP_ID;
					btnInfo.btnMsg.Format(_T("%u"), static_cast<unsigned short>(it->first));
					btnInfo.text = it->second;
					m_vec_Right_2_BTN_INFO.push_back(btnInfo);
				}
			}
			if(m_Right_2_Group!=NULL){
				m_Right_2_Group->ShowButtons(true);
				m_Right_2_Group->SetButtonTitle(m_BTN_GROUP_INFO[1].title);
				m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
				m_Right_2_Group->ClickFirstAvailButton();
				m_Right_2_Group->Invalidate();
			}
		}
	}
	else if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) {// 选择区段界面
		//  支付方式
		m_vec_Right_1_BTN_INFO.clear();
		m_BTN_GROUP_INFO[0].title = _tl(TXT_RECHARGE_SELECT_PAYMENT_METHOD);
		map<WORD,CString> availablePayTypeList = theTICKET_HELPER.GetAvailablePayTypeList(ISSUE_PRODUCT_SVC, model.mediumInfo.cardPHType);
		map<WORD,CString>::iterator it;
		for (it = availablePayTypeList.begin();it!= availablePayTypeList.end();it++){
			BTN_INFO btnInfo;
			btnInfo.btnID = RIGHT_1_GROUP_ID;
			btnInfo.btnMsg.Format(_T("%d"), static_cast<unsigned short>(it->first));
			btnInfo.text = it->second;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}
		if(m_Right_1_Group!=NULL){
			m_Right_1_Group->ShowButtons(true);
			m_Right_1_Group->SetButtonTitle(m_BTN_GROUP_INFO[0].title);
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
			m_Right_1_Group->ClickFirstAvailButton();
			m_Right_1_Group->Invalidate();
		}
		// 钱包产品不显示区段选择
		if (model.productInfo.productCategory == PURSE) 
		{
			HideCompoment(m_Right_2_Group);
			return;
		}else{
			ShowCompoment(m_Right_2_Group);
		}

	
		// 起始区段按钮
		if(lComponents & BEGINLOCATION)	
		{	
			m_vec_Right_2_BTN_INFO.clear();
			m_BTN_GROUP_INFO[1].title = _tl(TXT_START_SECTION);
			map<DWORD,CString> availableLocationNames = pSvc->GetAvailableLocationNames();
			map<DWORD,CString>::reverse_iterator it;
			for (it = availableLocationNames.rbegin();it!= availableLocationNames.rend();it++)
			{
				BTN_INFO btnInfo;
				btnInfo.btnID = RIGHT_2_GROUP_ID;
				btnInfo.btnMsg.Format(_T("%ld"), static_cast<long>(it->first));
				btnInfo.text = it->second;
				m_vec_Right_2_BTN_INFO.push_back(btnInfo);
			}
			if(m_Right_2_Group!=NULL){
				m_Right_2_Group->ShowButtons(true);
				m_Right_2_Group->SetButtonTitle(m_BTN_GROUP_INFO[1].title);
				m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
				m_Right_2_Group->ClickFirstAvailButton();
				m_Right_2_Group->Invalidate();
			}
		}

		// 目的区段按钮
		if(lComponents & ENDLOCATION)	
		{	
			m_BTN_GROUP_INFO[2].title = _tl(TXT_END_SECTION);
			map<DWORD,CString> availableLocationNames = pSvc->GetAvailableLocationNames();
			map<DWORD,CString>::reverse_iterator it;
			for (it = availableLocationNames.rbegin();it!= availableLocationNames.rend();it++)
			{
				BTN_INFO btnInfo;
				btnInfo.btnID = RIGHT_3_GROUP_ID;
				btnInfo.btnMsg.Format(_T("%ld"), static_cast<long>(it->first)); //因为btnInfo.btnMsg
				btnInfo.text = it->second;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化提示区域信息

@param      无

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueReceptionDlg::InitializeGuideArea()
{
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		//m_guideInfoArea->Invalidate(FALSE);
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
int CProductIssueReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, BASE_AREA|CASH_AREA|BTN_PRINT|BTN_OK) == -1) {
		return -1;
	}

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea->createWnd(this);
	language_mode = theAPP_SESSION.GetCurrentClientLanguageType();
	theSERVICE_MGR.SetState(RECEPT);
	// 初始化隐藏功能按钮区
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
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
void CProductIssueReceptionDlg::preShow(){
	__super::preShow();
	CProductIssueSvc::DataModel &model = pSvc->GetDataModel();
	language_mode = theAPP_SESSION.GetCurrentClientLanguageType();
	// 设置按钮不为选中状态
	m_btnOK->SetSelect(false);
	m_btnPrint->SetSelect(false);

	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_INIT ||
		pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) 
	{
		HideCompoment(BTN_PRINT);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	}
	// 初始界面：选择产品类型
	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_INIT) {
		m_guideInfo->tipsCount = 2;												// 向导提示区： 选择 + 确认
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_PLEASE_SELECT_PRODUCT_PAYMENT_SVT);
		m_guideInfo->detailList[0].strfileName = TVM_GIF_OPERATION_GUIDE;

		m_guideInfo->detailList[1].bitInfo =  _tl(TXT_GUIDE_PLEASE_CLICK_CONFIRM);
		m_guideInfo->detailList[1].strfileName = TVM_TIP_CONFIRM_NORMAL;
		// 如果存在可售产品
		if(pSvc->GetAvailableProductNames().size() > 0)						// 存在可添加的产品才可继续操作
		{
			ShowCompoment(BTN_OK);
		}else
			HideCompoment(BTN_OK);
		// 钱包类产品，显示现金区域
		if (model.productInfo.productCategory == PURSE)						// 产品选择界面 -- 钱包产品 -- 显示现金信息。
		{
			ShowCompoment(m_cashInfoArea);
		}else
			HideCompoment(m_cashInfoArea);// 隐藏现金区域
		InitializeFunctionBtns(PRODUCTTYPE);
	}
	else if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) {
		if (model.productInfo.productCategory == PURSE)						// 区段&支付方式选择界面 -- 钱包产品 -- 隐藏区段选择
		{
		}else{
		}	
		ShowCompoment(m_cashInfoArea);										// 区段&支付方式选择界面 -- 恢复现金信息区显示
		InitializeFunctionBtns(BEGINLOCATION|ENDLOCATION);

	}
	// 交易完成界面
	if(pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_FINISH){
		if(model.operationInfo.banknoteAcceptState != CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_INSERTED){
			theSERVICE_MGR.SetState(FINISH);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		}
		HideCompoment(BTN_OK);
		// 判断当前是否支持打印
		if (theAFC_TVM_OPERATION.SupportPrintSwitch()&& theTVM_STATUS_MGR.GetPrintMode() == 0){
			ShowCompoment(BTN_PRINT);
		}			

		HideCompoment(m_Right_1_Group);										// 充值成功界面	--	右侧按钮区 -- 隐藏
		HideCompoment(m_Right_2_Group);

		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_ADD_PRODUCT_HAS_FINISHED);
		m_guideInfo->detailList[0].strfileName = TVM_TIP_RECHARGE_FINISHED;
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示后

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueReceptionDlg::showComplete(){
	__super::showComplete();
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
void CProductIssueReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按钮响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::On1GroupBtnClick(WPARAM wParam, LPARAM lParam)
{	
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_INIT) {
		return OnProductTypeBtnClick(wParam, lParam);
	}
	else if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) {
		return OnPayTypeClick(wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按钮响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::On2GroupBtnClick(WPARAM wParam, LPARAM lParam)
{	
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_INIT) {
		return OnProductSubTypeBtnClick(wParam, lParam);
	}
	else if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) {
		return  OnProductBeginLocationBtnClick(wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      右侧第三按钮组消息响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::On3GroupBtnClick(WPARAM wParam, LPARAM lParam)
{
	
		CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	if (pSvc->GetCurDialogGroupID()==CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION) {
			return OnProductEndLocationBtnClick(wParam, lParam);
		}
	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      产品按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnProductTypeBtnClick(WPARAM wParam, LPARAM lParam)
{	
	theSERVICE_MGR.SetState(RECEPT);
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();
	
	// 对点击事件修改Svc model
	BTN_INFO* btnInfo = reinterpret_cast<BTN_INFO*>(lParam);
	WORD productType = static_cast<WORD>(_ttoi(btnInfo->btnMsg.Left(2)));
	model.productInfo = CProductIssueSvc::PRODUCT_INFO(); // 重置productInfo里的内容
	model.productInfo.productType = productType;
	model.productInfo.productCategory = static_cast<ProductCategory_t>(theACC_PRODUCT.GetTicketCategry(productType));
	if (model.productInfo.productCategory == PURSE) // 如果是钱包信息才可在INIT界面上显示票价信息
	{
		ShowCompoment(m_cashInfoArea);
	}else
		HideCompoment(m_cashInfoArea);
	InitializeFunctionBtns(SUBPRODUCTTYPE);
	pSvc->NotifyDataModelChanged();
	m_baseInfoArea->Invalidate();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      子产品按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnProductSubTypeBtnClick(WPARAM wParam, LPARAM lParam)
{	
	theSERVICE_MGR.SetState(RECEPT);
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	// 对点击事件修改Svc model
	BTN_INFO* btnInfo = reinterpret_cast<BTN_INFO*>(lParam);
	WORD productType = model.productInfo.productType;
	WORD subproductType;
	if (model.productInfo.productCategory == PURSE) {
		// 储值票情形，只有一种subproduct，从vecTicketProduct遍历得出
		map<WORD,CString> availableSubproductNames = pSvc->GetAvailableSubproductNames();
		map<WORD,CString>::iterator it = availableSubproductNames.begin();
		subproductType = it->first;
	}
	else { 
		subproductType = static_cast<WORD>(_ttoi(btnInfo->btnMsg.Left(2)));
		//model.issueProductInfo.subproductType = subproductType;
	}

	model.productInfo.productSubType = subproductType;

	// 更新新买的产品信息productInfo
	if (model.productInfo.productCategory == PURSE) {
		model.productInfo.lBalanceAmount = static_cast<DWORD>(_ttoi(btnInfo->btnMsg));
		pSvc->CalculatePrice(); // 储值票情形即计算票价
	}
	else if (model.productInfo.productCategory == MULTIRIDE) {
		model.productInfo.lBalanceRides = 0;//theACC_PRODUCT.GetProductMultiRideTicketCount(productType, subproductType);zhengxianle
		model.paymentInfo.lPriceAmount = 0; // 储值票定期票因为未选取段无法计算票价，此界面先将其置0
	}
	else if (model.productInfo.productCategory == PERIOD) {
		model.productInfo.lValidatePeriod = 0;//theACC_PRODUCT.GetProductValidatePeriod(productType, subproductType);zhengxianle
		model.paymentInfo.lPriceAmount = 0;
	}
//	model.productInfo.dtValidateDateTime = theACC_PRODUCT.GetProductValidateDate(productType, subproductType);zhengxianle

	pSvc->NotifyDataModelChanged();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      区段按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnProductBeginLocationBtnClick(WPARAM wParam, LPARAM lParam)
{	
	theSERVICE_MGR.SetState(RECEPT);
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	// 对点击事件修改Svc model
	BTN_INFO* btnInfo = reinterpret_cast<BTN_INFO*>(lParam);
	model.productInfo.beginLocation = static_cast<DWORD>(_ttol(btnInfo->btnMsg));
	pSvc->CalculatePrice();

	pSvc->NotifyDataModelChanged();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      区段按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnProductEndLocationBtnClick(WPARAM wParam, LPARAM lParam)
{	
	theSERVICE_MGR.SetState(RECEPT);
	CProductIssueSvc::DataModel& model = pSvc->GetDataModel();

	// 对点击事件修改Svc model
	BTN_INFO* btnInfo = reinterpret_cast<BTN_INFO*>(lParam);
	model.productInfo.endLocation = static_cast<DWORD>(_ttol(btnInfo->btnMsg));
	pSvc->CalculatePrice();

	pSvc->NotifyDataModelChanged();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      支付方式按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnPayTypeClick(WPARAM wParam,LPARAM lParam)
{	
	CProductIssueSvc::DataModel &model = pSvc->GetDataModel();

	// 对点击事件修改Svc model
	BTN_INFO* btnInfo = reinterpret_cast<BTN_INFO*>(lParam);
	model.paymentInfo.paymentMethod = static_cast<PaymentMethod_t>(_ttoi(btnInfo->btnMsg));

	//if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
	//	// 设置提示区显示信息
	//	m_guideInfo->tipsCount = 2;

	//	m_guideInfo->detailList[0].bitInfo = BanknoteShowword();
	//	m_guideInfo->detailList[0].strfileName =TVM_IMAGE_TIP_INSERT_NOTE/*_T(".\\res\\acceptable_banknotes.png")*/;

	//	m_guideInfo->detailList[1].bitInfo = CoinShowword();
	//	m_guideInfo->detailList[1].strfileName = _T(".\\res\\acceptable_coins.png");
	//}
	//else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD){
	//	// 设置提示区显示信息
	//	m_guideInfo->tipsCount = 1;
	//	m_guideInfo->detailList[0].bitInfo =  _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES);
	//	m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
	//}
	//else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_EFT){
	//	// 设置提示区显示信息
	//	m_guideInfo->tipsCount = 1;
	//	m_guideInfo->detailList[0].bitInfo =  _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES);
	//	m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
	//}
	showPaymethodGuide();
	// 显示确认按钮
	ShowCompoment(BTN_OK);
	pSvc->NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      允许纸币显示文言

@param      无

@retval     CString

@exception  None
*/
//////////////////////////////////////////////////////////////////////////

CString CProductIssueReceptionDlg::BanknoteShowword() {

	CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);
	int nCount = 0;
	// 充值接收的纸币面额 BIN	单位：元。无效时为65535
	CString banknote_showword = _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES)+_T("\n");
	if (billboxParam.acceptChrgrBillValue1 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue1);
		banknote_showword += strTemp+_tl(MONEY_UNIT) + _T(",");
		++nCount;
	}
	if (billboxParam.acceptChrgrBillValue2 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue2);
		banknote_showword += strTemp+_tl(MONEY_UNIT)+ _T(",");
		++nCount;
	}
	if (billboxParam.acceptChrgrBillValue3 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue3);
		banknote_showword += strTemp+_tl(MONEY_UNIT)+ _T(",");
		if (++nCount == 3)
		{
			banknote_showword += _T("\n");
		}
		
	}
	if (billboxParam.acceptChrgrBillValue4 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue4);
		banknote_showword += strTemp+_tl(MONEY_UNIT)+ _T(",");
		if (++nCount == 3)
		{
			banknote_showword += _T("\n");
		}
	}
	if (billboxParam.acceptChrgrBillValue5 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue5);
		banknote_showword += strTemp+_tl(MONEY_UNIT)+ _T(",");
		if (++nCount == 3)
		{
			banknote_showword += _T("\n");
		}
	}
	if (billboxParam.acceptChrgrBillValue6 != CAfcTVMOperationParam::BANKNOTE_INVALID_VALUE) {
		CString strTemp(_T(""));
		strTemp.Format(_T("%d"),billboxParam.acceptChrgrBillValue6);
		banknote_showword += strTemp+_tl(MONEY_UNIT)+ _T(",");
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

CString CProductIssueReceptionDlg::CoinShowword() {

	/*CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);

	CString coin_showword = _tl(TXT_RECHARGE_ACCEPTABLE_COINS)+_T("\n");
	if (billboxParam.acceptChrgrCoinValue1 != CAfcTVMOperationParam::COIN_INVALID_VALUE) {
		coin_showword += CTVMForeService::ShowCoinValue(billboxParam.acceptChrgrBillValue1);
	}
	if (billboxParam.acceptChrgrCoinValue2 != CAfcTVMOperationParam::COIN_INVALID_VALUE) {
		coin_showword += CTVMForeService::ShowCoinValue(billboxParam.acceptChrgrBillValue2);
	}
	if (billboxParam.acceptChrgrCoinValue3 != CAfcTVMOperationParam::COIN_INVALID_VALUE) {
		coin_showword += CTVMForeService::ShowCoinValue(billboxParam.acceptChrgrBillValue3);
	}
	if (billboxParam.acceptChrgrCoinValue4 != CAfcTVMOperationParam::COIN_INVALID_VALUE) {
		coin_showword += CTVMForeService::ShowCoinValue(billboxParam.acceptChrgrBillValue4);
	}
	return coin_showword;*/

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
/*
@brief      确认按钮点击响应事件（对应选择产品后和付款后两次确认，分别hasFinishPaid()==False和True）

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try
	{
		m_btnOK->SetSelect(false);
		theSERVICE_MGR.SetState(RECEPT);
		CProductIssueSvc::DataModel &model = pSvc->GetDataModel();
		HideCompoment(BTN_OK);

		if (theSERVICE_MGR.GetState() == RECEPT) {
			switch (pSvc->GetCurDialogGroupID())
			{
			case CProductIssueSvc::DIALOG_GROUP_INIT:
				pSvc->DlgNextPage();
				/*pSvc->NotifyDataModelChanged();*/
				break;
			case CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION:
				HideCompoment(m_Right_1_Group);HideCompoment(m_Right_2_Group);
				HideCompoment(BTN_OK);
				pSvc->StartReceiveMoney();
				//向导提示
				/*showPaymethodGuide();*/
				break;
			default: break;
			}
		}
		else if (theSERVICE_MGR.GetState() == COUNTING) {
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			m_guideInfo->tipsCount = 1;																		// 充值处理中
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_ADD_PRODUCT_IN_PROCESSING)/*_tl(TXT_RECHARGING_WAIT)*/;
			m_guideInfo->detailList[0].strfileName = TVM_IMAGE_CHARGING;
			InitializeGuideArea();
			m_guideInfoArea->renderGuideInfo();
			pSvc->AcceptMoney();

			if(pSvc->DoCardIssueProduct())																	// 开始写卡售产品
			{
				pSvc->ReturnFinishDlg();																		// 现金接收完成处理
			}
			else {
				pSvc->ReturnFinishDlg();															// 错误取消处理
			}
		}
		//pSvc->NotifyDataModelChanged();
		return TRUE;
	}
	catch( CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		pSvc->ReturnFinishDlg();
		//throw;
	}
	catch(...){
		pSvc->ReturnFinishDlg();
		//throw  CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("售产品异常"));
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnPrint(WPARAM wParam,LPARAM lParam)
{
	CProductIssueSvc::DataModel model = pSvc->GetDataModel();
	HideCompoment(BTN_PRINT);
	try{
		// 设置正在硬件动作中...
		theSERVICE_MGR.SetState(ISSUEING);
		HideCompoment(BTN_PRINT);
		// 正在打印中...
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_YOUR_CREDENTIALS_PRINTING);
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTING;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		pSvc->PrinterPrintReceipt();
		// 打印完成，请收好凭据。
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_GUIDE_PRINT_IS_COMPLEATED)/*T("打印完成，请取走您的凭条！")*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_PRINTED;
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS;
		// 设置打印完成
		theSERVICE_MGR.SetState(FINISH);
		return TRUE;
	}
	catch(CSysException& e){
		model.operationInfo.failurePrintState = CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		throw  CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("售产品凭条打印异常"));
		return FALSE;
	}
	/*pSvc->PrinterPrintReceipt();
	return TRUE;*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取消按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     UpdateUI()

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CProductIssueReceptionDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	if(theSERVICE_MGR.GetState() == FINISH || pSvc->GetCurDialogGroupID() == CProductIssueSvc::DIALOG_GROUP_FINISH){
		pMainSvc->StartDefaultService();
		return TRUE;
	}
	pSvc->OnCancel();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueReceptionDlg::UpdateUI()
{
	InitializeBaseInfo();				// 刷新车票信息区
	if ( m_CurDlg != pSvc->GetCurDialogGroupID())
	{
		InitializeGuideArea();			// 刷新向导提示区
		m_guideInfoArea->renderGuideInfo();
		m_CurDlg = pSvc->GetCurDialogGroupID();
	}
	if ( pSvc->GetCurDialogGroupID() == CProductIssueSvc::DIALOG_GROUP_CHOOSE_LOCATION)		// 投钱超限
	{
		if (pSvc->IsBanknoteMaxAmount() || pSvc->IsBanknoteMaxCount()) // 纸币超限
		{
			if(m_guideInfo != NULL)
			{
				m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_CURRENT_RECIVE_MAX_BANKNOTES);
				m_guideInfo->detailList[0].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
				InitializeGuideArea();
				m_guideInfoArea->renderGuideInfo();
			}
		}
		if (pSvc->IsCoinMaxCount())				// 硬币超限
		{
			//TVM_TIP_FORBID_INSERT_COINS  GUIDE_CURRENT_REVICE_MAX_COINS
			m_guideInfo->detailList[1].bitInfo =  _tl(GUIDE_CURRENT_REVICE_MAX_COINS);
			m_guideInfo->detailList[1].strfileName = TVM_TIP_FORBID_INSERT_COINS;
			InitializeGuideArea();
			m_guideInfoArea->renderGuideInfo();
		}
		if (pSvc->IsBothCoinBanknoteMaxCount())	// 纸币硬币都超限
		{
			m_guideInfo->detailList[0].bitInfo =  _tl(GUIDE_CURRENT_RECIVE_MAX_BANKNOTES);
			m_guideInfo->detailList[0].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
			m_guideInfo->detailList[1].bitInfo =  _tl(GUIDE_CURRENT_REVICE_MAX_COINS);
			m_guideInfo->detailList[1].strfileName = TVM_TIP_FORBID_INSERT_COINS;
			InitializeGuideArea();
			m_guideInfoArea->renderGuideInfo();
		}
	}
	
	if( pSvc->GetCurDialogGroupID() == CProductIssueSvc::DIALOG_GROUP_FINISH ||  pSvc->GetCurDialogGroupID() == CProductIssueSvc::DIALOG_GROUP_ERROR )
	{
		HideCompoment(m_Right_2_Group);
		HideCompoment(m_Right_1_Group);
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		支付向导提示

@param      
@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueReceptionDlg::showPaymethodGuide()
{
	CProductIssueSvc::DataModel &model = pSvc->GetDataModel();
	// 1.现金支付
	if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
		// 设置支付提示信息
		int index = 0;
		// 纸币
		if (!BanknoteShowword().IsEmpty())
		{
			// 判断当前模式是否支持纸币投入？
			// 当前模式不接受纸币投入
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
				m_guideInfo->detailList[index].bitInfo =  _tl(TXT_SHOW_NO_ACCEPT_BANKNOTE);
				m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
			}
			else{
				m_guideInfo->detailList[index].bitInfo =  BanknoteShowword();
				m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;
			}
		}
		// 硬币
		if (!CoinShowword().IsEmpty())
		{
			// 不支持硬币投入？
			if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
				m_guideInfo->detailList[index].bitInfo =  _tl(TXT_SHOW_NO_ACCEPT_COIN);
				m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
			}
			else{
				m_guideInfo->detailList[index].bitInfo =  CoinShowword();
				m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_COIN;
			}
		}
		m_guideInfo->tipsCount = index;		
	}
	// 2.银行卡支付
	else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD){
		// 设置提示区显示信息
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES);
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_BANKCARD;
	}
	// 3.EP支付
	//else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_EFT){
	//	// 设置提示区显示信息
	//	m_guideInfo->tipsCount = 1;
	//	m_guideInfo->detailList[0].bitInfo =  _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES);
	//	m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_EP;
	//}
	InitializeGuideArea();
	m_guideInfoArea->renderGuideInfo();
}
/*


*/
