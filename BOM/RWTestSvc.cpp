#include "stdafx.h"
#include "RWTestSvc.h"
#include "CardReadWriteMachineTestMaintainDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRWTestSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::CRWTestSvc()
:CBOMForeService(OUTRW_TEST_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG,new CCardReadWriteMachineTestMaintainDlg(this));

	//����TH���Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CARD_READWRITE_MACHINE,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_CHOOSE_ITEM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CARD_READWRITE_MACHINE,IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);

	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_CARD_READWRITE_MACHINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::~CRWTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ƭ��ȡ�ص�����

@param      (i)UINT errCode				�������
@param      (i)CRWReadCard* rwReadCard  ��ȡ��Ƭ�����ָ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::OnReadCard(UINT errCode,CRWReadCard* rwReadCard)
{
	CMessageBox::End();
	RW_READ_CARD_RESPONSE cardRspInfo ;
	rwReadCard->GetRWResponse(&cardRspInfo);
	// ����ʧ��
	if(errCode != RW_RSP_OK){
		if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode));
		}
	}
	else{
		CCardReadWriteMachineTestMaintainDlg* pReadDlg = (CCardReadWriteMachineTestMaintainDlg*)m_pDialogFlow->GetDialog(IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);
		// UL��
		if(cardRspInfo.CardPHType == ULTRALIGHT){
			CULCard ulCard;
			// ��ö�����Ϣ 			    
			ulCard.GetCardInfo(cardRspInfo.CardInfo);
			// ��ÿ���
			CString strTicketNum;
			strTicketNum.Format(_T("%09u"),ulCard.applicationData.CardLogicalNumber);
			// ��ʾ����
			pReadDlg->m_CardReadWriteInfo[0].nameRight = strTicketNum;
			// ��ʾ��Ʊ����
			pReadDlg->m_CardReadWriteInfo[1].nameRight = _T("UL��");
			// 
			pReadDlg->Invalidate();
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_READ_SUCCESS));
		}
		// CPU��
		else if(cardRspInfo.CardPHType == CPU_TICKET){
			CCPUCard cpuCard;
			// ��ö�����Ϣ 			    
			cpuCard.GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);
			// ��ÿ���
			CString strTicketNum;
			strTicketNum.Format(_T("%09u"),cpuCard.applicationData.CardLogicalNumber);
			// ��ʾ����
			pReadDlg->m_CardReadWriteInfo[0].nameRight = strTicketNum;
			// ��ʾ��Ʊ����
			pReadDlg->m_CardReadWriteInfo[1].nameRight = _T("CPU��");
			// 
			pReadDlg->Invalidate();
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_READ_SUCCESS));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_UNKNOWN_CARD));
		}
	}
}