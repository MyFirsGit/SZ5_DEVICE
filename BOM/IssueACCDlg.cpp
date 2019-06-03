#include "stdafx.h"
#include "IssueACCDlg.h"
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

static CRect ISSUE_CARD_BY_INNER_TPU_EDIT_RECT[] = {BASE_INFO_RECT_SP(4,7,0),BASE_INFO_RECT(5, MONEY_MAX_LEN)};
static CRect ISSUE_CARD_BY_OUT_TPU_EDIT_RECT[] = {BASE_INFO_RECT(4, MONEY_MAX_LEN)};
static CRect ISSUE_CARD_BY_OUT_TPU_MULTIRIDE_EDIT_RECT[] = {BASE_INFO_RECT(5, MONEY_MAX_LEN)};

static int const IDX_ISSUE_BY_INNER_TPU_PRICE = 0;
static int const IDX_ISSUE_BY_INNER_TPU_PIECE = 1;
static int const IDX_ISSUE_BY_INNER_TPU_PAID = 2;

static int const IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT = 0;
static int const IDX_ISSUE_BY_OUT_TPU_PAID = 1;

static char* const TXT_EXIT_TICKET_FREE = "免费";
static char* const MSG_EXIT_TICKET_FREE = "MSG_EXIT_TICKET_FREE";
static char* const TXT_EXIT_TICKET_PAID = "付费";
static char* const MSG_EXIT_TICKET_PAID = "MSG_EXIT_TICKET_PAID";

