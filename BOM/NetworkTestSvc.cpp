#include "stdafx.h"
#include "NetworkTestSvc.h"
#include "bominfo.h"
#include "networkitemmaintaindlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CNetworkTestSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::CNetworkTestSvc()
:CBOMForeService(NETWORK_TEST_SVC)
{
	GetDialogFlow()->AddDialog(IDD_20551_NETWORK_ITEM_DLG,new CNetworkItemMaintainDlg(this));

	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_NETWORK, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_NETWORK,IDD_20551_NETWORK_ITEM_DLG);

	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_NETWORK);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::~CNetworkTestSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ping上位服务器

@param      char* pBuff    输出BUFF

@retval     BOOL \n
TRUE:创建成功    FALSE:创建失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CNetworkTestSvc::PingSC(char* pBuff)
{
	try {

		// 获得安全信息
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead,hWrite;

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		// 创建管道
		if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}

		// 开始ping SC
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		CString trans_exe = _T("ping ");                       // 指定应用程序的路径
		CString sIPAddress = theMAINTENANCE_INFO.GetCpsHostID();
		trans_exe = trans_exe + sIPAddress;

		if (!CreateProcess(NULL,const_cast<LPTSTR>((LPCTSTR)trans_exe),NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}
		WaitForSingleObject(pi.hProcess, 60000 );        // 等待EXE结束(max 60sec)
		CloseHandle(hWrite);
		// 读取测试结果
		DWORD bytesRead;
		ReadFile(hRead,pBuff,MAX_PING_RESULT_LLENGTH-1,&bytesRead,NULL);

		// 返回网络测试结果
		if ( 0 == bytesRead ) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) 
	{
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

