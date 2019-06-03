/******************************************************************************/
/**	@class CCTLauncherApp
/******************************************************************************/
#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "Launcherdef.h"
#include <Shlwapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCTLauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CCTLauncherApp::CCTLauncherApp()
{
}

CCTLauncherApp theApp;

BOOL CCTLauncherApp::InitInstance()
{
#ifdef _DEBUG
	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif	
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	TCHAR   path[MAX_PATH];
	DWORD dwLen = GetModuleFileName(NULL, path, MAX_PATH);
	PathRemoveFileSpec(path);
	//GetCurrentDirectory(MAX_PATH,path); 

	CString strSource;
	CString strDest ;
	CString strExeFileName;
	CString strIniFileName;
	CString strGUISource; //GUIԴ�ļ�Ŀ¼
	CString strGUIDest;   //GUIĿ���ļ�Ŀ¼
	strSource      = path;
	strDest        = path;
	strGUISource   = path;
	strGUIDest     = path;
	strIniFileName = path;
	strSource    = strSource + _T("\\Download\\MC\\");
	
	//strGUISource = strGUISource + _T("\\Download\\GUI\\");
	//
	//strGUIDest   = strGUIDest   + _T("\\res\\GUI\\");

	strGUISource = strGUISource + _T("\\Download\\STATION_MAP\\");

	strGUIDest   = strGUIDest   + _T("\\res\\GUI\\layout\\");

	strIniFileName =strIniFileName  + _T("\\Maintenance.INI");
	strExeFileName = GetIniDataString(strIniFileName,_T("APPLICATION"),_T("ProgName"),_T("TOM")) + _T(".exe");
	
	strSource.TrimRight(_T("\\"));
	strDest.TrimRight(_T("\\"));

	CCTLauncherDlg* pBomStarterDlg = new CCTLauncherDlg();

	pBomStarterDlg->Create(IDD_CTLAUNCHER_DIALOG,CWnd::FromHandle(::GetDesktopWindow()));
	pBomStarterDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ); // style WS_EX_TOPMOST 
	pBomStarterDlg->ShowWindow(SW_SHOW);
	pBomStarterDlg->UpdateWindow(); 
	
	Sleep(3000);
	// ������ACC���ݸ�ʽ�仯��ִ���������ļ�ɾ��ԭ�������ļ�
	if (ComCheckFileExist(path+CString(_T("\\Download\\MC\\MC_upd.bat")))) {  
		STARTUPINFO StartInfo; 
		PROCESS_INFORMATION ProcInfo; 
		GetStartupInfo(&StartInfo);

		StartInfo.dwFlags = STARTF_USESHOWWINDOW;    // ��������
		StartInfo.wShowWindow = SW_HIDE;
	
		// ������
		CString exeFile = _T(".\\Download\\MC\\MC_upd.bat");

		// ���г���
		BOOL ret_code = CreateProcess(exeFile,                      
			NULL,
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS,
			NULL,
			NULL,
			&StartInfo,
			&ProcInfo);

		// �ȴ��������н���
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	}
	
	// ������ACC���ݸ�ʽ�仯��ִ���������ļ�ɾ��ԭ�������ļ�
	if (ComCheckFileExist(path+CString(_T("\\Download\\MC\\MC_ini.bat")))) {
		STARTUPINFO StartInfo; 
		PROCESS_INFORMATION ProcInfo; 
		GetStartupInfo(&StartInfo);

		StartInfo.dwFlags = STARTF_USESHOWWINDOW;    // ��������
		StartInfo.wShowWindow = SW_HIDE;

		// ������
		CString exeFile = _T(".\\Download\\MC\\MC_ini.bat");

		// ���г���
		BOOL ret_code = CreateProcess(exeFile,                      
			NULL,
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS,
			NULL,
			NULL,
			&StartInfo,
			&ProcInfo);

		// �ȴ��������н���
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	}
	
	// �Ƿ���Ҫ��ʽ��CF��
	//if (ComCheckFileExist(path+CString("\\CFCardFormat.bat"))) {
	//    STARTUPINFO StartInfo; 
	//    PROCESS_INFORMATION ProcInfo; 

	//    GetStartupInfo(&StartInfo);

	//    StartInfo.dwFlags = STARTF_USESHOWWINDOW;    // ��������
	//    StartInfo.wShowWindow = SW_HIDE;

	//    // ������
	//    CString exeFile = ".\\CFCardFormat.bat";

	//    // ���г���
	//    BOOL ret_code = CreateProcess(exeFile,                      
	//        NULL,
	//        NULL,
	//        NULL,
	//        FALSE,
	//        NORMAL_PRIORITY_CLASS,
	//        NULL,
	//        NULL,
	//        &StartInfo,
	//        &ProcInfo);

	//    // �ȴ��������н���
	//    WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	//}

	//�ر����س���
	ComKillAppEx(strExeFileName);
	
	//��Ⲣ����Ŀ¼
	ComCheckAndCreateDirectory(strGUIDest);
	//����GUI�ļ�
	if (FALSE == CopyDirectory(strGUISource,strGUIDest))
	{
	}
	else
	{
		SHFILEOPSTRUCT fos ;
		ZeroMemory( &fos, sizeof( fos)) ;
		fos.hwnd = NULL;
		fos.wFunc = FO_DELETE ;
		fos.fFlags = FOF_SILENT  | FOF_NOCONFIRMATION;

		TCHAR   DelFileSource[MAX_PATH];
		memset(DelFileSource, 0x00, MAX_PATH);
		_tcscpy(DelFileSource, path);
		_tcscat(DelFileSource, _T("\\Download\\STATION_MAP\\*.*"));

		fos.pFrom = DelFileSource;

		// ɾ���ļ��м�������
		SHFileOperation( &fos);
	}

	// �������س���
	if(FALSE == CopyDirectory(strSource,strDest))
	{
	}
	else
	{
		SHFILEOPSTRUCT fos ;
		ZeroMemory( &fos, sizeof( fos)) ;
		fos.hwnd = NULL;
		fos.wFunc = FO_DELETE ;
		fos.fFlags = FOF_SILENT  | FOF_NOCONFIRMATION;
		
		TCHAR   DelFileSource[MAX_PATH];
		memset(DelFileSource, 0x00, MAX_PATH);
		_tcscpy(DelFileSource, path);
		_tcscat(DelFileSource, _T("\\Download\\MC\\*.*"));

		fos.pFrom = DelFileSource;

		// ɾ���ļ��м�������
		SHFileOperation( &fos);
	}

	pBomStarterDlg->DestroyWindow();
	delete pBomStarterDlg;
	pBomStarterDlg = NULL;

	_tcscat_s(path,_T("\\"));
	_tcscat_s(path,strExeFileName);
	if (TRUE == ComCheckFileExist(path))
	{
		ComExecuteApp(path,_T(""));
	}

	return TRUE;
}
