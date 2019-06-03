#include "stdafx.h"
#include "THTestSvc.h"
#include "THTestMaintainDlg.h"
#include "GuideInfo.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::CTHTestSvc()
	:CBOMForeService(TH_TEST_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_TH_TEST_DLG, new CTHTestMaintainDlg(this));

	//设置TH测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_TEST, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_CHOOSE_ITEM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_TEST, IDD_TH_TEST_DLG);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::~CTHTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取服务的数据模型

@param      none

@retval     Model	激活数据模型

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::Model& CTHTestSvc::GetDataModel()
{
	return m_DataModel;
}