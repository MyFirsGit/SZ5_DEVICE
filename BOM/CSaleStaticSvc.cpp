#include "stdafx.h"
#include "CSaleStaticSvc.h"
#include "guideinfo.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::CSaleStaticSvc()
    :CBOMForeService(SALE_STATIC_SVC)
{
	// ���ò���Ա����
    m_PurcherDataSize = InitDetailInfo();
	m_pDialogFlow->AddDialog(IDD_01701_SALE_STATIC_DLG, new CSaleStaticBaseOfficeDlg(this));

	// ���ó˿ͻ���

	// ���û������     
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_AUDIT_PRINT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_01701_SALE_STATIC_DLG);

  	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::~CSaleStaticSvc()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��ͳ����Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CSaleStaticSvc::InitDetailInfo()
{
	int i = 0;
	PurchaseData  currentLoginPurchase = theSCAudit_MGR.GetCurrentLoginPurchase();

	// Title
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_BUSINESS,3);
    m_auditGUIInfo[i].nameCenter = add_hsp(TXT_COUNT,8);
    m_auditGUIInfo[i].nameRight = add_hsp(TXT_INCOME_AMOUNT,14);
   
	SCAuditValue summaryIssueValue;
	SCAuditValue summaryChargeValue;
	SCAuditValue summaryAdjustValue;
	SCAuditValue summaryRefundValue;
	SCAuditValue summaryCounteractValue;
	SCAuditValue summaryTVMRefundValue;
	SCAuditValue summaryNonBusinessIncomeValue;
	SCAuditValue summaryValue;
	for (PurchaseItemMap::iterator it = currentLoginPurchase.purchaseItems.begin();it!=currentLoginPurchase.purchaseItems.end();it++)
	{
		switch(it->first.serviceID)
		{
		case ISSUE_SVC:
			summaryIssueValue += it->second;
			summaryValue +=it->second;
			break;
		case CHARGE_SVC:
			summaryChargeValue+=it->second;
			summaryValue+=it->second;
			break;
		case ADJUST_SVC:
			if(it->first.flag!=0)
			{
				summaryAdjustValue+=it->second;
				summaryValue+=it->second;
			}
			break;
		case REFUND_SVC:
			summaryRefundValue+=it->second;
			summaryValue+=it->second;
			break;
		case COUNTERACT_SVC:
			summaryCounteractValue+=it->second;
			summaryValue+=it->second;
			break;
		case TVMFAILURE_REFUND_SVC:
			summaryTVMRefundValue+=it->second;
			summaryValue+=it->second;
			break;
		case NOBUSINESS_INCOME_SVC:
			summaryNonBusinessIncomeValue+=it->second;
			summaryValue+=it->second;
			break;
		default:
			break;
		}
	}
	 i++;  
    // ��Ʊ
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_ISSUE_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"), summaryIssueValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryIssueValue.transAmount),14);
    i++;  
	//��ֵ
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_CHARGE_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryChargeValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryChargeValue.transAmount),14);
    i++; 
     //����
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_ADJUST_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryAdjustValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryAdjustValue.transAmount),14);
    i++; 
    // �˿�
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_REFUND_MAIN,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryRefundValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryRefundValue.transAmount),14);
    i++; 
    // ����
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_CANCEL_READ_TICKET_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryCounteractValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryCounteractValue.transAmount),14);
    i++; 
    // TVM�����˿�������
    m_auditGUIInfo[i].nameLeft = add_hsp(TXT_TVMFAILURE_REFUND_SVC_CN,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryTVMRefundValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryTVMRefundValue.transAmount),14);

	i++;
	//��Ӫҵ����֧���
	m_auditGUIInfo[i].nameLeft = add_hsp(TXT_NOBUSINESS_INCOME_SVC_CN,3);
	m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryNonBusinessIncomeValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryNonBusinessIncomeValue.transAmount),14);
	
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ó�Ʊ�ֽ���Ϣ������

@param      void
@retval     void
@exception  void
*/
//////////////////////////////////////////////////////////////////////////
UINT CSaleStaticSvc::GetPurcherDataSize()
{
	return m_PurcherDataSize;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ������ͳ����Ϣ

@param      (o)LABEL_GROUP_THREE*& auditServiceInfo		ҵ�񲿷ֵ���Ϣ����
@param		(o)LABEL_GROUP_THREE*& auditOthersInfo		�������ֵ���Ϣ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP_THREE CSaleStaticSvc::GetAuditInfoGroup()
{
	return m_auditGUIInfo;
}

