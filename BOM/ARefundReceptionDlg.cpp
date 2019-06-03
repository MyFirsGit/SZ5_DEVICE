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
@brief      ���캯��

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
@brief      ��������

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
@brief      ������ƺ���

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

	// ��Ʊ����
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.strTicketType_CN.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strTicketType_CN,MAX_CASH_RIGHT_LEN);
	//msg_tmp.nameRight.nameEnglish = add_hsp_right(m_DataModel.strTicketType_EN,MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// ��Ч��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.strValidDate.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strValidDate,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// �˿�Ѻ��
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("�˿�Ѻ��"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_DEPOSIT_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.ticketDeposit <= 0 ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.ticketDeposit),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// �˿����
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("�˿����"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = m_DataModel.ticketBalance <= 0 ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.ticketBalance),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	// �˿��ܶ�
	msg_tmp.nameLeft.nameChinese = add_wsp(_T("�˿��ܶ�"),MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_REFUND_MONEY_EN,MAX_SBC_CASE_LEN);
	//msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.m_TotalValue),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameLeft.color = 255;
	msg_tmp.nameRight.color = 255;
	detailList.push_back(msg_tmp);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CARefundReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ����������Ϣ���򡢲�����������
	if (-1 == __super::OnCreate(lpCreateStruct,BUSINESS_AREA)){
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

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
		// �˿����
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_FINISH)
		{
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		CRefundApplySvc::Model & m_DataModel = pRefundApplySvc->GetDataModel();
		vector<MSG_NAME_GROUP> & detailList = m_BusinessInfo->detailList;
		// ��Ʊ����
		detailList[0].nameRight.nameChinese = m_DataModel.strTicketType_CN.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strTicketType_CN,MAX_CASH_RIGHT_LEN);

		// ��Ч��
		detailList[1].nameRight.nameChinese = m_DataModel.strValidDate.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(m_DataModel.strValidDate,MAX_CASH_RIGHT_LEN);

		// �˿�Ѻ��
		detailList[2].nameRight.nameChinese =add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundDeposit),MAX_CASH_RIGHT_LEN);

		// �˿����
		detailList[3].nameRight.nameChinese =add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundBalance),MAX_CASH_RIGHT_LEN);

		// �˿��ܶ�
		detailList[4].nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(m_DataModel.refundBalance + m_DataModel.refundDeposit),MAX_CASH_RIGHT_LEN);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}