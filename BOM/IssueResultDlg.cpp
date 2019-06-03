#include "stdafx.h"
#include "IssueResultDlg.h"
#include "IssueSvc.h"
#include "tickethelper.h"
const static TCHAR* const TXT_TICKET_SUCCESS_COUNT = _T("�ɹ�����");
const static TCHAR* const TXT_TICKET_FAILED_COUNT = _T("ʧ������");

IMPLEMENT_DYNAMIC(CIssueResultDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CIssueResultDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::CIssueResultDlg(CService* pService) : COperationDlg(CIssueResultDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
	CIssueSvc::Model &model = pSvc->GetDataModel();
	// �ڲ���д������
	if(model.issueWay == CTicketHelper::BY_INNER_TPU  ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
		m_TicketInfoGroup[0].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[1].nameLeft = add_wsp(CString(TXT_START_STATION)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[2].nameLeft = add_wsp(CString(TXT_END_STATION)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_TicketInfoGroup[4].nameLeft = add_wsp(CString(TXT_TICKET_COUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
		m_TicketInfoGroup[5].nameLeft = add_wsp(CString(TXT_TICKET_SUCCESS_COUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;	
		m_TicketInfoGroup[6].nameLeft = add_wsp(CString(TXT_TICKET_FAILED_COUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;	
		m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
		m_baseInfo->detailInfo.row = 7;
	}
	// �ⲿ��д������
	else{
		m_TicketInfoGroup[0].nameLeft = add_wsp(TXT_TICKET_KIND + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		// �ƴβ�Ʒ
		if(model.cardCategory == MULTIRIDE){
			m_TicketInfoGroup[1].nameLeft = add_wsp(CString(TXT_VALID_RIDES)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_TicketInfoGroup[2].nameLeft =add_wsp(CString(TXT_TICKET_VALIDATE_DATE)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[4].nameLeft = add_wsp(CString(TXT_TICKET_SALE_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
			m_baseInfo->detailInfo.row = 5;
		}
		// Ǯ��/���ڲ�Ʒ
		else{
			m_TicketInfoGroup[1].nameLeft =add_wsp(CString(TXT_TICKET_VALIDATE_DATE)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[2].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT)+CString(TXT_COLON),MAX_SBC_CASE_LEN) ;
			m_TicketInfoGroup[3].nameLeft = add_wsp(CString(TXT_TICKET_SALE_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_baseInfo->detailInfo.labelGroup = m_TicketInfoGroup;
			m_baseInfo->detailInfo.row = 4;
		}
	}
	
	m_cashInfo->detailInfo.editGroup.cnt = 0;
	m_cashInfo->detailInfo.editGroup.editRect = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::~CIssueResultDlg()
{
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct
//
//@retval     int    \n
//			0:�����ɹ�    -1:����ʧ��
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
int CIssueResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct,  BASE_AREA|CASH_AREA|GUIDE_AREA) == -1) {
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
void CIssueResultDlg::UpdateUI()
{
	CIssueSvc::Model& model = pSvc->GetDataModel();
	if(pSvc->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ERROR && model.issueWay == CTicketHelper::BY_OUTER_TPU){
		HideCompoment(BASE_AREA|CASH_AREA);
		return;
	}
	
	// �ڲ���д������
	if(model.issueWay == CTicketHelper::BY_INNER_TPU ||  pSvc->GetDataModel().issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
		m_TicketInfoGroup[0].nameRight = model.ticketTypeDetail.ticketTypeName_cn;
		CAccLineInfoParam::STATION_INFO beginStationInfo;
		theACC_LINE.GetStationInfobyCode(model.rwIssueRequest.BegainStation,beginStationInfo);
		CAccLineInfoParam::STATION_INFO endStationInfo;
		theACC_LINE.GetStationInfobyCode(model.rwIssueRequest.EndStation,endStationInfo);
		m_TicketInfoGroup[1].nameRight = model.rwIssueRequest.BegainStation == 0 ? CString(TXT_UNDEFINE) : beginStationInfo.station_Chinese_Name;
		m_TicketInfoGroup[2].nameRight = model.rwIssueRequest.EndStation == 0 ? CString(TXT_UNDEFINE) : endStationInfo.station_Chinese_Name;
		m_TicketInfoGroup[3].nameRight = ComFormatAmount(model.rwIssueRequest.SaleAmount) + MONEY_UNIT;
		m_TicketInfoGroup[4].nameRight= ComFormatCount(model.piece)+TICKET_UNIT;
		m_TicketInfoGroup[5].nameRight= ComFormatCount(model.issueSuccessPiece)+TICKET_UNIT;
		m_TicketInfoGroup[6].nameRight = ComFormatCount(model.piece-model.issueSuccessPiece)+TICKET_UNIT;	
	}
	// �ⲿ��д������
	else if(model.issueWay == CTicketHelper::BY_OUTER_TPU){
		m_TicketInfoGroup[0].nameRight = model.ticketTypeDetail.ticketTypeName_cn;
		// �ƴβ�Ʒ
		if(model.cardCategory == MULTIRIDE){
			if(model.pCPUCard!=NULL){
				m_TicketInfoGroup[1].nameRight = ComFormatCount(model.pCPUCard->applicationData.Balance) + TIMES_UNIT;
				m_TicketInfoGroup[2].nameRight =model.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d��%.2d��%.2d��"));
				m_TicketInfoGroup[3].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
			if(model.pULCard!=NULL){
				m_TicketInfoGroup[1].nameRight = ComFormatCount(model.pULCard->applicationData.Balance)+ TIMES_UNIT;
				m_TicketInfoGroup[2].nameRight =model.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d��%.2d��%.2d��"));
				m_TicketInfoGroup[3].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			}
		}
		// Ǯ��/���ڲ�Ʒ
		else{
			if(model.pCPUCard!=NULL){
				m_TicketInfoGroup[1].nameRight =model.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d��%.2d��%.2d��"));

			}
			if(model.pULCard!=NULL){
				m_TicketInfoGroup[1].nameRight =model.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d��%.2d��%.2d��"));

			}
			m_TicketInfoGroup[2].nameRight =ComFormatAmount(model.deposit) + MONEY_UNIT;
			m_TicketInfoGroup[3].nameRight =ComFormatAmount(model.rwIssueRequest.SaleAmount) + MONEY_UNIT;
		}
	}
	m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(model.dueOfSuccess)+MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[1].nameRight = ComFormatAmount(model.paid)+MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.changeOfSuccess)+MONEY_UNIT;
	m_baseInfoArea->Invalidate();
	m_cashInfoArea->Invalidate();
};