IMPLEMENT_DYNAMIC(CIssueACCDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CIssueACCDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnLineClick)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnStationClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPriceClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnPieceClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCDlg::CIssueACCDlg(CService* pService)
: COperationDlg(CIssueACCDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
	CIssueSvc::Model &model = pSvc->GetDataModel();
	// 内部读写器发售
	if(model.issueWay == CTicketHelper::BY_INNER_TPU || model.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU ){
		m_TicketInfoGroup[0].nameLeft = add_wsp(TXT_TICKET_KIND + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[1].nameLeft = add_wsp(CString(TXT_START_STATION)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[2].nameLeft = add_wsp(CString(TXT_END_STATION)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[3].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
		m_TicketInfoGroup[4].nameLeft =add_wsp(CString(TXT_TICKET_COUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
		m_TicketInfoGroup[4].nameRight = add_hsp(TICKET_UNIT,MAX_CASH_RIGHT_LEN);
		m_baseInfo->detailInfo.row = 5;
		m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
		m_baseInfo->detailInfo.editGroup.editRect = ISSUE_CARD_BY_INNER_TPU_EDIT_RECT;
		m_baseInfo->detailInfo.editGroup.cnt = 2;
		//钱包产品通过车站或者直接选择票价票卖
		if(model.ticketType == TICKET_EXIT_ONLY)
		{
			InitializeComponents(PAYTYPE_BUTTON|PIECE_BUTTON);
		}
		else if(model.cardCategory == PURSE)	{
			if(model.ticketType == TICKET_WELFARE)
			{
				InitializeComponents(LINE_BUTTON|STATION_BUTTON|PIECE_BUTTON);
			}
			else
			{
				InitializeComponents(LINE_BUTTON|STATION_BUTTON|PRICE_BUTTON|PIECE_BUTTON);
			}

		}
		else if(model.cardCategory == MULTIRIDE){
			model.rwIssueRequest.MultiRideTicketCount = 1;
			InitializeComponents(LINE_BUTTON|STATION_BUTTON|PIECE_BUTTON);
		}
		//定期产品通过车站取票价
		else if(model.cardCategory == PERIOD){
			InitializeComponents(LINE_BUTTON|STATION_BUTTON|PIECE_BUTTON);
		}
	}
	// 外部读写器发售票种
	else{
		m_TicketInfoGroup[0].nameLeft = add_wsp(TXT_TICKET_KIND + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		if(model.cardCategory == MULTIRIDE){
			m_TicketInfoGroup[1].nameLeft = add_wsp(CString(TXT_VALID_RIDES)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			//m_TicketInfoGroup[1].nameRight = add_hsp(TIMES_UNIT,MAX_CASH_RIGHT_LEN);
			m_TicketInfoGroup[2].nameLeft =add_wsp(CString(TXT_TICKET_VALIDATE_DATE)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			//m_TicketInfoGroup[3].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
			m_TicketInfoGroup[4].nameLeft = add_wsp(CString(TXT_TICKET_SALE_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			//m_TicketInfoGroup[4].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
			m_baseInfo->detailInfo.row = 5;
			m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
			m_baseInfo->detailInfo.editGroup.editRect = ISSUE_CARD_BY_OUT_TPU_MULTIRIDE_EDIT_RECT;
			m_baseInfo->detailInfo.editGroup.cnt = 1;
		}
		else{
			m_TicketInfoGroup[1].nameLeft =add_wsp(CString(TXT_TICKET_VALIDATE_DATE)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[2].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			//m_TicketInfoGroup[2].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
			m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_SALE_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			//m_TicketInfoGroup[3].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
			m_baseInfo->detailInfo.row = 4;
			m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
			m_baseInfo->detailInfo.editGroup.editRect = ISSUE_CARD_BY_OUT_TPU_EDIT_RECT;
			m_baseInfo->detailInfo.editGroup.cnt = 1;
		}
		if(model.ticketType == TICKET_STORE_VALUE)
		{
			InitializeComponents(CHARGEVALUE_BUTTON);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCDlg::~CIssueACCDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCDlg::InitializeComponents(long lComponents)
{
	m_lComponents = lComponents;

	if(lComponents & (LINE_BUTTON | STATION_BUTTON)){
		typedef CAccLineInfoParam::LINE_INFO LINE_t;
		const vector<LINE_t>& vecLines =theACC_LINE.GetLinInfo();
		for(unsigned int i=0;i<vecLines.size();i++){
			const LINE_t& line = vecLines[i];
			if(line.Active_flag){
				BTN_INFO btnInfo;
				btnInfo.btnMsg.Format(_T("%d"),line.lineNum);
				CString lineName = line.line_Chinese_Name;
				if (lineName.GetLength() > 16){
					lineName.Insert(12,_T("\n"));
				}
				btnInfo.text = lineName;
				btnInfo.isEnable = theFunction_INFO.IsLineEnabled(line.lineNum);
				m_vec_Left_2_BTN_INFO.push_back(btnInfo);
			}
		}
		m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
		m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
	}
	if(lComponents & PRICE_BUTTON){
		vector<DWORD> vecFareList;
		theACC_PRICELEVEL.GetPriceLevelParam(vecFareList);
		for(unsigned int i=0;i<vecFareList.size();i++){
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"),vecFareList[i]);
			btnInfo.text.Format(_T("%s%s"),ComFormatAmount(vecFareList[i]),MONEY_UNIT)  ;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	}
	if(lComponents & PAYTYPE_BUTTON)
	{
		BTN_INFO btnInfoExitTicketFree;
		btnInfoExitTicketFree.text = TXT_EXIT_TICKET_FREE;
		btnInfoExitTicketFree.btnMsg = MSG_EXIT_TICKET_FREE;

		BTN_INFO btnInfoExitTicketPaid;
		btnInfoExitTicketPaid.text = TXT_EXIT_TICKET_PAID;
		btnInfoExitTicketPaid.btnMsg = MSG_EXIT_TICKET_PAID;

		m_vec_Right_1_BTN_INFO.push_back(btnInfoExitTicketFree);
		m_vec_Right_1_BTN_INFO.push_back(btnInfoExitTicketPaid);
		
		m_Right_1_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	}
	if(lComponents & CHARGEVALUE_BUTTON)
	{
		const CStringArray& availableChargeValueList = CIssueSvc::GetAvailableChargeValueList();
		for (int i=0; i<availableChargeValueList.GetCount();i++)
		{
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), _ttoi(availableChargeValueList.GetAt(i))*100);
			btnInfo.text=availableChargeValueList.GetAt(i)+MONEY_UNIT  ;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}
		m_Right_1_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::NONE);
	}
	if(lComponents&PIECE_BUTTON)
	{
		const CStringArray& availablePieceList = CIssueSvc::GetAvailablePieceList();
		for (int i=0; i<availablePieceList.GetCount();i++)
		{
			BTN_INFO btnInfo;
			btnInfo.btnMsg=availablePieceList.GetAt(i);
			btnInfo.text=availablePieceList.GetAt(i)+TICKET_UNIT  ;
			m_vec_Right_2_BTN_INFO.push_back(btnInfo);
		}
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		m_Right_2_Group->SetSelectType(CButtonGroup::NONE);
	}
}


//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面创建函数
//
//@param      (i)LPCREATESTRUCT lpCreateStruct
//
//@retval     int    \n
//			0:创建成功    -1:创建失败
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
int CIssueACCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct,  BASE_AREA|CASH_AREA|GUIDE_AREA|KEYBOARD_AREA|BTN_OK ) == -1) {
		return -1;
	}
	CIssueSvc::Model &model = pSvc->GetDataModel();
	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  model.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		m_Left_2_Group->ClickFirstAvailButton();
		m_Right_2_Group->ClickFirstAvailButton();
		if(model.ticketType == TICKET_EXIT_ONLY)
		{
			m_Right_1_Group->ClickFirstAvailButton();
		}
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetReadOnly(TRUE);
		if(model.ticketType == TICKET_WELFARE)
		{
			GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetReadOnly(FALSE);
		}
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetScope(CGEdit::CASH);
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetLimitText(7);
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE)->SetLimitText(4);
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE)->SetFocus();
	}
	if(model.issueWay == CTicketHelper::BY_OUTER_TPU)
	{
		GetEdit(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT)->SetScope(CGEdit::CASH);
		GetEdit(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT)->SetLimitText(7);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	if(!ValidateInput())
	{
		return FALSE;
	}
	pSvc->OnOK();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证用户输入是否正确 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueACCDlg::ValidateInput()
{
	if(theISSUE_PERMIT.IsPermitIssueStation(pSvc->GetDataModel().rwIssueRequest.BegainStation))
	{
		CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),_T("本站限制发售"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		return FALSE;
	}

	if( theISSUE_PERMIT.IsPermitIssueStation(pSvc->GetDataModel().rwIssueRequest.EndStation))
	{
		CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),_T("限制发售该车站车票"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		return FALSE;
	}
	if(pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_TPU || pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		if(GetEditText(IDX_ISSUE_BY_INNER_TPU_PRICE)==""|| _ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PRICE)) >=100000)
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_PRICE_ERROR));
			GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetFocus();
			return false;
		}
		if( GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE).GetLength() ==0 || _ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE))==0 ||_ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE)) >=10000)
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_COUNT_ERROR));
			GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE)->SetFocus();
			return false;
		}

		if(GetEditText(IDX_ISSUE_BY_INNER_TPU_PAID)!="")
		{
			long price = ComGetAmount(GetEditText(IDX_ISSUE_BY_INNER_TPU_PRICE));
			int piece = _ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE));
			long due = price*piece;
			long paid = ComGetAmount(GetEditText(IDX_ISSUE_BY_INNER_TPU_PAID));
			if(paid<due)
			{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_CASH_ERROR));
				GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE)->SetFocus();
				return false;
			}
		}
		else if(GetEditText(IDX_ISSUE_BY_INNER_TPU_PAID)=="")
		{
			long price = ComGetAmount(GetEditText(IDX_ISSUE_BY_INNER_TPU_PRICE));
			int piece = _ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE));
			CString due = ComFormatAmount(price*piece);
			SetEditText(IDX_ISSUE_BY_INNER_TPU_PAID,due);
			pSvc->GetDataModel().paid = price*piece;
			pSvc->NotifyDataModelChanged();
		}
		return true;
	}
	if(pSvc->GetDataModel().issueWay == CTicketHelper::BY_OUTER_TPU)
	{
		if( GetEditText(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT).GetLength() ==0)
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_COUNT_ERROR));
			GetEdit(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT)->SetFocus();
			return false;
		}

		if(GetEditText(IDX_ISSUE_BY_OUT_TPU_PAID)!="")
		{
			if(pSvc->GetDataModel().paid<pSvc->GetDataModel().due)
			{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_CASH_ERROR));
				GetEdit(IDX_ISSUE_BY_OUT_TPU_PAID)->SetFocus();
				return false;
			}
		}
		else if(GetEditText(IDX_ISSUE_BY_OUT_TPU_PAID)=="")
		{
			SetEditText(IDX_ISSUE_BY_OUT_TPU_PAID,ComFormatAmount(pSvc->GetDataModel().due));
			pSvc->GetDataModel().paid = pSvc->GetDataModel().due;
			pSvc->NotifyDataModelChanged();
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      线路代码按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnLineClick(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD lineCode = _ttoi(btnInfo->btnMsg);
	typedef CAccLineInfoParam::STATION_INFO STATION_t; 
	const vector<STATION_t>& vecStations = theACC_LINE.GetStationInfo(lineCode);
	m_vec_Left_1_BTN_INFO.clear();
	for (unsigned int i=0;i<vecStations.size();i++){
		const STATION_t& station = vecStations[i];
		if(station.Active_flag){
			BTN_INFO btnInfo;
			//2012/2/15从车站编码改为计费编码
			//btnInfo.btnMsg.Format(_T(_"%d"),station.stationNum);
			btnInfo.btnMsg.Format(_T("%d"),station.fareStationNum);
			CString stationName = station.station_Chinese_Name;
			if (stationName.GetLength() > 16){
				stationName.Insert(12,_T("\n"));
			}
			btnInfo.text = stationName;
			btnInfo.isEnable =  theFunction_INFO.IsStationEnabled(station.stationNum);
			m_vec_Left_1_BTN_INFO.push_back(btnInfo);
		}
	}
	m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	m_Left_1_Group->SetSelectType(CButtonGroup::SIGLE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnStationClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	WORD selectedStationCode = _ttoi(btnInfo->btnMsg);
	if(theISSUE_PERMIT.IsPermitIssueStation(selectedStationCode)){
		CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),_T("限制发售该车站车票"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		return FALSE;
	}
	CIssueSvc::Model &model = pSvc->GetDataModel();
	TicketType_t ticketType = model.ticketType;
	model.rwIssueRequest.EndStation = selectedStationCode;
	if(model.cardCategory != MULTIRIDE)
	{
		RW_CMD_TICKET_PRICE rwCmdTicketPrice;
		DWORD currentStationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
		memcpy(rwCmdTicketPrice.BegainStation,&currentStationCode,2);
		memcpy(rwCmdTicketPrice.EndStation,&selectedStationCode,2);
		rwCmdTicketPrice.ProductCategory=model.cardCategory/*MAKEWORD(ticketType,model.cardCategory)*/;
		rwCmdTicketPrice.Time = ComGetCurTime();
		RW_CMD_TICKET_PRICE_RES res;
		long getPriceResult = -1;
		if(!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID))
		{
			getPriceResult = pSvc->GetRWHelper()->GetTicketPrice(rwCmdTicketPrice,res);
		}
		else if(!theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID))
		{
			getPriceResult = pSvc->GetTWHelper()->GetTicketPrice(rwCmdTicketPrice,res);
		}
		if(getPriceResult!=RW_RSP_OK)
		{		
			theAPP_SESSION.ShowOfficeGuide(pSvc->GetRWHelper()->GetTpuErrorInfo(res.DetailErrorCode));
			GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetWindowText(_T(""));
			return 0;
		}
		else
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_OLD_CARD_SELET_INFO));
		}
		pSvc->GetDataModel().rwIssueRequest.SaleAmount = res.TicketPrice;
		pSvc->GetDataModel().rwIssueRequest.SaleMode = BY_BEGIN_END_STATION;
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetWindowText(ComFormatAmount(res.TicketPrice));
	}
	pSvc->NotifyDataModelChanged();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnPriceClick(WPARAM wParam, LPARAM lParam)
{
	if (pSvc->GetDataModel().ticketType == TICKET_EXIT_ONLY){
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		if (btnInfo->btnMsg == MSG_EXIT_TICKET_FREE){//免费出站票
			pSvc->GetDataModel().rwIssueRequest.SaleAmount = 0;
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_NEW_CARD_SELET_INFO));
		}
		// 付费出站票时显示到当站的最高票价
		else if(btnInfo->btnMsg == MSG_EXIT_TICKET_PAID){
			long ticketPrice = 0;
			bool getTicketPriceSuccess = pSvc->GetMaxPrice(ticketPrice);
			if(!getTicketPriceSuccess)
			{
				GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetWindowText(_T(""));
				return TRUE;
			}
			pSvc->GetDataModel().rwIssueRequest.SaleAmount = ticketPrice;
		}
		pSvc->NotifyDataModelChanged();
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetWindowText(ComFormatAmount(pSvc->GetDataModel().rwIssueRequest.SaleAmount));
		return TRUE;
	}
	else if(pSvc->GetDataModel().ticketType == TICKET_STORE_VALUE)
	{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		DWORD price = _ttoi(btnInfo->btnMsg);
		pSvc->GetDataModel().rwIssueRequest.SaleAmount = price;
		pSvc->NotifyDataModelChanged();
		GetEdit(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT)->SetWindowText(ComFormatAmount(price));
		return TRUE;
	}
	else{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		DWORD price = _ttoi(btnInfo->btnMsg);
		pSvc->GetDataModel().rwIssueRequest.SaleMode = BY_PRICE;
		pSvc->GetDataModel().rwIssueRequest.EndStation = 0;
		pSvc->GetDataModel().rwIssueRequest.SaleAmount = price;
		pSvc->NotifyDataModelChanged();
		GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE)->SetWindowText(ComFormatAmount(price));
		return TRUE;
	}
	
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnPieceClick(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	int piece = _ttoi(btnInfo->btnMsg);
	CIssueSvc* pSvc = GetService<CIssueSvc>();
	pSvc->GetDataModel().piece = piece;
	pSvc->NotifyDataModelChanged();
	GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE)->SetWindowText(ComFormatCount(piece));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueACCDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CGEdit* currentEdit = (CGEdit*)lParam;
	CIssueSvc::Model &model = pSvc->GetDataModel();
	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		if(currentEdit==GetEdit(IDX_ISSUE_BY_INNER_TPU_PRICE))
		{
			int price = ComGetAmount(GetEditText(IDX_ISSUE_BY_INNER_TPU_PRICE));
			pSvc->GetDataModel().rwIssueRequest.SaleAmount = price;
		}
		else if(currentEdit == GetEdit(IDX_ISSUE_BY_INNER_TPU_PIECE))
		{
			int piece = _ttoi(GetEditText(IDX_ISSUE_BY_INNER_TPU_PIECE));
			pSvc->GetDataModel().piece = piece;
		}
		else if(currentEdit == GetEdit(IDX_ISSUE_BY_INNER_TPU_PAID))
		{
			long paid = ComGetAmount(GetEditText(IDX_ISSUE_BY_INNER_TPU_PAID));
			pSvc->GetDataModel().paid = paid;
		}
		pSvc->NotifyDataModelChanged();
	}
	else
	{
		if(currentEdit == GetEdit(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT))
		{
			int price = ComGetAmount(GetEditText(IDX_ISSUE_BY_OUT_TPU_SALE_AMOUNT));
			pSvc->GetDataModel().rwIssueRequest.SaleAmount = price;
		}
		else if(currentEdit == GetEdit(IDX_ISSUE_BY_OUT_TPU_PAID))
		{
			long paid = ComGetAmount(GetEditText(IDX_ISSUE_BY_OUT_TPU_PAID));
			pSvc->GetDataModel().paid = paid;
		}
		pSvc->NotifyDataModelChanged();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   服务数据模型发生变化时的更新界面   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCDlg::UpdateUI()
{
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		m_TicketInfoGroup[0].nameRight = model.ticketTypeDetail.ticketTypeName_cn;
		CAccLineInfoParam::STATION_INFO beginStationInfo;
		theACC_LINE.GetStationInfobyCode(model.rwIssueRequest.BegainStation,beginStationInfo);
		CAccLineInfoParam::STATION_INFO endStationInfo;
		theACC_LINE.GetStationInfobyCode(model.rwIssueRequest.EndStation,endStationInfo);
		m_TicketInfoGroup[1].nameRight = model.rwIssueRequest.BegainStation == 0 ? CString(TXT_UNDEFINE) : beginStationInfo.station_Chinese_Name;
		m_TicketInfoGroup[2].nameRight = model.rwIssueRequest.EndStation == 0 ? CString(TXT_UNDEFINE) : endStationInfo.station_Chinese_Name;
		if(model.rwIssueRequest.SaleMode == BY_PRICE)
		{
			m_Left_1_Group->ClearSelection();
			//m_Left_2_Group->ClearSelection();
		}
		else
		{
			m_Right_1_Group->ClearSelection();
		}
	}
	if(model.issueWay == CTicketHelper::BY_OUTER_TPU)
	{
		m_TicketInfoGroup[0].nameRight = model.ticketTypeDetail.ticketTypeName_cn;
		if(model.cardCategory == MULTIRIDE)
		{
			if(model.pCPUCard!=NULL)
			{
				m_TicketInfoGroup[1].nameRight = ComFormatCount(model.pCPUCard->applicationData.Balance) + TIMES_UNIT;
				m_TicketInfoGroup[2].nameRight =model.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日"));
				m_TicketInfoGroup[3].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
			if(model.pULCard!=NULL)
			{
				m_TicketInfoGroup[1].nameRight = ComFormatCount(model.pULCard->applicationData.Balance)+ TIMES_UNIT;
				m_TicketInfoGroup[2].nameRight =model.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日"));
				m_TicketInfoGroup[3].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
		}
		else
		{
			if(model.pCPUCard!=NULL)
			{
				m_TicketInfoGroup[1].nameRight =model.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日"));
				m_TicketInfoGroup[2].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
			if(model.pULCard!=NULL)
			{
				m_TicketInfoGroup[1].nameRight =model.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日"));
				m_TicketInfoGroup[2].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
		}
	}
	m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(model.due)+MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.change)+MONEY_UNIT;
	m_baseInfoArea->Invalidate();
	m_cashInfoArea->Invalidate();

}


void CIssueACCDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

}