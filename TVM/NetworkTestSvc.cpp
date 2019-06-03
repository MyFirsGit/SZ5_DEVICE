#include "stdafx.h"
#include "NetworkTestSvc.h"
#include "SYSInfo.h"
#include "networkitemmaintaindlg.h"
#include "guideinfo.h"

//��Ϣ����Ϣ������
BEGIN_MESSAGE_MAP(CNetworkTestSvc, CTVMForeService)
	////ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
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
	:CTVMForeService(NETWORK_TEST_SVC)
{
	//���ò���Ա����
	GetDialogFlow()->AddDialog(IDD_20551_NETWORK_ITEM_DLG,new CNetworkItemMaintainDlg(this));

	//���û������
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_NETWORK, _opl(GUIDE_ENTER_F3_ESC));
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
/*
@brief      ����ҵ��ĵ�һ����Ӧ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNetworkTestSvc::OnStart(){
	// ���ó�ʼ������
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_NETWORK);
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	__super::OnStart();

	// ��ʼ������
	CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
	if(NULL != pDlg){
		CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();
		MenuInfo = CNetworkItemMaintainDlg::Menu_Info::_Menu_Status_();
	}
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
		trans_exe = trans_exe + sIPAddress;                    //ping 10.1.1.30

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
			CString comment(_T("PingSC"));
			// �������--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			return FALSE;
		}
		else {
			CString comment(_T("PingSC"));
			// �������--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
			return TRUE;
		}
	}
	catch(CSysException&) {
		CString comment(_T("PingSC"));
		// �������--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
		throw;
	}
	catch (...) 
	{
		CString comment(_T("PingSC"));
		// �������--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]����������Ϣ

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CNetworkTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Enter����Ӧ��Ϣ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	//�쳣����
	try
	{
		int index = theAPP_SESSION.GetInputToInt();

		int nMenuPath = 0;

		//Get Dlg point
		CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
		CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();
		nMenuPath = _ttoi(MenuInfo.strMenuID);

		//����ǰ·��С�ڲ˵�·��ʱ,��Ϊ�������˵���
		if (nMenuPath <= 3)
		{
			switch(index)
			{
			case 1:
				{
					theAPP_SESSION.SetKeyboard(KEY_RETURN);
					theAPP_SESSION.SetMaxInputLength(0);
					//����SC��ͨ��
					theAPP_SESSION.ShowOfficeGuide(GUIDE_NET_TESTING);
					//����һ����ʾ
					pDlg->UpdateLableGroup(1);
					//ʵ��Ping����ʵ
					pDlg->PingScConn();

					//��������ʾ
					theAPP_SESSION.ShowOfficeGuide(GUIDE_SC_INFO_CONFIRM);
					//���õ�ǰλ����״̬
					MenuInfo.strMenuID = _T("10");        //��ʾ��ɲ��Բ�����1�����ϼ�Ŀ¼��0������Ŀ¼��\
					��Ϊ���Ӳ˵�������0��ʾ��
					MenuInfo.strMenu_1_Status = COMPLETED;
				}
				break;
			case 2:
				{
					if(!theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
						//��ʾ�������˵���š�
						theAPP_SESSION.ShowOfficeGuide(_T("���˺���Ȩ��ִ�иò�����"));
						return FALSE;
					}
					//ftp�ϴ�����
					pDlg->FtpUploading();
				}
				break;
			case 3:
				{
					if(!theAPP_SESSION.IsSuperUser() && theFunction_INFO.IsFounderOperation()){
						//��ʾ�������˵���š�
						theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR)/*GUIDE_MENU_CODE_ILLEGAL*/);
						return FALSE;
					}
					//FTP���ز���
					pDlg->FtpDownloading();
				}
				break;
			default:
				{
					//��ʾ�������˵���š�
					theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
					return FALSE;
				}
			}

		}
		else
		{
			//������ʾ���������Ϣ
			theAPP_SESSION.ShowOfficeGuide(GUIDE_STATION_SET_INPUT_ERROR);
		}
		NotifyDataModelChanged();
	}
	catch (CSysException& e)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(GUIDE_TEST_FAIL);
	}
	catch (...)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(DEVICE_TEST_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(GUIDE_TEST_FAIL);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      F3��Ӧ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	int nMenuPath = 0;

	//Get Dlg point
	CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
	CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();

	nMenuPath = _ttoi(MenuInfo.strMenuID);
	//AfxMessageBox(MenuInfo.strMenuID);

	if (nMenuPath > 4 )
	{
		pDlg->RebackToMenu();
		MenuInfo.strMenuID.Format(_T("0"));
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(1);
	}
	else
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���½�����ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNetworkTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}