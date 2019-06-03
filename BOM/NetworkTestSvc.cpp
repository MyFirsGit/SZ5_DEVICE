#include "stdafx.h"
#include "NetworkTestSvc.h"
#include "bominfo.h"
#include "networkitemmaintaindlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CNetworkTestSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::CNetworkTestSvc()
:CBOMForeService(NETWORK_TEST_SVC)
{
	GetDialogFlow()->AddDialog(IDD_20551_NETWORK_ITEM_DLG,new CNetworkItemMaintainDlg(this));

	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_NETWORK, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_NETWORK,IDD_20551_NETWORK_ITEM_DLG);

	// ���ó�ʼ������
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_NETWORK);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::~CNetworkTestSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ping��λ������

@param      char* pBuff    ���BUFF

@retval     BOOL \n
TRUE:�����ɹ�    FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CNetworkTestSvc::PingSC(char* pBuff)
{
	try {

		// ��ð�ȫ��Ϣ
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead,hWrite;

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		// �����ܵ�
		if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}

		// ��ʼping SC
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		CString trans_exe = _T("ping ");                       // ָ��Ӧ�ó����·��
		CString sIPAddress = theMAINTENANCE_INFO.GetCpsHostID();
		trans_exe = trans_exe + sIPAddress;

		if (!CreateProcess(NULL,const_cast<LPTSTR>((LPCTSTR)trans_exe),NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}
		WaitForSingleObject(pi.hProcess, 60000 );        // �ȴ�EXE����(max 60sec)
		CloseHandle(hWrite);
		// ��ȡ���Խ��
		DWORD bytesRead;
		ReadFile(hRead,pBuff,MAX_PING_RESULT_LLENGTH-1,&bytesRead,NULL);

		// ����������Խ��
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

