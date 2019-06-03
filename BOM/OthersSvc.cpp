#include "stdafx.h"
#include "BOM.h"
#include "OthersSvc.h"
#include "WelcomeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COthersSvc::COthersSvc()
:CBOMForeService(OTHERS_SVC)
{
    // 设置操作员画面
 //   m_pDialogFlow->AddDialog(CWelcomeDlg::IDD, new CWelcomeDlg(this));

 //   // 设置乘客画面

 //   // 设置画面分组
 //   m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, "");
 //   m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CWelcomeDlg::IDD);

 //   // 设置初始画面组
	//m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COthersSvc::~COthersSvc()
{
}