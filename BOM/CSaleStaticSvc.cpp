#include "stdafx.h"
#include "CSaleStaticSvc.h"
#include "guideinfo.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::CSaleStaticSvc()
    :CBOMForeService(SALE_STATIC_SVC)
{
	// 设置操作员画面
    m_PurcherDataSize = InitDetailInfo();
	m_pDialogFlow->AddDialog(IDD_01701_SALE_STATIC_DLG, new CSaleStaticBaseOfficeDlg(this));

	// 设置乘客画面

	// 设置画面分组     
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_AUDIT_PRINT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_01701_SALE_STATIC_DLG);

  	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::~CSaleStaticSvc()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化统计信息

@param      无

@retval     无

@exception  无
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
    // 售票
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_ISSUE_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"), summaryIssueValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryIssueValue.transAmount),14);
    i++;  
	//充值
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_CHARGE_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryChargeValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryChargeValue.transAmount),14);
    i++; 
     //更新
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_ADJUST_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryAdjustValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryAdjustValue.transAmount),14);
    i++; 
    // 退款
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_REFUND_MAIN,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryRefundValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryRefundValue.transAmount),14);
    i++; 
    // 抵消
    m_auditGUIInfo[i].nameLeft = add_wsp(TXT_CANCEL_READ_TICKET_SVC,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryCounteractValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryCounteractValue.transAmount),14);
    i++; 
    // TVM故障退款次数金额
    m_auditGUIInfo[i].nameLeft = add_hsp(TXT_TVMFAILURE_REFUND_SVC_CN,3);
    m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryTVMRefundValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryTVMRefundValue.transAmount),14);

	i++;
	//非营业性收支金额
	m_auditGUIInfo[i].nameLeft = add_hsp(TXT_NOBUSINESS_INCOME_SVC_CN,3);
	m_auditGUIInfo[i].nameCenter.Format(_T("%8d"),summaryNonBusinessIncomeValue.transCount);
	m_auditGUIInfo[i].nameRight = add_hsp(ComFormatAmount(summaryNonBusinessIncomeValue.transAmount),14);
	
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得车票现金信息的行数

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
@brief      取得销售统计信息

@param      (o)LABEL_GROUP_THREE*& auditServiceInfo		业务部分的信息内容
@param		(o)LABEL_GROUP_THREE*& auditOthersInfo		其他部分的信息内容

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP_THREE CSaleStaticSvc::GetAuditInfoGroup()
{
	return m_auditGUIInfo;
}

