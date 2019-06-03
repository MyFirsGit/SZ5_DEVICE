#include "StdAfx.h"
#include "TVMFailureRefundSvc.h"
#include "WelcomeReceptionDlg.h"
#include "guideinfo.h"
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

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundSvc::CTVMFailureRefundSvc()
    :CBOMForeService(TVMFAILURE_REFUND_SVC)
{
	// 初始化故障信息
    InitFailureInfo();

	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_01601_TVMFAILURE_REFUND_BASE_DLG, new CTVMFailureRefundBaseOfficeDlg(this));
	
	//设置乘客画面
	m_pDialogFlow->AddDialog(IDD_11601_TVMFAILURE_REFUND_DLG, new CWelcomeReceptionDlg(this));
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_CONFIRM),
                                theGUIDE_INFO.GetPassengerMessageCn(GUIDE_TVMFAILURE_CONFIRM_PASSENGER),
                                theGUIDE_INFO.GetPassengerMessageEn(GUIDE_TVMFAILURE_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_01601_TVMFAILURE_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_11601_TVMFAILURE_REFUND_DLG);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundSvc::~CTVMFailureRefundSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化故障信息数据构成

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::InitFailureInfo()
{
    // 站员画面
	int i=0;
	// 设备编号
	m_failureInfo[i].nameLeft = add_wsp(TXT_DEVICE_NUMBER,MAX_SBC_CASE_LEN);
	i++;

    // 退款凭条号
	m_failureInfo[i].nameLeft = add_wsp(TXT_FAILURE_NUMBER,MAX_SBC_CASE_LEN);
	i++;

	// 退款金额
	m_failureInfo[i].nameLeft = add_wsp(TXT_REFUND_MONEY,MAX_SBC_CASE_LEN);
	m_failureInfo[i].nameRight = add_wsp(MONEY_UNIT,MAX_SBC_CASE_LEN_RIGHT);
	i++;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得故障信息

@param      (o)LABEL_GROUP*&    failureInfo  故障信息

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CTVMFailureRefundSvc::GetFailureInfo()
{
    return m_failureInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得乘客画面信息

@param      (o)MSG_NAME_GROUP*&		receptionOperInfo	 乘客画面信息

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::GetReceptionInfo(MSG_NAME_GROUP*& receptionOperInfo)
{
	receptionOperInfo = m_receptionOperInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置故障信息

@param      (i)unsigned char lineNo	   要修改的行号
            (i)const CString& strItem  要修改的内容

@retval     void
            
@exception  CInnerException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::SetFailureInfo(unsigned char lineNo ,const CString& strItem)
{
    if ((lineNo <= FAILURE_INFO_SIZE) && (lineNo>0)){
        m_failureInfo[lineNo-1].nameRight = add_hsp(strItem,MAX_SBC_CASE_LEN_RIGHT);
    }
    else{
        // 异常处理
		throw CInnerException(GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
    }
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置乘客画面信息

@param      none

@retval     void

@exception  none
*/
////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::SetReceptionOperInfo()
{
	m_receptionOperInfo[0].nameRight.nameChinese = ComFormatAmount(m_refundMoney)+MONEY_UNIT;
	m_receptionOperInfo[0].nameRight.nameEnglish = ComFormatAmount(m_refundMoney)+MONEY_UNIT_EN;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置退款金额

@param      (i)WORD   refundMoney	退款金额

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::SetRefundMoney(DWORD refundMoney)
{
	m_refundMoney = refundMoney;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置故障发生日期

@param     (i)_DATE_TIME	date	故障发生日期

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::SetFailureDate(_DATE_TIME date)
{
	m_date = date;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置交易编号

@param     (i)CString& errorCode	交易编号

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::SetErrorCode(CString errorCode)
{
	m_errorCode = errorCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印方法

@param      (i)CString strDeviceNo			设备编号
			(i)CString strErrorTicketNo		退款凭条号
			(i)CString strAmount			退款金额

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::PrintServiceInfo(CString strDeviceNo,CString strErrorTicketNo)
{
	try{
		CString templateName= _T("TVM_REFUND.template");
		CStringArray printArr;

		for(int i=0;i<theBOM_INFO.GetReceiptPrintTimes(_T("TVM_REFUND"));i++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			CString strBillNum;
			strBillNum.Format(_T("%.10d"),theCOUNT_INFO.GetLastUsedTransactionNum());
			for(int i=0;i<printTexts.GetCount();i++)
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{BILL_NUM}"),strBillNum);
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				line.Replace(_T("{ERROR_DEVICE_CODE}"),strDeviceNo);
				line.Replace(_T("{ERROR_BILL_NUM}"),strErrorTicketNo);
				line.Replace(_T("{REFUND_AMOUNT}"),ComFormatAmount(m_refundMoney) + MONEY_UNIT);
				line.Replace(_T("{TRANS_TIME}"),ComGetCurTime().ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				CStringArray printLines;

				ComPrintLineFormatForTemplate(line,printLines);
				printArr.Append(printLines);
			}
		}
		m_pPRTHelper->PrinterPrint(&printArr,true);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新乘客画面操作区

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::RefreshReceptionOperArea()
{
	CTVMFailureRefundReceptionDlg* pTVMFailureRefundReceptionDlg = (CTVMFailureRefundReceptionDlg*)m_pDialogFlow->GetDialog(IDD_11601_TVMFAILURE_REFUND_DLG);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      一件明细处理

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMFailureRefundSvc::DoDetailInfo(int amount)
{
	try{
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

