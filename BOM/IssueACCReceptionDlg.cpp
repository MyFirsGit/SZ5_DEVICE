#include "stdafx.h"
#include "IssueACCReceptionDlg.h"

IMPLEMENT_DYNAMIC(CIssueACCReceptionDlg,CReceptionDlg)
BEGIN_MESSAGE_MAP(CIssueACCReceptionDlg,CReceptionDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCReceptionDlg::CIssueACCReceptionDlg(CService * pService) : CReceptionDlg(IDD,pService)
{
	pSvc = (CIssueSvc*)pService;
	std::vector<MSG_NAME_GROUP>& receptionDetailList = m_BusinessInfo->detailList;
	CIssueSvc::Model &model = pSvc->GetDataModel();

	MSG_NAME_GROUP ticketTypeGroup;
	ticketTypeGroup.nameLeft.nameChinese = TXT_TICKET_KIND;
	ticketTypeGroup.nameLeft.nameEnglish = TXT_TICKET_KIND_EN;
	typedef CAccTicketParam::TICKET_TYPE TICKET_TYPE_t;
	TICKET_TYPE_t &ticketTypeDetail = pSvc->GetDataModel().ticketTypeDetail;
	ticketTypeGroup.nameRight.nameChinese = ticketTypeDetail.ticketTypeName_cn;
	ticketTypeGroup.nameRight.nameEnglish = ticketTypeDetail.ticketTypeName_en;
	receptionDetailList.push_back(ticketTypeGroup);

	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		MSG_NAME_GROUP beginStationGroup;
		beginStationGroup.nameLeft.color = RED;
		beginStationGroup.nameLeft.nameChinese = TXT_START_STATION;
		beginStationGroup.nameLeft.nameEnglish = TXT_START_STATION_EN;
		beginStationGroup.nameRight.color = RED;
		receptionDetailList.push_back(beginStationGroup);

		MSG_NAME_GROUP endStationGroup;
		endStationGroup.nameLeft.color = RED;
		endStationGroup.nameLeft.nameChinese = TXT_END_STATION;
		endStationGroup.nameLeft.nameEnglish = TXT_END_STATION_EN;
		endStationGroup.nameRight.color = RED;
		receptionDetailList.push_back(endStationGroup);

		MSG_NAME_GROUP ticketPriceGroup;
		ticketPriceGroup.nameLeft.color = RED;
		ticketPriceGroup.nameLeft.nameChinese = TXT_TICKET_AMOUNT;
		ticketPriceGroup.nameLeft.nameEnglish = TXT_TICKET_AMOUNT_EN;
		ticketPriceGroup.nameRight.color = RED;
		ticketPriceGroup.nameRight.nameChinese=TXT_UNDEFINE;
		receptionDetailList.push_back(ticketPriceGroup);
	
		MSG_NAME_GROUP ticketCountGroup;
		ticketCountGroup.nameLeft.nameChinese = TXT_TICKET_COUNT;
		ticketCountGroup.nameLeft.nameEnglish = TXT_TICKET_COUNT_EN;
		ticketCountGroup.nameRight.nameChinese =TXT_UNDEFINE;
		receptionDetailList.push_back(ticketCountGroup);
	}
	else
	{
		if(model.cardCategory == MULTIRIDE)
		{
			MSG_NAME_GROUP ticketValidRidesGroup;
			ticketValidRidesGroup.nameLeft.nameChinese = TXT_VALID_RIDES;
			ticketValidRidesGroup.nameLeft.nameEnglish = TXT_VALID_RIDES_EN;
			ticketValidRidesGroup.nameRight.nameChinese =TXT_UNDEFINE;
			receptionDetailList.push_back(ticketValidRidesGroup);
		}
		MSG_NAME_GROUP ticketValiddateGroup;
		ticketValiddateGroup.nameLeft.nameChinese = TXT_TICKET_VALIDATE_DATE;
		ticketValiddateGroup.nameLeft.nameEnglish = TXT_TICKET_VALIDATE_DATE_EN;
		ticketValiddateGroup.nameRight.nameChinese =TXT_UNDEFINE;
		receptionDetailList.push_back(ticketValiddateGroup);

		MSG_NAME_GROUP ticketDepositGroup;
		ticketDepositGroup.nameLeft.nameChinese = TXT_TICKET_DEPOSIT;
		ticketDepositGroup.nameLeft.nameEnglish = TXT_TICKET_DEPOSIT_EN;
		ticketDepositGroup.nameRight.nameChinese =TXT_UNDEFINE;
		receptionDetailList.push_back(ticketDepositGroup);

		MSG_NAME_GROUP ticketSaleAmountGroup;
		ticketSaleAmountGroup.nameLeft.nameChinese = TXT_TICKET_SALE_AMOUNT;
		ticketSaleAmountGroup.nameLeft.nameEnglish = TXT_TICKET_SALE_AMOUNT_EN;
		ticketSaleAmountGroup.nameRight.nameChinese =TXT_UNDEFINE;
		receptionDetailList.push_back(ticketSaleAmountGroup);
	}
	MSG_NAME_GROUP amountDueGroup;
	amountDueGroup.nameLeft.nameChinese = TXT_TOTAL_RECEPTION;
	amountDueGroup.nameLeft.nameEnglish = TXT_TOTAL_RECEPTION_EN;
	amountDueGroup.nameRight.nameChinese = TXT_UNDEFINE;
	receptionDetailList.push_back(amountDueGroup);

	MSG_NAME_GROUP paidGroup;
	paidGroup.nameLeft.nameChinese = TXT_INCOME_RECEPTION;
	paidGroup.nameLeft.nameEnglish = TXT_INCOME_RECEPTION_EN;
	paidGroup.nameRight.nameChinese = TXT_UNDEFINE;
	receptionDetailList.push_back(paidGroup);

	MSG_NAME_GROUP changeGroup;
	changeGroup.nameLeft.color = RED;
	changeGroup.nameLeft.nameChinese = TXT_CHANGE_RECEPTION;
	changeGroup.nameLeft.nameEnglish = TXT_CHANGE_RECEPTION_EN;
	changeGroup.nameRight.color = RED;
	changeGroup.nameRight.nameChinese = TXT_UNDEFINE;
	receptionDetailList.push_back(changeGroup);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CIssueACCReceptionDlg::~CIssueACCReceptionDlg()
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
int CIssueACCReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,BUSINESS_AREA);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      画面更新

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueACCReceptionDlg::UpdateUI()
{
	if(pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ERROR)
	{
		HideCompoment(BUSINESS_AREA);
		return;
	}

	CIssueSvc::Model& model = pSvc->GetDataModel();
	TicketType_t m_TicketType = model.ticketType;
	RW_ISSUE_REQUEST& m_RWIssueRequest = model.rwIssueRequest;
	std::vector<MSG_NAME_GROUP>& receptionDetailList = m_BusinessInfo->detailList;
	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU)
	{
		int beginStationIndex = 1;
		int endStationIndex = 2;
		int ticketPriceIndex = 3;
		int ticketCountIndex = 4;
		int dueIndex = 5;
		int paidIndex = 6;
		int changeIndex = 7;
		CAccLineInfoParam::STATION_INFO beginStation;
		theACC_LINE.GetStationInfobyCode(m_RWIssueRequest.BegainStation,beginStation);
		CAccLineInfoParam::STATION_INFO endStation;
		theACC_LINE.GetStationInfobyCode(m_RWIssueRequest.EndStation,endStation);

		receptionDetailList[beginStationIndex].nameRight.nameChinese =m_RWIssueRequest.BegainStation == 0 ? CString(TXT_UNDEFINE) : beginStation.station_Chinese_Name;
		receptionDetailList[beginStationIndex].nameRight.nameEnglish =  m_RWIssueRequest.BegainStation == 0 ? CString(TXT_UNDEFINE) :beginStation.station_Eng_Name;

		receptionDetailList[endStationIndex].nameRight.nameChinese =m_RWIssueRequest.EndStation == 0 ? CString(TXT_UNDEFINE) : endStation.station_Chinese_Name;
		receptionDetailList[endStationIndex].nameRight.nameEnglish =  m_RWIssueRequest.EndStation == 0 ? CString(TXT_UNDEFINE) :endStation.station_Eng_Name;

		receptionDetailList[ticketPriceIndex].nameRight.nameChinese =TXT_YUAN_MARK+ComFormatAmount(m_RWIssueRequest.SaleAmount);
		receptionDetailList[ticketCountIndex].nameRight.nameChinese =TXT_YUAN_MARK+ComFormatAmount(m_RWIssueRequest.SaleAmount);

		bool isResult = pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_FINISH || pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ERROR;
		receptionDetailList[ticketCountIndex].nameRight.nameChinese = ComFormatCount(isResult ? model.issueSuccessPiece:model.piece)+_T("  ")+ TICKET_UNIT;

		receptionDetailList[dueIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(isResult ? model.dueOfSuccess : model.due);
		receptionDetailList[paidIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.paid);
		receptionDetailList[changeIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(isResult ? model.changeOfSuccess : model.change);

	}
	else
	{
		int validateDateIndex = 1;
		int depositIndex = 2;
		int saleAmountIndex = 3;
		int dueIndex =4;
		int paidIndex = 5;
		int changeIndex = 6;
		if(model.cardCategory == MULTIRIDE){
			int ridesIndex = 1;
			long balance = 0;
			if(model.pCPUCard!=NULL){
				balance = model.pCPUCard->applicationData.Balance;
			}
			if(model.pULCard!=NULL){
				balance = model.pULCard->applicationData.Balance;
			}
			receptionDetailList[ridesIndex].nameRight.nameChinese = ComFormatCount(balance);
			validateDateIndex+=1;
			depositIndex+=1;
			saleAmountIndex+=1;
			dueIndex+=1;
			paidIndex+=1;
			changeIndex+=1;
		}
		if(model.pCPUCard!=NULL)
		{
			receptionDetailList[validateDateIndex].nameRight.nameChinese = model.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d/%.2d/%.2d"));
		}
		else if(model.pULCard!=NULL)
		{
			receptionDetailList[validateDateIndex].nameRight.nameChinese = model.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d/%.2d/%.2d"));
		}
		receptionDetailList[depositIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.deposit);
		receptionDetailList[saleAmountIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.rwIssueRequest.SaleAmount);

		bool isResult = pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_FINISH || pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ERROR;
		receptionDetailList[dueIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(isResult ? model.dueOfSuccess : model.due);
		receptionDetailList[paidIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.paid);
		receptionDetailList[changeIndex].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(isResult ? model.changeOfSuccess : model.change);

	}
	Invalidate();

}