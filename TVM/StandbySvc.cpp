#include "stdafx.h"
#include "StandbySvc.h"
#include "StandbyDlg.h"


#include <map>

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStandbySvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStandbySvc::CStandbySvc():CTVMForeService(STANDBY_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20901_SYSTEM_OUT_DLG,new CStandbyDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_STANDBY);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_STANDBY, IDD_20901_SYSTEM_OUT_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STANDBY);	// 设置初始画面组
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStandbySvc::~CStandbySvc()
{
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStandbySvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      啟動服務

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStandbySvc::OnStart(){
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STANDBY);
	m_pDialogFlow->ShowFirstDialogGroup();
}

