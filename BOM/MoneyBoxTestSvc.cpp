#include "stdafx.h"
#include "MoneyBoxTestSvc.h"
#include "bominfo.h"
#include "MoneyBoxTestMaintainDlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CMoneyBoxTestSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestSvc::CMoneyBoxTestSvc()
:CBOMForeService(MONEYBOX_TEST_SVC)
{
	GetDialogFlow()->AddDialog(IDD_20541_MONEYBOX_TEST_DLG,new CMoneyBoxTestMaintainDlg(this));
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_MONEYBOX, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_MONEYBOX,IDD_20541_MONEYBOX_TEST_DLG);

	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_MONEYBOX);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestSvc::~CMoneyBoxTestSvc()
{

}

void CMoneyBoxTestSvc::OnOK()
{
	// 设置对话框忙
	theSERVICE_MGR.SetForeServiceBusy(true);
	// 显示向导信息——启动钱箱测试
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));
	// 关闭打印机
	GetPRTHelper()->PrinterClose();
	// 连接打印机
	GetPRTHelper()->PrinterConnect(theBOM_INFO.GetPrinterCommPort(), theBOM_INFO.GetPrinterBaudRate());
	// 打开钱箱
	GetPRTHelper()->PrinterOpenBox();	

	// 取消对话框忙
	theSERVICE_MGR.SetForeServiceBusy(false);
	// 显示向导信息——钱箱测试完成
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_TEST_FINISH));

	
}
