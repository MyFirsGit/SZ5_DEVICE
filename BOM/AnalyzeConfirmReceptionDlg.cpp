#include "stdafx.h"
#include "AnalyzeConfirmReceptionDlg.h"
#include "AnalyzeSvc.h"
#include "CAccTicketParam.h"
#include "tickethelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CAnalyzeConfirmReceptionDlg,CReceptionDlg)
BEGIN_MESSAGE_MAP(CAnalyzeConfirmReceptionDlg, CReceptionDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeConfirmReceptionDlg::CAnalyzeConfirmReceptionDlg(CService* pService)
	: CReceptionDlg(CAnalyzeConfirmReceptionDlg::IDD, pService)
{
	MSG_NAME_GROUP ticketTypeGroup;
	ticketTypeGroup.nameLeft.nameChinese = TXT_TICKET_KIND;
	ticketTypeGroup.nameLeft.nameEnglish = TXT_TICKET_KIND_EN;
	m_BusinessInfo->detailList.push_back(ticketTypeGroup);

	MSG_NAME_GROUP ticketValidateGroup;
	ticketValidateGroup.nameLeft.nameChinese = TXT_TICKET_VALIDATE_DATE;
	ticketValidateGroup.nameLeft.nameEnglish = TXT_TICKET_VALIDATE_DATE_EN;
	m_BusinessInfo->detailList.push_back(ticketValidateGroup);

	MSG_NAME_GROUP ticketStatusGroup;
	ticketStatusGroup.nameLeft.nameChinese = TXT_TICKET_STATUS;
	ticketStatusGroup.nameLeft.nameEnglish = TXT_TICKET_STATUS_EN;
	ticketStatusGroup.nameLeft.color =RED;
	ticketStatusGroup.nameRight.color  = RED;
	m_BusinessInfo->detailList.push_back(ticketStatusGroup);

	MSG_NAME_GROUP ticketBalanceGroup;
	ticketBalanceGroup.nameLeft.nameChinese = TXT_TICKET_BALANCE;
	ticketBalanceGroup.nameLeft.nameEnglish = TXT_TICKET_BALANCE_EN;
	m_BusinessInfo->detailList.push_back(ticketBalanceGroup);

	MSG_NAME_GROUP ticketDepositGroup;
	ticketDepositGroup.nameLeft.nameChinese = TXT_TICKET_DEPOSIT;
	ticketDepositGroup.nameLeft.nameEnglish = TXT_TICKET_DEPOSIT_EN;
	m_BusinessInfo->detailList.push_back(ticketDepositGroup);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeConfirmReceptionDlg::~CAnalyzeConfirmReceptionDlg()
{
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
int CAnalyzeConfirmReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return __super::OnCreate(lpCreateStruct,BUSINESS_AREA);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      更新画面

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeConfirmReceptionDlg::UpdateUI()
{
	CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
	try{
		// 是否是错误画面
		if( pAnalyzeSvc->GetCurDialogGroupID() == CAnalyzeSvc::DIALOG_GROUP_ERROR){
			HideCompoment(BUSINESS_AREA);
			return;
		}
		CAnalyzeSvc::DataModel & model = pAnalyzeSvc->GetDataModel();
		m_BusinessInfo->detailList.clear();
		MSG_NAME_GROUP ticketTypeGroup;
		ticketTypeGroup.nameLeft.nameChinese = TXT_TICKET_KIND;
		ticketTypeGroup.nameLeft.nameEnglish = TXT_TICKET_KIND_EN;
		WORD ticketType = 0;
		if(model.cpuCard!=NULL)
		{
			ticketType = model.cpuCard->applicationData.CardAppMainType;
		}
		if(model.ulCard != NULL)
		{
			ticketType = model.ulCard->applicationData.CardAppMainType;
		}
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		theACC_TICKET.GetTicketInfo(ticketType,ticketTypeDetail);
		ticketTypeGroup.nameRight.nameChinese = ticketTypeDetail.ticketTypeName_cn;
		ticketTypeGroup.nameRight.nameEnglish = ticketTypeDetail.ticketTypeName_en;
		m_BusinessInfo->detailList.push_back(ticketTypeGroup);

		MSG_NAME_GROUP ticketValidateGroup;
		ticketValidateGroup.nameLeft.nameChinese = TXT_TICKET_VALIDATE_DATE;
		ticketValidateGroup.nameLeft.nameEnglish = TXT_TICKET_VALIDATE_DATE_EN;
		if(model.cpuCard!=NULL)
		{
			ticketValidateGroup.nameRight.nameChinese = model.cpuCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日"));
		}
		if(model.ulCard!=NULL)
		{
			ticketValidateGroup.nameRight.nameChinese = model.ulCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日"));
		}
		m_BusinessInfo->detailList.push_back(ticketValidateGroup);

		MSG_NAME_GROUP ticketStatusGroup;
		ticketStatusGroup.nameLeft.nameChinese = TXT_TICKET_STATUS;
		ticketStatusGroup.nameLeft.nameEnglish = TXT_TICKET_STATUS_EN;
		ticketStatusGroup.nameLeft.color =RED;
		ticketStatusGroup.nameRight.color  = RED;
		if(model.cpuCard!=NULL)
		{
			ticketStatusGroup.nameRight.nameChinese = theTICKET_HELPER.GetCPUCardTicketStatusName(model.cpuCard->applicationData.TicketStatus);
		}
		if(model.ulCard!=NULL)
		{
			ticketStatusGroup.nameRight.nameChinese = theTICKET_HELPER.GetULCardTicketStatusName(model.ulCard->applicationData.TicketStatus);
		}
		m_BusinessInfo->detailList.push_back(ticketStatusGroup);
		ProductCategory_t cardCategory;
		theACC_TICKET.GetTicketCategry(ticketType,cardCategory);
		long balance =0;
		if(model.cpuCard!=NULL)
		{
			balance = model.cpuCard->applicationData.Balance;
		}
		if(model.ulCard!=NULL)
		{
			balance = model.ulCard ->applicationData.Balance;
		}
		if(cardCategory == PURSE)
		{
			MSG_NAME_GROUP ticketBalanceGroup;
			ticketBalanceGroup.nameLeft.nameChinese = TXT_TICKET_BALANCE;
			ticketBalanceGroup.nameLeft.nameEnglish = TXT_TICKET_BALANCE_EN;
			ticketBalanceGroup.nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(balance);
			m_BusinessInfo->detailList.push_back(ticketBalanceGroup);
		}
		else if(cardCategory == MULTIRIDE)
		{
			MSG_NAME_GROUP ticketBalanceGroup;
			ticketBalanceGroup.nameLeft.nameChinese = TXT_TICKET_RIDES;
			ticketBalanceGroup.nameLeft.nameEnglish = TXT_TICKET_BALANCE_EN;
			ticketBalanceGroup.nameRight.nameChinese = ComFormatCount(balance) + TIMES_UNIT;
			ticketBalanceGroup.nameRight.nameEnglish = ComFormatCount(balance) + TIMES_UNIT_EN;
			m_BusinessInfo->detailList.push_back(ticketBalanceGroup);
		}
		WORD deposit = 0;
		if(model.cpuCard!=NULL){
			deposit = model.cpuCard->applicationData.Deposit;
		}
		if(model.ulCard!=NULL){
			deposit = model.ulCard->applicationData.Deposit;
		}
		if(deposit!=0)
		{
			MSG_NAME_GROUP ticketDepositGroup;
			ticketDepositGroup.nameLeft.nameChinese = TXT_TICKET_DEPOSIT;
			ticketDepositGroup.nameLeft.nameEnglish = TXT_TICKET_DEPOSIT_EN;
			ticketDepositGroup.nameRight.nameChinese = TXT_YUAN_MARK + ComFormatAmount(deposit);
			m_BusinessInfo->detailList.push_back(ticketDepositGroup);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
