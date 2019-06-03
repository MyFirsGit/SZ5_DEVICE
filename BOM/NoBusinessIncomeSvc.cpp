#include "StdAfx.h"
#include "NoBusinessIncomeBaseOfficeDlg.h"
#include "NoBusinessIncomeSvc.h"
#include "GuideInfo.h"
#include "WelcomeReceptionDlg.h"
#include "TemplateHelper.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CNoBusinessIncomeSvc::CNoBusinessIncomeSvc()
    :CBOMForeService(NOBUSINESS_INCOME_SVC)
{
	// 初始化参数
	m_BusinessType = BUSINESS_TYPE_NONE;
	m_ProcessType = OTHER;
	m_Amount = 0;
	m_Fee = 0;

	// 初始化行政处理信息
    InitBusinessInfo();

	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_02701_NOBUSINESS_INCOME_BASE_DLG, new CNoBusinessIncomeBaseOfficeDlg(this));

	//设置乘客画面
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));

	//设置行政处理画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_02701_NOBUSINESS_INCOME_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10002_WELCOME_DLG);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CNoBusinessIncomeSvc::~CNoBusinessIncomeSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化基本信息数据构成

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::InitBusinessInfo()
{
	int i = 0;
    // 处理类别
	m_BusinessInfo[i].nameLeft = add_wsp(TXT_PROCESS_TYPE,MAX_SBC_CASE_LEN);
	m_BusinessInfo[i].nameRight = TXT_NOTHING;
	i++;
	// 收支类型
	m_BusinessInfo[i].nameLeft = add_wsp(TXT_INCOME_TYPE,MAX_SBC_CASE_LEN);
	m_BusinessInfo[i].nameRight = TXT_NOTHING;
	i++;
	// 手续费
	m_BusinessInfo[i].nameLeft = add_wsp(CString(TXT_SEVICE_FEE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_BusinessInfo[i].nameRight = ComFormatAmount(0) + MONEY_UNIT;
	i++;
	// 金额
	m_BusinessInfo[i].nameLeft = add_wsp(CString(TXT_INCOME_AMOUNT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_BusinessInfo[i].nameRight = add_wsp(MONEY_UNIT,6);
	i++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得收入信息

@param      none

@retval     PLABEL_GROUP 收入信息LABEL_GROUP

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CNoBusinessIncomeSvc::GetBusinessInfo()
{
    return m_BusinessInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置收支类型

@param      (i)int    businessType  收支类型: 0:默认,1:收入,2:支出

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::SetBusinessType(BUSINESS_TYPE businessType,CString businessTypeText)
{
	m_BusinessType = businessType;
	m_BusinessType_CN = businessTypeText;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置处理类别

@param      (i)AdministrationProcessType	processType		处理类别

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::SetProcessType(AdministrationProcessType processType,CString processTypeText)
{
	m_ProcessType = processType;
	m_ProcessType_CN = processTypeText;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置收支金额

@param      (i)long  amount  收支金额

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::SetAmount(long amount)
{
	m_Amount = amount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取手续费

@param      (i)long  amount  收支金额

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CNoBusinessIncomeSvc::GetFee(AdministrationProcessType processType)
{
	WORD serviceFee = 0;
	theACC_SERVICEFEE.GetServiceFeeInfo(processType,serviceFee);
	m_Fee = serviceFee;
	return serviceFee;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      非营业性收支处理函数

@param      (i)long amount 收支金额

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::DoAdministrationProcess()
{
	int iBusinessAmount = 0;
	// 如果是支出
	if(BUSINESS_TYPE_EXPENSE == m_BusinessType){
		iBusinessAmount = m_Fee - m_Amount;
	}
	// 如果是收入
	else if(BUSINESS_TYPE_INCOME == m_BusinessType){
		iBusinessAmount = m_Fee + m_Amount;
	}

	FinanceComHdr_t payInfo;
	payInfo.payMethod = BY_CASH;
	payInfo.amount = iBusinessAmount>0? iBusinessAmount : (0-iBusinessAmount);//金额填正值

	AdministrationProcess processInfo;
	processInfo.type = m_ProcessType;
	processInfo.incomeOrOutcome = iBusinessAmount>0? BUSINESS_TYPE_INCOME : BUSINESS_TYPE_EXPENSE; //正值是为收入，负值是为支出

	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertAdministrationProcess(payInfo,processInfo);

	SCAuditKey purchaseKey;
	purchaseKey.serviceID = NOBUSINESS_INCOME_SVC;
	
	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.transAmount = iBusinessAmount;

	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

	// 打印 开钱柜
	//if(!IsPrinterException()){
		PrinterPrint();
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断打印机是否正常

@param      none

@retval     bool true:  false:异常正常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//bool CNoBusinessIncomeSvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeSvc::PrinterPrint()
{
	try{
		CString templateName= _T("ADMIN_PROCESS.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("ADMIN_PROCESS"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			const int hspCount = 20;
			int i = 0;
			while(i < printTexts.GetCount())
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{PROCESS_TYPE}"),m_ProcessType_CN);
				line.Replace(_T("{BUSINESS_TYPE}"),m_BusinessType_CN);
				line.Replace(_T("{SERVICE_FEE}"),ComFormatAmount(m_Fee) + MONEY_UNIT);
				line.Replace(_T("{TRANS_AMOUNT}"),ComFormatAmount(m_Amount) + MONEY_UNIT);
				line.Replace(_T("{TRANS_TIME}"),ComGetCurTime().ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				CStringArray printLine;
				ComPrintLineFormatForTemplate(line,printLine);
				printArr.Append(printLine);
				i++;
			}
		}
		m_pPRTHelper->PrinterPrint(&printArr,true);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}