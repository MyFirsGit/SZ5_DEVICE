#include "stdafx.h"
#include "UDSNSetSvc.h"
#include "SYSInfo.h"
#include "udsnsetmaintaindlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CUDSNSetSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetSvc::CUDSNSetSvc()
	: CTVMForeService(UDSN_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_20119_UDSN_SET_DLG,new CUDSNSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UDSN_SET, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UDSN_SET, IDD_20119_UDSN_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_UDSN_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetSvc::~CUDSNSetSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置ACC流水号

@param      (i)ULONG    sAccUDSN  ACC流水号

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetAccUDSN(ULONG sAccUDSN)
{
	m_sAccUDSN = sAccUDSN;
	theCOUNT_INFO.SetUDSNForMain(m_sAccUDSN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置AFC流水号

@param      (i)ULONG    sAfcUDSN  AFC流水号

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetAfcUDSN(ULONG sAfcUDSN)
{
	m_sAfcUDSN = sAfcUDSN;
	theCOUNT_INFO.SetTransactionNumForMain(m_sAfcUDSN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置ECT流水号

@param      (i)ULONG    sEctUDSN  ECT流水号

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetEctUDSN(ULONG sEctUDSN)
{
	m_sEctUDSN = sEctUDSN;
	theCOUNT_INFO.SetECTNumForMain(m_sEctUDSN);
}