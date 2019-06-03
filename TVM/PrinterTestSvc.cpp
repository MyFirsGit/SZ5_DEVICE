#include "stdafx.h"
#include "PrinterTestSvc.h"
#include "printertestmaintaindlg.h"

BEGIN_MESSAGE_MAP(CPrinterTestSvc, CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::CPrinterTestSvc()
	:CTVMForeService(RECEIPT_RINTER_TEST_SVC)
{
	//添加画面
	GetDialogFlow()->AddDialog(IDD_20531_PRINTER_TEST_DLG,new CPrinterTestMaintainDlg(this));

	//设置画面组
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_PRINT, _opl(GUIDE_ENTER_F3_ESC));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_PRINT,IDD_20531_PRINTER_TEST_DLG);

	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_PRINT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::~CPrinterTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::OnStart(){
	__super::OnStart();
	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_PRINT);
	m_Modol = Modol();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [ENTER]按键处理消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	int index = theAPP_SESSION.GetInputToInt();
	switch(index)
	{
	case MENU_CODE_1:
		{
			m_OperationType = OPERATION_TYPE_RESET;
		}
		break;
	case MENU_CODE_2:
		{
			m_OperationType = OPERATION_TYPE_TEST_PRINT;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return FALSE;
	}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//根据选项，做相应的操作
	DoOperation();

	//更新界面显示
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPrinterTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]按键处理消息

@param      (i)WPARAM wParam  数字键值，例如：数字键7，值为7
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CPrinterTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型数据

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestSvc::Modol& CPrinterTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      数据改变后，通知窗体更新数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      具体操作

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestSvc::DoOperation()
{
	int  nMenuNum = 0;
	long errCode = 0;
	bool bIsConn = false;
	theSERVICE_MGR.SetForeServiceBusy(true);
	try
	{
		//1.重置
		if ( m_OperationType == OPERATION_TYPE_RESET)
		{
			nMenuNum = 0;
			//尝试关闭连接
			PRINTER_HELPER->PrinterClose();	//mxw
			//尝试再次打开连接
			PRINTER_HELPER->PrinterConnect();
			PRINTER_HELPER->CheckStatus();
		}
		//2.打印测试
		else if (m_OperationType == OPERATION_TYPE_TEST_PRINT)
		{
			nMenuNum = 1;
			CString sztxt=_T("硬件自检-打印测试:\n~!@#$%^&*()+-=_{}[]:;,./|0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\nHardware auto-test,teste for bem sucedido!\n硬件自检-测试成功!\n\n");
			CStringArray strlist;
			strlist.Add(sztxt);
			PRINTER_HELPER->CheckStatus();
			PRINTER_HELPER->PrintReceiptByText(&strlist,false,false);
		}
		m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		CString comment(_T("PrinterTest"));
		// 打印机模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,true,comment);
	}
	catch (CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		CString comment(_T("PrinterTest"));
		// 打印机模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		CString comment(_T("PrinterTest"));
		// 打印机模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PRINTER_UNIT_TEST,false,comment);
	}

	theSERVICE_MGR.SetForeServiceBusy(false);
}