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
@brief	TVMӦ�ó�����
*/

CTVMApp theApp;       // TVMӦ�ó�����Ψһ����


BEGIN_MESSAGE_MAP(CTVMApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMApp::CTVMApp()
:m_pMainWndRec(NULL)
{
	// ������Ļ����
	// ��Ļ����Ϊ5
	//if(5 == theSETTING.GetScreenBrightnessValue())
	//{
	//	theScreen_HELPER.recoverBrightness();
	//}
	//// ��Ϊ5
	//else
	//{
	//	theScreen_HELPER.setBrightness(theSETTING.GetScreenBrightnessValue());
	//}
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CTVMApp ��׼���ڻ�����

@param      ��

@retval     BOOL:   TRUE:�ɹ�, FALSE:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTVMApp::InitInstance()
{


#ifdef _DEBUG
	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif
#ifndef _DEBUG
	//���������ע�ͣ�������ʽ������ʹ��CShieldKeyHelper::SetHostProgarmEnable(true);
#endif
	AfxEnableControlContainer();

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ProcessUnHandleException);
	SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);

	// �ó����ļ��������ź�������ֹ�����������ʵ�����ź����ڳ������ʱ���Զ��رգ�
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("TVMNew")); 
	

	if (GetLastError() == ERROR_ALREADY_EXISTS) {   // ����������ʵ��
		return FALSE;
	}

	// ���ó������б�ʶ
	bool bFirstRun = theTVM_SETTING.IsProgFirstRun();
	theAPP_SESSION.SetIsFirstRun(bFirstRun);
	if(bFirstRun){
		theTVM_SETTING.SetIsFirstRun(false);
	}

	// ������Ļ����
	// ��Ļ����Ϊ5
	if(5 == theTVM_SETTING.GetScreenBrightnessValue())
	{
		theScreen_HELPER.recoverBrightness();
	}
	// ��Ϊ5
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
		SetLocalTime(&dSystemTime);			// ���õ�ǰϵͳʱ��

		theTVM_SETTING.SetTimeChangeValue(0);
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
CWnd* CTVMApp::GetMainWndRec()
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
CWnd* CTVMApp::GetMainWndRecWnd()
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
int CTVMApp::ExitInstance()
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
BOOL CTVMApp::OnIdle(LONG lCount)
{
	return CWinApp::OnIdle(lCount);
}

long  CTVMApp::ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo)
{
	// ����ϵͳ������
	CreateDump(lpExceptionInfo);

	// ��ʾϵͳ��������
	CMessageBox::Show(_T(""),_tl(TXT_SYSTEM_RESTART),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	CShieldKeyHelper::SetFunctionKeysEnable(true);
	CShieldKeyHelper::SetTaskManagerEnable(true);
	
	// �����Լ���Ȼ����������
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);

	return EXCEPTION_EXECUTE_HANDLER;
}
