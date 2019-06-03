#include "stdafx.h"
#include "DeviceModeSwitchSvc.h"
#include "DeviceModeSwitchDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CDeviceModeSwitchSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchSvc::CDeviceModeSwitchSvc() : CTVMForeService(MODE_SWITCH_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20116_MODE_SWITCH_DLG,new CDeviceModeSwitchDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MODE_SWITCH, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_MODE_SWITCH_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MODE_SWITCH, IDD_20116_MODE_SWITCH_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MODE_SWITCH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchSvc::~CDeviceModeSwitchSvc()
{
}