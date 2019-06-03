#include "stdafx.h"
#include "ChargeSvc.h"
#include "ChargeReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CChargeReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CChargeReceptionDlg,CReceptionDlg)
	ON_WM_ACTIVATE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CChargeReceptionDlg::CChargeReceptionDlg(CService* pService)
    :CReceptionDlg(CChargeReceptionDlg::IDD, pService)
{

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CChargeReceptionDlg::~CChargeReceptionDlg()
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
int CChargeReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,BUSINESS_AREA);
    return 0;
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
void CChargeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	try{	
		// 不能充值
		if (pChargeSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_CHARGE_NOT){
			HideCompoment(BUSINESS_AREA);
			theSERVICE_MGR.SetState(FINISH);
		}
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pChargeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}


void CChargeReceptionDlg::UpdateUI()
{
	CChargeSvc* pSvc = GetService<CChargeSvc>();
	CChargeSvc::Model& dataModel = pSvc->GetDataModel();

	vector<MSG_NAME_GROUP> & detailList = m_BusinessInfo->detailList;
	detailList.clear();
	MSG_NAME_GROUP msg_tmp;
	// 票卡类型
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(dataModel.strTicketName,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(dataModel.strTicketEnName,MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// 有效期
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(dataModel.validDate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d")),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// 押金
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_DEPOSIT,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_DEPOSIT_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.deposit),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	if(dataModel.ticketCategory == PURSE){
		// 票内余额
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.balance),MAX_CASH_RIGHT_LEN);
		detailList.push_back(msg_tmp);
		// 充值金额
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGE_AMOUNT,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGE_AMOUNT_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.chargeValue),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 255;
		msg_tmp.nameRight.color = 255;
		detailList.push_back(msg_tmp);
		// 充值后余额
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGEOVER_AMOUNT,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGEOVER_AMOUNT_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.valueAfterCharge),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 0;
		msg_tmp.nameRight.color = 0;
		detailList.push_back(msg_tmp);
	}
	else if(dataModel.ticketCategory == MULTIRIDE){
		// 票内余次
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_RIDES_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.balance) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		detailList.push_back(msg_tmp);
		// 充值次数
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGE_RIDE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGE_RIDE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.chargeValue) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 255;
		msg_tmp.nameRight.color = 255;
		detailList.push_back(msg_tmp);

		// 充值后余次
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGEOVER_RIDE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGEOVER_RIDE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.valueAfterCharge) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 0;
		msg_tmp.nameRight.color = 0;
		detailList.push_back(msg_tmp);

	}
	// 应收金额
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TOTAL_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TOTAL_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.due),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// 已付金额
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_INCOME_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_INCOME_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.paid),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// 找零金额
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHANGE_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHANGE_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.change),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameLeft.color = 255;
	msg_tmp.nameRight.color = 255;
	detailList.push_back(msg_tmp);

	m_BusinessArea->Invalidate();
}