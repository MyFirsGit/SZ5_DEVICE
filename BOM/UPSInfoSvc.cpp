#include "stdafx.h"
#include "UPSInfoSvc.h"
#include "UPSInfoMaintainDlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CUPSInfoSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoSvc::CUPSInfoSvc()
: CBOMForeService(UPS_INFO_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_UPS_INFO_DLG,new CUPSInfoMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UPS_INFO, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_WAITING));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UPS_INFO, IDD_UPS_INFO_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_UPS_INFO);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoSvc::~CUPSInfoSvc()
{
}