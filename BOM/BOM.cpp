#include "stdafx.h"
#include "BOM.h"
#include "utilobject.h"
#include "util.h"
#include "mainfrm.h"
#include "mainfrmrec.h"
#include "bominfo.h"
#include "servicemgr.h"
#include "servicedefinition.h"
#include "InnerException.h"
#include "CoreAPI.h"
#include "DebugLog.h"
#include "ExceptionMgr.h"
#include "BitmapManager.h"
#include "delegate.h"
#include <winbase.h>
#include "shieldkeyhelper.h"




//#include "MainFrm.h"
//#include "MainFrmRec.h"
//#include "GlobalParameter.h"
//#include "bitmapmanager.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	BOM应用程序类
*/

CBOMApp theApp;       // BOM应用程序类唯一对象


BEGIN_MESSAGE_MAP(CBOMApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBOMApp::CBOMApp()
:m_pMainWndRec(NULL)
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CBOMApp 标准初期化函数

@param      无

@retval     BOOL:   TRUE:成功, FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CBOMApp::InitInstance()
{


#ifdef _DEBUG
	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif

	CShieldKeyHelper::SetHostProgarmEnable(true);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ProcessUnHandleException);
	SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);

	// 用程序文件名创建信号量，防止程序启动多个实例（信号量在程序结束时会自动关闭）
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("BOMNew")); 
	

	if (GetLastError() == ERROR_ALREADY_EXISTS) {   // 程序已启动实例
		return FALSE;
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

	//// Initialize OLE libraries
	//if (!AfxOleInit()) {
	//	AfxMessageBox(IDP_OLE_INIT_FAILED);
	//	return FALSE;
	//}

	//AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Create and show MainFrm
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
	
	if (theBOM_INFO.IsTopmost()) {
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
CWnd* CBOMApp::GetMainWndRec()
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
CWnd* CBOMApp::GetMainWndRecWnd()
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
int CBOMApp::ExitInstance()
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
BOOL CBOMApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
}

long  CBOMApp::ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo)
{
	CMessageBox::Show(_T(""),_T("BOM系统出现异常，需要重启！"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	return EXCEPTION_EXECUTE_HANDLER;
}
