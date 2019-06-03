/******************************************************************************/
/**	@class CTGuardianApp
/******************************************************************************/
#include "stdafx.h"
#include "Guardian.h"
#include "GuardianDlg.h"
#include "Guardiandef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTGuardianApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CTGuardianApp::CTGuardianApp()
{
}

CTGuardianApp theApp;

BOOL CTGuardianApp::InitInstance()
{
#ifdef _DEBUG
	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif	

	//CString strGuardianExe = "Guardian.exe";
	//DWORD guardianProcessID = GetProcessIdFromName(strGuardianExe);
	//if(guardianProcessID>0)
	//{
	//	return FALSE;
	//}

	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	TCHAR   path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,path); 

	CString strSource;
	CString strExeFileName;
	CString strIniFileName;
	strSource  = path;
	strIniFileName = strSource + _T("\\Maintenance.INI");
	strExeFileName = GetIniDataString(strIniFileName,_T("APPLICATION"),_T("ProgName"),_T("TOM")) + _T(".exe");

	while(1)
	{
		::Sleep(300);
		DWORD ProcessID = GetProcessIdFromName(strExeFileName);
		if(ProcessID <= 0)
		{
			ComExecuteApp(strExeFileName,_T(""));
			//::Sleep(1*1000);
		}
		HANDLE m_hComfirmAlive = OpenEvent(NULL,NULL,_T("CONFIRM_ALIVE"));
		if(m_hComfirmAlive!=NULL && m_hComfirmAlive!=INVALID_HANDLE_VALUE)
		{
			DWORD waitResult =WaitForSingleObject(m_hComfirmAlive,2*1000);
			if(waitResult == WAIT_TIMEOUT)
			{
				ComKillAppEx(strExeFileName);
			}
		}
	}
}
