#include "stdafx.h"
#include "TVM.h"
#include "utilobject.h"
#include "util.h"
#include "mainfrm.h"
#include "mainfrmrec.h"
#include "SYSInfo.h"
#include "servicemgr.h"
#include "servicedefinition.h"
#include "Command.h"
#include "InnerException.h"
#include "CoreAPI.h"
#include "DebugLog.h"
#include "ExceptionMgr.h"
#include "BitmapManager.h"
#include "delegate.h"
#include <winbase.h>
#include "shieldkeyhelper.h"
#include "LanguageDialog.h"
#include "Resource.h"
#include "ScreenHelper.h"
#include "tvmsetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	TVM应用程序类
*/

CTVMApp theApp;       // TVM应用程序类唯一对象


BEGIN_MESSAGE_MAP(CTVMApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTVMApp::CTVMApp()
:m_pMainWndRec(NULL)
{
	// 设置屏幕亮度
	// 屏幕亮度为5
	//if(5 == theSETTING.GetScreenBrightnessValue())
	//{
	//	theScreen_HELPER.recoverBrightness();
	//}
	//// 不为5
	//else
	//{
	//	theScreen_HELPER.setBrightness(theSETTING.GetScreenBrightnessValue());
	//}
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CTVMApp 标准初期化函数

@param      无

@retval     BOOL:   TRUE:成功, FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTVMApp::InitInstance()
{


#ifdef _DEBUG
	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif
#ifndef _DEBUG
	//方便测试先注释，发布正式版事需使用CShieldKeyHelper::SetHostProgarmEnable(true);
#endif
	AfxEnableControlContainer();

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ProcessUnHandleException);
	SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);

	// 用程序文件名创建信号量，防止程序启动多个实例（信号量在程序结束时会自动关闭）
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("TVMNew")); 
	

	if (GetLastError() == ERROR_ALREADY_EXISTS) {   // 程序已启动实例
		return FALSE;
	}

	// 设置程序运行标识
	bool bFirstRun = theTVM_SETTING.IsProgFirstRun();
	theAPP_SESSION.SetIsFirstRun(bFirstRun);
	if(bFirstRun){
		theTVM_SETTING.SetIsFirstRun(false);
	}

	// 设置屏幕亮度
	// 屏幕亮度为5
	if(5 == theTVM_SETTING.GetScreenBrightnessValue())
	{
		theScreen_HELPER.recoverBrightness();
	}
	// 不为5
	else
	{
		theScreen_HELPER.setBrightness(theTVM_SETTING.GetScreenBrightnessValue());
	}

	SYSTEMTIME dSystemTime;
	if(theTVM_SETTING.GetTimeChangeValue())
	{
		CString TimeDate = theTVM_SETTING.GetCurSystemTime();
		CString YearNumber = TimeDate.Left(4);
		CString MonthNumber = TimeDate.Mid(4,2);
		CString DayNumber = TimeDate.Mid(6,2);
		CString HourNumber = TimeDate.Mid(8,2);
		CString MinuteNumber = TimeDate.Mid(10,2);
		CString SecondNumber = TimeDate.Right(2);
		CString DataStr = _T("");
		DataStr.Format(_T("%s-%s-%s %s:%s:%s"),YearNumber,MonthNumber,DayNumber,HourNumber,MinuteNumber,SecondNumber);
		COleDateTime TimNum;
		TimNum.ParseDateTime(DataStr);
		TimNum.GetAsSystemTime(dSystemTime);
		SetLocalTime(&dSystemTime);			// 设置当前系统时间

		theTVM_SETTING.SetTimeChangeValue(0);
	}

	// 清空回收站
	SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|SHERB_NOSOUND);

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//InitCommonControls();

	////设置内存分配断点(用于内存泄漏检测)
	//_CrtSetBreakAlloc(1034);

	// 启动主Service
	//theSERVICE_MGR.StartService(ROOT_SVC, false);

	CWinApp::InitInstance();
	theBitmap_MGR.Initialize();
	theGUI_INFO.Initialize();

	m_pMainWnd = new CMainFrm;

	((CMainFrm*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	((CMainFrm*)m_pMainWnd)->m_wndView.Invalidate(FALSE);
	((CMainFrm*)m_pMainWnd)->m_wndView.UpdateWindow();

	// Create and show MainFrmRec
	m_pMainWndRec = new CMainFrmRec;
	((CMainFrmRec*)m_pMainWndRec)->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
	m_pMainWndRec->ShowWindow(SW_SHOW);
	m_pMainWndRec->UpdateWindow();
	
	if (theTVM_INFO.IsTopmost()) {
		// 显示在最前面
		m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		m_pMainWndRec->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	}	

	//// 启动主Service
	theSERVICE_MGR.StartService(ROOT_SVC,false);
	theSERVICE_MGR.StartService(MAIN_SVC, false);
	theSERVICE_MGR.StartService(CONFIRM_ALIVE_SVC,false);
	theSERVICE_MGR.StartService(INIT_SVC, true);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到乘客主窗口的指针

@param      无

@retval     CWnd* 乘客主窗口的指针

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CTVMApp::GetMainWndRec()
{
	return m_pMainWndRec;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到乘客主窗口的视窗的指针

@param      无

@retval     CWnd* 乘客主窗口的视窗的指针

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CTVMApp::GetMainWndRecWnd()
{
	return m_pMainWndRec;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOMApp 标准终止函数

@param      无

@retval     int: 0 indicates no errors, and values greater than 0 indicate an error.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMApp::ExitInstance()
{
	// 关闭所有Service对象
	theSERVICE_MGR.StopService(MAIN_SVC);
	theSERVICE_MGR.StopService(ROOT_SVC);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	CShieldKeyHelper::SetHostProgarmEnable(false);
	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      空闲函数

@param      LONG lCount

@retval     BOOL: TRUE to receive more idle processing time; FALSE no more idle time is needed.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTVMApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
}

long  CTVMApp::ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo)
{
	// 创建系统上下文
	CreateDump(lpExceptionInfo);

	// 提示系统出现问题
	CMessageBox::Show(_T(""),_tl(TXT_SYSTEM_RESTART),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	
	// 结束自己，然后重启程序
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);

	return EXCEPTION_EXECUTE_HANDLER;
}
