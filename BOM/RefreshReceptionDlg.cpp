#include "stdafx.h"
#include "RefreshSvc.h"
#include "RefreshReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRefreshReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CRefreshReceptionDlg,CReceptionDlg)
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
CRefreshReceptionDlg::CRefreshReceptionDlg(CService* pService)
    :CReceptionDlg(CRefreshReceptionDlg::IDD, pService)
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
CRefreshReceptionDlg::~CRefreshReceptionDlg()
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
int CRefreshReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // ����������Ϣ���򡢲�����������
    if(-1 == __super::OnCreate(lpCreateStruct,BUSINESS_AREA )){
        return -1;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CRefreshReceptionDlg::UpdateUI()
{
	CRefreshSvc* pSvc = (CRefreshSvc*)GetService();
	try{	
		
		if (pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_REFRESH_NOT) {
			//�����������	
			HideCompoment(BUSINESS_AREA);
			return;
		}
		CRefreshSvc::DataModel& model = pSvc->GetDataModel();
		vector<MSG_NAME_GROUP> &detailList = m_BusinessInfo->detailList;
		detailList.clear();
		// Ʊ������
		MSG_NAME_GROUP msgTicketType;
		msgTicketType.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
		msgTicketType.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
		msgTicketType.nameRight.nameChinese = model.ticketTypeDetail.ticketTypeName_cn;
		msgTicketType.nameRight.nameEnglish = model.ticketTypeDetail.ticketTypeName_en;
		detailList.push_back(msgTicketType);

		// Ʊ�����
		MSG_NAME_GROUP msgBalance;

		if(model.cardCategory == PURSE)
		{
			msgBalance.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
			msgBalance.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
			msgBalance.nameRight.nameChinese = TXT_YUAN_MARK + ComFormatAmount(model.balance);
			m_BusinessInfo->detailList.push_back(msgBalance);
		}
		else if(model.cardCategory == MULTIRIDE)
		{
			msgBalance.nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
			msgBalance.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
			msgBalance.nameRight.nameChinese =  ComFormatCount(model.balance);
			m_BusinessInfo->detailList.push_back(msgBalance);
		}
		else if(model.cardCategory == PERIOD)
		{
		}
	
		//ԭ��Ч��
		MSG_NAME_GROUP msgOldValidate;
		msgOldValidate.nameLeft.nameChinese = add_wsp(TXT_OLD_VALIDATE,MAX_SBC_CASE_LEN);
		msgOldValidate.nameLeft.nameEnglish = add_wsp(TXT_OLD_VALIDATE_EN,MAX_SBC_CASE_LEN);
		msgOldValidate.nameRight.nameChinese = model.orignalValidate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
		m_BusinessInfo->detailList.push_back(msgOldValidate);

		if(pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_FINISH)
		{
			//����Ч��
			MSG_NAME_GROUP msgNewValidate;
			msgNewValidate.nameLeft.nameChinese = add_wsp(TXT_NEW_VALIDATE,MAX_SBC_CASE_LEN);
			msgNewValidate.nameLeft.nameEnglish = add_wsp(TXT_NEW_VALIDATE_EN,MAX_SBC_CASE_LEN);
			msgNewValidate.nameRight.nameChinese = model.newValidate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
			m_BusinessInfo->detailList.push_back(msgNewValidate);
		}
		m_BusinessArea->Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
