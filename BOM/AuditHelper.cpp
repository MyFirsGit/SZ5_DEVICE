#include "stdafx.h"
#include "AuditHelper.h"
#include "Templatehelper.h"

/**
@brief RW Helper
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::CAuditHelper(CService &service):CServiceHelper(service)
{
	m_pPRTHelper = new PRTHelper(service);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::~CAuditHelper()
{
	delete m_pPRTHelper;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       打印审计信息

@param       (i)SALESTAT_KIND kind            
@param       (o)PURCHASE_DATA& scData
@param       (i)bool bAutoBusiChange

@retval      无

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAuditHelper::PrintAuditReport(SALESTAT_KIND kind, PurchaseData& purchaseData)
{
	try
	{
		//SCAuditValue summaryACCIssue;
		//SCAuditValue summaryACCAdjust;
		//SCAuditValue summaryACCRefund;
		//SCAuditValue summaryACCCounteract;
		//SCAuditValue summaryECTIssue;
		//SCAuditValue summaryECTCharge;
		//SCAuditValue summaryECTAdjustCash;
		//SCAuditValue summaryNonBusinessIncome;
		//SCAuditValue summaryTVMRefund;
		//SCAuditValue summary;

		//for (PurchaseItemMap::iterator it = purchaseData.purchaseItems.begin();it!=purchaseData.purchaseItems.end();it++)
		//{
		//	if(it->first.serviceID == ISSUE_SVC && it->first.cardIssuancer == ACC)
		//	{
		//		summaryACCIssue += it->second;
		//		summary +=it->second;
		//	}
		//	else if(it->first.serviceID == ISSUE_SVC && it->first.cardIssuancer == ECT)
		//	{
		//		summaryECTIssue +=it->second;
		//		summary +=it->second;
		//	}
		//	else if(it->first.serviceID == CHARGE_SVC && it->first.cardIssuancer == ECT)
		//	{
		//		summaryECTCharge += it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == ADJUST_SVC && it->first.cardIssuancer == ACC && it->first.flag!=0)
		//	{
		//		summaryACCAdjust +=it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == ADJUST_SVC && it->first.cardIssuancer == ECT && it->first.flag!=0)
		//	{
		//		summaryECTAdjustCash +=it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == REFUND_SVC && it->first.cardIssuancer == ACC)
		//	{
		//		summaryACCRefund +=it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == COUNTERACT_SVC)
		//	{
		//		summaryACCCounteract += it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == TVMFAILURE_REFUND_SVC)
		//	{
		//		summaryTVMRefund += it->second;
		//		summary+=it->second;
		//	}
		//	else if(it->first.serviceID == NOBUSINESS_INCOME_SVC)
		//	{
		//		summaryNonBusinessIncome += it->second;
		//		summary+=it->second;
		//	}
		//}
		//CString strBillNum;
		//strBillNum.Format("%.10d",theCOUNT_INFO.GetLastUsedTransactionNum());

		//CString templateName=kind == STAFF_ONCE ? "SALE_STAT_PERSON.template":"SALE_STAT_DATE.template";
		//CStringArray printTexts;
		//theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
		//for(int i=0;i<printTexts.GetCount();i++)
		//{
		//	CString& line = printTexts.ElementAt(i);
		//	line.Replace("{BILL_NUM}",strBillNum);
		//	line.Replace("{ISSUE_ACC_AMOUNT}",ComFormatAmount(summaryACCIssue.transAmount + summaryACCIssue.depositAmount));
		//	line.Replace("{ISSUE_ACC_COUNT}",ComFormatCount(summaryACCIssue.transCount));
		//	line.Replace("{ADJUST_ACC_AMOUNT}",ComFormatAmount(summaryACCAdjust.transAmount));
		//	line.Replace("{ADJUST_ACC_COUNT}",ComFormatCount(summaryACCAdjust.transCount));
		//	line.Replace("{REFUND_ACC_AMOUNT}",ComFormatAmount(summaryACCRefund.transAmount));
		//	line.Replace("{REFUND_ACC_COUNT}",ComFormatCount(summaryACCRefund.transCount));
		//	line.Replace("{COUNTERACT_ACC_AMOUNT}",ComFormatAmount(summaryACCCounteract.transAmount));
		//	line.Replace("{COUNTERACT_ACC_COUNT}",ComFormatCount(summaryACCCounteract.transCount));

		//	line.Replace("{ISSUE_ECT_AMOUNT}",ComFormatAmount(summaryECTIssue.transAmount + summaryECTIssue.depositAmount));
		//	line.Replace("{ISSUE_ECT_COUNT}",ComFormatCount(summaryECTIssue.transCount));
		//	line.Replace("{CHARGE_ECT_AMOUNT}",ComFormatAmount(summaryECTCharge.transAmount));
		//	line.Replace("{CHARGE_ECT_COUNT}",ComFormatCount(summaryECTCharge.transCount));
		//	line.Replace("{ADJUST_ECT_AMOUNT}",ComFormatAmount(summaryECTAdjustCash.transAmount));
		//	line.Replace("{ADJUST_ECT_COUNT}",ComFormatCount(summaryECTAdjustCash.transCount));
		//	
		//	line.Replace("{NOBUSINESS_INCOME_AMOUNT}",ComFormatAmount(summaryNonBusinessIncome.transAmount));
		//	line.Replace("{NOBUSINESS_INCOME_COUNT}",ComFormatCount(summaryNonBusinessIncome.transCount));
		//	line.Replace("{TVM_REFUND_AMOUNT}",ComFormatAmount(summaryTVMRefund.transAmount));
		//	line.Replace("{TVM_REFUND_COUNT}",ComFormatCount(summaryTVMRefund.transCount));

		//	line.Replace("{REFUND_ECT_AMOUNT}",ComFormatAmount(0));
		//	line.Replace("{REFUND_ECT_COUNT}",ComFormatCount(0));
		//	line.Replace("{REFUND_ECT_DEPOSIT_AMOUNT}",ComFormatAmount(0));
		//	line.Replace("{REFUND_ECT_DEPOSIT_COUNT}",ComFormatCount(0));
		//	line.Replace("{COST_ECT_AMOUNT}",ComFormatAmount(0));
		//	line.Replace("{COST_ECT_COUNT}",ComFormatCount(0));
		//	
		//	line.Replace("{TOTAL_AMOUNT}",ComFormatAmount(summary.transAmount + summary.depositAmount));
		//	line.Replace("{DEVICE_CODE}",theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
		//	
		//	if(kind == SALESTAT_KIND::STAFF_ONCE)
		//	{
		//		line.Replace("{USER_ID}",theAPP_SESSION.GetUserInfo().sUserID);
		//	}
		//	line.Replace("{BEGIN_DATE_TIME}",purchaseData.auditTimeFrom.ToString("%.4d-%.2d-%.2d %.2d:%.2d:%.2d"));
		//	line.Replace("{END_DATA_TIME}",purchaseData.auditTimeTo.ToString("%.4d-%.2d-%.2d %.2d:%.2d:%.2d"));
		//	line.Replace("{STATION_NAME}",theAPP_SESSION.GetStationNameCN());
		//	ComPrintLineFormatForTemplate(line);
		//}

		//m_pPRTHelper->PrinterPrint(&printTexts,false);
	}
	catch (CSysException& e) 
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) 
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

