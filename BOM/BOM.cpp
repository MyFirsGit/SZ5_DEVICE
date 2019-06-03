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
@brief	BOMӦ�ó�����
*/

CBOMApp theApp;       // BOMӦ�ó�����Ψһ����


BEGIN_MESSAGE_MAP(CBOMApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBOMApp::CBOMApp()
:m_pMainWndRec(NULL)
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CBOMApp ��׼���ڻ�����

@param      ��

@retval     BOOL:   TRUE:�ɹ�, FALSE:ʧ��

@exception  ��
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

	// �ó����ļ��������ź�������ֹ�����������ʵ�����ź����ڳ������ʱ���Զ��رգ�
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("BOMNew")); 
	

	if (GetLastError() == ERROR_ALREADY_EXISTS) {   // ����������ʵ��
		return FALSE;
	}

	// ��ջ���վ
	SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|SHERB_NOSOUND);

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	//InitCommonControls();

	////�����ڴ����ϵ�(�����ڴ�й©���)
	//_CrtSetBreakAlloc(1034);

	// ������Service
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
		// ��ʾ����ǰ��
		m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		m_pMainWndRec->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	}
	
	

	//// ������Service
	theSERVICE_MGR.StartService(ROOT_SVC,false);
	theSERVICE_MGR.StartService(MAIN_SVC, false);
	theSERVICE_MGR.StartService(CONFIRM_ALIVE_SVC,false);
	theSERVICE_MGR.StartService(INIT_SVC, true);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��˿������ڵ�ָ��

@param      ��

@retval     CWnd* �˿������ڵ�ָ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CBOMApp::GetMainWndRec()
{
	return m_pMainWndRec;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��˿������ڵ��Ӵ���ָ��

@param      ��

@retval     CWnd* �˿������ڵ��Ӵ���ָ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CBOMApp::GetMainWndRecWnd()
{
	return m_pMainWndRec;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOMApp ��׼��ֹ����

@param      ��

@retval     int: 0 indicates no errors, and values greater than 0 indicate an error.

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CBOMApp::ExitInstance()
{
	// �ر�����Service����
	theSERVICE_MGR.StopService(MAIN_SVC);
	theSERVICE_MGR.StopService(ROOT_SVC);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	CShieldKeyHelper::SetHostProgarmEnable(false);
	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���к���

@param      LONG lCount

@retval     BOOL: TRUE to receive more idle processing time; FALSE no more idle time is needed.

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CBOMApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
}

long  CBOMApp::ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo)
{
	CMessageBox::Show(_T(""),_T("BOMϵͳ�����쳣����Ҫ������"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	return EXCEPTION_EXECUTE_HANDLER;
}
