#include "stdafx.h"
#include "BOM.h"
#include "MoneyBoxTestMaintainDlg.h"
#include "guideinfo.h"
#include "MoneyBoxTestSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMoneyBoxTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CMoneyBoxTestMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestMaintainDlg::CMoneyBoxTestMaintainDlg(CService* pService)
: COperationDlg(CMoneyBoxTestMaintainDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestMaintainDlg::~CMoneyBoxTestMaintainDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      OnCreate

@param      (i)LPCREATESTRUCT lpCreateStruct 窗体创建的区域

@retval     int		\n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CMoneyBoxTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 显示BOM文言：请按确认键开始测试
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));

	return __super::OnCreate(lpCreateStruct,   GUIDE_AREA | BTN_OK);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      确认按钮消息处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMoneyBoxTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//// 获取Service
	CMoneyBoxTestSvc* pMoneyBoxTestSvc = (CMoneyBoxTestSvc*)GetService();
	pMoneyBoxTestSvc->OnOK();

	HideCompoment(BTN_OK);

	return 0;
}