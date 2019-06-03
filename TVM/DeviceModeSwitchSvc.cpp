#include "stdafx.h"
#include "DeviceModeSwitchSvc.h"
#include "DeviceModeSwitchDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CDeviceModeSwitchSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchSvc::CDeviceModeSwitchSvc() : CTVMForeService(MODE_SWITCH_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20116_MODE_SWITCH_DLG,new CDeviceModeSwitchDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MODE_SWITCH, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_MODE_SWITCH_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MODE_SWITCH, IDD_20116_MODE_SWITCH_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MODE_SWITCH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchSvc::~CDeviceModeSwitchSvc()
{
}