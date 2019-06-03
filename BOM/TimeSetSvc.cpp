#include "stdafx.h"
#include "TimeSetSvc.h"
#include "TimeSetBaseOfficeDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CTimeSetSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::CTimeSetSvc()
:CBOMForeService(SYSTEM_TIME_SET_SVC)
{
	// 初始化对话框
	m_pDialogFlow->AddDialog(IDD_01801_TIMESET_BASE_DLG,new CTimeSetBaseOfficeDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TIMESET,theGUIDE_INFO.GetOperationMessage(GUIDE_TIMESET_INPUT_AND_CONFIRM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TIMESET, IDD_01801_TIMESET_BASE_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TIMESET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::~CTimeSetSvc()
{
}