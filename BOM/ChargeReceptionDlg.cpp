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
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CChargeReceptionDlg::CChargeReceptionDlg(CService* pService)
    :CReceptionDlg(CChargeReceptionDlg::IDD, pService)
{

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CChargeReceptionDlg::~CChargeReceptionDlg()
{
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
int CChargeReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,BUSINESS_AREA);
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
void CChargeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	try{	
		// ���ܳ�ֵ
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
	// Ʊ������
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(dataModel.strTicketName,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(dataModel.strTicketEnName,MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// ��Ч��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(dataModel.validDate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d")),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// Ѻ��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_DEPOSIT,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_DEPOSIT_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.deposit),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);

	if(dataModel.ticketCategory == PURSE){
		// Ʊ�����
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.balance),MAX_CASH_RIGHT_LEN);
		detailList.push_back(msg_tmp);
		// ��ֵ���
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGE_AMOUNT,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGE_AMOUNT_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.chargeValue),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 255;
		msg_tmp.nameRight.color = 255;
		detailList.push_back(msg_tmp);
		// ��ֵ�����
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGEOVER_AMOUNT,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGEOVER_AMOUNT_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.valueAfterCharge),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 0;
		msg_tmp.nameRight.color = 0;
		detailList.push_back(msg_tmp);
	}
	else if(dataModel.ticketCategory == MULTIRIDE){
		// Ʊ�����
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_RIDES_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.balance) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		detailList.push_back(msg_tmp);
		// ��ֵ����
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGE_RIDE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGE_RIDE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.chargeValue) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 255;
		msg_tmp.nameRight.color = 255;
		detailList.push_back(msg_tmp);

		// ��ֵ�����
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHARGEOVER_RIDE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHARGEOVER_RIDE_EN,MAX_SBC_CASE_LEN);
		msg_tmp.nameRight.nameChinese = add_hsp_right(ComFormatCount(dataModel.valueAfterCharge) + CString(TIMES_UNIT),MAX_CASH_RIGHT_LEN);
		msg_tmp.nameLeft.color = 0;
		msg_tmp.nameRight.color = 0;
		detailList.push_back(msg_tmp);

	}
	// Ӧ�ս��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TOTAL_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TOTAL_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.due),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// �Ѹ����
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_INCOME_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_INCOME_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.paid),MAX_CASH_RIGHT_LEN);
	detailList.push_back(msg_tmp);
	// ������
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_CHANGE_RECEPTION,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_CHANGE_RECEPTION_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(CString(TXT_YUAN_MARK) + ComFormatAmount(dataModel.change),MAX_CASH_RIGHT_LEN);
	msg_tmp.nameLeft.color = 255;
	msg_tmp.nameRight.color = 255;
	detailList.push_back(msg_tmp);

	m_BusinessArea->Invalidate();
}