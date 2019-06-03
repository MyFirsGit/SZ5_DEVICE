#include "stdafx.h"
#include "RefundReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRefundReceptionDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CRefundReceptionDlg, CReceptionDlg)
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
CRefundReceptionDlg::CRefundReceptionDlg(CService* pService)
    : CReceptionDlg(CRefundReceptionDlg::IDD, pService)
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
CRefundReceptionDlg::~CRefundReceptionDlg() 
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
void CRefundReceptionDlg::OnDraw(CDC* pDC)
{
     __super::OnDraw(pDC);
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
int CRefundReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CRefundReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

}

void CRefundReceptionDlg::InitBaseInfo()
{
	vector<MSG_NAME_GROUP>& detailList = m_BusinessInfo->detailList;
	detailList.clear();
	// ��Ʊ����
	MSG_NAME_GROUP msg_tmp;
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	detailList.push_back(msg_tmp);

	// ��Ч��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	detailList.push_back(msg_tmp);

	// Ʊ�����
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	
	// Ʊ��Ѻ��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_DEPOSIT,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_DEPOSIT_EN,MAX_SBC_CASE_LEN);
	detailList.push_back(msg_tmp);

	// �˿���
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_REFUND_MONEY,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_REFUND_MONEY_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.color = 255;
	msg_tmp.nameRight.color = 255;
	detailList.push_back(msg_tmp);
}


void CRefundReceptionDlg::UpdateUI()
{
	
	CRefundSvc* pRefundSvc = (CRefundSvc*)GetService();
	try{	
		// �����˿�
		if (pRefundSvc->GetCurDialogGroupID() == CRefundSvc::DIALOG_GROUP_REFUND_NOT) {
			//�����������	
			HideCompoment(BUSINESS_AREA);
			theSERVICE_MGR.SetState(FINISH);
		}
		// �˿����
		else if(pRefundSvc->GetCurDialogGroupID() == CRefundSvc::DIALOG_GROUP_FINISH)
		{
			theSERVICE_MGR.SetState(FINISH);
		}
		else
		{
			CRefundSvc::Model& dataModel = pRefundSvc->GetDataModel();
			// ��Ʊ����
			m_BusinessInfo->detailList[0].nameRight.nameChinese = dataModel.strTicketType_CN.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(dataModel.strTicketType_CN,MAX_CASH_RIGHT_LEN);
			m_BusinessInfo->detailList[0].nameRight.nameEnglish = add_hsp_right(dataModel.strTicketType_EN,MAX_CASH_RIGHT_LEN);

			// ��Ч��
			m_BusinessInfo->detailList[1].nameRight.nameChinese = dataModel.strValidDate.IsEmpty() ? add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN) : add_hsp_right(dataModel.strValidDate,MAX_CASH_RIGHT_LEN);
			m_BusinessInfo->detailList[1].nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);

			// Ʊ�����
			if(dataModel.cardCategory == PURSE){
				m_BusinessInfo->detailList[2].nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.ticketBalance),MAX_CASH_RIGHT_LEN);
			}
			else if(dataModel.cardCategory == MULTIRIDE){
				m_BusinessInfo->detailList[2].nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.ticketBalance) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
			}
			else if(dataModel.cardCategory == PERIOD){
				m_BusinessInfo->detailList[2].nameRight.nameChinese = add_hsp_right(TXT_UNDEFINE,MAX_CASH_RIGHT_LEN);
			}

			// Ʊ��Ѻ��
			m_BusinessInfo->detailList[3].nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.deposit),MAX_CASH_RIGHT_LEN);
			m_BusinessInfo->detailList[3].nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);

			// �˿���
			m_BusinessInfo->detailList[4].nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.refundValue + dataModel.refundDeposit),MAX_CASH_RIGHT_LEN);
			m_BusinessInfo->detailList[4].nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
			m_BusinessArea->Invalidate();
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

	
	
	
}