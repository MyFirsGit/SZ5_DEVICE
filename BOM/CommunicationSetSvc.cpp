#include "stdafx.h"
#include "CommunicationSetSvc.h"
#include "CommunicationSetMaintainDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CCommunicationSetSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::CCommunicationSetSvc() : CBOMForeService(COMMUNICATION_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_20114_COMMUNICATION_SET_DLG,new CCommunicationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMMUNICATION_SET,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMMUNICATION_SET, IDD_20114_COMMUNICATION_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::~CCommunicationSetSvc()
{
}