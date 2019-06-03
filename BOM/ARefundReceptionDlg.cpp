#include "stdafx.h"
#include "ARefundReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CARefundReceptionDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CARefundReceptionDlg, CReceptionDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CARefundReceptionDlg::CARefundReceptionDlg(CService* pService)
: CReceptionDlg(CARefundReceptionDlg::IDD, pService)
{
	InitBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CARefundReceptionDlg::~CARefundReceptionDlg() 
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CARefundReceptionDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
}

void CARefundReceptionDlg::InitBaseInfo()
{
	vector<MSG_NAME_GROUP> & detailList = m_BusinessInfo->detailList;

	MSG_NAME_GROUP msg_tmp;

	// 车票种类
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.strTicketType_CN.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strTicketType_CN,MAX_CASH_RIGHT_LEN);
	//msg_tmp.nameRight.nameEnglish = add_hsp_right(m_DataModel.strTicketType_EN,MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// 有效期
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.strValidDate.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strValidDate,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// 退款押金
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("退款押金"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_DEPOSIT_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.ticketDeposit <= 0 ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.ticketDeposit),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// 退款余额
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("退款余额"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.ticketBalance <= 0 ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.ticketBalance),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// 退款总额
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("退款总额"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_REFUND_MONEY_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.m_TotalValue),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameLeft.color = 255;
	msg_tmp.nameRight.color = 255;
	detailList.push_back(msg_tmp);
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
int CARefundReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域、操作数据区域
	if (-1 == __super::OnCreate(lpCreateStruct,BUSINESS_AREA)){
		return -1;
	}

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
void CARefundReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
}

void CARefundReceptionDlg::UpdateUI()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	
	try{	
		// 退款结束
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_FINISH)
		{
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		CRefundApplySvc::Model & m_DataModel = pRefundApplySvc->GetDataModel();
		vector<MSG_NAME_GROUP> & detailList = m_BusinessInfo->detailList;
		// 车票种类
		detailList[0].nameRight.nameChinese = m_DataModel.strTicketType_CN.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strTicketType_CN,MAX_CASH_RIGHT_LEN);

		// 有效期
		detailList[1].nameRight.nameChinese = m_DataModel.strValidDate.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strValidDate,MAX_CASH_RIGHT_LEN);

		// 退款押金
		detailList[2].nameRight.nameChinese =add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundDeposit),MAX_CASH_RIGHT_LEN);

		// 退款余额
		detailList[3].nameRight.nameChinese =add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundBalance),MAX_CASH_RIGHT_LEN);

		// 退款总额
		detailList[4].nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundBalance + m_DataModel.refundDeposit),MAX_CASH_RIGHT_LEN);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}