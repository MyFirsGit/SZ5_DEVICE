#include "stdafx.h"
#include "RemoveExceptionSvc.h"
#include "RemoveExceptionDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRemoveExceptionSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::CRemoveExceptionSvc():CBOMForeService(REMOVE_EXCEPTION_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20701_REMOVE_EXCEPTION_DLG, new CRemoveExceptionDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_REMOVE_EXCEPTION));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REMOVE_EXCEPTION, IDD_20701_REMOVE_EXCEPTION_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::~CRemoveExceptionSvc(void)
{
}