#include "stdafx.h"
#include "UPSInfoSvc.h"
#include "UPSInfoMaintainDlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CUPSInfoSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoSvc::CUPSInfoSvc()
: CBOMForeService(UPS_INFO_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(IDD_UPS_INFO_DLG,new CUPSInfoMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UPS_INFO, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_WAITING));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UPS_INFO, IDD_UPS_INFO_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_UPS_INFO);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoSvc::~CUPSInfoSvc()
{
}