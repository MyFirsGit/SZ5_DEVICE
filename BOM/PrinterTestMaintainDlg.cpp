#include "stdafx.h"
#include "BOM.h"
#include "PrinterTestMaintainDlg.h"
#include "guideinfo.h"
#include "PrinterTestSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPrinterTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CPrinterTestMaintainDlg,COperationDlg)
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
CPrinterTestMaintainDlg::CPrinterTestMaintainDlg(CService* pService)
: COperationDlg(CPrinterTestMaintainDlg::IDD, pService)
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
CPrinterTestMaintainDlg::~CPrinterTestMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      窗体创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct 窗体创建的区域

@retval     int		\n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	return __super::OnCreate(lpCreateStruct,   GUIDE_AREA | BTN_OK) ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try
	{
		CPrinterTestSvc* pPrinterTestSvc = (CPrinterTestSvc*)GetService();

		// 设置对话框处于繁忙状态
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 设置向导信息
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_TESTING));
		// 关闭打印机
		pPrinterTestSvc->GetPRTHelper()->PrinterClose();
		// 连接打印机
		pPrinterTestSvc->GetPRTHelper()->PrinterConnect(theBOM_INFO.GetPrinterCommPort(), theBOM_INFO.GetPrinterBaudRate());
		// 启动打印测试
		pPrinterTestSvc->PrinterPrint();
		// 移除异常
		theEXCEPTION_MGR.RemoveException(CPrinterException::MODULE_ID);
		// 显示打印机测试完成信息
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_TEST_FINISH));
		// 取消对话框繁忙状态
		theSERVICE_MGR.SetForeServiceBusy(false);

		HideCompoment(BTN_OK);
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_PRINTER_CHOOSE_ITEM));
	}
	return 0;
}

