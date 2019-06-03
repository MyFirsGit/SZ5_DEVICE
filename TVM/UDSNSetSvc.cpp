#include "stdafx.h"
#include "UDSNSetSvc.h"
#include "SYSInfo.h"
#include "udsnsetmaintaindlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CUDSNSetSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetSvc::CUDSNSetSvc()
	: CTVMForeService(UDSN_SET_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(IDD_20119_UDSN_SET_DLG,new CUDSNSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UDSN_SET, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UDSN_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UDSN_SET, IDD_20119_UDSN_SET_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_UDSN_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUDSNSetSvc::~CUDSNSetSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ACC��ˮ��

@param      (i)ULONG    sAccUDSN  ACC��ˮ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetAccUDSN(ULONG sAccUDSN)
{
	m_sAccUDSN = sAccUDSN;
	theCOUNT_INFO.SetUDSNForMain(m_sAccUDSN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����AFC��ˮ��

@param      (i)ULONG    sAfcUDSN  AFC��ˮ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetAfcUDSN(ULONG sAfcUDSN)
{
	m_sAfcUDSN = sAfcUDSN;
	theCOUNT_INFO.SetTransactionNumForMain(m_sAfcUDSN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ECT��ˮ��

@param      (i)ULONG    sEctUDSN  ECT��ˮ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CUDSNSetSvc::SetEctUDSN(ULONG sEctUDSN)
{
	m_sEctUDSN = sEctUDSN;
	theCOUNT_INFO.SetECTNumForMain(m_sEctUDSN);
}